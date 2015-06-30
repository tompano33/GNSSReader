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

	FileReader::FileReader(std::vector<std::string> fnames,uint64_t readBufferSize, uint64_t intermediateBufferSize,const char* origPath,const char** addlPaths,uint64_t pathCount){
		this->readBufferSize = readBufferSize;
		ib = new IBuffer(intermediateBufferSize);
		buff = new char[readBufferSize];
		killThreadFlag = false;
		this->fnames = fnames;
		filePtr = 0;

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

	//Assumes that sdrFile, buff both defined.
	void FileReader::readFile()
	{
		//Perhaps there is a more elegant way to run and kill the thread, but this works
		while(bytesRead < fileSize.QuadPart && !killThreadFlag)
		{
			//don't overread
			if(readBufferSize > fileSize.QuadPart - bytesRead)
				readBufferSize = fileSize.QuadPart - bytesRead;

			//Does all the reading
			DWORD i;
			int readFile = ReadFile(sdrFile, buff, readBufferSize, &i, NULL);

			if(!readFile && GetLastError() != ERROR_IO_PENDING)
				printf ("ReadFile failed with error %d.\n", GetLastError());

			if(readFile){
				//Todo: find out how to abandon this thead for a while.
				while(!ib->write(buff,readBufferSize) && !killThreadFlag){;}
				bytesRead = bytesRead + readBufferSize;
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

	void FileReader::getBufferedBytes(char* b, int count)
	{
		ib->read(count,b);
	}


	void FileReader::readAll()
	{
		_beginthread(FileReader::ThreadEntry, 0, this);
	};

	void FileReader::ThreadEntry(void *p)
	{
		((FileReader *) p)->readFile();   
		_endthread();
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
		ib->skip(count);
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
			std::cin.get();
		}
	}

	FileReader::~FileReader(){
		delete [] buff;
		delete ib;
	}


