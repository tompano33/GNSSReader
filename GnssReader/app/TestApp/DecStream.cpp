/**
 * File: DecStream.cpp
 * Author: WJLIDDY
 */

#include "DecStream.h"
#include <stdint.h>
#include "GnssMetadata\Stream.h"
#include <iostream>

	DecStream::DecStream(uint64_t sampleCap, std::string id,GnssMetadata::Stream * corStream, bool hasComplexPart, bool complexPartFirst)
	{
		done = false;
		sampleCapacity = sampleCap;
		sampleBuf = new double[sampleCapacity];
		samplePtr = 0;
		this->id = id;
		this->correspondingStream = corStream;
		this->hasCPart = hasComplexPart;
		this->CPartFirst = complexPartFirst;

		InitializeCriticalSection(&crit);
	};

	DecStream::~DecStream()
	{
		delete [] sampleBuf;
	};


	void DecStream::putSample(double sample){

		while(samplePtr >= sampleCapacity){;}
		//wait for a withdraw
        EnterCriticalSection(&crit);
		sampleBuf[samplePtr] = sample;
		samplePtr++;
        LeaveCriticalSection(&crit);

	};

	std::string DecStream::getID(){
		return id;
	};

	uint64_t DecStream::getSamplePtr(){
		return samplePtr;
	};

	double* DecStream::getBuf(){
		return DecStream::sampleBuf;
	};

	uint64_t DecStream::getBufSize(){
		return sampleCapacity;
	}

	void DecStream::clear(){
		samplePtr = 0;
	};

	GnssMetadata::Stream* DecStream::getCorrespondingStream(){
		return correspondingStream;
	};

	void DecStream::setCorrespondingStream(GnssMetadata::Stream* s)
	{
		correspondingStream = s;
	}

	void DecStream::flushOutputStream(double* d, uint64_t* byteCount)
	{	
        EnterCriticalSection(&crit);

		memcpy(d,sampleBuf,sizeof(double) * samplePtr);
		*byteCount = samplePtr;
		samplePtr = 0;
		LeaveCriticalSection(&crit);
	}

	bool DecStream::hasComplexPart()
	{
		return hasCPart;
	}

	bool DecStream::complexPartFirst()
	{
		return CPartFirst;
	}