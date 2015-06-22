#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "FileReader.h"
#include <process.h>
#include "IBuffer.h"

//Reads an SDR File. Puts the file contents in it's intermediateBuffer

	//Needs the name of the file and a count of how many bytes to load from the file. Also needs the size of intermediate buffer. Puts file handle in memory. Inits buffer.
	FileReader::FileReader(LPCWSTR fname,long readBufferSize, long intermediateBufferSize){
		this->readBufferSize = readBufferSize;
		ib = new IBuffer(intermediateBufferSize);
		buff = new char[readBufferSize];
		bytesRead = 0;
		killThreadFlag = false;

		sdrFile = CreateFile(fname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

		if(sdrFile == INVALID_HANDLE_VALUE)
		{
			printf("Could not open %S.\n", fname);
			CloseHandle(sdrFile);
			std::cin.get();
		}	
		GetFileSizeEx(sdrFile,&fileSize);
	}

		//Tries to populate intermediate buffer with samples. To be called as a thread.
	void FileReader::readFile()
	{
		//More elegant way to kill thread?
		while(bytesRead < fileSize.QuadPart && !killThreadFlag)
		{
			if(readBufferSize > fileSize.QuadPart - bytesRead)
				readBufferSize = fileSize.QuadPart - bytesRead;
			DWORD i;
			int readFile = ReadFile(sdrFile, buff, readBufferSize, &i, NULL);

			if(!readFile && GetLastError() != ERROR_IO_PENDING)
					printf ("ReadFile failed with error %d.\n", GetLastError());

			if(readFile){
				while(!ib->write(buff,readBufferSize) && !killThreadFlag){;}
				bytesRead = bytesRead + readBufferSize;
			//	std::cout << "Read All! " << bytesRead << " / " << fileSize.QuadPart << std::endl;
			} else {
				std::cout << "Error, Not a full read"   << std::endl;
			}
		}
		std::cout << "Reading Ended.\n";
	};

	//returns true if bytes were put in buffer, false otherwise
	void FileReader::getBufferedBytes(char* b, int count)
	{
		ib->read(count,b);
	}
	void FileReader::close(){
	//close file
		if(CloseHandle(sdrFile) != 0)
			printf("file handle closed successfully!\n");
	};

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

	long FileReader::numBytesLeftToReadFromFile(){
		return fileSize.QuadPart - bytesRead;
	}

	long FileReader::numBytesLeftInBuffer(){
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

	FileReader::~FileReader(){
		delete [] buff;
		delete ib;
		CloseHandle(sdrFile);
	}

int main2(void)
{
	FileReader* fr = new FileReader(L"C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\ThreadTest.txt",6L,16L);
	//fr->readFile();
	char* d = new char[5];
	fr->getBufferedBytes(d,5);
	std::cout << d[0] << d[1] << d[2] << d[3] << d[4] << d[5];
	std::cin.get();
	return -1;
}


