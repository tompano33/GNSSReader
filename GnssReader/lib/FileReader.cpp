/**
 * File: FileReader.cpp
 * Author: WJLIDDY
 */
#include <iostream>
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
#include "GnssReader.h"


	FileReader::FileReader(std::vector<std::string> fnames,uint64_t inBlockSize, uint64_t inBlockCount,const char* origPath,const char** addlPaths,uint64_t pathCount){
	
		this->readBufferSize = inBlockSize;

		ib = new IBuffer(inBlockSize,inBlockCount);
		killThreadFlag = false;
		this->fnames = fnames;
		filePtr = 0;
		bytesRead = 0;

		startByteLocation = 0;

		//copy const strings to something that wont go out of scope.
		pathNames = new char*[pathCount+1];
		pathNames[0] = strdup(origPath);

		for(int i = 1 ; i < pathCount+1; i++)
		{
			pathNames[i] = strdup(addlPaths[i-1]);
		}

		pathNameCount = pathCount+1;
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
			c = ib->tryRead(count);
		}

		return c;

	}

	void FileReader::readAll()
	{
		#ifdef _WIN32
			_beginthread(FileReader::ThreadEntry, 0, this);
		#else 
			pthread_create (&readThread, NULL, (void *) FileReader::ThreadEntry, (void *) &this);
		#endif
	};

	std::string FileReader::fileBeingDecoded()
	{
		//transitioning
		if(filePtr == 0)
		{
			return "loading...";
		}
		return fnames.at(filePtr-1);
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
		ib->doneReading(count);
	}

	//TODO linux
	bool FileReader::hasReadWholeFile(){
		#ifdef _WIN32
		return (bytesRead == fileSize.QuadPart && (ib->getFileReadCount() == filePtr));
		#else
		return true;
		#endif
		
	}

	//TODO linux
	uint64_t FileReader::numBytesLeftToReadFromFile(){
		#ifdef _WIN32
		return fileSize.QuadPart - bytesRead;
		#else
		return 0;
		#endif
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

	//TODO linux
	void FileReader::prepareHandle()
	{
		#ifdef _WIN32

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

		#endif
	}

	double FileReader::getIBufPercent()
	{
		return ib->getPercent();
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

		for(int i = 0; i < pathNameCount; i++)
		{			
			GNSSReader::changeWD(pathNames[i]);
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
		filePtr = loc;
		startByteLocation = bytesSkip;
	}

	uint64_t FileReader::filesFullyReadCount()
	{
		return ib->getFileReadCount();
	}

	void FileReader::readFileWin()
	{
		#ifdef _WIN32
		//prep first handle
		prepareHandle();

		if(startByteLocation != 0)
			SetFilePointer(sdrFile,startByteLocation,NULL,FILE_BEGIN);

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
	}

	void FileReader::readFileNix(){

		//test for Tom
		
		while(true)
		{
		double sin(double x);
		}
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
}
	
long FileReader::getFileSizeNix(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}



