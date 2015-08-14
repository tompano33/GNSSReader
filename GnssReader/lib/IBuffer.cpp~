#ifdef _WIN32
	#include <windows.h>
#else
	//linux
#endif
#include <iostream>
#include <cstring>
#include "IBuffer.h"

	IBuffer::IBuffer(uint64_t writeBlockSize, uint64_t writeBlockCount){

		this->writeBlockSize = writeBlockSize;
		this->writeBlockCount = writeBlockCount;
		this->normalBufferSize = writeBlockSize*(writeBlockCount);
		this->preBufferSize = writeBlockSize;
		this->totalBufferSize = normalBufferSize+preBufferSize;
		this->numBytesStored = 0;
		ibuf = new char[totalBufferSize];
		finish = false;
		bufPtr = preBufferSize;
		oldPtr = preBufferSize;
		filesFullyRead = 0;

		#ifdef _WIN32
			InitializeCriticalSection(&crit);
		#else 
			 pthread_mutex_init(&mutex, NULL);
		#endif
	}


	//returns true if a block of 'writeblocksize' can be written, false if not

	//This gets called REALLY often.
	char* IBuffer::canWriteBlock()
	{
		char* retval;

		//std::cout << "Awaiting Mutex... WRITE \n";
		lockMutex();
		//std::cout << "Done! \n";

		if(bufPtr == oldPtr)
		{
			retval = ibuf+bufPtr;
		}

		//we need not worry about overlap
		if(bufPtr < oldPtr)
		{
			if((oldPtr - bufPtr) > writeBlockSize)
				retval = ibuf+bufPtr;
			else
				retval = NULL;
		} else 
		{
			uint64_t endOfBufferDataSize = (totalBufferSize) - bufPtr;

			if(endOfBufferDataSize >= writeBlockSize)
			{
				retval =  ibuf+bufPtr;
			} else
			{
				retval =  NULL;
			}
		}

		unlockMutex();
		return retval;
	}

	void IBuffer::doneWritingBlock()
	{	
		//std::cout << "Awaiting Mutex... DRB \n";
		lockMutex();
		//std::cout << "Done! \n";

		numBytesStored += writeBlockSize;
		bufPtr+=writeBlockSize;

		if(bufPtr == totalBufferSize && oldPtr != preBufferSize)
		{
			bufPtr = preBufferSize;
		}

		unlockMutex();
	}

	double IBuffer::getPercent()
	{
		return (double)numBytesStored / (double)normalBufferSize;
	}
	
	char* IBuffer::tryRead(uint64_t count)
	{
		//std::cout << "Awaiting Mutex... READ \n";
		lockMutex();
		//std::cout << "Done! \n";
		
		char* retval;

		//trick the buffer into letting us write to the same place
		if(finish)
		{
			count--;
		}

		//buffer is clear!
		if(bufPtr == oldPtr)
		{
			retval =  NULL;
		}

		//we need not worry about overlap
		else if(bufPtr > oldPtr)
		{

			if((bufPtr - oldPtr) > count)
				retval = ibuf+oldPtr;
			else
				retval = NULL;
		} else 
		{
			uint64_t endOfBufferDataSize = (totalBufferSize) - oldPtr;
			uint64_t startBufferDataSize = bufPtr - preBufferSize;

	
			if(endOfBufferDataSize > count)
			{
				 retval =  ibuf+oldPtr;
			} 

			else if( (startBufferDataSize + endOfBufferDataSize) > count)
			{
				std::memcpy(ibuf+(preBufferSize-endOfBufferDataSize),ibuf+oldPtr,endOfBufferDataSize);
				retval =  ibuf+(preBufferSize-endOfBufferDataSize);
			} 
			
			else retval = NULL;
			
		}

		unlockMutex();

		return retval;
	};

	void IBuffer::doneReading(uint64_t count)
	{
		//std::cout << "Awaiting Mutex... DONE_READ \n";
		lockMutex();
		//std::cout << "Done! \n";

		numBytesStored -= count;
		//we need not worry about overlap
		if(bufPtr > oldPtr)
		{
			oldPtr += count;


			if(bufPtr == totalBufferSize)
				bufPtr = preBufferSize;
		} else 
		{
			uint64_t endOfBufferDataSize = (totalBufferSize) - oldPtr;
			uint64_t startBufferDataSize = bufPtr - (preBufferSize) ;

			if(endOfBufferDataSize > count)
			{
				oldPtr += count;
				unlockMutex();		
				return;
			} 

			if( (startBufferDataSize + endOfBufferDataSize) > count)
			{
				uint64_t post = count - (endOfBufferDataSize);
				oldPtr = preBufferSize+post;

			} 
			
			
		}

		if(finish)
		{
			if(bufPtr == preBufferSize && oldPtr == totalBufferSize +-writeBlockSize + bytesInFinish)
			{
				bufPtr = preBufferSize;
				oldPtr = preBufferSize;
				finish = false;
				numBytesStored = 0;
				filesFullyRead++;
			}
			else if(oldPtr == bufPtr +- writeBlockSize + bytesInFinish)
			{
				bufPtr = preBufferSize;
				oldPtr = preBufferSize;
				finish = false;
				numBytesStored = 0;
				filesFullyRead++;
			}
		}

		unlockMutex();
	};



	uint64_t IBuffer::getNumBytesStored()
	{
		return numBytesStored;
	}
	void IBuffer::dbg_printPtrs()
	{
		std::cout << oldPtr << " " << bufPtr << "\n";

	}

	IBuffer::~IBuffer(){
		//need to delete- but causes errors
	//	delete [] ibuf;
	}

	void IBuffer::finishWrite(uint64_t bytes)
	{
		finish = true;
		bytesInFinish = bytes;
	}

	bool IBuffer::isFinished()
	{
		return finish;
	}

	int IBuffer::getFileReadCount()
	{
		return filesFullyRead;
	}

	void IBuffer::lockMutex()
	{
		#ifdef _WIN32
       		EnterCriticalSection(&crit);
		#else 
		   pthread_mutex_lock (&mutex);
		#endif
	}

	void IBuffer::unlockMutex()
	{
		#ifdef _WIN32
        		LeaveCriticalSection(&crit);
		#else
			pthread_mutex_unlock (&mutex);
		#endif
	}
