/**
 * File: Decoder.cpp
 * Author: WJLIDDY
 */
#include "Decoder.h"

#include <list>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

#include <stdio.h>
#include <sys/stat.h>

//thread specific includes

#ifdef _WIN32
	#include <direct.h>
	//To check memleaks
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#else
	#include <unistd.h>
	#include <pthread.h>
#endif

#include<GnssMetadata/Metadata.h>

#include "ChunkBuffer.h"
#include "StreamAnalytics.h"
#include "XMLtoMeta.h"
#include "DecStream.h"

using namespace GnssMetadata;

	Decoder::Decoder(const char* pathToFile, uint64_t readSize, uint64_t buffSize, uint64_t streamSize, uint64_t blockTotal, const char** addlPaths, uint64_t pathCount)
	{
		_done = false;
		_blocksLeftToRead = blockTotal;
		_printStats = false;
		_printSamples = false;
		_mdPtr = 0;
		//to parse the first XML file, we need to change the WD to the first path.
		changeWD(pathToFile);
		std::string fname = std::string(pathToFile,strlen(pathToFile));
		this->_streamSize = streamSize;

		try
		{
			//Make list of _mdList _sdrFileNames, and associated data.
			makeFileList(fname);
			//Create but do not start filereader object.
			_fr = new FileReader(*_sdrFileNames,readSize,buffSize,pathToFile,addlPaths,pathCount);
			fetchFileSizes();

		} catch (TranslationException e)
		{
			//See if file even exists, since x2m does not throw this.
			//TODO: Functionize this.
			struct stat buf;
			if(stat(pathToFile, &buf) == -1)
				std::cout<< "Specified metadata file does not exist.\n";
			else
				std::cout << "XML error: " <<  e.what() << "\n";

			//g++ does not like this: throw std::exception("File was not read\n");
		}

		//If the file was a converted trigr file, then we run a special test on the footers.
		_TRIGRmode = false;
		if(_sdrFileNames->at(0).substr(_sdrFileNames->at(0).find_last_of(".") + 1) == "tgd")
			{
				_TRIGRmode = true;
				_TRIGRlastFooterValSet = false;
				//TODO: To be read from XML somewhere.
				_TRIGRmask = 0x00FFFFFF;
			}

	}

	void Decoder::readChunkCycles(Block * block, uint32_t cycles)
	{

		//for every cycle:
		for(;cycles != 0; cycles--)
		{
			//for every chunk in the block:
			for(int i = 0; i != block->chunkCount; i++)
			{

				Chunk* chunk = block->chunkArray[i];

				uint8_t sizeWord = chunk->SizeWord();
				uint8_t countWord = chunk->CountWords();

				int chunkBufferSize = sizeWord*countWord;

				char* buf = _fr->getBufferedBytes(chunkBufferSize);

				ChunkBuffer cb = ChunkBuffer(chunkBufferSize,buf);

				//while there are still bits left in the chunk:
				//TODO: Error if lump chunk doesn't fully read lump.
				while(!cb.chunkFullyRead())
				{
					//for each lump
					for(int i = 0; i != chunk->lumpCount; i++)
					{
						Lump* lump = chunk->lumpArray[i];

						//skip any padding at the start.
						if( (chunk->Shift() == chunk->Left && chunk->Padding() == chunk->Head) ||
						(chunk->Shift() == chunk->Right && chunk->Padding() == chunk->Tail))
						{
							cb.skipBits((8*chunk->SizeWord()) - lump->lumpSize);
						}

						//for each stream
						for(int i = 0; i != lump->streamCount; i++)
						{
							Stream* stream;

							//If RIGHT-SHIFTED read the last stream first. Else, read from the left stream.
							if(chunk->Shift() == chunk->Right)
								stream = lump->streamArray[(lump->streamCount - 1) - i];
							else
								stream = lump->streamArray[i];

							//find the correct decoded-Stream based on address
							for(int i = 0; i != _decStreamCount; i++)
							{

								if(_decStreamArray[i]->getCorrespondingStream() == stream){
									//decode this sample.
									decodeFormattedStream(stream,&cb,i);
								}
							}
						}

						//skip padding at the end.
						if( (chunk->Shift() == chunk->Right && chunk->Padding() == chunk->Head) ||
						(chunk->Shift() == chunk->Left && chunk->Padding() == chunk->Tail) )
						{
							cb.skipBits((8*chunk->SizeWord()) - lump->lumpSize);
						}
					}
				}

				//now that we are done reading the chunk, we can alert the filereader to release this chunk.
				_fr->doneReading(chunkBufferSize);
			}
		}
	}

	void Decoder::makeFileList(std::string pathToFile)
	{

		int totalBlocksDiscovered = 0;
		_mdList = new std::vector<Metadata*>;
		_sdrFileNames = new std::vector<std::string>;

		_mdBlockSizes = new std::vector<int*>;
		_mdBlockSizesCount = new std::vector<int>;

		//get the metadata of first file.
		XMLtoMeta* x2m = new XMLtoMeta(pathToFile.c_str());
		Metadata* nextMeta = x2m->getNonRefdMetadata();

		//add it to the queue
		_mdList->push_back(nextMeta);
		_sdrFileNames->push_back(nextMeta->Files().front().Url().Value());

		//add all block sizes of this metadata.
		_mdBlockSizesCount->push_back(nextMeta->Files().front().Lane().blockCount);
		_mdBlockSizes->push_back(generateBlockSizeArray(&nextMeta->Files().front().Lane()));

		//If there is a next file, and there are still blocks to read (or we elect to read infinite)
		while(nextMeta->Files().front().Next().IsDefined() && (totalBlocksDiscovered < _blocksLeftToRead || _blocksLeftToRead == -1))
		{
			//add the metadata and it's information to the list.
			//g++ not happy with this address of reference
			x2m = new XMLtoMeta((&(nextMeta->Files().front().Next().toString()))->c_str());
			nextMeta = x2m->getNonRefdMetadata();
			int bc = nextMeta->Files().front().nLane->blockCount;
			totalBlocksDiscovered += bc;
			_mdList->push_back(nextMeta);
			_sdrFileNames->push_back(nextMeta->Files().front().Url().Value());

		}
	}

	void Decoder::fetchFileSizes()
	{
		_sdrFileSize = new std::vector<uint64_t>;
		for(int i = 0; i != _sdrFileNames->size(); i++)
		{
			_sdrFileSize->push_back(_fr->getSizeOfFile(_sdrFileNames->at(i)));
		}
	}

	void Decoder::start(){

		//start the filereading thread.
		_fr->readAll();

		//pointer to next metadata to read
		Metadata* md;

		//for all metadatas
		while(_mdPtr < _mdList->size())
		{
			//increment the pointer.
			md =  _mdList->at(_mdPtr++);

			//repair the decoded streams, since we use addresses to find the right decoding stream.
			repairDecStreams(md);

			//Breaks things
			//TODO filereader should really handle this
			_fr->skipBufferedBytes(md->Files().front().Offset());

			//we don't do filesets yet, but almost
			if(md->FileSets().size() > 0 || md->Files().size() != 1)
			{
				printf("Unsupported Format: Spatial/Temporal File");
			}

			//TODO: add nLane to API. nLane points to the non referenced lane of this file.
			Lane* singleLane =  md->Files().front().nLane;

			//keep reading blocks until the amount in the file has expired
			//test!! will not work on a multi-file approach
			//also fr might read the whole file, but the ibuffer might still have something in it.

			//Continue if there are still samples in the file, or there still bytes left in the ibuffer.
			//UNIX hack to force file continuance for demo purposes
		//	while(fr->filesFullyReadCount() < mdPtr)
			#ifndef _WIN32
			while(_fr->filesFullyReadCount() == 0)
			#endif
			{
		//		std::cout << "Pairing " << mdPtr - 1 << " with " << fr->filesFullyReadCount() << "\n";

			uint64_t blockCount = 0; //for testing purposes
			//for each block in lane
				for(int i = 0; i != singleLane->blockCount && _blocksLeftToRead != 0; i++)
				{

					if(_blocksLeftToRead > 0)
						_blocksLeftToRead--;

					Block* block = singleLane->blockArray[i];

					//how many chunk cycles are there?
					uint32_t cycles = block->Cycles();

					//skip headers/footers

					uint32_t headerSize = block->SizeHeader();
					uint32_t footerSize = block->SizeFooter();


					_fr->skipBufferedBytes(headerSize);
					readChunkCycles(block, cycles);

					if(!_TRIGRmode)
					{
						_fr->skipBufferedBytes(footerSize);
					} else
					{
						//Only in TRIGGERMODE do overlaps exist.
						//Also, in TRIGGERMODE. loop blocks with overlap
						//we are in trigger mode. so we need to assure the file has integrity.
						//It's still a footer, but we have to mask it with the system.
						//Write a Generic alrgorithm for footers of any size.
						//TODO more elegant way
						blockCount++;

						//TODO clean this up, fr->get buffered bytes to void pointer, so much more
						//Including use a DWORD if on windows, but that isnt cross platfrom.
						uint32_t unMaskedFooter = 0;
						unsigned char* vals = reinterpret_cast<unsigned char*>(_fr->getBufferedBytes(4));

						//This is the only way I could get it to work. Reinterpreting to unsignedint didn't work. DWORD is better proabably.
						unMaskedFooter = (vals[3])*(256*256*256) + (vals[2])*256*256 + (vals[1])*256 + (vals[0]);

						_fr->doneReading(4);

						if(!_TRIGRlastFooterValSet)
						{
							_TRIGRlastFooterVal = unMaskedFooter & _TRIGRmask;
							_TRIGRlastFooterValSet= true;
						}
						else{
							uint32_t maskedFooter = unMaskedFooter & _TRIGRmask;
							//increment and compare
							//wrap-around

							//E06A9C00
							//E16A9C00
							//E26A9C00


							_TRIGRlastFooterVal = ((1+_TRIGRlastFooterVal) % (1+_TRIGRmask));

							std::cout << unMaskedFooter << std::endl;
							std::cout << maskedFooter << std::endl;
							std::cout << _TRIGRlastFooterVal << std::endl;

							if(maskedFooter != _TRIGRlastFooterVal)
								std::cout << "\n\n Warning: TRIGR footer didn't match for " << blockCount <<"\n\n";
							}


						//std::cout << "Footer @ " << blockCount << " " << TRIGRlastFooterVal << "\n";

					}
				//In order to test, this class will print data from a block.
				//This will be discarded when code is fully tests.

				StreamAnalytics sa;
				for(int i = 0; i != _decStreamCount; i++)
				{
						sa.setStream(_decStreamArray[i]);

						if(_printSamples)
							sa.printAllSamples();
						if(_printStats)
							sa.printMeanAndVar();
				}

				}

			}
		}

		//Done reading all blocks specified by metadata. TODO: Loop over a block set, but that's easy.
		//see if there are any bytes in the File or IBuffer. If there are, print a warning but kill the process anyhow.
		if(!_fr->hasReadWholeFile())
		{
			std::cout << "Warning: Some samples did not get read! In Buffer: " << _fr->numBytesLeftInBuffer() << " In File: " << _fr->numBytesLeftToReadFromFile() << "\n" ;
			_fr->killReadThread();
		}

		//done! Signal that we are done reading.
		_done = true;
	}

	//TODO Improve arbitrary array to vector.
	void Decoder::makeDecStreams(){

		//Count of all the streams we have found so far.
		_decStreamCount = 0;
		//TODO magic number!
		_decStreamArray = new DecStream*[256];

		/** I need to find how many streams there are, and also, data about them.
		/ * How can I get that?
		/ * Well, I need to iterate through every xml element and count the num of streams there are
		/ * I need to be careful to avoid duplicates...
		/ * We also need to get the size of a lump, in order to be able to do padding properly.
        **/

		//Start with the lane of the first metadata file.
		Lane* singleLane = _mdList->at(0)->Files().front().nLane;

		//for every stream in every lump in every chunk in every block
		for(int i = 0; i != singleLane->blockCount; i++){
			Block* b = singleLane->blockArray[i];
			for(int j = 0; j != b->chunkCount;j++){
				Chunk* c = b->chunkArray[j];
				for(int k = 0; k!= c->lumpCount;k++){
					Lump* l = c->lumpArray[k];
					l->lumpSize = 0;

					for(int i2  = 0; i2 != l->streamCount;i2++)
					{
						Stream* s = l->streamArray[i2];

						l->lumpSize += s->Packedbits();

						//Account for extra samples that would included w/ complex data
						//That is, add another sample in.
						if(!(s->Format() == s->IF || s->Format() == s->IFn))
							l->lumpSize += s->Packedbits();

						//This is a newstream until proven not a new stream.
						bool newStream = true;

						//Iterate through all streams, if the address of this stream matches another, it's not new.
						for(int c = 0; newStream &&  c != _decStreamCount; c++){
							if(s == _decStreamArray[c]->getCorrespondingStream())
									newStream = false;
						}

						//But if it is a new stream:
						if(newStream)
						{
							//Create a new decoded stream, make sure that the name is there, and other information useful in decoding.
							_decStreamArray[_decStreamCount] = new DecStream(_streamSize,s->Id(),s,!((s->Format() == s->IF) || (s->Format() == s->IFn)),
								(s->Format() == s->QI) || (s->Format() == s->QnIn) || (s->Format() == s->QIn) || (s->Format() == s->QnI));

							/**
							*
							*GN3S file and let it plot the waterfall
							 open up the 1-bit TRIGR L1/L2/L5/B1 fileset (found in ChameleonChips FTP site under ‘GpsBeiDou’) and show 4 waterfalls for the individual streams. The B1 band should contain some interesting comm signals. Add an extra 10-20 seconds if the B1 stream contains dynamically varying signals to highlight those.
							 Open up the Fraunhoffer files (hosted on ION’s SDR page) that contain multi-rate streams and plot its waterfalls
							 repeat for IFEN SDR files (which are spatially spliced)
							*
							*/

							//TODO fix this hardcoding.
							if(_sdrFileNames->at(0).find("GN3S") != std::string::npos)
								_decStreamArray[_decStreamCount]->setFreqs(16368000,-1,-1);

							//??
							if(_sdrFileNames->at(0).find("TRIGR") != std::string::npos)
								_decStreamArray[_decStreamCount]->setFreqs(16368000,-1,-1);

							if(_sdrFileNames->at(0).find("fraun") != std::string::npos)
							{
								//20 000 000, 40 000 000 IQ
								_decStreamArray[_decStreamCount]->setFreqs(20000000,-1,-1);
								if(_decStreamArray[_decStreamCount]->getID().find("L5") != std::string::npos)
									_decStreamArray[_decStreamCount]->setFreqs(40000000,-1,-1);
							}
							if(_sdrFileNames->at(0).find("IFEN") != std::string::npos)
								_decStreamArray[_decStreamCount]->setFreqs(20480000,-1,-1);

							_decStreamCount++;

						}
					}
				}
			}
		}

	}

	uint64_t Decoder::getDecStreamCount(){
		return _decStreamCount;
	}

	//TODO Do actually try to prevent memory leaks. Complicated becuase others may use my decoder
	Decoder::~Decoder(){
	//	delete fr;
	//	for(int i = 0; i != decStreamCount; i++)
	//	{
	//		delete decStreamArray[i];
	//	}
	//	delete [] decStreamArray;
	}

	void Decoder::repairDecStreams(Metadata* md)
	{

		//for stream in lump in block in lane
		for(int i = 0; i != md->Files().front().nLane->blockCount; i++){
			Block* b = md->Files().front().nLane->blockArray[i];
			for(int j = 0; j != b->chunkCount;j++){
				Chunk* c = b->chunkArray[j];
				for(int k = 0; k!= c->lumpCount;k++){
					Lump* l = c->lumpArray[k];

					l->lumpSize = 0;

					for(int i2  = 0; i2 != l->streamCount;i2++)
					{
						Stream* s = l->streamArray[i2];

						//TODO dup'd from makeDec'dStreams
						l->lumpSize += s->Packedbits();

						if(!(s->Format() == s->IF || s->Format() == s->IFn))
							l->lumpSize += s->Packedbits();

						bool streamReassigned = false;
						//we got a stream! Match it with a predefined one, based on the ID.
						for(int c = 0; c != _decStreamCount; c++){
							if(s->Id().compare(_decStreamArray[c]->getCorrespondingStream()->Id()) == 0)
							{
								_decStreamArray[c]->setCorrespondingStream(s);
								streamReassigned = true;
								break;
							}
						}

						//TODO throw error if stream could not be reassigned.

					}
				}
			}
		}

	}

	void Decoder::setPrintOptions(bool stat, bool samp)
	{
		_printSamples = samp;
		_printStats = stat;
	}

	double Decoder::getIBufPercent()
	{
		return _fr->getIBufPercent();
	}

	void Decoder::changeWD(const char* pathToFile)
	{
		//Simply change the working directory.
		std::string fname = std::string(pathToFile,strlen(pathToFile));
		std::string dir;
                #ifndef _WIN32
                const size_t last_slash_idx = fname.rfind('/');
                #else
		const size_t last_slash_idx = fname.rfind('\\');
                #endif

		if (std::string::npos != last_slash_idx)
		{
			 dir = fname.substr(0, last_slash_idx);
		}
		chdir(dir.c_str());
	}

	void Decoder::ThreadEntry(void *p)
	{
		#ifdef _WIN32
			((Decoder *) p)->start();
			_endthread();
		#else
			((Decoder *) p)->start();
			pthread_exit(NULL);
		#endif

	}

	void Decoder::startAsThread()
	{
		#ifdef _WIN32
			_beginthread(Decoder::ThreadEntry, 0, this);
		#else
			pthread_create (&_decThread, NULL, (void *) Decoder::ThreadEntry, (void *) this);
		#endif
	};

	bool Decoder::isDone()
	{
		return _done;
	};

	void Decoder::readAndPutSample(ChunkBuffer * cb,GnssMetadata::Stream * s, int i, bool negate)
	{

		int64_t read= cb->readBits(s->Quantization(),s->Encoding());

		//TODO this won't work on negated floating point samples.
		if(negate)
			read = -read;

		//TODO No support for FP-16
		if(cb->wasSampleFloat())
		{
			if(s->Quantization() == 64)
				_decStreamArray[i]->putSample(*(reinterpret_cast<double*>(&read)));
			if(s->Quantization() == 32)
				_decStreamArray[i]->putSample(*(reinterpret_cast<float*>(&read)));

		} else
		{
				_decStreamArray[i]->putSample((read));
		}
	}

	void Decoder::skipLeftPackedBits(GnssMetadata::Stream * stream, ChunkBuffer * cb)
		{
			if(stream->Alignment() == stream->Left)
				cb->skipBits(stream->Packedbits() - stream->Quantization());
		}

	void Decoder::skipRightPackedBits(GnssMetadata::Stream * stream, ChunkBuffer * cb)
		{
			if(stream->Alignment() == stream->Right)
				cb->skipBits(stream->Packedbits() - stream->Quantization());
		}

	std::string Decoder::fileBeingDecoded()
	{
		return _fr->fileBeingDecoded();
	}

	void Decoder::decodeFormattedStream(GnssMetadata::Stream* stream, ChunkBuffer * cb, int i)
		{
			//based on format, skip bits
			switch(stream->Format())
			{

				case stream->IF:

					skipLeftPackedBits(stream,cb);
					readAndPutSample(cb,stream,i,false);
					skipRightPackedBits(stream,cb);
					break;

				case stream->IFn:
					skipLeftPackedBits(stream,cb);
					readAndPutSample(cb,stream,i,true);
					skipRightPackedBits(stream,cb);
					break;

				case stream->IQ:
				case stream->QI:
					skipLeftPackedBits(stream,cb);
					readAndPutSample(cb,stream,i,false);
					skipRightPackedBits(stream,cb);

					skipLeftPackedBits(stream,cb);
					readAndPutSample(cb,stream,i,false);
					skipRightPackedBits(stream,cb);
					break;

				case stream->IQn:
				case stream->QIn:
					skipLeftPackedBits(stream,cb);
					readAndPutSample(cb,stream,i,false);
					skipRightPackedBits(stream,cb);

					skipLeftPackedBits(stream,cb);
					readAndPutSample(cb,stream,i,true);
					skipRightPackedBits(stream,cb);
					break;

				case stream->InQn:
				case stream->QnIn:
					skipLeftPackedBits(stream,cb);
					readAndPutSample(cb,stream,i,true);
					skipRightPackedBits(stream,cb);

					skipLeftPackedBits(stream,cb);
					readAndPutSample(cb,stream,i,true);
					skipRightPackedBits(stream,cb);
					break;

				case stream->InQ:
				case stream->QnI:

					skipLeftPackedBits(stream,cb);
					readAndPutSample(cb,stream,i,true);
					skipRightPackedBits(stream,cb);

					skipLeftPackedBits(stream,cb);
					readAndPutSample(cb,stream,i,false);
					skipRightPackedBits(stream,cb);
					break;

				default:
					//should probably throw instead of print
					printf("Format error");

	}
}

	void Decoder::startAtBlock(uint64_t targetBlock)
	{
		//go from one-indexed to zero indexed
		targetBlock--;
		if(targetBlock == 0)
		{
			return;
		}

		uint64_t bytesToSkip = 0;

		for(int fileNo = 0; fileNo != _mdList->size();)
		{
			//for each block in the file
			for(int block = 0; block != _mdBlockSizesCount->size() && bytesToSkip < _sdrFileSize->at(fileNo); block++)
			{
				targetBlock--;
				bytesToSkip = bytesToSkip + _mdBlockSizes->at(fileNo)[block];

				//we found the target block!
				//TODO Metadata will still not start at the correct block
				if(targetBlock == 0)
				{
					_fr->setStartLocation(fileNo,bytesToSkip);
					return;
				}
			}

			if(bytesToSkip < _sdrFileSize->at(fileNo))
				continue;
			else
				fileNo++;
			//TODO Error if we skipped more bytes than in file.
		}
	}

	int* Decoder::generateBlockSizeArray(GnssMetadata::Lane * l)
	{
		//we need all the block sizes to quickly skip to the correct block
		int * blockSizeArray = new int[l->blockCount];
		int size = 0;
		//for each block
		for(int i = 0; i != l->blockCount; i++)
		{
			Block * b = l->blockArray[i];
			//get the size of the block.
			for(int j = 0; j != b->chunkCount ; j++)
			{
				Chunk* c = b->chunkArray[j];
				size += b->SizeFooter() + b->SizeHeader() + (c->CountWords() * c->SizeWord() * b->Cycles());
			}
			blockSizeArray[i] = size;
		}
		return blockSizeArray;
	}

	DecStream** Decoder::getDecStreamArray(){
		return _decStreamArray;
	}
