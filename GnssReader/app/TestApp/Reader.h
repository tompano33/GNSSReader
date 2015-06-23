#ifndef READER_H_H
#define READER_H_H

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
#include "FileReader.h"
#include <vector>
using namespace GnssMetadata;

class GNSSReader {
	//Current metadata and lane object being worked on.
	Metadata md;
	std::vector<Metadata*>* mdList;
	Lane* lane;
	DecStream** decStreamArray;
	int decStreamCount;
	long streamSize;
	FileReader* fr;
	void readChunkCycles(Block * block, uint32_t cycles);
	int blocksLeftToRead;

public:

	bool printStats, printSamples;
	//Returns Decoded Streams.
	DecStream** getDecStreamArray();
	//takes the metadata file given and parses it's XML. Does not yet work with filesets.
	GNSSReader::GNSSReader(const char* pathToFile,long readSize, long buffSize, long streamSize,const char** addlPaths = NULL, int blockTotal = -1);
	void start();
	void makeDecStreams();
	int getDecStreamCount();
	void makeFileList(std::string* first);
	~GNSSReader();
};


#endif


