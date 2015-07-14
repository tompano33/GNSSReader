/**
 * File: GnssReader.cpp
 * Author: WJLIDDY
 */
#include "GnssReader.h"

#include<list>
#include<cstdint>
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<sys/stat.h>
//For chdir
//#include <unistd.h>
#include <direct.h>

#include<GnssMetadata/Metadata.h>

#include "ChunkBuffer.h"
#include "StreamAnalytics.h"
#include "XMLtoMeta.h"
#include "DecStream.h"

//To check memleaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>



using namespace GnssMetadata;
	
	void GNSSReader::readChunkCycles(Block * block, uint32_t cycles)
	{
		//for every cycle:
		for(;cycles != 0; cycles--)
		{
			//for every chunk:
			for(int i = 0; i != block->chunkCount; i++)
			{
				

				Chunk* chunk = block->chunkArray[i];

				uint8_t sizeWord = chunk->SizeWord();
				uint8_t countWord = chunk->CountWords();
	
				int chunkBufferSize = sizeWord*countWord;

				char* buf = fr->getBufferedBytes(chunkBufferSize);

				ChunkBuffer cb = ChunkBuffer(chunkBufferSize,buf);


				//while there are still bits left in the chunk:
				while(!cb.chunkFullyRead())
				{					
					//for each lump
					for(int i = 0; i != chunk->lumpCount; i++)
					{

					Lump* lump = chunk->lumpArray[i];

					//skip any padding.
					if( (chunk->Shift() == chunk->Left && chunk->Padding() == chunk->Head) ||
						(chunk->Shift() == chunk->Right && chunk->Padding() == chunk->Tail))
					{
						/** The Padding is at the start */	
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
							for(int i = 0; i != decStreamCount; i++)
							{
								
								if(decStreamArray[i]->getCorrespondingStream() == stream){
									//given a metadata-stream, a chunkbuffer, and an output stream, 
									//puts samples away according to their packedbits/quant/align.
									decodeFormattedStream(stream,&cb,i);
								}

							
							}
						}

						//skip tail padding here!
						if( (chunk->Shift() == chunk->Right && chunk->Padding() == chunk->Head) ||
							(chunk->Shift() == chunk->Left && chunk->Padding() == chunk->Tail) )
						{

							cb.skipBits((8*chunk->SizeWord()) - lump->lumpSize);
						}
					}
				}

				fr->doneReading(chunkBufferSize);
			}
		}
	}
	
	DecStream** GNSSReader::getDecStreamArray(){
		return decStreamArray;
	}

	GNSSReader::GNSSReader(const char* pathToFile, uint64_t readSize, uint64_t buffSize, uint64_t streamSize, uint64_t blockTotal, const char** addlPaths, uint64_t pathCount)
	{
		//total amout of blocks to read, -1 means read all.
		done = false;
		blocksLeftToRead = blockTotal;
		printStats = false;
		printSamples = false;
		mdPtr = 0;
		//to parse XML.
		changeWD(pathToFile);
		std::string fname = std::string(pathToFile,strlen(pathToFile));
		this->streamSize = streamSize;
		
		try
		{

			makeFileList(fname);
			fr = new FileReader(*sdrFileNames,readSize,buffSize,pathToFile,addlPaths,pathCount);

		} catch (TranslationException e)
		{
			//See if file even exists, since x2m does not throw this.
			struct stat buf;
			if(stat(pathToFile, &buf) == -1)
				std::cout<< "Specified metadata file does not exist.\n";
			else
				std::cout << "XML error: " <<  e.what() << "\n";

			throw std::exception("File was not read\n");
		}

	}

	void GNSSReader::makeFileList(std::string pathToFile)
	{

		int totalBlocksDiscovered = 0;
		mdList = new std::vector<Metadata*>;
		sdrFileNames = new std::vector<std::string>;

		//get the metadata of first file.
		XMLtoMeta* x2m = new XMLtoMeta(pathToFile.c_str());
		Metadata* nextMeta = x2m->getNonRefdMetadata();
		
		//add it to the queue
		mdList->push_back(nextMeta);
		sdrFileNames->push_back(nextMeta->Files().front().Url().Value());
 
		//If there is a next file, and there are still blocks to read (or we elect to read infinite)
		while(nextMeta->Files().front().Next().IsDefined() && (totalBlocksDiscovered < blocksLeftToRead || blocksLeftToRead == -1))
		{
			//std::cout << "Adding File to list: " << nextMeta->Files().front().Next().Value() << std::endl;
			
			x2m = new XMLtoMeta((&(nextMeta->Files().front().Next().toString()))->c_str());
			nextMeta = x2m->getNonRefdMetadata();
			int bc = nextMeta->Files().front().nLane->blockCount;
			totalBlocksDiscovered += bc;
			mdList->push_back(nextMeta);
			sdrFileNames->push_back(nextMeta->Files().front().Url().Value());
		}
	}

	//Start decoding the file into stream(s)
	void GNSSReader::start(){

	//	std::cout << getDecStreamArray()[0]->getID();
		fr->readAll();


		Metadata* md;



		//for all metadatas
		while(mdPtr < mdList->size())
		{
			md =  mdList->at(mdPtr++);
		
			repairDecStreams(md);
	
			//we don't do filesets yet, but almost
			if(md->FileSets().size() > 0 || md->Files().size() != 1)
			{
				printf("Unsupported Format: Spatial/Temporal File");
			}
	
			Lane* singleLane =  md->Files().front().nLane;

			for(int i = 0; i != singleLane->blockCount && blocksLeftToRead != 0; i++)
			{
				if(blocksLeftToRead > 0)
					blocksLeftToRead--;

				Block* block = singleLane->blockArray[i];
			
				//how many chunk cycles are there?
				uint32_t cycles = block->Cycles();
				//is there a header or footer we need to skip?

				uint32_t headerSize = block->SizeHeader();
				//std::cout << "\n" << headerSize << "\n";
				uint32_t footerSize = block->SizeFooter();
			
				fr->skipBufferedBytes(headerSize);
				readChunkCycles(block, cycles);
				fr->skipBufferedBytes(footerSize);
				
				//TODO enable testmode
				
				
				StreamAnalytics sa;
				for(int i = 0; i != decStreamCount; i++)
				{
					if(decStreamArray[i]->getSamplePtr() != 0)
					{
						sa.setStream(decStreamArray[i]);
					
						if(printSamples)
							sa.printAllSamples();
						if(printStats)
							sa.printMeanAndVar();
						if(printSamples || printStats)
							decStreamArray[i]->clear();
					}
				}
				
			}
		}

		//done! see if there are any bytes in the DecStream. If there are, print a warning but kill the process anyhow.
		if(!fr->hasReadWholeFile())
		{
			std::cout << "Warning: Some samples did not get read! In Buffer: " << fr->numBytesLeftInBuffer() << " In File: " << fr->numBytesLeftToReadFromFile() << "\n" ;
			fr->killReadThread();
		}

		done = true;
	}

	//TODO Improve arbitrary array to vector.
	//Prepares decoded streams.
	//TODO If lumpsize changes things break
	void GNSSReader::makeDecStreams(){

		//TODO use a vector or something
		decStreamCount = 0;
		//TODO magic number!
		decStreamArray = new DecStream*[256];
		//i need to read how many streams there are, and also, data about them. 
		//how can I get that?
		//well, I need to iterate through every xml element and count the num of streams there are
		//I need to be careful to avoid duplicates...

		Lane* singleLane = mdList->at(0)->Files().front().nLane;

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
						//we got a stream! Make sure it's not a duplicate

						//TODO modifiying this does nothing? design flaw?
						l->lumpSize += s->Packedbits();
						//extra samples included w/ complex data
						if(!(s->Format() == s->IF || s->Format() == s->IFn))	
							l->lumpSize += s->Packedbits();

						bool newStream = true;

						for(int c = 0; newStream &&  c != decStreamCount; c++){	

							if(s == decStreamArray[c]->getCorrespondingStream())
									newStream = false;
						}

						if(newStream)
						{
							//record if it in complex, and if so, what value comes first.
							decStreamArray[decStreamCount] = new DecStream(streamSize,s->Id(),s,!((s->Format() == s->IF) || (s->Format() == s->IFn)),
								(s->Format() == s->QI) || (s->Format() == s->QnIn) || (s->Format() == s->QIn) || (s->Format() == s->QnI));
							decStreamCount++;
						}
					}
				}
			}
		}

	}

	uint64_t GNSSReader::getDecStreamCount(){
		return decStreamCount;
	}

	GNSSReader::~GNSSReader(){
	//	delete fr;
	//	for(int i = 0; i != decStreamCount; i++)
	//	{
			
	//		delete decStreamArray[i];
	//	}
	//	delete [] decStreamArray;
	}
	
	//When we load a new XML file, we must assoicate the already-open streams with those found in the file.
	void GNSSReader::repairDecStreams(Metadata* md)
	{

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
				
						
						l->lumpSize += s->Packedbits();

						//duplicated
						if(!(s->Format() == s->IF || s->Format() == s->IFn))	
							l->lumpSize += s->Packedbits();

						bool streamReassigned = false;
						//we got a stream! Match it with a predefined one, based on the ID.
						for(int c = 0; c != decStreamCount; c++){	
							if(s->Id().compare(decStreamArray[c]->getCorrespondingStream()->Id()) == 0)
							{
								decStreamArray[c]->setCorrespondingStream(s);
								streamReassigned = true;
								break;
							}
						}

					}
				}
			}
		}

	}
	
	void GNSSReader::setPrintOptions(bool stat, bool samp)
	{
		printSamples = samp;
		printStats = stat;
	}

	void GNSSReader::changeWD(const char* pathToFile)
	{
		
		std::string fname = std::string(pathToFile,strlen(pathToFile));
		std::string dir;
		const size_t last_slash_idx = fname.rfind('\\');
		if (std::string::npos != last_slash_idx)
		{
			 dir = fname.substr(0, last_slash_idx);
		}
		chdir(dir.c_str());
	}
	
	void GNSSReader::ThreadEntry(void *p)
	{
		((GNSSReader *) p)->start(); 
		_endthread();
	}

	void GNSSReader::startAsThread()
	{
		_beginthread(GNSSReader::ThreadEntry, 0, this);
	};

	
	bool GNSSReader::isDone()
	{
		return done;
	};

	void GNSSReader::readAndPutSample(ChunkBuffer * cb,GnssMetadata::Stream * s, int i, bool negate)
	{
		
		int64_t read= cb->readBits(s->Quantization(),s->Encoding());	

		//This should not work?
		if(negate) 
			read = -read;

		if(cb->wasSampleFloat())
		{
		//need to convert read here! putSample takes a double.
			if(s->Quantization() == 64)
				decStreamArray[i]->putSample(*(reinterpret_cast<double*>(&read)));
			if(s->Quantization() == 32)
				decStreamArray[i]->putSample(*(reinterpret_cast<float*>(&read)));

		} else
		{
				decStreamArray[i]->putSample((read));
		}
	}

		void GNSSReader::skipLeftPackedBits(GnssMetadata::Stream * stream, ChunkBuffer * cb)
		{
			if(stream->Alignment() == stream->Left)
				cb->skipBits(stream->Packedbits() - stream->Quantization());
		}

		void GNSSReader::skipRightPackedBits(GnssMetadata::Stream * stream, ChunkBuffer * cb)
		{
			if(stream->Alignment() == stream->Right)
				cb->skipBits(stream->Packedbits() - stream->Quantization());
		}

		void GNSSReader::decodeFormattedStream(GnssMetadata::Stream* stream, ChunkBuffer * cb, int i)
		{
			
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
					printf("Format error");

			}
		}
	
