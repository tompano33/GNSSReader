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


	//I need to test 64 bit values to make sure nothing breaks.
	int64_t ChunkBuffer::readBits(uint8_t bitsToRead, std::string  encoding)
	{
		uint8_t totalBitCount = bitsToRead;
		int64_t sampleValue = 0;	//value that will be returned


		
		//std::cout << "Starting" << std::endl;
		while(bitsToRead > 0)
		{
			
			//points to next readable bit
			if(bufferBitPointer == 0) 
			{
				if(bitsToRead >=8)
				{
					//Since our buffer pointer is at 0, and we have at least 8 bytes to read, 
					//we can just shift the return value left by 8 and add.
					sampleValue = sampleValue << 8;
					sampleValue |= ((uint8_t)chunkInputBuffer[bufferBytePointer]);
					bufferBytePointer++;
					bitsToRead = bitsToRead - 8;
				} else 
				{
					//std::cout << "(" << (int)bitsToRead << ")" << std::endl;
					//There are less than 8 bits, but our pointer is still at zero.
					//make room in the sample value for some new bits.
					sampleValue = sampleValue << bitsToRead;
					//The bits will be at the beginning of the buffer, so we can simply shift them right.
					sampleValue |= ((uint8_t)((uint8_t)(chunkInputBuffer[bufferBytePointer]) >> (8 - bitsToRead)));
					bufferBitPointer = bufferBitPointer + bitsToRead;
					bitsToRead = 0;
				}
			} else
			{
				//Here, the buffer-bit-pointer is not at zero. 
				//So, we cannot take any shortcuts.
				uint8_t remainingBits = 8 - bufferBitPointer;

				if(bitsToRead >= remainingBits)
				{
					//Is plus dangerous with 64 bits?
					//Here, we have more bits to read than there are in the buffer.
					//So, take the rest of the sample, put it in return value.
					sampleValue = sampleValue << (remainingBits);

					sampleValue |= (chunkInputBuffer[bufferBytePointer] & (0xFF >> bufferBitPointer));

					bufferBytePointer++;
					bufferBitPointer = 0;
					bitsToRead -= (remainingBits);
				} else 
				{
					//The bit pointer is zero, and we don't want to read the bit twice. 
					//So we will have to shift both ways to remove undesired bits from the buffer.
					sampleValue = sampleValue << (bitsToRead);
					//gets rid of "used" part of byte.
					uint8_t bitsToMask = chunkInputBuffer[bufferBytePointer] ;
					bitsToMask &= (0xFF >> bufferBitPointer);
					bitsToMask = bitsToMask >> (remainingBits - bitsToRead);
					sampleValue |= bitsToMask;
					bufferBitPointer += bitsToRead;
					bitsToRead = 0;
				}
			}
		}

		//Time to convert the value based on encoding.
		
		//Prime territory for function pointers here:

		//Signed bit?
		if(totalBitCount == 1)
		{
			return (sampleValue == 0 ? 1.0 : -1.0);
		}


		//Fun fact: Offset binary may be converted into two's complement by inverting the most significant bit. 
		if(encoding.at(0) == 'B' || encoding.at(0) == 'b')
		{
			sampleValue ^= 1 << (totalBitCount-1);
		}


		if(encoding.at(0) == 'I' || encoding.at(0) == 'i' || encoding.at(0) == 'B' || encoding.at(0) == 'b')
		{
			if( ((sampleValue >> (totalBitCount-1)) & 0x01) == 1)
			{
				sampleValue = sampleValue | (((int64_t)(-1)) << (totalBitCount));
			}
			return sampleValue;
		}

		//Sign-Mag? 
		if(encoding.at(1) == 'M' || encoding.at(1) == 'm')
		{
			if( ((sampleValue >> (totalBitCount-1)) & 0x01) == 1)
			{
				//Because this number is signed, subtract the value of the signedbit and Negate.
				sampleValue = -(sampleValue - (0x01 << (totalBitCount-1)));
			}
			return sampleValue;
		}

		//Float? Not supported yet.
		if(encoding.at(0) == 'F' || encoding.at(0) == 'f')
		{
			//hmmm
			//The simple solution, for 32 and 64 bits, is to just 
			//int i = 1;
			//float * d = reinterpret_cast<float *>(&i);
			//std:: cout << *d;
		}
		//Error
		std::cout << "Error: Encoding is bad:" << encoding << std::endl;
		return 0;
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

