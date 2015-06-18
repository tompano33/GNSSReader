/**
 * File: Reader.cpp
 * Author: W.J. Liddy
 * Given a Gnss Metadata file, decodes the streams and outputs them in a DecStreamBuffer.
 */

#include<GnssMetadata/Metadata.h>
#include<list>
#include<cstdint>
#include<stdio.h>

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

using namespace GnssMetadata;
	//Helper method that reads chunks from a block. Soon to be removed and replaced with readBlock.
	void GNSSReader::readChunkCycles(Metadata md, Block * block, uint32_t cycles, FILE *sdrfile)
	{
		for(;cycles != 0; cycles--)
		{
			for(int i = 0; i != block->chunkCount; i++){
				Chunk* chunk = block->chunkArray[i];

				uint8_t sizeWord = chunk->SizeWord();
				uint8_t countWord = chunk->CountWords();
	
				//Is it faster to read a whole block of data than just a bit? Maybe?
				ChunkBuffer cb = ChunkBuffer(sizeWord,countWord,sdrfile);
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
							int8_t read= cb.readBits(packedBitCount,encoding);

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
				
			//cb.freeBuffer();
		
			}
		}
	}
	
	//Returns Decoded Streams.
	DecStream** GNSSReader::getDecStreamArray(){
		return decStreamArray;
	}

	//takes the metadata file given and parses it's XML. Does not yet work with filesets.
	GNSSReader::GNSSReader(const char* directory, const char* metadataFile)
	{
		chdir(directory);
		//TODO check if file exists.
		try
		{
			x2m = new XMLtoMeta(metadataFile);
			md = x2m->getNonRefdMetadata();
			lane = x2m->getNonRefdLane();
		} catch (TranslationException e){
			std::cout << "Could not read xml: " <<  e.what();
			//throw exception here
		}
	}

	//Starts decoding the file into stream(s)
	void GNSSReader::start(){
		//we don't do filesets yet.
		if(md.FileSets().size() > 0 || md.Files().size() != 1)
		{
			printf("Unsupported Format: Spatial/Temporal File");
		}

		File singleFile = md.Files().front();
		std::cout << "The file holding the SDR data is called '" << singleFile.Url().toString() << "'\n";

		FILE *sdrfile = NULL;

		//Convert to WINAPI handle
		//fopen_s?
		if ((sdrfile = fopen(singleFile.Url().Value().c_str(), "rb")) == NULL) // Open the file in binary mode using the "rb" format string
		{
			std::cout << "Error: Could not open data file: "  << singleFile.Url().Value() << std::endl; 
			//throw Exception
		}

		Lane* singleLane = lane;

		//for each lump
		for(int i = 0; i != singleLane->blockCount; i++)
		{

			Block* block = singleLane->blockArray[i];
			
			//how many chunk cycles are there?
			uint32_t cycles = block->Cycles();
			//is there a header or footer we need to skip?
			uint32_t headerSize = block->SizeHeader();
			uint32_t footerSize = block->SizeFooter();
			
	
			//skip Header
			fseek(sdrfile,headerSize,SEEK_CUR);
			readChunkCycles(md, block, cycles, sdrfile);
			//skip Footer
			fseek(sdrfile,footerSize,SEEK_CUR);

			StreamAnalytics sa;
			for(int i = 0; i != decStreamCount; i++)
			{
				sa.setStream(decStreamArray[i]);
				sa.printMeanAndVar();
				decStreamArray[i]->clear();
			}

		}

		//do stream anaylitics if we are testing.


		fclose(sdrfile);
	}

	//Full of magic numbers, improve arbitrary array to vector and decide how outputstreams will work.
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
							//more magic numbers
							decStreamArray[decStreamCount] = new DecStream(100000,s->Id(),s);
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



int main(int argc, char** argv)
{
	
	clock_t tStart = clock();
	try{
		//prepare the file 'singlestream' for reading'
		GNSSReader test ("../../../Tests/singlestream","test.xml");
		test.makeDecStreams();
		test.start();
		std::cout << "Done!" << std::endl;
	} catch (std::exception& e) {
		printf(e.what());
	}

	printf("Execution Time: %.2f s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	std::cin.get();

	
	return 0;
}


