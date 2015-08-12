/**
 * File: IBuffer.h
 * Author: WJLIDDY
 * Description: Intermediate buffer, stores data between file on disk  and outbut buffer. Circular.
 */
#ifndef IBUFFER_H_H
#define IBUFFER_H_H

#include <stdint.h>

#ifdef _WIN32
	#include <windows.h>
#endif

class IBuffer{

	uint64_t writeBlockSize;
	uint64_t writeBlockCount; 
	uint64_t normalBufferSize;
	uint64_t preBufferSize;
	uint64_t totalBufferSize;
	uint64_t bytesInFinish;

	char* ibuf; //The buffer itself

	uint64_t bufPtr; //Pointer to where we may write new bytes to the buffer.
	uint64_t oldPtr; //Pointer to oldest data in buffer

	volatile int numBytesStored; //count of all the bytes that are stored

	volatile bool finish;

	int filesFullyRead;

	#ifdef _WIN32
		//do not write and read
		CRITICAL_SECTION crit; 
	#else 
		pthread_mutex_t mutex;
	#endif


public:

	//makes a buffer.
	IBuffer(uint64_t writeBlockSize, uint64_t writeBlockCount);

	//If there is space in the buffer, returns a pointer to the buffer where writing may begin for 'n' bytes. Returns 'null' if no such space exists
	char* canWriteBlock();

	//After you write to the buffer, you MUST call this to confirm that the bytes were written.
	void doneWritingBlock();

	//blocks until it can read.
	char* tryRead(uint64_t count);

	void doneReading(uint64_t count);

	~IBuffer();

	uint64_t getNumBytesStored();

	void dbg_printPtrs();

	void finishWrite(uint64_t bytes);

	bool isFinished();

	double getPercent();

	int getFileReadCount();

	void lockMutex();
	void unlockMutex();

};
#endif

