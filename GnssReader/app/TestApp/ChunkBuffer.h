/**
 * File: ChunkBuffer.h
 * Author: WJLIDDY
 */
#ifndef CHUNKBUFFER_H_H
#define CHUNKBUFFER_H_H

class ChunkBuffer{
	//The buffer holding the chunk.
	uint8_t* chunkInputBuffer;
	//The byte our "cursor" is at in the buffer
	uint16_t bufferBytePointer;
	//The bit our "cursor" is at.
	uint8_t bufferBitPointer;
	//size of buffer.
	uint16_t sizeOfBuffer;


public:
	ChunkBuffer::ChunkBuffer(uint8_t sizeWord, uint8_t countWord, FILE* sdrFile);
	uint64_t readBits(int bitsToRead, char* encoding);
	bool chunkFullyRead();
	ChunkBuffer::~ChunkBuffer();
};


#endif
