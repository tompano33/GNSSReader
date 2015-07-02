/**
 * File: IBuffer.h
 * Author: WJLIDDY
 * Description: Intermediate buffer, stores data between file on disk  and outbut buffer. Circular.
 */
#ifndef IBUFFER_H_H
#define IBUFFER_H_H

#include <stdint.h>
#include <windows.h>

class IBuffer{
	uint64_t bufferSize; // size of the intermediatebuffer
	char* ibuf; //The buffer
    int bufPtr; //Pointer to where we may write new bytes to the buffer.
    int oldPtr; //Pointer to oldest data in buffer
	char* returnbuf; //return buffer for when we want to read bytes from this buffer.
    volatile int numBytesStored; //count of all the bytes that are stored

public:

	IBuffer::IBuffer(uint64_t bufferSize);

	//returns true if write was successful.
	bool write(char* bytes, uint64_t count);

	//blocks until it can read.
	void read(uint64_t size, char* buf);
	~IBuffer();

	uint64_t getNumBytesStored();
	//skips over "count" bytes in the buffer
	void skip(uint64_t count);

};
#endif

