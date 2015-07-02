/**
 * File: ChunkBuffer.h
 * Author: WJLIDDY
 * Description: Stores a chunk of raw SDR data, and gives individual samples based on the requested encoding.
 */
#ifndef CHUNKBUFFER_H_H
#define CHUNKBUFFER_H_H
#include <stdint.h>
#include <string>

class ChunkBuffer{

	char* chunkInputBuffer;		//The buffer holding the chunk.
	uint64_t bufferBytePointer; //The byte our "cursor" is at in the buffer
	uint8_t bufferBitPointer; 	//The bit our "cursor" is at in the buffer, relative to the byte.
	uint64_t sizeOfBuffer;		//The size of the buffer.

public:

	/** Pass in the size of a buffer, and a pre-allocated buffer with the chunk already defined */
	ChunkBuffer::ChunkBuffer(uint64_t size, char* buf); 

	/** Given a number of bitsToRead and an encoding, pops a sample from the buffer, formats it to the encoding, returns it */
	int64_t readBits(uint8_t bitsToRead, std::string  encoding);

	/** Returns true if every value in the chunkBuffer has been read */
	bool chunkFullyRead();

	void skipBits(uint8_t bitsToSkip);

	/** Deallocates chunkInputBuffer */
	ChunkBuffer::~ChunkBuffer();
};

#endif
