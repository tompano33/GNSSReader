/**
 * File: DecStream.h
 * Author: WJLIDDY
 * Description: A double buffer to store decoded SDR samples from a single stream.
 */
#ifndef DECSTREAM_H_H
#define DECSTREAM_H_H

#include<stdint.h>

//For critical sections
#ifdef _WIN32
	#include <windows.h>
#endif

#include<GnssMetadata/Xml/XmlProcessor.h>


class DecStream{

	//TODO: Consider getting rid of a double-buffered approach, or having an option to choose the buffer count.
	//Advantage is destructible data for in-place transforms, but takes more memory

	private: 

		//one buffer for samples
		double* _sampleBuf1;			
		//other buffer for samples
		double* _sampleBuf2;	
		//true if sample buffer 1 is locked and samples are not to be put in it. If false, sampleBuf2 is locked.
		bool _lockSampleBuf1;
		//Maximum capacity of each buffer (it's size)
		uint64_t _sampleCapacity;	
		//Where we are pointing in the buffer. Refers to the unlocked one.
		volatile uint64_t _samplePtr;	
		//Name of this buffer's stream. TODO: Do I need this? I don't think I ever call it
		std::string _id;	
		//True if the sample has a complex part; it is IQ or QI data.
		bool _hasCPart;
		//True if the complex part comes first, it is QI data.
		bool _CPartFirst;

		//TODO:
		//sample rate of this stream in Hz
		uint64_t _sampleRate;
		//center frequency of this stream in Hz
		uint64_t _centerFreq;
		//intermediate frequency of this stream in HZ
		uint64_t _interFreq;

		//Pointer to the XML Stream this stream buffer represents.
		//This is important, because when I am ready to put a sample in a stream, 
		//I compare the address of the XMLstream to this field in the DecStream 
		//If they are the same I know this is the right buffer to write to..
		GnssMetadata::Stream* _correspondingStream;

		//Thread stuff based on Windows or not.
		#ifdef _WIN32
			CRITICAL_SECTION _crit; 
		#else 
			pthread_mutex_t _mutex;
		#endif

	public:
	
		//Pass in a sample capacity, an ID, and a stream. Creates a buffer. TODO: id???
		DecStream(uint64_t sampleCap, std::string id,GnssMetadata::Stream* corStream,bool hasComplexPart, bool complexPartFirst);
		//Puts a sample in the buffer. Only supports doubles for now
		void putSample(double sample);
		//returns ID of stream
		std::string getID();
		//returns correspoding XML stream.
		GnssMetadata::Stream* getCorrespondingStream();
		//Sets the corresponding XML stream. Needed when entering a new XML file and the reference to the old stream is lost.
		void setCorrespondingStream(GnssMetadata::Stream* s);
		//Flushes buffer. returns reference.
		double* flushOutputStream(uint64_t* byteCount);
		//returns buffer size
		uint64_t getBufSize();
		//true if IQ or QI data
		bool hasComplexPart();
		//if hasComplexPart() and complexPartFirst(), QI data. if hasComplexPart() and complexPartFirst(), IQ data.
		bool complexPartFirst();
		//locks the mutex, or critical section.
		void lockMutex();
		//unlocks the mutex, or critical section.
		void unlockMutex();
		//sets the sample, center, tranlated frequency.
		void setFreqs(uint64_t,uint64_t,uint64_t);
		//get sample rate
		uint64_t getSampleRate();
		//get center frequency
		uint64_t getCenterFreq();
		//get intermediate frequency
		uint64_t getInterFreq();

		~DecStream();
};

#endif
