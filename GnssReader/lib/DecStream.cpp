/**
 * File: DecStream.cpp
 * A double buffered stream. 
 * Author: WJLIDDY
 * TODO: Document
 * c-p untested
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
		#else 
			 pthread_mutex_init(&mutex, NULL);
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

		lockMutex();
		//write to the sample buf that isn't locked
		if(!lockSampleBuf1)
			sampleBuf1[samplePtr] = sample;
		else
			sampleBuf2[samplePtr] = sample;

		samplePtr++;
	
		unlockMutex();

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
		lockMutex();
		
		*byteCount = samplePtr;
		samplePtr = 0;
		lockSampleBuf1 = !lockSampleBuf1;

		unlockMutex();

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

	void DecStream::lockMutex()
	{
		#ifdef _WIN32
       		EnterCriticalSection(&crit);
		#else 
		   pthread_mutex_lock (&mutex);
		#endif
	}

	void DecStream::unlockMutex()
	{
		#ifdef _WIN32
        	LeaveCriticalSection(&crit);
		#else
			 pthread_mutex_unlock (&mutex);
		#endif
	}
