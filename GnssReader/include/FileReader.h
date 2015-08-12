/**
 * File: FileReader.h
 * Author: WJLIDDY
 * Description: From a vector of SDR data files, puts them into an intermediate buffer based on several parameters.
 * Then, lets other objects read from this buffer.
 */
#ifndef FILEREADER_H_H
#define FILEREADER_H_H



#include <vector>
#include <stdio.h>

#ifdef _WIN32
	#include <windows.h>
	#include <process.h>
#else

#endif

#include <stdint.h>

#include "IBuffer.h"

class FileReader{

#ifdef _WIN32
	//Handle to the file that is currently being read.
	HANDLE sdrFile;		
	//Count of total bytes in the file
	LARGE_INTEGER fileSize;
#else
	pthread_t readThread;
#endif

	//How many bytes to read at once
	uint64_t readBufferSize;	
	//Count of total bytes read from the current file so far
	volatile int64_t bytesRead;	
	
	//A Flag that kills the thread if made true.
	bool killThreadFlag;				
	//Holds all the SDR files names in consecutive order.
	std::vector<std::string> fnames;	
	//points to which file we are on in the vector.
	int filePtr;			
	//list of path names
	char** pathNames;			
	//count of path names
	uint64_t pathNameCount;	
	//Buffer that holds the files that have been read.
	IBuffer* ib;						
	//The byte to start reading from
	uint64_t startByteLocation;

	//Helper function to start a thread of a class method at runtime.
	static void ThreadEntry(void *p);

	//Populates intermediate buffer with samples until there are no samples left to be read.
	void readFile();

	void readFileWin();
	
	void readFileNix();

	long getFileSizeNix(std::string);

public:

	//Needs a list of sdr data file names, the count of how many bytes to read a time, and the size of the buffer to store the bytes in terms of readBufferSize
	FileReader(std::vector<std::string> fname,uint64_t readBufferSize, uint64_t intermediateBufferSize, const char *origPath, const char** paths, uint64_t pathCount);

	//Starts a thread that populates intermediatebuffer.
	void readAll();

	//given a request for count bytes, puts them in predefined buffer b.
	char* getBufferedBytes(int count);

	//Returns true if the whole file has been read.
	//This is useful because once all the blocks have been read we can check for extraneous data.
	bool hasReadWholeFile();

	//returns about of bytes that have not been placed into the buffer yet
	uint64_t numBytesLeftToReadFromFile();

	//returns count of bytes in buffer
	uint64_t numBytesLeftInBuffer();

	//kills the thread that reads the file.
	void killReadThread();

	//to be called when the read operation has finished
	void doneReading(uint64_t count);

	//skips over "count" bytes (good for getting over headers/footers)
	void skipBufferedBytes(int count);

	//When it is time to open a new datafile, this is called to make the transition 
	void prepareHandle();

	//returns the amount of bytes in the intermediate buffer
	double getIBufPercent();

	//returns a string that is the file being decoded
	std::string fileBeingDecoded();

	//returns the size of the file passed in. It looks on all defined paths
	uint64_t getSizeOfFile(std::string);

	//sets the start file and start byte of the file. Used if blocks want to be skipped
	void setStartLocation(int file, uint64_t bytesToSkip);

	//count of all files read.
	uint64_t filesFullyReadCount();

	//Kills off all buffers
	~FileReader();



};
#endif


