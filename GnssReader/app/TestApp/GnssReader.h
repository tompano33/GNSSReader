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
using namespace GnssMetadata;

class GNSSReader {
	std::vector<Metadata*>* mdList;	//List of all metadata objects.
	uint64_t mdPtr; //Pointer to metadata object we are currently decoding.
	std::vector<std::string>* sdrFileNames; //List of all SDR file names to decode, in order. This can be disposed of when i have some time
	DecStream** decStreamArray; //Array of Decoded Sample Output Streams
	uint64_t decStreamCount; //The count of these streams.
	uint64_t streamSize; //The size of an output stream.
	FileReader* fr; //File reader, which decods sdrFileNames.
	void readChunkCycles(Block * block, uint32_t cycles); //Given a block, reads it 
	uint64_t blocksLeftToRead; //count of total blocks the user wanted read.
	bool printStats, printSamples; //IF we want to print anything as we decode blocks, turn one of these on

	// When an XML file switches, associates streams with the same ID to the existing one.
	void repairDecStreams(Metadata* md); 

	//given the name of the first XML file, crawls through all the other XML files to build a list of data files
	void makeFileList(std::string first);

	//changes Working Directory.
	void changeWD(const char* pathToFile);
public:

	//Returns Decoded Streams as an array
	DecStream** getDecStreamArray();
	//Takes a path to an XML file, the size of the readbuffer, the size of the intermediate buffer, the output streamsize, any additional paths to search, and count of blocks to read.
	GNSSReader::GNSSReader(const char* pathToFile,uint64_t readSize,uint64_t buffSize,uint64_t streamSize,const char** addlPaths = NULL, uint64_t blockTotal = -1);
	//Starts decoding a file and puts it in outputstream
	void start();
	//Instantiates the decoded streams.
	void makeDecStreams();
	//Count of decoded streams
	uint64_t getDecStreamCount();
	//Call this if you want to print block statisitics to the console
	void setPrintOptions(bool printStats, bool printSamples);
	~GNSSReader();
};


#endif


