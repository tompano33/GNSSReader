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

using namespace GnssMetadata;

class GNSSReader {
	//Current metadata and lane object being worked on.
	Metadata md;
	Lane* lane;
	XMLtoMeta* x2m;
	DecStream** decStreamArray;
	int decStreamCount;
	void readChunkCycles(Metadata md, Block * block, uint32_t cycles, FILE *sdrfile);

public:

	//Returns Decoded Streams.
	DecStream** getDecStreamArray();
	//takes the metadata file given and parses it's XML. Does not yet work with filesets.
	GNSSReader::GNSSReader(const char* directory, const char* metadataFile);
	void start();
	void makeDecStreams();
	int getDecStreamCount();
};


#endif


