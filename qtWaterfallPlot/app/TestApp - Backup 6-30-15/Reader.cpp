/**
 * File: Reader.cpp
 * Author: W.J. Liddy
 * Given a Gnss Metadata file, decodes the streams and outputs them in a DecStreamBuffer.
 */
#include<GnssMetadata/Metadata.h>
#include<list>
#include<cstdint>
#include<stdio.h>
#include <sys/stat.h>

//ChDir
//#include <unistd.h>
#include <direct.h>

//Helps me debug:
#include <time.h>
#include<iostream>

#include "ChunkBuffer.h"
#include "StreamAnalytics.h"
#include "XMLtoMeta.h"
#include "DecStream.h"
#include "Reader.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace GnssMetadata;
	
//Helper method that reads chunks from a block. Soon to be removed and replaced with readBlock.
	void GNSSReader::readChunkCycles(Block * block, uint32_t cycles)
	{
		for(;cycles != 0; cycles--)
		{
			for(int i = 0; i != block->chunkCount; i++){
				Chunk* chunk = block->chunkArray[i];

				uint8_t sizeWord = chunk->SizeWord();
				uint8_t countWord = chunk->CountWords();
	
				//Is it faster to read a whole block of data than just a bit? Maybe?
				int chunkBufferSize = sizeWord*countWord;
				char* buf = new char[chunkBufferSize];
				fr->getBufferedBytes(buf,chunkBufferSize);
				//std::cout << "Loaded a chunk of size " << chunkBufferSize << "\n";

				ChunkBuffer cb = ChunkBuffer(chunkBufferSize,buf);
					//chunk->Endian(),chunk->Padding(),chunk->Shift());

				//TODO What if lump runs out of words?
				//TODO does not account for padding or shift (though that is chunkbuffers job)
				while(!cb.chunkFullyRead())
				{					
					//for each lump
					for(int i = 0; i != chunk->lumpCount; i++)
					{
					Lump* lump = chunk->lumpArray[i];

						//for each stream
						for(int i = 0; i != lump->streamCount; i++)
						{
							Stream* stream = lump->streamArray[i];

							//TODO get storage type
							int8_t packedBitCount = stream->Packedbits();

							//stream has multiple encodings?
							//why doesn't the author use enums instead of chars? oh well whatever.
							char *encoding = &stream->Encoding().front();
							//--------------------------------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
							//TODO fix this, it's not fliexible.
							int16_t read= cb.readBits(packedBitCount,encoding);

							//Use Function Pointers instead of if here.

							for(int i = 0; i != decStreamCount; i++)
							{
								if(decStreamArray[i]->getCorrespondingStream() == stream){
									//This is sloppy. I will need this to work for 'N' bits.
									//but for 1 or 8 it will work. We need 'DecStreams' to decide.
									if(packedBitCount == 1)
										decStreamArray[i]->putSample(read == 0 ? 1 : -1);
									else 
										decStreamArray[i]->putSample(read);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	
	//Returns Decoded Streams.
	DecStream** GNSSReader::getDecStreamArray(){
		return decStreamArray;
	}

	//takes the metadata file given and parses it's XML. Does not yet work with filesets.
	//TODO clean up this constructor.
	GNSSReader::GNSSReader(const char* pathToFile, long readSize, long buffSize, long streamSize)
	{

		std::string* fname = new std::string(pathToFile,strlen(pathToFile));
		//get WD
		std::string dir;
        const size_t last_slash_idx = fname->rfind('/');
		if (std::string::npos != last_slash_idx)
		{
			 dir = fname->substr(0, last_slash_idx);
		}
		//std::cout << "Wdir is now " << dir << std::endl;
        chdir(dir.c_str());

		//chdir();
		this->streamSize = streamSize;
		//TODO check if file exists.
		try
		{
			x2m = new XMLtoMeta(pathToFile);
			md = x2m->getNonRefdMetadata();
			lane = x2m->getNonRefdLane();
			//Windows filereader
			std::string s = md.Files().front().Url().toString();
			std::cout << "Opening: " << s << "\n";

			std::wstring stemp = std::wstring(s.begin(), s.end());
			LPCWSTR fname = stemp.c_str();
			fr = new FileReader(fname,readSize,buffSize);

		} catch (TranslationException e){
			//See if file even exists, since x2m does not throw this.
			struct stat buf;
			if(stat(pathToFile, &buf) == -1)
				std::cout<< "Specified metadata file does not exist.\n";
			else
				std::cout << "XML error: " <<  e.what() << "\n";

			throw std::exception("File was not read\n");
		}
	}

	//Starts decoding the file into stream(s)
	void GNSSReader::start(){
		//start the file reader thread
		fr->readAll();
		//we don't do filesets yet.
		if(md.FileSets().size() > 0 || md.Files().size() != 1)
		{
			printf("Unsupported Format: Spatial/Temporal File");
		}
		
		File singleFile = md.Files().front();
		//TODO open the windows handle here

		Lane* singleLane = lane;

		for(int i = 0; i != singleLane->blockCount; i++)
		{

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
			//	sa.printAllSamples();
			//	sa.printMeanAndVar();
			}

		}

		//done! see if there are any bytes in the DecStream. If there are, print a warning but kill the process anyhow.
		if(!fr->hasReadWholeFile())
		{
			std::cout << "Warning: Some samples did not get read! In Buffer: " << fr->numBytesLeftInBuffer() << " In File: " << fr->numBytesLeftToReadFromFile() << "\n" ;
			fr->killReadThread();
		}
        // CLOSE FILE
        fr->close();
	}

	//TODO Improve arbitrary array to vector.
	//Prepares decoded streams.
	void GNSSReader::makeDecStreams(){
		//TODO use a vector or something
		decStreamCount = 0;
		//TODO magic number!
		decStreamArray = new DecStream*[256];
		//i need to read how many streams there are, and also, data about them. 
		//how can I get that?
		//well, I need to iterate through every xml element and count the num of streams there are
		//I need to be careful to avoid duplicates...
		for(int i = 0; i != lane->blockCount; i++){
			Block* b = lane->blockArray[i];
			for(int j = 0; j != b->chunkCount;j++){
				Chunk* c = b->chunkArray[j];
				for(int k = 0; k!= c->lumpCount;k++){
					Lump* l = c->lumpArray[k];
					for(int i2  = 0; i2 != l->streamCount;i2++)
					{
						Stream* s = l->streamArray[i2];
						//we got a new stream! Make sure it's not a dupe
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

	int GNSSReader::getDecStreamCount(){
		return decStreamCount;
	}

	GNSSReader::~GNSSReader(){
        delete fr;
        for(int i = 0; i != decStreamCount; i++)
        {
            delete decStreamArray[i];
        }
        delete [] decStreamArray;
        delete x2m;
	}


//int main(int argc, char** argv)
//{
//	{

//		clock_t tStart = clock();
//		try{
//			//prepare the file 'singlestream' for reading'
//			//Input size, intermediate buffer size, output size
//			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sine\\test.xml",100000L,200000L,1000000L);
//			test.makeDecStreams();
//			test.start();
//			std::cout << "Done!" << std::endl;
//		} catch (std::exception& e) {
//			printf(e.what());
//		}

//		printf("Execution Time: %.2f s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//	}

//	_CrtDumpMemoryLeaks();
//	std::cin.get();

	
//	return 0;
//}


