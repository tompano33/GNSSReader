//Tests the GnssReader using files in the Test directory
#include "GnssReader.h"
#include <iostream>

std::string pathToTests;

//TODO memory leaks, but it's just a test so it's ok
const char* pathTo(char * address)
{
	std::string * localPath = new std::string;
	localPath->append(std::string (pathToTests));
	localPath->append(std::string (address));
	return (localPath->c_str());
}

void profile()
{



}

void testSuite()
{
	std::cout << "Enter the path to your 'Tests' Directory:\n";
	std::string path;
	std::getline (std::cin, path);
	
	//Change this to the path to your test suite if you want to set a default directory
	pathToTests = ("C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Tests\\");
	if(path.size() != 0)
	{
		pathToTests = path; 
	}

	try
	{

		//alignment: Tests quantization and aligned bits. 
		{
			printf("Expecting: [5, -2, -16, 15]\n");
			GNSSReader test (pathTo("alignment\\test.xml"),50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}
	

		//altPaths: Tests quantization and aligned bits. 
		{
			printf("Expecting: A nice introduction \n");

			const char** paths = new const char*[3];
			paths[0] = (pathTo("altPaths\\storage\\"));
			paths[1] = (pathTo("altPaths\\storage\\superstorage\\"));
			paths[2] = (pathTo("altPaths\\storage\\superstorage\\extremestorage\\"));
		
			GNSSReader test (pathTo("altPaths\\test.xml"),6L,3L,1000000L,-1,paths,3);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}
		

		//exception: throws error if invalid XML exists.
		{
			try{
				printf("Expecting: Printed 'foostream1' \n");
				GNSSReader test (pathTo("exception\\test.xml"),50L,1000L,1000000L);
				test.makeDecStreams();
				test.setPrintOptions(false,true);
				test.startAsThread();
				while(!test.isDone()){;}
		
			} catch (std::exception& e) {
				printf(e.what());
			}
		}

		//floatingpoint: tests reading of floating point numbers
		{
			printf("Expecting: [-5.168466E29 -2.4479542E-19 6.3108872E-30 0 ] and two doubles. \n");
			GNSSReader test (pathTo("floatingPoint\\test.xml"),50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//formatOne: tests IQ, IF formats
		{
			printf("Expecting reals of 10, complexes of -2\n");
			GNSSReader test (pathTo("formatOne\\test.xml"),50L,1000L,1000000L,-1);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//header: tests skipping of headers and footers of various sizes.
		{
			printf("Expecting: [HELLO___SDR_WORLD___THISIS__A___TEST_!_]  \n");
			GNSSReader test (pathTo("header\\test.xml"),50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){
			//std::cout << test.getIBufPercent() << "\n";
			}
		}

		//multistream: outputs four 1-bit streams.
		//INVESTIGATE the mean & variance are wrong
		{
			GNSSReader test (pathTo("multiStream\\test.xml"),50L,1000L,1000000L,1);
			test.makeDecStreams();
			test.setPrintOptions(true,false);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//offset-binary
		{
			printf("Expecting 0100 0101 0100 1100 0100 1111  -2 -3 -2 0 2 -3 -3 3 -4 -3 -4 4 -4 7\n");
			GNSSReader test (pathTo("offsetBin\\test.xml"),50L,1000L,1000000L,-1);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//padAndShift: Tests padding and shifting.
		{
			printf("Expecting repeated pattern\n");
			GNSSReader test (pathTo("padAndShift\\lshp.xml"),50L,1000L,1000000L,-1);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//padding: Tests Padding 
		{
			printf("Expecting repeated pattern\n");
			GNSSReader test (pathTo("padding\\test.xml"),50L,1000L,1000000L,-1);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//SampleBits: tests samples of various sizes
		{
			GNSSReader test (pathTo("sampleBits\\test.xml"),50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//Seven Eight: a 7 bit and 8 bit stream
		{
			GNSSReader test (pathTo("sevenEight\\test.xml"),50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//Sign-Magnitude:
		{
			GNSSReader test ( pathTo("signMag\\test.xml"),50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//SixtyFour
		{
			GNSSReader test (pathTo("sixtyFour\\test.xml"),50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){std::cout << test.fileBeingDecoded();}
		}

			printf("\n sine \n");
		
	
		//Sine as does work, but if you try to open the file again later an error is thrown. Perhaps the handle is not closed as i thought?
		{
			//fix a chunkbuffer that is too small
			GNSSReader test (pathTo("sine\\test.xml"),5000L,10L,1000000L);
		
			test.makeDecStreams();
			//test.setPrintOptions(true,false);
			test.startAsThread();
			while(!test.isDone()){

				uint64_t count = 0; 
				test.getDecStreamArray()[0]->flushOutputStream(&count);				
			//	std::cout << "Count is" << count << std::endl;
			
		//			for(int i = 0; i != count; i++)
		//			{
		//				std::cout << inbuf[i] << std::endl;
		//			}


				}
			
		}


		//SingleStream
		{
			GNSSReader test (pathTo("singleStream\\test.xml"),5000L,5L,1000000L,5);
			test.makeDecStreams();
			test.setPrintOptions(true,false);

			//This won't always work on multi-blocked chunks
			//Also add support for repeating blocks
			test.startAtBlock(1);
			test.startAsThread();
			while(!test.isDone()){;}
			//while(!test.isDone()){std::cout << test.getIBufPercent() << "\n";}
		}

		{
			GNSSReader test (pathTo("trigr\\TRIGRDATA_56320kHz_04bit_Ch0123_2014-06-09-13-01-43-546.sdrx"),10000L,10L,1000000L,5);
			test.makeDecStreams();
			test.setPrintOptions(true,false);

			printf("\ntest!\n");
			//This won't always work on multi-blocked chunks [offset could be wrong]
			//Also add support for repeating blocks
			test.startAtBlock(1);
			test.startAsThread();
			while(!test.isDone()){;}
			//while(!test.isDone()){std::cout << test.getIBufPercent() << "\n";}
		}

		{
			GNSSReader test (pathTo("trigrBadData\\TRIGRDATA.sdrx"),5000L,10L,1000000L,5);
			test.makeDecStreams();
			test.setPrintOptions(true,false);

			printf("\ntest!\n");
			//This won't always work on multi-blocked chunks [offset could be wrong]
			//Also add support for repeating blocks
			test.startAtBlock(1);
			test.startAsThread();
			while(!test.isDone()){;}
			//while(!test.isDone()){std::cout << test.getIBufPercent() << "\n";}
		}



	} catch (std::exception& e) {
		printf("\n\n ***TEST FAILED*** \n\n");
		printf(e.what());
	}
}

int main(int argc, char** argv)
{
	clock_t tStart = clock();

	testSuite();
	
	printf("Execution Time: %.2f s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	//_CrtDumpMemoryLeaks();
	std::cin.get();
	return 0;
}

