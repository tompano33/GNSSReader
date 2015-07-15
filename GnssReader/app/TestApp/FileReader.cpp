/**
 * File: FileReader.cpp
 * Author: WJLIDDY
 */
#include <iostream>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <process.h>

#include "FileReader.h"
#include "IBuffer.h"
#include "GnssReader.h"

//I dont want to have to do this but I cant fnid this bug
#include "Test.h"

	FileReader::FileReader(std::vector<std::string> fnames,uint64_t inBlockSize, uint64_t inBlockCount,const char* origPath,const char** addlPaths,uint64_t pathCount){
	
		this->readBufferSize = inBlockSize;

		ib = new IBuffer(inBlockSize,inBlockCount);
		killThreadFlag = false;
		this->fnames = fnames;
		filePtr = 0;
		bytesRead = 0;

		//copy const strings to something that wont go out of scope.
		pathNames = new char*[pathCount+1];
		pathNames[0] = strdup(origPath);

		for(int i = 1 ; i < pathCount+1; i++)
		{
			pathNames[i] = strdup(addlPaths[i-1]);
		}

		pathNameCount = pathCount+1;
		prepareHandle();
	}

	void FileReader::readFile()
	{


		//Perhaps there is a more elegant way to run and kill the thread, but this works
		while(bytesRead < fileSize.QuadPart && !killThreadFlag)
		{
			uint64_t thisLoopReadBufferSize = readBufferSize;

			while(ib->isFinished())
			{;}

			int finish = false;

			if(readBufferSize >= fileSize.QuadPart - bytesRead)
			{
				thisLoopReadBufferSize = fileSize.QuadPart - bytesRead;
				finish = true;
			}

			//Does all the reading
			DWORD numBytesRead;
			char* space = NULL;
			while(space == NULL && !killThreadFlag)
			{
				space = ib->canWriteBlock();
			}

			if(finish)
				{
					ib->finishWrite(thisLoopReadBufferSize);
				}

			int readFile = ReadFile(sdrFile, space, thisLoopReadBufferSize, &numBytesRead, NULL);

			if(killThreadFlag)
				return;

			if(numBytesRead != thisLoopReadBufferSize)
				std::cout << "Final Read" <<  numBytesRead << "\n";

			if(!readFile && GetLastError() != ERROR_IO_PENDING)
			{

				printf ("ReadFile failed with error %d.\n", GetLastError());
				
				//attempt to recover
				filePtr--;
				int oldFp = filePtr;
				prepareHandle();
				if(filePtr == oldFp)
				{
					printf ("\nLost access to file and no alternates exist, aborting.\n");
					killThreadFlag = true;
					//read fail,abort!
					return;
				} else {					
					printf ("\nLost access to file but recovered.\n");
					//recovery successful!
					//TODO: test on files > 4 GB (for low part)
					SetFilePointer(sdrFile,bytesRead,NULL,FILE_BEGIN);

				}

			}

			if(readFile){
				//Todo: find out how to abandon this thead for a while.
				ib->doneWritingBlock();

				bytesRead = bytesRead + numBytesRead;

			} else {
				std::cout << "Error, Not a full read"   << std::endl;
			}
			//load in next file
			//(that is, file is done reading and there are more files).
			if(bytesRead == fileSize.QuadPart && filePtr < fnames.size())
			{
				CloseHandle(sdrFile);
				prepareHandle();
				std::cout << "Opening file " << filePtr << " \n";
			}

		}
		
		CloseHandle(sdrFile);
	};

	char* FileReader::getBufferedBytes(int count)
	{
		char* c = NULL;
		while(c == NULL)
		{
			c = ib->tryRead(count);
		}

		return c;

	}

	void FileReader::readAll()
	{
		_beginthread(FileReader::ThreadEntry, 0, this);
	};

	std::string FileReader::fileBeingDecoded()
	{
		return fnames.at(filePtr-1);
	}

	void FileReader::ThreadEntry(void *p)
	{
		((FileReader *) p)->readFile();   
		_endthread();
	}

	void FileReader::doneReading(uint64_t count)
	{
		ib->doneReading(count);
	}

	bool FileReader::hasReadWholeFile(){

		return (bytesRead == fileSize.QuadPart && ib->getNumBytesStored() == 0);
	}

	uint64_t FileReader::numBytesLeftToReadFromFile(){
		return fileSize.QuadPart - bytesRead;
	}

	uint64_t FileReader::numBytesLeftInBuffer(){
		return  ib->getNumBytesStored();
	}

	void FileReader::killReadThread()
	{
		killThreadFlag = true;
	}

	void FileReader::skipBufferedBytes(int count)
	{
		if(count != 0)
		{
			getBufferedBytes(count);
			doneReading(count);
		}
	}

	void FileReader::prepareHandle()
	{
		
		bytesRead = 0;
		std::wstring stemp = std::wstring(fnames.at(filePtr).begin(), fnames.at(filePtr).end());
		LPCWSTR wfname = stemp.c_str();
		boolean fileFound = false;
		
		for(int i = 0; i < pathNameCount; i++)
		{			
			GNSSReader::changeWD(pathNames[i]);
			sdrFile = CreateFile(wfname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

			if(sdrFile == INVALID_HANDLE_VALUE)
			{
				continue;
			}	

			GetFileSizeEx(sdrFile,&fileSize);
			filePtr++;
			fileFound = true;
			break;
		}

		if(!fileFound)
		{
			printf("Data file could not be found");
		}
	}

	double FileReader::getIBufPercent()
	{
		return ib->getPercent();
	}

	FileReader::~FileReader(){
	//	delete ib;
	}


