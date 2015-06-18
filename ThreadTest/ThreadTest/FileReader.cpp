#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "FileReader.h"
#include <process.h>
#include "IBuffer.h"


volatile int bytesRead = 0;
int fileSize = 36;

//Reads an SDR File.
class FileReader{
	HANDLE sdrFile;
	DWORD dwBytesRead, dwBytesWritten, dwPos;
	const static unsigned long readBufferSize = 3;
	char* buff;
	//OVERLAPPED olap;
public:
	//Creates the file.
	FileReader::FileReader(){
		LPCWSTR fname = L"C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\ThreadTest.txt";
		buff = new char[readBufferSize];
		sdrFile = CreateFile(
		fname, //filename.
		GENERIC_READ, // open for reading
		0, // do not share
		NULL, // default security
		OPEN_EXISTING, // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL); // no attribute template

		//ZeroMemory(&olap, sizeof(OVERLAPPED));



		if(sdrFile == INVALID_HANDLE_VALUE)
		{
			printf("Could not open %S.\n", fname);
			CloseHandle(sdrFile);
			std::cin.get();
		}
	}

	void readFile(void* tb)
	{
			int readFile = ReadFile(sdrFile, buff, readBufferSize, &dwBytesRead, NULL);

			 if(!readFile && GetLastError() != ERROR_IO_PENDING)
				printf ("ReadFile failed with error %d.\n", GetLastError());
			if(readFile){
				if(((IBuffer*)tb)->write(buff,readBufferSize))
					std::cout << "Read All! " << bytesRead << " / " << fileSize << std::endl;
			} else {
				if(((IBuffer*)tb)->write(buff,dwBytesRead))
					std::cout << "Read Some " << bytesRead << " / " << fileSize << std::endl;
			}

	};

	void close(){
	//close file
	if(CloseHandle(sdrFile) != 0)
		printf("file handle closed successfully!\n");

	}
};

void readBuf(void* tb)
{
	while(bytesRead < fileSize)
	{
		
		char* foobuf = new char[10];
		if(((IBuffer*)tb)->read(2,foobuf))
		{
			bytesRead = bytesRead + 2;
			std::cout<< "Got a read! ";
			std::cout<< foobuf[0];
			std::cout<< foobuf[1];
			std::cout<< "\n";
		}
	}
};

void readFile(void* params){
	void** oparams = ((void**)params);
	FileReader* fr = ((FileReader*)&oparams[0]); 	
	IBuffer* tb = ((IBuffer*)&oparams[1]); 
	while(bytesRead < fileSize)
	{
		fr->readFile(tb);
	}

}

int main(void)
{
	FileReader* fr = new FileReader();
	IBuffer* tb = new IBuffer();
	_beginthread(readBuf,0,tb);
	//somethign abt overlap
	void** params = new void*[2];
	params[0] = fr;
	params[1] = tb;
	//fails when multithreaded
	//_beginthread(readFile,0,params);

	while(true){
		std::cin.get();
		fr->readFile(tb);
	}

	fr->close();
	return 0;

}


