#ifndef IBUFFER_H_H
#define IBUFFER_H_H

#include <windows.h>

//Intermediate Buffer. Stores raw SDR data from file before it gets read.
class IBuffer{
	long BUFFERSIZE;
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

	IBuffer::IBuffer(long BUFFERSIZE);

	//returns true if write was successful
	bool write(char* bytes, int count);

	//returns true if read was successful
	bool read(int size, char* buf);
	~IBuffer();

	int getNumBytesStored();

};
#endif

