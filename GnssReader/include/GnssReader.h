/**
 * File: GnssReader.h
 * Author: WJLIDDY
 * Description: Using XML metadata files and associated source files, puts samples in output buffers.
 */
#ifndef GNSSREADER_H_H
#define GNSSREADER_H_H

#include<list>
#include<cstdint>
#include <vector>
#include<GnssMetadata/Metadata.h>
#include "DecStream.h"
#include "FileReader.h"
#include "ChunkBuffer.h"
using namespace GnssMetadata;

class GNSSReader {

	//List of all metadata XML files converted to Metadata objects
	std::vector<Metadata*>* mdList;  
	//List of lengths of each block size for each metadata. Needed for fast skipping to an exact block.
	std::vector<int*>* mdBlockSizes; 
	//Count of block sizes in mdBlockSizes
	std::vector<int>* mdBlockSizesCount;  

	//Pointer to next target metadata object to decode.
	unsigned int mdPtr;

	//List of all SDR file names to decode.
	std::vector<std::string>* sdrFileNames; 
	//Lengths of SDR files in sdrFileNames.
	std::vector<uint64_t>* sdrFileSize; 

	//Array of Decoded Sample Output Streams
	DecStream** decStreamArray; 
	//The count of these streams.
	uint64_t decStreamCount; 

	//The size of an output stream.
	uint64_t streamSize; 
	//File reader, which reads an sdr file and puts it in a buffer.
	FileReader* fr; 

	//Given a block, puts it in the correct DecStreams
	void readChunkCycles(Block * block, uint32_t cycles); 

	//Count of total blocks the caller wants to read.
	uint64_t blocksLeftToRead; 

	//If we want to print statistics as we decode blocks, then one or both of these will be true.
	//Should not be in release version because these clear the decodedStreams.
	bool printStats, printSamples; 

	//When we load in a new Metadata, we must associate the already-open streams with those found in the file.
	void repairDecStreams(Metadata* md); 

	//Given the name of the first XML file, crawls through all the other XML files to build mdList, mdBlockSizes, mdBlockSizesCount, sdrFileList.
	void makeFileList(std::string first);

	//Helper function to start a thread of a class method at runtime.
	static void ThreadEntry(void *p);

	//Reads and puts a sample from a chunkbuffer. Helper.
	void readAndPutSample(ChunkBuffer * cb, Stream* s, int i, bool negate);

	//Helpers for readChunkCycles
	void skipLeftPackedBits(GnssMetadata::Stream * stream, ChunkBuffer * cb);
	void skipRightPackedBits(GnssMetadata::Stream * stream, ChunkBuffer * cb);
	//'i' represent the target stream in DecStreamArray
	void decodeFormattedStream(GnssMetadata::Stream * stream, ChunkBuffer * cb, int i);

	//flag if there are no more samples to read.
	bool done;

	//Starts the reader, but not as a thread.
	void start();

	//get sizes of all sdr files. Used if we need to skip to a certain block.
	void fetchFileSizes();

	//From a lane, generates the size of all of the blocks in the lane. Needed for skipping.
	int* generateBlockSizeArray(GnssMetadata::Lane*);

	bool TRIGRmode;
	bool TRIGRlastFooterValSet;
	uint64_t TRIGRmask ;//= 0x00FFFFFF;
	uint64_t TRIGRlastFooterVal ;//= 0x00FFFFFF;

public:

	//Takes a path to an XML file, the size of the readbuffer, the size of the intermediate buffer, the output streamsize, any additional paths to search, and count of blocks to read.
	GNSSReader::GNSSReader(const char* pathToFile,uint64_t readSize,uint64_t buffSize,uint64_t streamSize, uint64_t blockTotal = -1,const char** addlPaths = NULL, uint64_t pathCount = 0);

	//Changes Working Directory.
	static void changeWD(const char* pathToFile);

	//Returns Decoded Streams as an array
	DecStream** getDecStreamArray();

	//Count of decoded streams
	uint64_t getDecStreamCount();

	//Instantiates the decoded streams. Must call before decoding!
	void makeDecStreams();

	//Call this if you want to print block statisitics to the console
	void setPrintOptions(bool printStats, bool printSamples);

	//Starts the Reader as a thread.
	void startAsThread();

	//Returns true if the thread is finished working.
	bool isDone();

	//Returns capacity of intermediate buffer.
	double getIBufPercent();

	//Returns name of file currently being moved to the IBuf
	std::string fileBeingDecoded();

	//Starts decoding at block specified. By default, this is block1.
	void startAtBlock(uint64_t);

	//TODO: Decide how to handle memory management for decoded streams
	~GNSSReader();

};


#endif


