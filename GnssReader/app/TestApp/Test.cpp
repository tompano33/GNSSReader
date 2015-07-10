//Tests the GnssReader using files in the Tests/ directory

#include "GnssReader.h"
#include <iostream>
#include "Test.h"

void testSuite()
{

	
	//alignment: Tests quantization and aligned bits. 
	{
		printf("Expecting: [5, -2, -16, 15]\n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\alignment\\test.xml",1L,10L,10L);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}


	{
		printf("Expecting: [HELLO___SDR_WORLD___THISIS__A___TEST_!_]  \n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\header\\test.xml",7L,3L,1000000L);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

		//offset-binary: tests the offset binary format
	{
		printf("Expecting 0100 0101 0100 1100 0100 1111  -2 -3 -2 0 2 -3 -3 3 -4 -3 -4 4 -4 7\n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\offsetBin\\test.xml",1L,10L,1000000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	
	{
		printf("SampleBits has three samples of various sizes. Expecting [2 1 2 4 6 1 1 7 4 5 4 12 4 15 69 76 (FF)] as two's complement \n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sampleBits\\test.xml",1L,10L,1000000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}


	{
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\singleStream\\test.xml",1000L,10L,100000L);
		
		test.makeDecStreams();
		test.setPrintOptions(true,false);
		test.start();
	}
	



		/**
		//offset-binary: tests the offset binary format
	{
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\singleStream\\test.xml",500L,8L,100000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(true,false);
		test.start();
	}

	/**

		/**
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
	/**



	//header:tests skipping of headers and footers of various sizes.
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



	//padAndShift: Tests padding and shifting.
	{
		printf("Expecting repeated pattern\n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\padAndShift\\lshp.xml",50L,1000L,1000000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//padding: See original specification for the example of a padded lump.
	
	{
		printf("Expecting Three Streams \n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\padding\\test.xml",50L,1000L,1000000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//samplebits: Demonstrates streams with various sample bits.

	//SDR ION Fraun test: Tests reading from an actual file
	{
		try{

			printf("???\n");
			GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sdrion\\flexi\\L1\\test.binx",50L,1000L,1000000L,-1);
			test.makeDecStreams();
	//		test.setPrintOptions(false,true);
			test.start();

		} catch (std::exception& e) {
			printf(e.what());
		}


	}

	//sevenEight: Demonstrates reading a stream of seven, then eight bytes.All values should be -1.
	{
		printf("SampleBits has two samples, one 7 and one 8. Expecting all -1's. \n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sevenEight\\test.xml",50L,1000L,1000000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//signMagnitde : tests the sign-magnitude format
	{
		printf("Expecting: 2 1 2 0 -2 1 1 -3  4 5 4 -4 4 7 \n");
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\signMag\\test.xml",50L,1000L,1000000L,-1);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
	}

	//sine wave: reads a sine wave sample, but as a thread.
	{
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sine\\test.xml",10000L,20000L,1000000L);
		test.makeDecStreams();
		//test.setPrintOptions(true,true);
		test.startAsThread();
	
		while(!test.isDone())
		{		
			double* inbuf = new double[test.getDecStreamArray()[0]->getBufSize()];
			uint64_t count = 0; 
			test.getDecStreamArray()[0]->flushOutputStream(inbuf, &count);
			
			for(int i = 0; i != count; i++)
			{
			//  You can print this to verify it works.
			//	std::cout << inbuf[i] << std::endl;
			}

			delete[] inbuf; //(crashes for some reason??!?)

		}

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
	


	/**	{
		
		*/
	
}

void testBuffer()
{
	/**
	IBuffer b = IBuffer (2,3);
	b.dbg_printPtrs();
	
	printf("\nTrying 1 \n");
	std::cout << (int)b.canWriteBlock() << "\n";
	b.doneWritingBlock();
	b.dbg_printPtrs();

	std::cout << (int)b.canWriteBlock() << "\n";
	b.doneWritingBlock();
	b.dbg_printPtrs();

	std::cout << (int)b.canWriteBlock() << "\n";
	b.doneWritingBlock();	
	b.dbg_printPtrs();

	std::cout << (NULL == b.tryRead(5)) << "\n";
	b.doneReading(5);
	b.dbg_printPtrs();

	std::cout << (int)b.canWriteBlock() << "\n";
	b.doneWritingBlock();
	b.dbg_printPtrs();

	std::cout << (int)b.canWriteBlock() << "\n";
	b.doneWritingBlock();	
	b.dbg_printPtrs();

	std::cout << ((int)b.tryRead(1)) << " [3] \n";
	b.doneReading(1);
	b.dbg_printPtrs();
	*/
}

int main(int argc, char** argv)
{
	clock_t tStart = clock();
	testSuite();
//	testBuffer();
	printf("Execution Time: %.2f s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	_CrtDumpMemoryLeaks();
	std::cin.get();
	return 0;
}

