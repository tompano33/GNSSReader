/**
 * File: ChunkBuffer.h
 * Author: WJLIDDY
 */
#ifndef CHUNKBUFFER_H_H
#define CHUNKBUFFER_H_H

class ChunkBuffer{
	//The buffer holding the chunk.
	char* chunkInputBuffer;
	//The byte our "cursor" is at in the buffer
	uint16_t bufferBytePointer;
	//The bit our "cursor" is at.
	uint8_t bufferBitPointer;
	//size of buffer.
	uint16_t sizeOfBuffer;


public:
	ChunkBuffer::ChunkBuffer(uint32_t size, char* buf);
	int64_t readBits(int bitsToRead, char* encoding);
	bool chunkFullyRead();
	ChunkBuffer::~ChunkBuffer();
};


#endif
