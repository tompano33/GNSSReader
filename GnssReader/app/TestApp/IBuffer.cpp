#include <windows.h>
#include <iostream>
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
		
		InitializeCriticalSection(&crit);
	}

	//returns true if a block of 'writeblocksize' can be written, false if not

	char* IBuffer::canWriteBlock()
	{
		char* retval;

        EnterCriticalSection(&crit);

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

		LeaveCriticalSection(&crit);
		return retval;
	}

	void IBuffer::doneWritingBlock()
	{

        EnterCriticalSection(&crit);

		numBytesStored += writeBlockSize;
		bufPtr+=writeBlockSize;

		if(bufPtr == totalBufferSize && oldPtr != preBufferSize)
		{
			bufPtr = preBufferSize;
		}

		
		LeaveCriticalSection(&crit);
	}

	double IBuffer::getPercent()
	{
		return (double)numBytesStored / (double)normalBufferSize;
	}
	
	char* IBuffer::tryRead(uint64_t count)
	{
		
        EnterCriticalSection(&crit);
		
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
				memcpy(ibuf+(preBufferSize-endOfBufferDataSize),ibuf+oldPtr,endOfBufferDataSize);
				retval =  ibuf+(preBufferSize-endOfBufferDataSize);
			} 
			
			else retval = NULL;
			
		}

		
		LeaveCriticalSection(&crit);
		return retval;
	};

	void IBuffer::doneReading(uint64_t count)
	{
		

        EnterCriticalSection(&crit);

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
				LeaveCriticalSection(&crit);			
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
			}
			else if(oldPtr == bufPtr +- writeBlockSize + bytesInFinish)
			{
				bufPtr = preBufferSize;
				oldPtr = preBufferSize;
				finish = false;
			}
		}
		LeaveCriticalSection(&crit);

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