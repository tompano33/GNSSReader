//Tests the GnssReader using files in the Tests/ directory

#include "GnssReader.h"
#include <iostream>
void testSuite()
{
	try{
	
		{
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\header\\test.xml",5L,10L,1000L,1,NULL,0);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
		}
		printf("1");
		
		{
		GNSSReader test2 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\temporalforeach\\test.xml",10000L,20000L,1000000L);
		test2.makeDecStreams();
		test2.setPrintOptions(false,true);
		test2.start();
		}
		printf("2");
		
		{
		GNSSReader test3 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sine\\test.xml",10000L,20000L,1000000L);
		test3.makeDecStreams();
		test3.setPrintOptions(true,false);
		test3.start();
		}

		printf("3");

		{
		//Initialize the GNSS Reader by providing an argument to it's metadata.
		//Then the size of the read input, the size of the intermediate buffer, and the size of the output buffer
		//Optionally, pass the number of blocks you want to read, and additional paths to look for files on
		//Finally, pass the number of paths that you passed
		GNSSReader test4 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\singleStream\\test.xml",100000L,200000L,10L,5,NULL,0);

		//Makes test4.decStreamCount() decStreams
		test4.makeDecStreams();
		//Starts this as a thread.
		test4.startAsThread();
	
		while(!test4.isDone()){

			//TODO what if there is no space in the buffer?
			//Once test4 finishes, you will still have to flush the stream one more time.
			uint64_t byteCount;
			double* d = new double[test4.getDecStreamArray()[0]->getBufSize()];
			test4.getDecStreamArray()[0]->flushOutputStream(d,&byteCount);
			std::cout << byteCount << std::endl;
		}
		
		}

		printf("4");
	
		{
		GNSSReader test5 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\multiStream\\test.xml",10000L,20000L,1000000L);
		test5.makeDecStreams();
		test5.setPrintOptions(true,false);
		test5.start();
		}

		printf("5");

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

		printf("6");
	
		{
		GNSSReader test7 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\padAndShift\\lshp.xml",10000L,20000L,1000000L);
		test7.makeDecStreams();
		test7.setPrintOptions(false,true);
		test7.start();
		}

		printf("7");

	} catch (std::exception& e) {
			printf(e.what());
	}
}

int main(int argc, char** argv)
{
	clock_t tStart = clock();
	testSuite();
	printf("Execution Time: %.2f s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	_CrtDumpMemoryLeaks();
	std::cin.get();
	return 0;
}

