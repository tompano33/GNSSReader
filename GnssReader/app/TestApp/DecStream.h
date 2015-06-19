#ifndef DECSTREAM_H_H
#define DECSTREAM_H_H

#include<GnssMetadata/Xml/XmlProcessor.h>
#include<stdint.h>

//Puts values in a stream.
class DecStream{
	//Buffer full of samples
	double* sampleBuf;
	uint32_t sampleCapacity;
	uint32_t samplePtr;
	//Name of Buffer
	std::string id;
	//Pointer to the XMLID of the stream.
	void* correspondingStream;


public:
	
	DecStream::DecStream(uint32_t sampleCap, std::string id,void * corStream);
	DecStream::DecStream();
	void putSample(double sample);
	std::string getID();
	int getSamplePtr();
	double* getBuf();
	void clear();
	void* getCorrespondingStream();
	int getBufSize();
	~DecStream();
};

#endif