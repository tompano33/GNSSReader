/**
 * File: ChunkBuffer.h
 * Author: WJLIDDY
 * Description: Points to a chunk of raw SDR data, and returns individual samples in order based on the requested encoding.
 * TODO: While the current implementation works fine, I feel like I need to return something with Generic Types rather than making everything 64bit.
 */

#ifndef CHUNKBUFFER_H_H
#define CHUNKBUFFER_H_H
#include <stdint.h>
#include <string>

class ChunkBuffer{

	//Pointer to buffer holding the chunk. Not to be modified.
	char* chunkInputBuffer;	
	//The byte our "cursor" is at in the buffer; the next byte to read
	uint64_t bufferBytePointer; 
	//The bit our "cursor" is at in the buffer; the next bit to read
	uint8_t bufferBitPointer; 	
	//The size of the buffer
	uint64_t sizeOfBuffer;	
	//returns true if the last value decoded was floatingpoint.
	bool wasFloat;

public:

	/** Pass in the size of a buffer, and a pre-allocated buffer with the chunk already defined */
	ChunkBuffer(uint64_t size, char* buf); 

	/** Given a number of bitsToRead and an encoding, pops a sample from the buffer, formats it to the encoding, returns it */
	int64_t readBits(uint8_t bitsToRead, std::string  encoding);

	/** Returns true if every value in the chunkBuffer has been read */
	bool chunkFullyRead();

	/** Skips over bits in the buffer */
	void skipBits(uint8_t bitsToSkip);

	/** returns true if the last sample should be read as a float */
	bool wasSampleFloat();
};

#endif
