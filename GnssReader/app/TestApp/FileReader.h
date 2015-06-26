/**
 * File: FileReader.h
 * Author: WJLIDDY
 * Description: From a vector of SDR data files, puts them into it's own buffer based on several parameters.
 * Then, lets other objects read from this buffer.
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

	HANDLE sdrFile;						//Handle to the file that is currently being read.
	uint64_t readBufferSize;			//How many bytes to read at once
	char* buff;							//Buffer to handle transfers between the file and the intermediate buffer.
	volatile uint64_t bytesRead;		//Count of total bytes read from the current file so far
	LARGE_INTEGER fileSize;				//Count of total bytes in the file
	IBuffer* ib;						//Buffer that holds the files that have been read.
	bool killThreadFlag;				//A Flag that can kill all file reading operations if need be.
	std::vector<std::string> fnames;	//Holds all the SDR files in consecutive order.
	int filePtr;						//points to which file we are on in the vector.
	char** pathNames;
	uint64_t pathNameCount;

	//Helper function to start a thread of a class method at runtime.
	static void ThreadEntry(void *p);

	//Populates intermediate buffer with samples until there are no samples left to be read.
	void readFile();

public:

	//Needs a list of sdr data file names, the count of how many bytes to read a time, and the size of the buffer to store the bytes. 
	FileReader::FileReader(std::vector<std::string> fname,uint64_t readBufferSize, uint64_t intermediateBufferSize, const char *origPath, const char** paths, uint64_t pathCount);

	//Starts a thread that populates intermediatebuffer.
	void readAll();

	//given a request for count bytes, puts them in predefined buffer b.
	void getBufferedBytes(char* b, int count);

	//Returns true if the whole file has been read.
	//This is useful because once all the blocks have been read we can check for extraneous data.
	bool hasReadWholeFile();

	//returns about of bytes that have not been placed into the buffer yet
	uint64_t numBytesLeftToReadFromFile();

	//returns count of bytes in buffer
	uint64_t numBytesLeftInBuffer();

	//kills the thread that reads the file.
	void killReadThread();

	//skips over "count" bytes (good for getting over headers/footers)
	void skipBufferedBytes(int count);

	//When it is time to open a new datafile, this is called to make the transition 
	void prepareHandle();
	
	//Kills off all buffers
	~FileReader();
};
#endif


