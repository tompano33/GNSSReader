#include "DecStream.h"
#include "GnssMetadata\Stream.h"
#include <stdint.h>

	DecStream::DecStream(uint32_t sampleCap, std::string id,GnssMetadata::Stream * corStream)
	{
		sampleCapacity = sampleCap;
		sampleBuf = new double[sampleCapacity];
		samplePtr = 0;
		this->id = id;
		this->correspondingStream = corStream;
	};

	DecStream::DecStream()
	{
	
	};

	DecStream::~DecStream()
	{
		delete [] sampleBuf;
	};


	void DecStream::putSample(double sample){
		sampleBuf[samplePtr] = sample;
		samplePtr++;
	};

	std::string DecStream::getID(){
		return id;
	};

	int DecStream::getSamplePtr(){
		return samplePtr;
	};

	double* DecStream::getBuf(){
		return DecStream::sampleBuf;
	};

	int DecStream::getBufSize(){
		return samplePtr;
	}
	void DecStream::clear(){
		samplePtr = 0;
	};

	GnssMetadata::Stream* DecStream::getCorrespondingStream(){
		return correspondingStream;
	};

	void DecStream::setCorrespondingStream(GnssMetadata::Stream* s)
	{
		correspondingStream = s;
	}