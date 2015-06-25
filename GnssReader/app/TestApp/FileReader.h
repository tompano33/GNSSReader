/**
 * File: FileReader.h
 * Author: WJLIDDY
 * Description: From a vector of SDR data files, puts them into a buffer based on several parameters.
 */
#ifndef FILEREADER_H_H
#define FILEREADER_H_H


#include <vector>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <stdint.h>

#include "IBuffer.h"

class FileReader{

	//Handle to the file that is currently being read. TODO: probably obsolete due to the fnames
	HANDLE sdrFile;

	//How many bytes to read at once?
	uint64_t readBufferSize;

	//Buffer for reading the file,
	char* buff;
	//Count of total bytes read from file
	volatile long bytesRead;
	//Count of total bytes in the file
	LARGE_INTEGER fileSize;
	IBuffer* ib;
	bool killThreadFlag;
	bool xmlWasValid;
	std::vector<std::string> fnames;
	int filePtr;

	//starts a thread
	static void ThreadEntry(void *p);
	//Populates samples as a thread until there are none left.
	//Tries to populate intermediate buffer with samples. 
	void readFile();

public:
	//Needs the name of the file and a count of how many bytes to load from the file. Also needs the size of intermediate buffer. Puts file handle in memory. Inits buffer.
	FileReader::FileReader(std::vector<std::string> fname,long readBufferSize, long intermediateBufferSize);

	void readAll();

	//returns true if bytes were put in buffer, false otherwise
	void getBufferedBytes(char* b, int count);

	void close();

	bool hasReadWholeFile();

	long numBytesLeftToReadFromFile();

	long numBytesLeftInBuffer();

	void killReadThread();

	void skipBufferedBytes(int count);

	void prepareHandle();

	~FileReader();

	bool parseSuccess();
};
#endif


