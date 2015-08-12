/**
 * File: DecStream.cpp
 * A double buffered stream. 
 * Author: WJLIDDY
 * TODO: Document
 */

#include "DecStream.h"
#include <stdint.h>
#include <GnssMetadata/Stream.h>
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

		#ifdef _WIN32
			InitializeCriticalSection(&crit);
		#endif
	};

	DecStream::~DecStream()
	{
		delete [] sampleBuf1;
		delete [] sampleBuf2;
	};


	void DecStream::putSample(double sample){

		while(samplePtr >= sampleCapacity){;}
		//wait for a withdraw

		#ifdef _WIN32
       		EnterCriticalSection(&crit);
		#endif

		//write to the sample buf that isn't locked
		if(!lockSampleBuf1)
			sampleBuf1[samplePtr] = sample;
		else
			sampleBuf2[samplePtr] = sample;

		samplePtr++;
	
		#ifdef _WIN32
        	LeaveCriticalSection(&crit);
		#endif

	};

	std::string DecStream::getID(){
		return id;
	};

	uint64_t DecStream::getBufSize(){
		return sampleCapacity;
	}

	//You can call this to get all of the data
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
		#ifdef _WIN32
       			EnterCriticalSection(&crit);
		#endif
		
		*byteCount = samplePtr;
		samplePtr = 0;
		lockSampleBuf1 = !lockSampleBuf1;

		#ifdef _WIN32
		LeaveCriticalSection(&crit);
		#endif

		//return the sample that isn't locked, (we just intverted it)
		if(lockSampleBuf1)
			return sampleBuf1;
		else
			return sampleBuf2;

	}

	bool DecStream::hasComplexPart()
	{
		return hasCPart;
	}

	bool DecStream::complexPartFirst()
	{
		return CPartFirst;
	}
