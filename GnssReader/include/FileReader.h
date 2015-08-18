/**
 * File: FileReader.h
 * Author: WJLIDDY
 * Description: From a vector of SDR data files, puts them into an intermediate buffer based on several parameters.
 * Then, lets the Decoder read from this buffer via a chunkbuffer.
 */
#ifndef FILEREADER_H_H
#define FILEREADER_H_H

#include <vector>
#include <stdio.h>
#include <stdint.h>

//Needed for multithreading and also changing the working directory.
#ifdef _WIN32
	#include <windows.h>
	#include <process.h>
#else
	//linux chdir commands
#endif

#include "IBuffer.h"

class FileReader{

	private:

	#ifdef _WIN32
		//Handle to the file that is currently being read.
		HANDLE _sdrFile;		
		//Count of total bytes in the file
		LARGE_INTEGER _fileSize;
	#else
		pthread_t _readThread;
	#endif

	//How many bytes to read at once from any SDR File.
	uint64_t _readBufferSize;	
	//Count of total bytes read from the current file so far
	volatile int64_t _bytesRead;	
	//A flag that kills the entire reading thread if true. Use this if something goes wrong in the decoder.
	bool _killThreadFlag;				
	//Holds all the SDR files names in consecutive order. TODO: Isn't this in the decoder??
	std::vector<std::string> _fnames;	
	//points to which file we are on in the vector.
	int _filePtr;			
	//list of path names to search through
	char** _pathNames;			
	//count of path names
	uint64_t _pathNameCount;	
	//Buffer that holds the files that have been read.
	IBuffer* _ib;						
	//The byte to start reading from. used for block offsets (such as in TRIGR files)
	uint64_t _startByteLocation;

	//Helper function to start a thread of a class method at runtime.
	static void ThreadEntry(void *p);
	
	//populates the Ibuffer. To be called on windows machines
	void readFileWin();
	//populate the Ibuffer. To be called on Unix machines.
	void readFileNix();
	//OS independdently populates the Ibuffer. Called by thread Entry.
	void readFile();
	//unused?
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


