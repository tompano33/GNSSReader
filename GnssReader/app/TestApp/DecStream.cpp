#include "DecStream.h"
#include <stdint.h>

	DecStream::DecStream(uint32_t sampleCap, std::string id,void * corStream)
	{
		sampleCapacity = sampleCap;
		sampleBuf = new char[sampleCapacity];
		samplePtr = 0;
		this->id = id;
		this->correspondingStream = corStream;
	};

	DecStream::DecStream()
	{
	};


	void DecStream::putSample(char sample){
		sampleBuf[samplePtr] = sample;
		samplePtr++;
	};

	std::string DecStream::getID(){
		return id;
	};

	int DecStream::getSamplePtr(){
		return samplePtr;
	};

	char* DecStream::getBuf(){
		return DecStream::sampleBuf;
	};

	int DecStream::getBufSize(){
		return samplePtr;
	}
	void DecStream::clear(){
		samplePtr = 0;
	};

	void* DecStream::getCorrespondingStream(){
		return correspondingStream;
	};