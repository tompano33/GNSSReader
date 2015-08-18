/**
 * File: ChunkBuffer.h
 * Author: WJLIDDY
 * Description: Points to a chunk of raw SDR data, and gets individual samples in order based on the encoding.
 * TODO: While the current implementation works fine, I feel like I need to return something with Generic Types rather than making everything 64bit, then casting.
 * Note: The chunkbuffer does not allocate a new buffer, it simply uses the one passed to it from fileReader
 */

#ifndef CHUNKBUFFER_H_H
#define CHUNKBUFFER_H_H
#include <stdint.h>
#include <string>

class ChunkBuffer{

	private:
		//Pointer to a buffer holding the chunk. Not to be modified.
		char* _chunkInputBuffer;	
		//The byte our "cursor" is at in the buffer. Signifies the next byte to read
		uint64_t _bufferBytePointer; 
		//The bit our "cursor" is at in the buffer. Signifies the next bit to read
		uint8_t _bufferBitPointer; 	
		//The size of the buffer
		uint64_t _sizeOfBuffer;	
		//True if the last value decoded was floatingpoint. TODO: Bad choice for a varaible here. There is a more elegant solution.
		bool _wasFloat;

	public:

		// Pass in the size of a buffer, and a pre-allocated buffer with the chunk already inside it*/
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
