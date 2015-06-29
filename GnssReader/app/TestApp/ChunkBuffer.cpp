/**
 * File: ChunkBuffer.cpp
 * Author: WJLIDDY
 */
#include "ChunkBuffer.h"

#include<cstdint>
#include<iostream>
#include<list>
#include<stdio.h>

#include<GnssMetadata/Metadata.h>
#include<GnssMetadata/Xml/XmlProcessor.h>


using namespace GnssMetadata;

	ChunkBuffer::ChunkBuffer(uint64_t size, char* buffer) 
	{
		sizeOfBuffer = size;
		chunkInputBuffer = buffer;
		bufferBytePointer = 0;
		bufferBitPointer = 0;
	};


	int64_t ChunkBuffer::readBits(uint8_t bitsToRead, char* encoding)
	{
		uint8_t totalBitCount = bitsToRead;
		uint64_t sampleValue = 0;	//value that will be returned
		while(bitsToRead > 0)
		{
			if(bufferBitPointer == 0)
			{
				if(bitsToRead >=8)
				{
					//Since our buffer pointer is at 0, and we have at least 8 bytes to read, 
					//we can just shift the return value left by 8 and add.
					sampleValue = sampleValue << 8;
					sampleValue = sampleValue + chunkInputBuffer[bufferBytePointer];
					bufferBytePointer++;
					bitsToRead = bitsToRead - 8;
				} else 
				{
					//There are less than 8 bits, but our pointer is still at zero.
					//make room in the sample value for some new bits.
					sampleValue = sampleValue << bitsToRead;
					//The bits will be at the beginning of the buffer, so we can simply shift them right.
					sampleValue = sampleValue + (chunkInputBuffer[bufferBytePointer] >> (8-bitsToRead));
					bufferBitPointer = bufferBitPointer + bitsToRead;
					bitsToRead = 0;
				}
			} else
			{
				//Here, the buffer-bit-pointer is not at zero. 
				//So, we cannot take any shortcuts.
				int remainingBits = 8 - bufferBitPointer;

				if(bitsToRead >= remainingBits)
				{
					//Here, we have more bits to read than there are in the buffer.
					//So, take the rest of the sample, put it in return value.
					sampleValue = sampleValue << remainingBits;
					sampleValue = sampleValue + (chunkInputBuffer[bufferBytePointer] & (0xFF >> bufferBitPointer));
					bufferBytePointer++;
					bufferBitPointer = 0;
					bitsToRead -= (remainingBits);
				} else 
				{
					//The bit pointer is zero, and we don't want to read the bit twice. 
					//So we will have to shift both ways to remove undesired bits from the buffer.
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

		//Now our sample value has the correct # bits and we can return it. 
		//int64_t will be just have to be cast.
		return sampleValue;
	};

	void ChunkBuffer::skipBits(uint8_t bitsToRead)
	{
		uint8_t totalBitCount = bitsToRead;
		while(bitsToRead > 0)
		{
			if(bufferBitPointer == 0)
			{
				if(bitsToRead >=8)
				{
					bufferBytePointer++;
					bitsToRead = bitsToRead - 8;
				} else 
				{
					bufferBitPointer = bufferBitPointer + bitsToRead;
					bitsToRead = 0;
				}
			} else
			{
				int remainingBits = 8 - bufferBitPointer;

				if(bitsToRead >= remainingBits)
				{
					bufferBytePointer++;
					bufferBitPointer = 0;
					bitsToRead -= (remainingBits);
				} else 
				{
					bufferBitPointer += bitsToRead;
					bitsToRead = 0;
				}
			}
		}
	};

	ChunkBuffer::~ChunkBuffer(){
		delete [] chunkInputBuffer;
	}

	bool ChunkBuffer::chunkFullyRead(){
		return ((int)bufferBytePointer == (int)sizeOfBuffer);
	};

