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
	GnssMetadata::Stream* correspondingStream;


public:
	
	DecStream::DecStream(uint32_t sampleCap, std::string id,GnssMetadata::Stream* corStream);
	DecStream::DecStream();
	void putSample(double sample);
	std::string getID();
	int getSamplePtr();
	double* getBuf();
	void clear();
	GnssMetadata::Stream* getCorrespondingStream();
	void setCorrespondingStream(GnssMetadata::Stream* s);
	int getBufSize();
	~DecStream();
};

#endif