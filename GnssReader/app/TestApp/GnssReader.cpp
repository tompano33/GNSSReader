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
		for(;cycles != 0; cycles--)
		{
			for(int i = 0; i != block->chunkCount; i++)
			{
				Chunk* chunk = block->chunkArray[i];

				uint8_t sizeWord = chunk->SizeWord();
				uint8_t countWord = chunk->CountWords();
	
				int chunkBufferSize = sizeWord*countWord;

				//I can be smarter about allocating this buffer. It only needs realloc'd if the word size changes.
				char* buf = new char[chunkBufferSize];
				fr->getBufferedBytes(buf,chunkBufferSize);

				ChunkBuffer cb = ChunkBuffer(chunkBufferSize,buf);
				//chunk->Endian(),chunk->Padding(),chunk->Shift());

				while(!cb.chunkFullyRead())
				{					
					//for each lump
					for(int i = 0; i != chunk->lumpCount; i++)
					{
					Lump* lump = chunk->lumpArray[i];



					if( (chunk->Shift() == chunk->Left && chunk->Padding() == chunk->Head) ||
						(chunk->Shift() == chunk->Right && chunk->Padding() == chunk->Tail))
					{
						/** The Padding is at the start */	
						//std::cout << "Skipping! H" << std::endl;
						cb.skipBits((8*chunk->SizeWord()) - lump->lumpSize);
					}

						//for each stream
						for(int i = 0; i != lump->streamCount; i++)
						{
							Stream* stream;
							//If RIGHT-SHIFTED read the last stream first.
							if(chunk->Shift() == chunk->Right)
								stream = lump->streamArray[(lump->streamCount - 1) - i];
							else
								stream = lump->streamArray[i];

							int8_t packedBitCount = stream->Packedbits();

							char *encoding = &stream->Encoding().front();
							int64_t read= cb.readBits(packedBitCount,encoding);

							//find the correct decStream based on address
							for(int i = 0; i != decStreamCount; i++)
							{
								if(decStreamArray[i]->getCorrespondingStream() == stream){
									//special case if one bit
									if(packedBitCount == 1)
										decStreamArray[i]->putSample(read == 0 ? 1.0 : -1.0);
									else 
										decStreamArray[i]->putSample(read);
									break;
								}
							}
						}

					//skip tail padding here!
					if( (chunk->Shift() == chunk->Right && chunk->Padding() == chunk->Head) ||
						(chunk->Shift() == chunk->Left && chunk->Padding() == chunk->Tail) )
					{
						/** The Padding is at the start */
					//	std::cout << "Skipping!! T" << std::endl;
						cb.skipBits((8*chunk->SizeWord()) - lump->lumpSize);
					}
					}
				}
			}
		}
	}
	
	DecStream** GNSSReader::getDecStreamArray(){
		return decStreamArray;
	}

	GNSSReader::GNSSReader(const char* pathToFile, uint64_t readSize, uint64_t buffSize, uint64_t streamSize, uint64_t blockTotal, const char** addlPaths, uint64_t pathCount)
	{
		//total amout of blocks to read, -1 means read all.
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
			std::cout << "Adding File to list: " << nextMeta->Files().front().Next().Value() << std::endl;
			
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
		//start the file reader thread. 
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

				StreamAnalytics sa;
				for(int i = 0; i != decStreamCount; i++)
				{
					sa.setStream(decStreamArray[i]);
				
					if(printSamples)
						sa.printAllSamples();
					if(printStats)
						sa.printMeanAndVar();

					decStreamArray[i]->clear();
				}

			}
		}

		//done! see if there are any bytes in the DecStream. If there are, print a warning but kill the process anyhow.
		if(!fr->hasReadWholeFile())
		{
			std::cout << "Warning: Some samples did not get read! In Buffer: " << fr->numBytesLeftInBuffer() << " In File: " << fr->numBytesLeftToReadFromFile() << "\n" ;
			fr->killReadThread();
		}
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

						l->lumpSize += s->Packedbits();

						bool newStream = true;

						for(int c = 0; newStream &&  c != decStreamCount; c++){	
							if(s == decStreamArray[c]->getCorrespondingStream())
									newStream = false;
						}

						if(newStream)
						{
							decStreamArray[decStreamCount] = new DecStream(streamSize,s->Id(),s);
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
		delete fr;
		for(int i = 0; i != decStreamCount; i++)
		{
			delete decStreamArray[i];
		}
		delete [] decStreamArray;
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

