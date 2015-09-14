/**
 * File: FileReader.cpp
 * Author: WJLIDDY
 */
#include <iostream>
#include <fstream>
#include <vector>

#ifdef _WIN32
	#include <windows.h>
	#include <process.h>
#else
	//linux
	#include <pthread.h>
#endif

#include <stdio.h>
#include <sys/stat.h>

#include "FileReader.h"
#include "IBuffer.h"
#include "Decoder.h"


	FileReader::FileReader(std::vector<std::string> fnames,uint64_t inBlockSize, uint64_t inBlockCount,const char* origPath,const char** addlPaths,uint64_t pathCount){

		this->_readBufferSize = inBlockSize;

		_ib = new IBuffer(inBlockSize,inBlockCount);
		_killThreadFlag = false;
		this->_fnames = fnames;
		_filePtr = 0;
		_bytesRead = 0;

		_startByteLocation = 0;

		//copy const strings to something that wont go out of scope.
		_pathNames = new char*[pathCount+1];
		_pathNames[0] = strdup(origPath);

		for(int i = 1 ; i < pathCount+1; i++)
		{
			_pathNames[i] = strdup(addlPaths[i-1]);
		}

		_pathNameCount = pathCount+1;
	}

	void FileReader::readFile()
	{
		#ifdef _WIN32
			readFileWin();
		#else
			readFileNix();
		#endif

	};

	char* FileReader::getBufferedBytes(int count)
	{
		char* c = NULL;
		while(c == NULL)
		{


			c = _ib->tryRead(count);
		}

		return c;

	}

	void FileReader::readAll()
	{
		#ifdef _WIN32
			_beginthread(FileReader::ThreadEntry, 0, this);
		#else
			pthread_create (&_readThread, NULL, (void *) FileReader::ThreadEntry, (void *) this);
		#endif
	};

	std::string FileReader::fileBeingDecoded()
	{
		//transitioning
		if(_filePtr == 0)
		{
			return "loading...";
		}
		return _fnames.at(_filePtr-1);
	}

	void FileReader::ThreadEntry(void *p)
	{
		#ifdef _WIN32
 			((FileReader *) p)->readFile();
			_endthread();
		#else
			((FileReader *) p)->readFile();
			pthread_exit(NULL);
		#endif
	}

	void FileReader::doneReading(uint64_t count)
	{
		_ib->doneReading(count);
	}

	//TODO linux
	bool FileReader::hasReadWholeFile(){
		#ifdef _WIN32
		return (_bytesRead == _fileSize.QuadPart && (_ib->getFileReadCount() == _filePtr));
		#else
		return true;
		#endif

	}

	//TODO linux
	uint64_t FileReader::numBytesLeftToReadFromFile(){
		#ifdef _WIN32
		return _fileSize.QuadPart - _bytesRead;
		#else
		return 0;
		#endif
	}

	uint64_t FileReader::numBytesLeftInBuffer(){
		return  _ib->getNumBytesStored();
	}

	void FileReader::killReadThread()
	{
		_killThreadFlag = true;
	}

	void FileReader::skipBufferedBytes(int count)
	{
		if(count != 0)
		{
			getBufferedBytes(count);
			doneReading(count);
		}
	}

	//TODO linux
	void FileReader::prepareHandle()
	{
		#ifdef _WIN32

		_bytesRead = 0;
		std::wstring stemp = std::wstring(_fnames.at(_filePtr).begin(), _fnames.at(_filePtr).end());
		LPCWSTR wfname = stemp.c_str();
		boolean fileFound = false;

		for(int i = 0; i < _pathNameCount; i++)
		{
			Decoder::changeWD(_pathNames[i]);
			_sdrFile = CreateFile(wfname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

			if(_sdrFile == INVALID_HANDLE_VALUE)
			{
				continue;
			}

			GetFileSizeEx(_sdrFile,&_fileSize);
			_filePtr++;
			fileFound = true;
			break;
		}

		if(!fileFound)
		{
			printf("Data file could not be found");
		}

		#endif
	}

	double FileReader::getIBufPercent()
	{
		return _ib->getPercent();
	}

	FileReader::~FileReader(){
	//	delete ib;
	}

	//TODO linux
	uint64_t FileReader::getSizeOfFile(std::string fname)
	{
		#ifdef _WIN32
		std::wstring stemp = std::wstring(fname.begin(), fname.end());
		LPCWSTR wfname =  stemp.c_str();
		HANDLE tempSdrFile;

		for(int i = 0; i < _pathNameCount; i++)
		{
			Decoder::changeWD(_pathNames[i]);
			tempSdrFile = CreateFile(wfname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

			if(tempSdrFile == INVALID_HANDLE_VALUE)
			{
				continue;
			}
			//dword?
			LARGE_INTEGER tempFileSize;
			GetFileSizeEx(tempSdrFile,&tempFileSize);
			CloseHandle(tempSdrFile);
			return tempFileSize.QuadPart;
		}

		//throw error
		printf("Error: Could not find a file \n");
		#endif
		return 0;
	}

	void FileReader::setStartLocation(int loc, uint64_t bytesSkip)
	{
		_filePtr = loc;
		_startByteLocation = bytesSkip;
	}

	uint64_t FileReader::filesFullyReadCount()
	{
		return _ib->getFileReadCount();
	}

	void FileReader::readFileWin()
	{
		#ifdef _WIN32
		//prep first handle
		prepareHandle();

		if(_startByteLocation != 0)
			SetFilePointer(_sdrFile,_startByteLocation,NULL,FILE_BEGIN);

		//Perhaps there is a more elegant way to run and kill the thread, but this works
		while(_bytesRead < _fileSize.QuadPart && !_killThreadFlag)
		{
			uint64_t thisLoopReadBufferSize = _readBufferSize;

			while(_ib->isFinished())
			{;}

			int finish = false;

			if(_readBufferSize >= _fileSize.QuadPart - _bytesRead)
			{
				thisLoopReadBufferSize = _fileSize.QuadPart - _bytesRead;
				finish = true;
			}

			//Does all the reading
			DWORD numBytesRead;
			char* space = NULL;
			while(space == NULL && !_killThreadFlag)
			{
				space = _ib->canWriteBlock();
			}

			if(finish)
				{
					_ib->finishWrite(thisLoopReadBufferSize);
				}

			int readFile = ReadFile(_sdrFile, space, thisLoopReadBufferSize, &numBytesRead, NULL);

			if(_killThreadFlag)
				return;

			if(numBytesRead != thisLoopReadBufferSize)
				std::cout << "Final Read" <<  numBytesRead << "\n";

			if(!readFile && GetLastError() != ERROR_IO_PENDING)
			{

				printf ("ReadFile failed with error %d.\n", GetLastError());

				//attempt to recover
				_filePtr--;
				int oldFp = _filePtr;
				prepareHandle();
				if(_filePtr == oldFp)
				{
					printf ("\nLost access to file and no alternates exist, aborting.\n");
					_killThreadFlag = true;
					//read fail,abort!
					return;
				} else {
					printf ("\nLost access to file but recovered.\n");
					//recovery successful!
					//TODO: test on files > 4 GB (for low part)
					SetFilePointer(_sdrFile,_bytesRead,NULL,FILE_BEGIN);

				}

			}

			if(readFile){
				//Todo: find out how to abandon this thead for a while.
				_ib->doneWritingBlock();

				_bytesRead = _bytesRead + numBytesRead;

			} else {
				std::cout << "Error, Not a full read"   << std::endl;
			}
			//load in next file
			//(that is, file is done reading and there are more files).
			if(_bytesRead == _fileSize.QuadPart && _filePtr < _fnames.size())
			{
				CloseHandle(_sdrFile);
				prepareHandle();
				std::cout << "Opening file " << _filePtr << " \n";
			}

		}

		CloseHandle(_sdrFile);
		#endif
	}

	void FileReader::readFileNix(){
		#ifndef _WIN32
		Decoder::changeWD(_fnames.at(0).c_str());
		std::ifstream sdr (_fnames.front().c_str(), std::ios::in|std::ios::binary|std::ios::ate);

		if(!sdr.is_open())
		{
			std::cout << "Error: file not opened \n";
			return;
		}

		sdr.seekg(0,std::ios::beg);

		struct timespec tim1, tim2;
		tim1.tv_sec = 0;
		tim1.tv_nsec = 5000L;

		//don't kill it with the writes
		while(true)
		{
		//std::cout << "Running!";
		char* space = NULL;

			while(space == NULL)
			{
				space = _ib->canWriteBlock();
			}
		//write here
		sdr.read(space,_readBufferSize);

		nanosleep(&tim1,&tim2);
		//for(int j = 0; i j=

		_ib->doneWritingBlock();
		}
		sdr.close();
	/**

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
		#endif
		*/



		/**
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

	if(startByteLocation != 0)
		ifs.seekg(startByteLocation, ios::beg);
	else
		ifs.seekg(0, ios::beg);

	//Perhaps there is a more elegant way to run and kill the thread, but this works
	while(bytesRead < fileSize.QuadPart && !killThreadFlag)
	{
	}


  //  ifs.read(&result[0], pos);
  */
#endif
}

long FileReader::getFileSizeNix(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}
