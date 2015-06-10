/**
 * File: ChunkBuffer.cpp
 * Author: W.J. Liddy
 * Allocates a buffer for a chunk and allows pulling samples of various sizes out.
 */

#include<GnssMetadata/Metadata.h>
#include<GnssMetadata/Xml/XmlProcessor.h>
#include<iostream>
#include<list>
#include<cstdint>
#include<stdio.h>
#include "ChunkBuffer.h"

using namespace GnssMetadata;
//Immediately allocates a buffer and puts data in it.
	ChunkBuffer::ChunkBuffer(uint8_t sizeWord, uint8_t countWord, FILE* sdrFile) 
	{
		sizeOfBuffer = sizeWord*countWord;
		chunkInputBuffer = new uint8_t[sizeOfBuffer];
		fread(chunkInputBuffer, sizeWord, countWord, sdrFile);
		bufferBytePointer = 0;
		bufferBitPointer = 0;
	};

	//Reads a sample from the chunkbuffer.
	//TODO throw error if out of bytes to read.
	uint64_t ChunkBuffer::readBits(int bitsToRead, char* encoding)
	{

		uint64_t sampleValue = 0;
		while(bitsToRead > 0)
		{
			if(bufferBitPointer == 0)
			{
				if(bitsToRead >=8)
				{
					//just write a whole byte.
					sampleValue = sampleValue << 8;
					sampleValue = sampleValue + chunkInputBuffer[bufferBytePointer];
					bufferBytePointer++;
					bitsToRead = bitsToRead - 8;
				} else {
					//there are less than 8 bits
					//make room in the sample value for some new bits
					sampleValue = sampleValue << bitsToRead;
					//The bits will be at the beginning, simply shift them right.
					sampleValue = sampleValue + (chunkInputBuffer[bufferBytePointer] >> (8-bitsToRead));
					bufferBitPointer = bufferBitPointer + bitsToRead;
					bitsToRead = 0;
				}
			} else
			{
				int remainingBits = 8 - bufferBitPointer;
				if(bitsToRead >= remainingBits)
				{
					//read the whole rest of byte! 
					sampleValue = sampleValue << remainingBits;
					sampleValue = sampleValue + (chunkInputBuffer[bufferBytePointer] & (0xFF >> bufferBitPointer));
					bufferBytePointer++;
					bufferBitPointer = 0;
					bitsToRead -= (remainingBits);
				} else 
				{
					//we are offset and can't just read a whole byte
					sampleValue = sampleValue << bitsToRead;
					//gets rid of "used" part of byte.
					uint8_t bitsToMask = chunkInputBuffer[bufferBytePointer] ;
					bitsToMask &= (0xFF >> bufferBitPointer);
					bitsToMask = bitsToMask >> (remainingBits - bitsToRead);
					sampleValue = sampleValue + bitsToMask;
					bufferBitPointer += bitsToRead;
					bitsToRead = 0;
				}
			}
		}

		return sampleValue;
	};

	//returns true if the chunk is out of bytes to read.
	bool ChunkBuffer::chunkFullyRead(){
		return ((int)bufferBytePointer == (int)sizeOfBuffer);
	};

	//There must be a better way to do this. Destructor perhaps?
	void ChunkBuffer::freeBuffer()
	{
		delete [] chunkInputBuffer;
	};

