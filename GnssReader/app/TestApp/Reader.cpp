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

	//TODO: remove this flag. find better way to fix bad references.
	bool objWasRef;

	//helper funtion to read XML. returns a metadata object.
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

	//Given a reference in a list, search through all XML items for the original object.
	//sets the 'objwasref' flag to true if it was a reference and it was repaired.
	//if objwasref, the item needs deleted from the list.
	template<typename T, typename U> T findNonRefObj(Metadata* md, AttributedObject* obj,std::list<U, std::allocator<U>>* objList)
	{
		if(obj->IsReference())
		{
			//The object is a reference, so we are going to need to search to find the original object.
			AttributedObject::SearchItem::List returnList;
			int objectsFound = md->FindObject(returnList,obj->Id(),*md);
			if(objectsFound == 0)
			{
				//no matching XML found!
				BadID e ;
				std::string badIDName = obj->Id().c_str();
				char * badIdc = new char[badIDName.size() + 1];
				std::copy(badIDName.begin(), badIDName.end(), badIdc);
				badIdc[badIDName.size()] = '\0'; 
				e.setBadIDName(badIdc);
				throw e;
			}
			//Got an non-referenced object! now, put it on the list.
			//TODO: don't tell objWasRef to get rid of it, do it here.
			const AttributedObject* foundObject = (returnList.front().pObject);
			AttributedObject* foundObjectNoConst = const_cast<AttributedObject*>(foundObject);			
			objList->push_front(*dynamic_cast<T>(foundObjectNoConst));
			objWasRef = true;
			return dynamic_cast<T>(foundObjectNoConst);
			
		} else {
			//The object wasn't a reference, meaning it was the original.
			objWasRef = false;
			return dynamic_cast<T>(obj);
		}
	}

	//this version does not try to make a reference.
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

	//Helper method that reads chunks from a block.
	void readChunkCycles(Metadata md, Block * block, BlockAnalytics* ba, uint32_t cycles, FILE *sdrfile)
	{
		for(;cycles != 0; cycles--)
		{
			for(std::list<Chunk>::iterator chunkiter = block->Chunks().begin(), chunknext;
			chunkiter != block->Chunks().end(); chunkiter = chunknext)
			{
				chunknext = chunkiter;
				++chunknext;

				Chunk* chunk = findNonRefObj<Chunk*,Chunk>(&md,&*chunkiter,&block->_chunklist);  
		
				if(objWasRef)
					block->_chunklist.erase(chunkiter);

				uint8_t sizeWord = chunk->SizeWord();
				uint8_t countWord = chunk->CountWords();
	
				ChunkBuffer cb = ChunkBuffer(sizeWord,countWord,sdrfile);
					//chunk->Endian(),chunk->Padding(),chunk->Shift());

				//TODO What if lump runs out of words?
				//TODO does not account for padding or shift
				while(!cb.chunkFullyRead())
				{					
					
					//for each lump
					for(std::list<Lump>::iterator lumpiter = chunk->Lumps().begin(), lumpnext;
					lumpiter != chunk->Lumps().end(); lumpiter = lumpnext)
					{
						lumpnext = lumpiter;
						++lumpnext;

						Lump* lump = findNonRefObj<Lump*,Lump>(&md,&*lumpiter,&chunk->_lumplist);  
					
						if(objWasRef)
							chunk->_lumplist.erase(lumpiter);

						//for each stream
						for(std::list<Stream>::iterator streamiter = lump->Streams().begin(), streamnext;
							streamiter != lump->Streams().end(); streamiter = streamnext)
							{
							streamnext = streamiter;
							++streamnext;
							
							Stream* stream = findNonRefObj<Stream*,Stream>(&md,&*streamiter,&lump->_streamlist);
						
							if(objWasRef)
								lump->_streamlist.erase(streamiter);

							//TODO get storage type
							int8_t packedBitCount = stream->Packedbits();
							//stream has multiple encodings?
							//why doesn't the author use enums instead of chars? oh well whatever.

							char *encoding = &stream->Encoding().front();
							//For getting average.
							int8_t read= cb.readBits(packedBitCount,encoding);

							//for the multi-stream test.

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


		for(std::list<Block>::iterator iter = singleLane->Blocks().begin(), next;
			iter != singleLane->Blocks().end(); iter = next)
		{
			next = iter;
			++next;
			
			Block* block = findNonRefObj<Block*,Block>(&md,&*iter,&singleLane->Blocks());  
			if(objWasRef)
				singleLane->_blocklist.erase(iter);
			

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
		GNSSReader test("../../../Tests/singlestream","test.xml"); 
		test.printSamples();
		std::cout << "Done!" << std::endl;
	} catch (std::exception& e) {
		printf(e.what());
	}

	printf("Execution Time: %.2f s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	std::cin.get();
	return 0;
}


