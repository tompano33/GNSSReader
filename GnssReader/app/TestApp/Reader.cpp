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
	
				int chunkBufferSize = sizeWord*countWord;
				char* buf = new char[chunkBufferSize];
				fr->getBufferedBytes(buf,chunkBufferSize);

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
							int64_t read= cb.readBits(packedBitCount,encoding);

							//Use Function Pointers instead of if here.

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
	GNSSReader::GNSSReader(const char* pathToFile, long readSize, long buffSize, long streamSize,const char** addlPaths, int blockTotal)
	{
		//total amout of blocks to read, -1 means read all.
		blocksLeftToRead = blockTotal;
		printStats = false;
		printSamples = false;

		//changes working directory so we can access the first file.
		std::string* fname = new std::string(pathToFile,strlen(pathToFile));
		std::string dir;
		const size_t last_slash_idx = fname->rfind('\\');
		if (std::string::npos != last_slash_idx)
		{
			 dir = fname->substr(0, last_slash_idx);
		}
		chdir(dir.c_str());

		//sets outputstream size
		this->streamSize = streamSize;
		
		try
		{
			//mm.. filereader needs to go to a list. XML does too.
			XMLtoMeta* x2m = new XMLtoMeta(pathToFile);
			md = x2m->getNonRefdMetadata();




			//Windows filereader
			std::string s = md->Files().front().Url().toString();
			std::cout << "Opening: " << s << "\n";

			std::wstring stemp = std::wstring(s.begin(), s.end());
			LPCWSTR wfname = stemp.c_str();
			
			makeFileList(fname);
			fr = new FileReader(*sdrFileNames,readSize,buffSize);


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

	void GNSSReader::makeFileList(std::string* pathToFile)
	{
		int totalBlocksDiscovered = 0;
		mdList = new std::vector<Metadata*>;
		sdrFileNames = new std::vector<std::string>;

		//get the metadata of first file.
		XMLtoMeta* x2m = new XMLtoMeta(pathToFile->c_str());
		Metadata* nextMeta = x2m->getNonRefdMetadata();

		sdrFileNames->push_back(nextMeta->Files().front().Url().Value());
 
		while(nextMeta->Files().front().Next().IsDefined() && (totalBlocksDiscovered < blocksLeftToRead || blocksLeftToRead == -1))
		{
			std::cout << "Adding File to list: " << nextMeta->Files().front().Next().Value() << std::endl;
			
			x2m = new XMLtoMeta((&(nextMeta->Files().front().Next().toString()))->c_str());
			nextMeta = x2m->getNonRefdMetadata();
			
			int bc = nextMeta->Files().front().nLane->blockCount;
			totalBlocksDiscovered += bc;
			mdList->push_back(nextMeta);

		}



	}

	//Start decoding the file into stream(s)
	void GNSSReader::start(){
		//start the file reader thread. 
		fr->readAll();
		//we don't do filesets yet.
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

		//done! see if there are any bytes in the DecStream. If there are, print a warning but kill the process anyhow.
		if(!fr->hasReadWholeFile())
		{
			std::cout << "Warning: Some samples did not get read! In Buffer: " << fr->numBytesLeftInBuffer() << " In File: " << fr->numBytesLeftToReadFromFile() << "\n" ;
			fr->killReadThread();
		}
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

		//Fun fact: this returns a shallow instance of lane
		//does not have correct reference
		//so we use nLane.
		Lane* singleLane = md->Files().front().nLane;


		for(int i = 0; i != singleLane->blockCount; i++){
			Block* b = singleLane->blockArray[i];
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
	}


//tests everything by flooding the console. yay.
void testSuite()
{
	/**
	try{
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\exception\\test.xml",100000L,200000L,1000000L);
		test.makeDecStreams();
		test.start();
	} catch (std::exception& e) {
		printf(e.what());
	}
	*/

	try{
		
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\header\\test.xml",100000L,200000L,1000000L,NULL);
		test.makeDecStreams();
		test.printSamples = true;
		test.start();
		
		GNSSReader test2 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\temporalforeach\\test.xml",100000L,200000L,1000000L);
		test2.makeDecStreams();
		test2.printSamples = true;
		test2.start();
		/**
		GNSSReader test3 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sine\\test.xml",100000L,200000L,1000000L);
		test3.makeDecStreams();
		test3.printStats = true;
		test3.start();

		GNSSReader test4 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\singleStream\\test.xml",100000L,200000L,1000000L);
		test4.makeDecStreams();
		test4.printStats = true;
		test4.start();
		*/
	} catch (std::exception& e) {
			printf(e.what());
	}


}

int main(int argc, char** argv)
{
	clock_t tStart = clock();
	testSuite();
	printf("Execution Time: %.2f s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	_CrtDumpMemoryLeaks();
	std::cin.get();
	return 0;
}


