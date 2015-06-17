#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "FileReader.h"

//try std::vector?
class TestBuffer{
	static const int BUFFERSIZE = 7;
	//The intermediate buffer between the raw file on disk and the decoder
	char* ibuf;
	//Pointer to where we may write new bytes to the buffer if there is space.
	int bufPtr;
	//Pointer to where the start of the oldest data is in the buffer
	int oldPtr;
	//return buffer for when we want to read bytes.
	char* returnbuf;
	//count of all the bytes that are stored
	volatile int numBytesStored;

public:
	TestBuffer::TestBuffer(){
		ibuf = new char[BUFFERSIZE];
		bufPtr = 0;
		oldPtr = 0;
		numBytesStored = 0;
	}

	bool write(char* bytes, int count)
	{
		//Not enough room to put bytes!
		if((BUFFERSIZE-numBytesStored) < count){
			return false;
		} else
		{
			numBytesStored += count;

			if(count < (BUFFERSIZE - bufPtr) ) 
			{
				memcpy(ibuf+bufPtr, bytes, count);
				bufPtr += count;
			}
			else
			{
				int spaceLeftAtEnd = (BUFFERSIZE - bufPtr);
				memcpy(ibuf+bufPtr,bytes,spaceLeftAtEnd);
				memcpy(ibuf,bytes+spaceLeftAtEnd,count-spaceLeftAtEnd);
				bufPtr = count-spaceLeftAtEnd;
			}
			return true;
		}
	}

	bool read(int size, char* buf){
		if(size > numBytesStored)
		{
			return false;
		} else
		{
			numBytesStored -=size;
			if(size < (BUFFERSIZE - oldPtr))
			{
				memcpy(buf,ibuf+oldPtr,size);
				oldPtr+= size;
			} else 
			{
				int spaceLeftAtEnd = (BUFFERSIZE - oldPtr);
				memcpy(buf,ibuf+oldPtr,spaceLeftAtEnd);
				memcpy(buf+spaceLeftAtEnd,ibuf,size-spaceLeftAtEnd);
				oldPtr=size-spaceLeftAtEnd;
			}
			return true;
		}
	}

	~TestBuffer(){
		delete [] ibuf;
	}
};



FileReader::FileReader(void)
{
}


FileReader::~FileReader(void)
{

}


int main(void)
{

	HANDLE sdrFile;
	DWORD dwBytesRead, dwBytesWritten, dwPos;
	dwBytesRead = 0;
	const unsigned long fileSize = 36;
	const unsigned long readBufferSize = 3;
	unsigned long bufferPointer;
	int bytesRead = 0;
	LPCWSTR fname = L"C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\ThreadTest.txt";
	char buff[readBufferSize];
	// Open the existing file.

	sdrFile = CreateFile(
	fname, //filename.
	GENERIC_READ, // open for reading
	0, // do not share
	NULL, // default security
	OPEN_EXISTING, // existing file only
	FILE_ATTRIBUTE_NORMAL, // normal file
	NULL); // no attribute template

	if(sdrFile == INVALID_HANDLE_VALUE)
	{
		printf("Could not open %S.\n", fname);
		CloseHandle(sdrFile);
		std::cin.get();
		return 0;
	}

	TestBuffer* tb = new TestBuffer();
	char* foobuf = new char[10];
	while(bytesRead < fileSize){
		bool readFile = ReadFile(sdrFile, buff, readBufferSize, &dwBytesRead, NULL);
		if(readFile){
			while(!tb->write(buff,readBufferSize)){
				tb->read(2,foobuf);
				bytesRead += 2;
				std::cout<< foobuf[0];
				std::cout<< foobuf[1];
				std::cin.get();
			}

			std::cout << "Read All! " << bytesRead << " / " << fileSize << std::endl;
		} else {
			while(!tb->write(buff,dwBytesRead));
			bytesRead += dwBytesRead;
			std::cout << "Read Some " << bytesRead << " / " << fileSize << std::endl;
		}
	} 

	//close file
	if(CloseHandle(sdrFile) != 0)
		printf("\n%S file handle closed successfully!\n", fname);


	std::cin.get();
	return 0;

}