#ifndef FILEREADER_H_H
#define FILEREADER_H_H

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <process.h>
#include "IBuffer.h"

//Reads an SDR File. Puts the file contents in it's intermediateBuffer
class FileReader{
	//File Handle
	HANDLE sdrFile;
	//How many bytes to read at once?
	long readBufferSize;
	//Buffer for reading the file,
	char* buff;
	//Count of total bytes read from file
	volatile long bytesRead;
	//Count of total bytes in the file
	LARGE_INTEGER fileSize;
	IBuffer* ib;
	bool killThreadFlag;
	bool xmlWasValid;

	//starts a thread
	static void ThreadEntry(void *p);
	//Populates samples as a thread until there are none left.
	//Tries to populate intermediate buffer with samples. 
	void readFile();

public:
	//Needs the name of the file and a count of how many bytes to load from the file. Also needs the size of intermediate buffer. Puts file handle in memory. Inits buffer.
	FileReader::FileReader(LPCWSTR fname,long readBufferSize, long intermediateBufferSize);

	void readAll();

	//returns true if bytes were put in buffer, false otherwise
	bool getBufferedBytes(char* b, int count);

	void close();

	bool hasReadWholeFile();

	long numBytesLeftToReadFromFile();

	long numBytesLeftInBuffer();

	void killReadThread();

	void skipBufferedBytes(int count);

	~FileReader();

	bool parseSuccess();
};
#endif


