#include <windows.h>
#include <iostream>
#include "IBuffer.h"

	IBuffer::IBuffer(uint64_t bufSize){
		this->bufferSize = bufSize;
		ibuf = new char[bufSize];
		bufPtr = 0;
		oldPtr = 0;
		numBytesStored = 0;
	}

	//returns true if write was successful
	bool IBuffer::write(char* bytes, uint64_t count)
	{
		//Not enough room to put bytes!
		if((bufferSize-numBytesStored) < count){
			return false;
		} else
		{
			//the buffer has space, and we aren't going to wrap around, we can just memcpy
			if(count < (bufferSize- bufPtr) ) 
			{
				memcpy(ibuf+bufPtr, bytes, count);
				bufPtr += count;
			}
			else
			{
				//we need to write twice, once before the end, one after
				int spaceLeftAtEnd = (bufferSize - bufPtr);
				memcpy(ibuf+bufPtr,bytes,spaceLeftAtEnd);
				memcpy(ibuf,bytes+spaceLeftAtEnd,count-spaceLeftAtEnd);
				bufPtr = count-spaceLeftAtEnd;
			}
			numBytesStored += count;
			return true;
		}
	}

	//TODO throw error if overreading
	void IBuffer::read(uint64_t size, char* buf)
	{
		if(size > bufferSize)
		{
			printf("Error: chunk is bigger than buffer");
			std::cin.get();
			return;
		}

		while(size > numBytesStored){;}

		if(size < (bufferSize - oldPtr))
		{
			memcpy(buf,ibuf+oldPtr,size);
			oldPtr+= size;
		} else 
		{
			int spaceLeftAtEnd = (bufferSize - oldPtr);
			memcpy(buf,ibuf+oldPtr,spaceLeftAtEnd);
			memcpy(buf+spaceLeftAtEnd,ibuf,size-spaceLeftAtEnd);
			oldPtr=size-spaceLeftAtEnd;
		}	
			
		numBytesStored -=size;
	};

	uint64_t IBuffer::getNumBytesStored()
	{
		return numBytesStored;
	};

	void IBuffer::skip(uint64_t size)
	{
		//wait for there to be enough bytes to skip
		while(size > numBytesStored){;}
			
		if(size < (bufferSize - oldPtr))
		{
				oldPtr+= size;
		} else 
		{
			uint64_t spaceLeftAtEnd = (bufferSize - oldPtr);
			oldPtr=size-spaceLeftAtEnd;
		}
			
		numBytesStored -=size;
	};

	IBuffer::~IBuffer(){
		delete [] ibuf;
	}

