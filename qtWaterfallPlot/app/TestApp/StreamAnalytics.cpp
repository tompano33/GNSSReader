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

	void StreamAnalytics::printMeanAndVar(){
		calcMean();
		calcVar();
		std::cout << "On stream: " << s->getID();
		printf(" Block has mean %f and variance %f\n",mean, variance);
	};

	void StreamAnalytics::printAllSamples()
	{
		for(int i = 0; i != s->getSamplePtr(); i++)
		{
			std::cout << s->getBuf()[i] <<  " " <<  (uint8_t)s->getBuf()[i] << std::endl;
		}

	}

