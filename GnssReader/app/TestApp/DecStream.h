/**
 * File: DecStream.h
 * Author: WJLIDDY
 * Description: A buffer to store decoded SDR samples from a single stream.
 */
#ifndef DECSTREAM_H_H
#define DECSTREAM_H_H

#include<stdint.h>
#include <windows.h>
#include<GnssMetadata/Xml/XmlProcessor.h>


class DecStream{

	double* sampleBuf1;			//A buffer for samples
	double* sampleBuf2;			//A buffer for samples
	bool lockSampleBuf1;
	uint64_t sampleCapacity;	//Maximum capacity of buffer (it's size)
	volatile uint64_t samplePtr;			//Where we are pointing in the buffer.
	std::string id;				//Name of this buffer's stream. TODO: Do I need this?
	bool done;
	bool hasCPart;
	bool CPartFirst;

	//Band?
	uint64_t sampleRate;

	//Pointer to the XML object this stream represents.
	//This is important, because when I am ready to put a sample in a stream, 
	//I compare the address of the XMLstream to this field in the DecStream 
	//If they are the same I know this is the right buffer.
	GnssMetadata::Stream* correspondingStream;

	//do not write and read
	CRITICAL_SECTION crit; 


public:
	
	//Pass in a sample capacity, an ID, and a stream. Creates a buffer. TODO: id???
	DecStream::DecStream(uint64_t sampleCap, std::string id,GnssMetadata::Stream* corStream,bool hasComplexPart, bool complexPartFirst);

	//Puts a sample in the buffer. Only supports doubles for now
	void putSample(double sample);
	//returns ID of stream
	std::string getID();
	//resets the samplePtr, that is to say, resets the buffer.
	void clear();
	//returns correspoding XML stream.
	GnssMetadata::Stream* getCorrespondingStream();
	//Sets the corresponding XML stream. Needed when entering a new XML file and the reference to the old stream is lost.
	void setCorrespondingStream(GnssMetadata::Stream* s);
	//Flushes buffer. returns reference.
	double* flushOutputStream(uint64_t* byteCount);
	//returns buffer size
	uint64_t getBufSize();

	bool hasComplexPart();
	bool complexPartFirst();

	~DecStream();
};

#endif