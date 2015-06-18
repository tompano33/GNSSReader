#include <windows.h>
//Intermediate Buffer. Stores raw SDR data from file before it gets read.
class IBuffer{
	static const int BUFFERSIZE = 7;
	char* ibuf;

	//Pointer to where we may write new bytes to the buffer.
	int bufPtr;
	//Pointer to where the start of the oldest data is in the buffer
	int oldPtr;
	//return buffer for when we want to read bytes from this buffer.
	char* returnbuf;
	//count of all the bytes that are stored
	volatile int numBytesStored;

public:

	IBuffer::IBuffer(){
		ibuf = new char[BUFFERSIZE];
		bufPtr = 0;
		oldPtr = 0;
		numBytesStored = 0;
	}

	//returns true if write was successful
	bool write(char* bytes, int count)
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
	bool read(int size, char* buf){
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
	~IBuffer(){
		delete [] ibuf;
	}
};

