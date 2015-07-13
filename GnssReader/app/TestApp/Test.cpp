//Tests the GnssReader using files in the Tests/ directory

#include "GnssReader.h"
#include <iostream>
void testSuite()
{
	//alignment: Tests quantization and aligned bits. 
	{
		printf("Expecting: [5, -2, -16, 15]\n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\alignment\\test.xml",50L,1000L,1000000L);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	/**
	//altPaths: Tests quantization and aligned bits. 
	{
		//Todo, test local paths, limited block reads.
		printf("Expecting: A nice introduction \n");

		char** paths = new char*[3];
		paths[0] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\";
		paths[1] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\superstorage\\";
		paths[2] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\superstorage\\extremestorage\\";
		
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\test.xml",50L,1000L,1000000L,-1,(const char**)paths,3);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}
	*/
		
	//exception: throws error if invalid XML exists.
	{
		try{
			printf("Expecting: Printed 'foostream1' \n");
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\exception\\test.xml",50L,1000L,1000000L);
			test.makeDecStreams();
			test.setPrintOptions(false,true);
			test.start();
		} catch (std::exception& e) {
			printf(e.what());
		}
	}

	//floatingpoint: tests reading of floating point numbers
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!--------------------------------------ERROR!
	{
		printf("Expecting: [-5.168466E29 -2.4479542E-19 6.3108872E-30 0 ] and two doubles. \n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\floatingPoint\\test.xml",50L,1000L,1000000L);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//formatOne: tests IQ, IF formats
	{
		printf("Expecting reals of 10, complexes of -2\n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\formatOne\\test.xml",50L,1000L,1000000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//header: tests skipping of headers and footers of various sizes.
	{
		printf("Expecting: [HELLO___SDR_WORLD___THISIS__A___TEST_!_]  \n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\header\\test.xml",50L,1000L,1000000L);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//multistream: outputs four 1-bit streams.
	//INVESTIGATE the mean & variance are wrong
	{
		printf("ERROR!:  \n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\multiStream\\test.xml",50L,1000L,1000000L,1);
		test.makeDecStreams();
		test.setPrintOptions(true,false);
		test.start();
	}

	//offset-binary
	{
		printf("Expecting 0100 0101 0100 1100 0100 1111  -2 -3 -2 0 2 -3 -3 3 -4 -3 -4 4 -4 7\n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\offsetBin\\test.xml",50L,1000L,1000000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//padAndShift: Tests padding and shifting.
	{
		printf("Expecting repeated pattern\n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\padAndShift\\lshp.xml",50L,1000L,1000000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	/**padding: Tests Padding */
	{
		printf("Expecting repeated pattern\n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\padding\\test.xml",50L,1000L,1000000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//SampleBits: tests samples of various sizes
	{
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sampleBits\\test.xml",50L,1000L,1000000L);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//Seven Eight: a 7 bit and 8 bit stream
	{
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sevenEight\\test.xml",50L,1000L,1000000L);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//Sign-Magnitude:
	{
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\signMag\\test.xml",50L,1000L,1000000L);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	
	//SixtyFour
	{
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sixtyFour\\test.xml",50L,1000L,1000000L);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//Sine as stream
	{
		printf("sineasstream");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sine\\test.xml",50L,1000L,1000000L);
		test.makeDecStreams();
		test.setPrintOptions(true,false);
		test.startAsThread();
		while(!test.isDone()){;}
	}

	//SingleStream
	{
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\singleStream\\test.xml",50L,1000L,1000000L,5);
		test.makeDecStreams();
		test.setPrintOptions(true,false);
		test.startAsThread();
		while(!test.isDone()){;}
	}
		/**
			char** paths = new char*[1];
			paths[0] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPathRead\\sine2\\";

			GNSSReader test4 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPathRead\\test.xml",1,500000L,1000000L,-1, (const char**)paths,1);
			test4.makeDecStreams();
			test4.setPrintOptions(true,false);
			test4.startAsThread();
			while(!test4.isDone()){;}
	
		
		*/
	


	/*	{
			GNSSReader test4 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sine\\test.xml",10000L,20000L,1000000L);
			test4.makeDecStreams();
			test4.setPrintOptions(false,true);
			test4.startAsThread();
	
		
			while(!test4.isDone())
			{		
				
				double* inbuf = new double[test4.getDecStreamArray()[0]->getBufSize()];
				uint64_t count = 0; 
				test4.getDecStreamArray()[0]->flushOutputStream(inbuf, &count);
				
				std::cout << "Count is" << count << std::endl;
			
				for(int i = 0; i != count; i++)
				{
					std::cout << inbuf[i] << std::endl;
				}

				delete[] inbuf; //(crashes for some reason??!?)

			}

			//delete inbuf when you're done
		
		*/

		/**
		{
		GNSSReader test4 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sevenEight\\test.xml",10000L,20000L,1000000L);
		test4.makeDecStreams();
		test4.setPrintOptions(false,true);
		test4.start();
		}

		{

	
		/*
		
		{
		char** paths = new char*[3];
		paths[0] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\";
		paths[1] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\superstorage\\";
		paths[2] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\superstorage\\extremestorage\\";

		GNSSReader test6 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\test.xml",10000L,20000L,1000000L,-1,(const char**)paths,3);
		test6.makeDecStreams();
		test6.setPrintOptions(false,true);
		test6.start();
		}
		
	
		{
		GNSSReader test7 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\padAndShift\\lshp.xml",10000L,20000L,1000000L);
		test7.makeDecStreams();
		test7.setPrintOptions(false,true);
		test7.start();
		}
		

		{
			GNSSReader test8 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\padAndShift\\lshp.xml",10000L,20000L,1000000L);
			test8.makeDecStreams();
			test8.setPrintOptions(false,true);
			test8.start();
		}
		*/
	
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

