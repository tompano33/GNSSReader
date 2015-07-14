/**
 * File: DecStream.cpp
 * A double buffered stream. 
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
		sampleBuf1 = new double[sampleCapacity];
		sampleBuf2 = new double[sampleCapacity];
		lockSampleBuf1 = false;
		samplePtr = 0;
		this->id = id;
		this->correspondingStream = corStream;
		this->hasCPart = hasComplexPart;
		this->CPartFirst = complexPartFirst;

		InitializeCriticalSection(&crit);
	};

	DecStream::~DecStream()
	{
		delete [] sampleBuf1;
		delete [] sampleBuf2;
	};


	void DecStream::putSample(double sample){

		while(samplePtr >= sampleCapacity){;}
		//wait for a withdraw

        EnterCriticalSection(&crit);
		//write to the sample buf that isn't locked
		if(!lockSampleBuf1)
			sampleBuf1[samplePtr] = sample;
		else
			sampleBuf2[samplePtr] = sample;

		samplePtr++;
        LeaveCriticalSection(&crit);

	};

	std::string DecStream::getID(){
		return id;
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

	//does not care if sample not paired. Todo?
	double* DecStream::flushOutputStream(uint64_t* byteCount)
	{	
        EnterCriticalSection(&crit);
		*byteCount = samplePtr;
		samplePtr = 0;

		//return the sample that isn't locked
		if(!lockSampleBuf1)
			return sampleBuf1;
		else
			return sampleBuf2;

		lockSampleBuf1 = !lockSampleBuf1;
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