//Used to find the mean and variance of a block.
/**
 * File: BlockAnalytics.cpp
 * Author: WJLIDDY
 * Given some values (of a block), finds the mean and variance
 */

#include<cstdint>
#include<stdio.h>
#include "BlockAnalytics.h"

	BlockAnalytics::BlockAnalytics(uint64_t size)
	{
		sampleBuffer = new int64_t[size];
		sizeOfBuffer = size;
		bufferPointer = 0;
	};

	void BlockAnalytics::putValue(int64_t val){
		sampleBuffer[bufferPointer++] = val;
	};

	void BlockAnalytics::calcMean(){
		double sum = 0;
		for(int i = 0; i != bufferPointer; i++)
		{
			sum += sampleBuffer[i];
		}
		mean = sum / bufferPointer;
	};

	void BlockAnalytics::calcVar(){
		double sum = 0;
		for(int i = 0; i != bufferPointer; i++)
		{
			double sumval = ((double)sampleBuffer[i] - mean);
			sumval *= sumval;
			sum += sumval;
		}
		variance = sum / (bufferPointer-1);
	};

	double BlockAnalytics::getVar(){
		return variance;
	};

	double BlockAnalytics::getMean(){
		return mean;
	};

	void BlockAnalytics::printMeanAndVar(){
		calcMean();
		calcVar();
		printf("Block has mean %f and variance %f\n",mean, variance);
	};


