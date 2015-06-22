#include <windows.h>
#include "IBuffer.h"

//Intermediate Buffer. Stores raw SDR data from file before it gets read.
	IBuffer::IBuffer(long BUFFERSIZE){
		this->BUFFERSIZE = BUFFERSIZE;
		ibuf = new char[BUFFERSIZE];
		bufPtr = 0;
		oldPtr = 0;
		numBytesStored = 0;
	}

	//returns true if write was successful
	bool IBuffer::write(char* bytes, int count)
	{
		//Not enough room to put bytes!
		if((BUFFERSIZE-numBytesStored) < count){
			return false;
		} else
		{
			if(count < (BUFFERSIZE - bufPtr) ) 
			{
				memcpy(ibuf+bufPtr, bytes, count);
				bufPtr += count;
			}
			else
			{
				int spaceLeftAtEnd = (BUFFERSIZE - bufPtr);
				memcpy(ibuf+bufPtr,bytes,spaceLeftAtEnd);
				memcpy(ibuf,bytes+spaceLeftAtEnd,count-spaceLeftAtEnd);
				bufPtr = count-spaceLeftAtEnd;
			}
			numBytesStored += count;
			return true;
		}
	}

	//returns true if read was successful
	bool IBuffer::read(int size, char* buf){
		if(size > numBytesStored)
		{
			return false;
		} else
		{
			if(size < (BUFFERSIZE - oldPtr))
			{
				memcpy(buf,ibuf+oldPtr,size);
				oldPtr+= size;
			} else 
			{
				int spaceLeftAtEnd = (BUFFERSIZE - oldPtr);
				memcpy(buf,ibuf+oldPtr,spaceLeftAtEnd);
				memcpy(buf+spaceLeftAtEnd,ibuf,size-spaceLeftAtEnd);
				oldPtr=size-spaceLeftAtEnd;
			}
			
			numBytesStored -=size;
			return true;
		}
	}

	int IBuffer::getNumBytesStored()
	{
		return numBytesStored;
	};

	bool IBuffer::skip(int size)
	{
	if(size > numBytesStored)
		{
			return false;
		} else
		{
			if(size < (BUFFERSIZE - oldPtr))
			{
				oldPtr+= size;
			} else 
			{
				int spaceLeftAtEnd = (BUFFERSIZE - oldPtr);
				oldPtr=size-spaceLeftAtEnd;
			}
			
			numBytesStored -=size;

	};
	}

	IBuffer::~IBuffer(){
		delete [] ibuf;
	}

