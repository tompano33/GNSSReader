/**
 * File: DecStream.h
 * Author: WJLIDDY
 * Description: A buffer to store decoded SDR samples from a single stream.
 */
#ifndef DECSTREAM_H_H
#define DECSTREAM_H_H

#include<stdint.h>
#include<GnssMetadata/Xml/XmlProcessor.h>

class DecStream{

	double* sampleBuf;			//A buffer for samples
	uint64_t sampleCapacity;	//Maximum capacity of buffer (it's size)
	uint64_t samplePtr;			//Where we are pointing in the buffer.
	std::string id;				//Name of this buffer's stream. TODO: Do I need this?
	
	//Pointer to the XML object this stream represents.
	//This is important, because when I am ready to put a sample in a stream, 
	//I compare the address of the XMLstream to this field in the DecStream 
	//If they are the same I know this is the right buffer.
	GnssMetadata::Stream* correspondingStream;

public:
	
	//Pass in a sample capacity, an ID, and a stream. Creates a buffer. TODO: id???
	DecStream::DecStream(uint64_t sampleCap, std::string id,GnssMetadata::Stream* corStream);

	//Puts a sample in the buffer. Only supports doubles for now
	void putSample(double sample);
	//returns ID of stream
	std::string getID();
	//Points to the next open spot in the decoded stream.
	uint64_t getSamplePtr();
	//returns a pointer to the buffer. Only supports double sample sizes.
	double* getBuf();
	//resets the samplePtr, that is to say, resets the buffer.
	void clear();
	//returns correspoding XML stream.
	GnssMetadata::Stream* getCorrespondingStream();
	//Sets the corresponding XML stream. Needed when entering a new XML file and the reference to the old stream is lost.
	void setCorrespondingStream(GnssMetadata::Stream* s);
	//returns buffer size
	uint64_t getBufSize();
	~DecStream();
};

#endif