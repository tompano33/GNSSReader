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
	}

	//returns true if a block of 'writeblocksize' can be written, false if not

	char* IBuffer::canWriteBlock()
	{
		//buffer is clear!
		if(bufPtr == oldPtr)
		{
			return ibuf+bufPtr;
		}

		//we need not worry about overlap
		if(bufPtr < oldPtr)
		{
			if((oldPtr - bufPtr) > writeBlockSize)
				return ibuf+bufPtr;
			else
				return NULL;
		} else 
		{
			uint64_t endOfBufferDataSize = (totalBufferSize) - oldPtr;

			if(endOfBufferDataSize >= writeBlockSize)
			{
				return ibuf+bufPtr;
			} else
			{
				return NULL;
			}
		}
	}

	void IBuffer::doneWritingBlock()
	{
		numBytesStored += writeBlockSize;
		bufPtr+=writeBlockSize;
		if(bufPtr == totalBufferSize && oldPtr != preBufferSize)
		{
			bufPtr = preBufferSize;
		}
	}
	
	char* IBuffer::tryRead(uint64_t count)
	{

		if(finish)
		{
			count--;
		}

		//buffer is clear!
		if(bufPtr == oldPtr)
		{
			return NULL;
		}

		//we need not worry about overlap
		if(bufPtr > oldPtr)
		{

			if((bufPtr - oldPtr) > count)
				return ibuf+oldPtr;
			else
				return NULL;
		} else 
		{
			uint64_t endOfBufferDataSize = (totalBufferSize) - oldPtr;
			uint64_t startBufferDataSize = bufPtr - preBufferSize;

	
			if(endOfBufferDataSize > count)
			{
				return ibuf+oldPtr;
			} 

			if( (startBufferDataSize + endOfBufferDataSize) > count)
			{
				memcpy(ibuf+(preBufferSize-endOfBufferDataSize),ibuf+oldPtr,endOfBufferDataSize);
				return ibuf+(preBufferSize-endOfBufferDataSize);
			} 
			
			return NULL;
			
		}
	};

	void IBuffer::doneReading(uint64_t count)
	{
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
							
				return;
			} 

			if( (startBufferDataSize + endOfBufferDataSize) > count)
			{
				uint64_t post = count - (endOfBufferDataSize);
				oldPtr = preBufferSize+post;

			} 
			
			
		}

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

	void IBuffer::finishWrite()
	{
		finish = true;
	}
