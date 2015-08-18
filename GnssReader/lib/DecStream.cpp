/**
 * File: DecStream.cpp
 * Author: WJLIDDY
 */

#include "DecStream.h"
#include <stdint.h>
#include <GnssMetadata/Stream.h>
#include <iostream>

	DecStream::DecStream(uint64_t sampleCap, std::string id,GnssMetadata::Stream * corStream, bool hasComplexPart, bool complexPartFirst)
	{
		_sampleCapacity = sampleCap;
		_sampleBuf1 = new double[_sampleCapacity];
		_sampleBuf2 = new double[_sampleCapacity];
		_lockSampleBuf1 = false;
		_samplePtr = 0;
		this->_id = id;
		this->_correspondingStream = corStream;
		this->_hasCPart = hasComplexPart;
		this->_CPartFirst = complexPartFirst;

		#ifdef _WIN32
			InitializeCriticalSection(&_crit);
		#else 
			 pthread_mutex_init(&_mutex, NULL);
		#endif
	};

	DecStream::~DecStream()
	{
		delete [] _sampleBuf1;
		delete [] _sampleBuf2;
	};


	void DecStream::putSample(double sample){

		//TODO: Callback, instead of freezing the thread.
		//If this loop triggers, the output stream is over-capacity. Need to wait for someone to flush it.
		while(_samplePtr >= _sampleCapacity){;}

		lockMutex();

		//write to the sample buf that isn't locked
		if(!_lockSampleBuf1)
			_sampleBuf1[_samplePtr] = sample;
		else
			_sampleBuf2[_samplePtr] = sample;

		//added a sample, increment pointer.
		_samplePtr++;
		unlockMutex();

	};

	//Name of stream but is this obsoloete?
	std::string DecStream::getID(){
		return _id;
	};

	//Size of the buffer.
	uint64_t DecStream::getBufSize(){
		return _sampleCapacity;
	}

	//You can call this to get the assocaited metadata stream.
	GnssMetadata::Stream* DecStream::getCorrespondingStream(){
		return _correspondingStream;
	};

	void DecStream::setCorrespondingStream(GnssMetadata::Stream* s)
	{
		_correspondingStream = s;
	}

	//does not care if sample not paired with complex data. Todo?
	double* DecStream::flushOutputStream(uint64_t* byteCount)
	{	
		lockMutex();
		
		*byteCount = _samplePtr;
		_samplePtr = 0;
		_lockSampleBuf1 = !_lockSampleBuf1;

		unlockMutex();

		//return the sample that isn't locked, (we just intverted it)
		if(_lockSampleBuf1)
			return _sampleBuf1;
		else
			return _sampleBuf2;
	}

	bool DecStream::hasComplexPart()
	{
		return _hasCPart;
	}

	bool DecStream::complexPartFirst()
	{
		return _CPartFirst;
	}

	void DecStream::lockMutex()
	{
		#ifdef _WIN32
       		EnterCriticalSection(&_crit);
		#else 
		   pthread_mutex_lock (&_mutex);
		#endif
	}

	void DecStream::unlockMutex()
	{
		#ifdef _WIN32
        	LeaveCriticalSection(&_crit);
		#else
			 pthread_mutex_unlock (&_mutex);
		#endif
	}

	void DecStream::setFreqs(uint64_t sample, uint64_t center, uint64_t inter)
	{
		_sampleRate = sample;
		_centerFreq = center;
		_interFreq = inter;
	}

	uint64_t DecStream::getSampleRate(){return _sampleRate;}
	uint64_t DecStream::getCenterFreq(){return _centerFreq;}
	uint64_t DecStream::getInterFreq(){return _interFreq;}
