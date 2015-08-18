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

	ChunkBuffer::ChunkBuffer(uint64_t size, char* buffer) 
	{
		_sizeOfBuffer = size;
		_chunkInputBuffer = buffer;
		//Start at the beginning of the buffer
		_bufferBytePointer = 0;
		_bufferBitPointer = 0;
	};

	//TODO: I think passing in a string is probably very slow.
	int64_t ChunkBuffer::readBits(uint8_t bitsToRead, std::string  encoding)
	{

		//Don't toggle the last sample was a float flag until we know it was a float.
		_wasFloat = false;

		//Because we will be modifying bitsToRead, save the total amount.
		uint8_t totalBitCount = bitsToRead;

		//sampleValue holds the value of the sample that will eventually be returned
		int64_t sampleValue = 0;	

		while(bitsToRead > 0)
		{
			if(_bufferBitPointer == 0) 
			{
				if(bitsToRead >=8)
				{
					//Since our buffer pointer is at 0, and we have at least 8 bits to read, 
					//Just shift the return value left by 8 and OR the byte in.
					sampleValue = sampleValue << 8;
					sampleValue |= ((uint8_t)_chunkInputBuffer[_bufferBytePointer]);
					_bufferBytePointer++;
					bitsToRead = bitsToRead - 8;
				} else 
				{
					//There are less than 8 bits, but our pointer is still at zero.
					//First make room for the bits in the return value
					sampleValue = sampleValue << bitsToRead;
					//The bits will be at the beginning of the buffer, so we can simply shift them out.
					sampleValue |= ((uint8_t)((uint8_t)(_chunkInputBuffer[_bufferBytePointer]) >> (8 - bitsToRead)));
					_bufferBitPointer = _bufferBitPointer + bitsToRead;
					bitsToRead = 0;
				}
			} else
			{
				//Here, the buffer-bit-pointer is not at zero. 
				//So, we cannot take any shortcuts.
				//This value is the amount of bits we need to pull from this byte.
				uint8_t remainingBits = 8 - _bufferBitPointer;

				if(bitsToRead >= remainingBits)
				{
					//Here, we have more bits to read than there are in the byte.
					//So, take the rest of the sample, put it in return value.
					sampleValue = sampleValue << (remainingBits);
					sampleValue |= (_chunkInputBuffer[_bufferBytePointer] & (0xFF >> _bufferBitPointer));
					_bufferBytePointer++;
					_bufferBitPointer = 0;
					bitsToRead -= (remainingBits);
				} else 
				{
					//The bit pointer is zero.
					//So we will have to shift both ways to remove undesired bits from the buffer.
					sampleValue = sampleValue << (bitsToRead);
					//gets rid of "used" part of byte.
					uint8_t bitsToMask = _chunkInputBuffer[_bufferBytePointer] ;
					bitsToMask &= (0xFF >> _bufferBitPointer);
					bitsToMask = bitsToMask >> (remainingBits - bitsToRead);
					sampleValue |= bitsToMask;
					_bufferBitPointer += bitsToRead;
					bitsToRead = 0;
				}
			}
		}
		
		//TODO: This belongs in a subfunction. All the code after this takes the raw sample value and changes it based on the format.
		//Such as, 2's complement, Signed magniute, floating point, etc..

		//If totalBitCount is one, the rule is that a '1' represents a signed 1 (-1) and '0' is positive 1.
		if(totalBitCount == 1)
		{
			return (sampleValue == 0 ? 1 : -1);
		}

		//Offset binary may be converted into two's complement by inverting the most significant bit. 
		//So we will convert to two's complement here.
		if(encoding.at(0) == 'B' || encoding.at(0) == 'b')
		{
			sampleValue ^= 1 << (totalBitCount-1);
		}
		
		//If two's complement or converted offsetBinary, get the signed bit, and mask the rest of the int64_t with the signed bit.
		if(encoding.at(0) == 'I' || encoding.at(0) == 'i' || encoding.at(0) == 'B' || encoding.at(0) == 'b')
		{
			if( ((sampleValue >> (totalBitCount-1)) & 0x01) == 1)
			{
				sampleValue = sampleValue | (((int64_t)(-1)) << (totalBitCount));
			}
			return sampleValue;
		}

		if(encoding.at(1) == 'M' || encoding.at(1) == 'm')
		{
			if( ((sampleValue >> (totalBitCount-1)) & 0x01) == 1)
			{
				//Because this number is signed, subtract the value of the signedbit and Negate.
				//TODO test 64-bit sign-magnitude
				sampleValue = -(sampleValue - (0x01 << (totalBitCount-1)));
			}
			return sampleValue;
		}

		//Float? well, just return the integer, and a flag saying this is a float. 
		//TODO This could be done better later.
		if(encoding.at(0) == 'F' || encoding.at(0) == 'f')
		{
			_wasFloat = true;
			return sampleValue;
		}

		//No encoding found. TODO: Throw Error.
		std::cout << "Error: Encoding is bad:" << encoding << std::endl;
		return 0;
	};

	//TODO lots of code reuse from readBits.
	void ChunkBuffer::skipBits(uint8_t bitsToRead)
	{
		uint8_t totalBitCount = bitsToRead;
		while(bitsToRead > 0)
		{
			if(_bufferBitPointer == 0)
			{
				if(bitsToRead >=8)
				{
					_bufferBytePointer++;
					bitsToRead = bitsToRead - 8;
				} else 
				{
					_bufferBitPointer = _bufferBitPointer + bitsToRead;
					bitsToRead = 0;
				}
			} else
			{
				int remainingBits = 8 - _bufferBitPointer;

				if(bitsToRead >= remainingBits)
				{
					_bufferBytePointer++;
					_bufferBitPointer = 0;
					bitsToRead -= (remainingBits);
				} else 
				{
					_bufferBitPointer += bitsToRead;
					bitsToRead = 0;
				}
			}
		}

		
	};

	bool ChunkBuffer::chunkFullyRead(){
		//TODO Experiment with not casting, I don't know why these are both INTs.
		return ((int)_bufferBytePointer == (int)_sizeOfBuffer);
	};

	bool ChunkBuffer::wasSampleFloat(){
		return (_wasFloat);
	};
