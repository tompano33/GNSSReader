/**
 * File: BlockAnalytics.h
 * Author: WJLIDDY
 * Given some values (of a block), finds the mean and variance
 */
#ifndef BLOCKANALYTICS_H_H
#define BLOCKANALYTICS_H_H

class BlockAnalytics{

	//An array to hold all of the samples read.
	int64_t* sampleBuffer;

	//size of array.
	uint64_t sizeOfBuffer;

	//Where to put next value.
	uint32_t bufferPointer;

	double mean;
	double variance;

public:
	BlockAnalytics::BlockAnalytics(uint64_t size);
	void putValue(int64_t size);
	void calcMean();
	void calcVar();
	double getMean();
	double getVar();
	int getBufferPointer();
	void printMeanAndVar();
};


#endif
