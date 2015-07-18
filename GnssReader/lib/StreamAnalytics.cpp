/**
 * File: StreamAnalytics.cpp
 * Author: WJLIDDY
 */

#include<cstdint>
#include<iostream>
#include<stdio.h>
#include<GnssMetadata/Metadata.h>
#include "StreamAnalytics.h"
#include "DecStream.h"

	void StreamAnalytics::setStream(DecStream* s)
	{
		this->s = s;
	};

	void StreamAnalytics::calcMean(uint64_t sampCount, double * samps){
		double sum = 0;
	
		for(int i = 0; i != sampCount; i++)
		{
			sum += samps[i];
		}

		mean = sum / sampCount;
	};

	void StreamAnalytics::calcVar(uint64_t sampCount, double * samps){

		double sum = 0;
		for(int i = 0; i != sampCount; i++)
		{
			double sumval = ((double)samps[i] - mean);
			sumval *= sumval;
			sum += sumval;
		}
		variance = sum / (sampCount-1);
	};

	void StreamAnalytics::printMeanAndVar(){
		uint64_t sampCount;
		double* d= s->flushOutputStream(&sampCount);

		calcMean(sampCount,d);
		calcVar(sampCount,d);

		std::cout << "On stream: " << s->getID();
		printf(" Block has mean %f and variance %f\n",mean, variance);
	};

	void StreamAnalytics::printAllSamples()
	{
		
		uint64_t sampCount;
		double* d= s->flushOutputStream(&sampCount);

		for(int i = 0; i != sampCount; i++)
		{
			std::cout << d[i] <<  " " <<  (uint8_t)d[i] << std::endl;
		}

	}

