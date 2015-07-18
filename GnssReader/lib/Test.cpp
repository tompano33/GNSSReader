//Tests the GnssReader using files in the Test directory
#include "GnssReader.h"
#include <iostream>

void testSuite()
{
	std::cout << "Enter the path to your 'Tests' Directory:\n";
	std::string path;
	std::getline (std::cin, path);
	
	//Change this to the path to your test suite if you want to set a default directory
	std::string pathToTests ("C:\\Users\\ANTadmin\\Desktop\\SDR\ STANDARD\\Tests\\");
	if(path.size() != 0)
	{
		pathToTests = path; 
	}

	try
	{

		//alignment: Tests quantization and aligned bits. 
		{
			printf("Expecting: [5, -2, -16, 15]\n");
			std::string localPath;
			pathToTests.append(std::string ("alignment\\test.xml"));
			GNSSReader test (pathToTests.c_str(),50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}
	

		//altPaths: Tests quantization and aligned bits. 
		{
			printf("Expecting: A nice introduction \n");

			char** paths = new char*[3];
			paths[0] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\";
			paths[1] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\superstorage\\";
			paths[2] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\superstorage\\extremestorage\\";
		
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\test.xml",6L,3L,1000000L,-1,(const char**)paths,3);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}
		
		//exception: throws error if invalid XML exists.
		{
			try{
				printf("Expecting: Printed 'foostream1' \n");
				GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\exception\\test.xml",50L,1000L,1000000L);
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
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\floatingPoint\\test.xml",50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//formatOne: tests IQ, IF formats
		{
			printf("Expecting reals of 10, complexes of -2\n");
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\formatOne\\test.xml",50L,1000L,1000000L,-1);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//header: tests skipping of headers and footers of various sizes.
		{
			printf("Expecting: [HELLO___SDR_WORLD___THISIS__A___TEST_!_]  \n");
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\header\\test.xml",50L,1000L,1000000L);
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
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\multiStream\\test.xml",50L,1000L,1000000L,1);
			test.makeDecStreams();
			test.setPrintOptions(true,false);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//offset-binary
		{
			printf("Expecting 0100 0101 0100 1100 0100 1111  -2 -3 -2 0 2 -3 -3 3 -4 -3 -4 4 -4 7\n");
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\offsetBin\\test.xml",50L,1000L,1000000L,-1);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//padAndShift: Tests padding and shifting.
		{
			printf("Expecting repeated pattern\n");
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\padAndShift\\lshp.xml",50L,1000L,1000000L,-1);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//padding: Tests Padding 
		{
			printf("Expecting repeated pattern\n");
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\padding\\test.xml",50L,1000L,1000000L,-1);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//SampleBits: tests samples of various sizes
		{
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sampleBits\\test.xml",50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//Seven Eight: a 7 bit and 8 bit stream
		{
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sevenEight\\test.xml",50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//Sign-Magnitude:
		{
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\signMag\\test.xml",50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){;}
		}

		//SixtyFour
		{
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sixtyFour\\test.xml",50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.startAsThread();
			while(!test.isDone()){std::cout << test.fileBeingDecoded();}
		}

			printf("\n sine \n");
		
	
		//Sine as does work, but if you try to open the file again later an error is thrown. Perhaps the handle is not closed as i thought?
		{
			//fix a chunkbuffer that is too small
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sine\\test.xml",5000L,10L,1000000L);
		
			test.makeDecStreams();
			//test.setPrintOptions(true,false);
			test.startAsThread();
			while(!test.isDone()){

				uint64_t count = 0; 
				test.getDecStreamArray()[0]->flushOutputStream(&count);				
				std::cout << "Count is" << count << std::endl;
			
		//			for(int i = 0; i != count; i++)
		//			{
		//				std::cout << inbuf[i] << std::endl;
		//			}


				}
			
		}


		//SingleStream
		{
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\singleStream\\test.xml",5000L,5L,1000000L,5);
			test.makeDecStreams();
			test.setPrintOptions(true,false);

			//This won't always work on multi-blocked chunks
			//Also add support for repeating blocks
			test.startAtBlock(90);
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

