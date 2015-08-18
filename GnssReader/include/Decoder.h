/**
 * File: Decoder.h
 * Author: WJLIDDY
 * Description: Using XML metadata files and associated source files, puts samples in output buffers.
 */

//TODO: remove unused includes
#ifndef Decoder_H_H
#define Decoder_H_H

#include <list>
#include <cstdint>
#include <vector>
#include <GnssMetadata/Metadata.h>
#include <stdexcept>

#include "DecStream.h"
#include "FileReader.h"
#include "ChunkBuffer.h"

using namespace GnssMetadata;

class Decoder {
	
	private:

		//If not on windows, we need an instance of a thread for decoding.
		#ifndef _WIN32
			pthread_t _decThread;
		#endif

		/** EXTRACTED METADATA FILE FIELDS */

		//List of all metadata XML files that have been converted to Metadata objects
		std::vector<Metadata*>*_mdList;  
		//List of lengths of each block size for each metadata. Needed for fast skipping to an exact block.
		std::vector<int*>* _mdBlockSizes; 
		//Count of block sizes in mdBlockSizes
		std::vector<int>* _mdBlockSizesCount;  
		//Pointer to next target metadata file object to decode.
		unsigned int _mdPtr;

		/** RAW SDR FIELDS */

		//List of all SDR file names to decode.
		std::vector<std::string>* _sdrFileNames; 
		//Lengths of SDR files in sdrFileNames.
		std::vector<uint64_t>* _sdrFileSize; 

		/** DECODED STREAM FIELDS */

		//Array of Decoded Sample Output Streams
		DecStream** _decStreamArray; 
		//The count of these streams.
		uint64_t _decStreamCount; 
		//TODO: Belongs in decoder class, remove this variable
		//The max size of an output stream. If an output stream reaches this size, decoding stops.
		uint64_t _streamSize; 

		/** OTHER FIELDS */

		//File reader, which reads an sdr file and puts it in a buffer.
		FileReader* _fr; 
		//Count of total blocks the caller wants to read.
		uint64_t _blocksLeftToRead; 
		//If we want to print statistics as we decode blocks, then one or both of these will be true.
		//Should not be in release version because these clear the decodedStreams.
		bool _printStats, _printSamples; 
		//flag if there are no more samples to read.
		bool _done;

		/** TRIGR PROTOCOLS */

		//If true, check footers of TRIGR files for consistency
		bool _TRIGRmode;
		//If true, then the last footer value has been set and is not just garbage,
		bool _TRIGRlastFooterValSet;
		//The last read footer value, to compare against the current read footer values
		uint64_t _TRIGRlastFooterVal;
		//TODO: Acutally pull this from file. It's hardcoded somewhere.
		uint64_t _TRIGRmask;

		/** PRIVATE METHODS */

		//Given a block with cycle cycles, puts it in the correct DecStreams
		void readChunkCycles(Block * block, uint32_t cycles); 

		//When we load in a new metadata, we must associate the already-open streams with those found in the file. This reassigns the streams in the metadata object.
		//TODO: Metadata object still has hacked-on pointers.
		void repairDecStreams(Metadata* md); 

		//Given the name of the first XML file, crawls through all the other XML files to build mdList, mdBlockSizes, mdBlockSizesCount, sdrFileList.
		//TODO: Any more?
		void makeFileList(std::string first);

		//Helper function to start a thread of a class method at runtime. 
		//TODO: Needed by windows only?
		static void ThreadEntry(void *p);

		//Reads and puts a sample from a chunkbuffer into decStream[i]. negates it if need be. 
		//TODO: i is crappy variable name
		void readAndPutSample(ChunkBuffer * cb, Stream* s, int i, bool negate);

		//Helpers for readChunkCycles
		void skipLeftPackedBits(GnssMetadata::Stream * stream, ChunkBuffer * cb);
		void skipRightPackedBits(GnssMetadata::Stream * stream, ChunkBuffer * cb);

		//'i' represent the target stream in DecStreamArray
		void decodeFormattedStream(GnssMetadata::Stream * stream, ChunkBuffer * cb, int i);

		//Starts the reader, but as a single main thread. Eventually called by startAsThread();
		void start();

		//get sizes of all sdr files. Used if we need to skip to a certain block.
		void fetchFileSizes();

		//From a lane, generates the size of all of the blocks in the lane. Needed for skipping.
		int* generateBlockSizeArray(GnssMetadata::Lane*);

	public:

		//Takes a path to an XML file, the size of the readbuffer, the size of the intermediate buffer, the output streamsize, any additional paths to search, and count of blocks to read.
		Decoder(const char* pathToFile,uint64_t readSize,uint64_t buffSize,uint64_t streamSize, uint64_t blockTotal = -1,const char** addlPaths = NULL, uint64_t pathCount = 0);

		//Changes Working Directory.
		static void changeWD(const char* pathToFile);

		//Returns Decoded Streams as an array
		DecStream** getDecStreamArray();

		//Count of decoded streams
		uint64_t getDecStreamCount();

		//Instantiates the decoded streams. Must call before decoding!
		void makeDecStreams();

		//Call this if you want to print block statisitics to the console
		void setPrintOptions(bool printStats, bool printSamples);

		//Starts the Reader as a thread.
		void startAsThread();

		//Returns true if the thread is finished working.
		bool isDone();

		//Returns capacity of intermediate buffer.
		double getIBufPercent();

		//Returns name of file currently being moved to the IBuf
		std::string fileBeingDecoded();

		//Starts decoding at block specified. By default, this is block1.
		void startAtBlock(uint64_t);

		//TODO: Decide how to handle memory management for decoded streams
		~Decoder();
};



#endif

