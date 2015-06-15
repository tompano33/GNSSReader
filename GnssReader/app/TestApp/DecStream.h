#ifndef DECSTREAM_H_H
#define DECSTREAM_H_H

#include<GnssMetadata/Xml/XmlProcessor.h>

//T representing the size of the sample in this stream.
//using generics was probably a mistake, I guess we will enum or something
//maybe just have an int represent packedbitcount
template <class T>
class DecStream{
	T* sampleBuf;
	uint32_t sampleCapacity;
	uint32_t samplePtr;
	std::string id;

public:
	
	void* correspondingStream;
	DecStream::DecStream(uint32_t sampleCap, std::string id,void * corStream)
	{
		sampleCapacity = sampleCap;
		sampleBuf = new T[sampleCapacity];
		samplePtr = 0;
		this->id = id;
		this->correspondingStream = corStream;
	};


	void putSample(T sample){
		sampleBuf[samplePtr] = sample;
		samplePtr++;
	}

	std::string getID(){
		return id;
	}

};

#endif