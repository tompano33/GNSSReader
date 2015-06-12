/**
 * File: Reader.cpp
 * Author: W.J. Liddy
 * Given a Gnss Metadata file, decodes the streams and outputs them in a buffer.
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
#include "BlockAnalytics.h"
#include "XMLtoMeta.h"

using namespace GnssMetadata;

class GNSSReader {
	//Current metadata and lane object being worked on.
	Metadata md;
	Lane* lane;

	//Helper method that reads chunks from a block. Soon to be removed and replaced with readBlock.
	void readChunkCycles(Metadata md, Block * block, BlockAnalytics* ba, uint32_t cycles, FILE *sdrfile)
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
							//For getting average.
							int8_t read= cb.readBits(packedBitCount,encoding);

							

							//if(stream->Id().compare("fooStream0") == 0)
							//{
							//	ba->putValue(read == 0 ? 1 : -1);
							//} 
							//for single stream
							
							if(ba != NULL)
								ba->putValue(read);
							//TODO: Put it in a band somewhere using bandSrc.
						}
					}
				}
				
			//cb.freeBuffer();
		
			}
		}
	}

public:

	//takes the metadata file given and parses it's XML. Does not yet work with filesets.
	GNSSReader::GNSSReader(const char* directory, const char* metadataFile)
	{
		chdir(directory);
		//TODO check if file exists.
		try
		{
			XMLtoMeta x2m(metadataFile);
			md = x2m.getNonRefdMetadata();
			lane = x2m.getNonRefdLane();
			printSamples();
		} catch (TranslationException e){
			std::cout << "Could not read xml: " <<  e.what();
			//throw exception here
		}
	}

	//Soon to be replaced with start.
	void printSamples(){


		//we need to try to get the first file if it is not found.
		if(md.FileSets().size() > 0 || md.Files().size() != 1)
		{
			printf("Unsupported Format: Spatial/Temporal File");
			//throw exception
		}
	
		//We have a single file for this type of stream.
		File singleFile = md.Files().front();
		std::cout << "The file holding the SDR data is called '" << singleFile.Url().toString() << "'\n";

		FILE *sdrfile = NULL;

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
			

			BlockAnalytics* ba = NULL;
			//comment this out if we are testing speed, not accuracy
			ba = (new BlockAnalytics(1024*1024));

			
			//how many chunk cycles are there?
			uint32_t cycles = block->Cycles();
			//is there a header or footer we need to skip?
			uint32_t headerSize = block->SizeHeader();
			uint32_t footerSize = block->SizeFooter();
			
	
			//skip Header
			fseek(sdrfile,headerSize,SEEK_CUR);
			readChunkCycles(md, block, ba, cycles, sdrfile);
			//skip Footer
			fseek(sdrfile,footerSize,SEEK_CUR);
			if(ba != NULL)
				ba->printMeanAndVar();
		}

		fclose(sdrfile);
	}
};


int main(int argc, char** argv)
{
	
	clock_t tStart = clock();
	try{
		GNSSReader test ("../../../Tests/singlestream","test.xml");
		std::cout << "Done!" << std::endl;
	} catch (std::exception& e) {
		printf(e.what());
	}

	printf("Execution Time: %.2f s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	std::cin.get();

	
	return 0;
}


