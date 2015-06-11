/**
 * File: Reader.cpp
 * Author: W.J. Liddy
 * Reads an SDR Metadata file and pulls out SDR data to create waterfall plot
 *
 * TODO: Further break code down into functions.
 * TODO: Deal with different encodings
 * TODO: Compile on Linux.
 */
#include<GnssMetadata/Metadata.h>
#include<GnssMetadata/Xml/XmlProcessor.h>
#include<list>
#include<cstdint>
#include<stdio.h>

//test code speed
#include <time.h>

//Preferred over printf, will delete on release.
#include<iostream>

//ChDir for Linux:
//#include <unistd.h>

//ChDir for Windows:
#include <direct.h>

#include "ChunkBuffer.h"
#include "BlockAnalytics.h"

using namespace GnssMetadata;

//If there are zero declarations of an XML object, this error is thrown.
class BadID: public std::exception
{
  char* badIDName;

  virtual const char* what() const throw()
  {
    return badIDName;
  }

public:
	void setBadIDName(char * name)
	{
		badIDName = name;
	}
};


class GNSSReader {
	Metadata md;

	//Reads XML files as Metadata object. returns a metadata object.
	Metadata ReadXmlFile(const char* pszFilename)
	{
		Metadata md;
		XmlProcessor xproc;
		if( xproc.Load( pszFilename, false, md) )
		{
			printf("Xml Processed successfully. \n");
			return md;
		}
	}

	//Converts lists contaning referenced objects to an array without referenced objects. Run this before doing any XML operations!
	//It fixes references and increases speed.
	template<typename T, typename PT> void fixRefdObjs(Metadata* md,std::list<T, std::allocator<T>>* objList, T** objArray){

		int i = 0;
		for(std::list<T>::iterator iter = objList->begin();
			iter !=objList->end();iter++)
		{
			//The object is a reference, so we are going to need to search to find the original object.
			if(iter->IsReference())
			{
				AttributedObject::SearchItem::List returnList;
				int objectsFound = md->FindObject(returnList,iter->Id(),*md);
				if(objectsFound == 0)
				{
					//no matching object found!
					BadID e ;
					std::string badIDName = iter->Id().c_str();
					char * badIdc = new char[badIDName.size() + 1];
					std::copy(badIDName.begin(), badIDName.end(), badIdc);
					badIdc[badIDName.size()] = '\0'; 
					e.setBadIDName(badIdc);
					throw e;
				}
				//Got an non-referenced object! now, put it in the array.
				const AttributedObject* foundObject = (returnList.front().pObject);
				AttributedObject* foundObjectNoConst = const_cast<AttributedObject*>(foundObject);	
				objArray[i] = dynamic_cast<PT>(foundObjectNoConst);
			} else {
				objArray[i] = &*iter;
			}
			i++;
		}
	}

	//Fixes a single XML Link (not for lists).
	template<typename T> T findNonRefObj(Metadata* md, AttributedObject* obj)
	{
		if(obj->IsReference())
		{
			//The object is a reference, so we are going to need to search to find the original object.
			AttributedObject::SearchItem::List returnList;
			int objectsFound = md->FindObject(returnList,obj->Id(),*md);
			if(objectsFound == 0)
			{
				BadID e ;
				std::string badIDName = obj->Id().c_str();
				char * badIdc = new char[badIDName.size() + 1];
				std::copy(badIDName.begin(), badIDName.end(), badIdc);
				badIdc[badIDName.size()] = '\0'; // don't forget the terminating 
				e.setBadIDName(badIdc);
				throw e;
			}
		
			const AttributedObject* foundObject = (returnList.front().pObject);
			AttributedObject* foundObjectNoConst = const_cast<AttributedObject*>(foundObject);
			return dynamic_cast<T>(foundObjectNoConst);
		} else {
			//The object wasn't a reference, meaning it was the original.
			return dynamic_cast<T>(obj);
		}
	}

	//gets rid of referenced objects for all XML.
	void fixAllRefdObjs(){
		File singleFile = md.Files().front();
		Lane* singleLane = findNonRefObj<Lane*>(&md,&singleFile.Lane());

		//first thing: init block array.
		singleLane->blockCount = singleLane->Blocks().size();
		singleLane->blockArray = new Block*[singleLane->blockCount];

		fixRefdObjs<Block,Block*>(&md,&singleLane->Blocks(),singleLane->blockArray);

		for(int i = 0; i != singleLane->blockCount; i++)
		{
			Block* block = singleLane->blockArray[i];
			//init this block's chunks.
			block->chunkCount = block->Chunks().size();
			block->chunkArray = new Chunk*[block->chunkCount];
			fixRefdObjs<Chunk,Chunk*>(&md,&block->Chunks(),block->chunkArray); 

			for(int i = 0; i != block->chunkCount; i++){
				Chunk* chunk = block->chunkArray[i];
				//init this chunk's lumps.
				chunk->lumpCount = chunk->Lumps().size();
				chunk->lumpArray = new Lump*[chunk->lumpCount];
				//then lump array...
				fixRefdObjs<Lump,Lump*>(&md,&chunk->Lumps(),chunk->lumpArray); 

					for(int i = 0; i != chunk->lumpCount; i++)
					{
					Lump* lump = chunk->lumpArray[i];
					lump->streamCount = lump->Streams().size();
					lump->streamArray = new Stream*[lump->streamCount];
					//finally, streams
					fixRefdObjs<Stream,Stream*>(&md,&lump->Streams(),lump->streamArray); 
				}
			}
		}
	}
							
	//Helper method that reads chunks from a block.
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

							

							if(stream->Id().compare("fooStream0") == 0)
							{
								ba->putValue(read == 0 ? 1 : -1);
							} 
							//for single stream
							
							//if(ba != NULL)
							//	ba->putValue(read);
							//TODO: Put it in a band somewhere using bandSrc.
						}
					}
				}
				
			//cb.freeBuffer();
		
			}
		}
	}

public:
	//takes the metadata file and parses it's XML.

	GNSSReader::GNSSReader(const char* directory, const char* metadataFile)
	{
		//TODO: More elegantly set the Working Directory
		chdir(directory);
		//TODO check if file exists.
		try
		{
			md = ReadXmlFile(metadataFile);
		} catch (TranslationException e){
			std::cout << "Could not read xml: " <<  e.what();
			//throw exception here
		}
	}

	void printSamples(){


		//we need to try to get the first file if it is not found.
		if(md.FileSets().size() > 0 || md.Files().size() != 1)
		{
			printf("Unsupported Format: Spatial/Temporal File");
			std::cin.get();
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
			std::cin.get();
			//throw Exception
		}

		//Every file has just one lane.
		Lane* singleLane = findNonRefObj<Lane*>(&md,&singleFile.Lane());

		fixAllRefdObjs();

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
		GNSSReader test("../../../Tests/multistream","test.xml"); 
		test.printSamples();
		std::cout << "Done!" << std::endl;
	} catch (std::exception& e) {
		printf(e.what());
	}

	printf("Execution Time: %.2f s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	std::cin.get();

	
	return 0;
}


