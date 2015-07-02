/**
 * File: StreamAnalytics.h
 * Author: WJLIDDY
 * Given a decoded outputstream, finds the mean, variance.
 */
#ifndef STREAMANALYTICS_H_H
#define STREAMANALYTICS_H_H

#include<GnssMetadata/Metadata.h>
#include "DecStream.h"
class StreamAnalytics{

	//This will get destroyed by Reader. no need to kill it.
	DecStream* s;
	double mean;
	double variance;

public:
	void setStream(DecStream* s);
	void calcMean();
	void calcVar();
	void printMeanAndVar();
	void printAllSamples();
};


#endif
