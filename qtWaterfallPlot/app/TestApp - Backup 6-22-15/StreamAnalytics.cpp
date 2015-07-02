//Used to find the mean and variance of a block.
/**
 * File: BlockAnalytics.cpp
 * Author: WJLIDDY
 * Given some values (of a block), finds the mean and variance
 */

#include<cstdint>
#include<stdio.h>
#include<iostream>
#include<GnssMetadata/Metadata.h>
#include "StreamAnalytics.h"
#include "DecStream.h"


	void StreamAnalytics::setStream(DecStream* s)
	{
		this->s = s;
	};

	void StreamAnalytics::calcMean(){
		double sum = 0;
		for(int i = 0; i != s->getSamplePtr(); i++)
		{
			sum += s->getBuf()[i];
		}
		mean = sum / s->getSamplePtr();
	};

	void StreamAnalytics::calcVar(){
		double sum = 0;
		for(int i = 0; i != s->getSamplePtr(); i++)
		{
			double sumval = ((double)s->getBuf()[i] - mean);
			sumval *= sumval;
			sum += sumval;
		}
		variance = sum / (s->getSamplePtr()-1);
	};

	double StreamAnalytics::getVar(){
	//	return variance;
		return -1;
	};

	double StreamAnalytics::getMean(){
	//	return mean;
		return -1;
	};


	void StreamAnalytics::printMeanAndVar(){
		calcMean();
		calcVar();
		std::cout << "On stream: " << s->getID();
		printf(" Block has mean %f and variance %f\n",mean, variance);
	};



