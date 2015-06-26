//Tests the GnssReader using files in the Tests/ directory

#include "GnssReader.h"
#include <iostream>
void testSuite()
{
	try{
		
		GNSSReader test ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\header\\test.xml",5L,10L,1000L,1,NULL,0);
		test.makeDecStreams();
		test.setPrintOptions(false,true);
		test.start();
		
		GNSSReader test2 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\temporalforeach\\test.xml",100000L,200000L,1000000L);
		test2.makeDecStreams();
		test2.setPrintOptions(false,true);
		test2.start();
		
		GNSSReader test3 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\sine\\test.xml",100000L,200000L,1000000L);
		test3.makeDecStreams();
		test3.setPrintOptions(true,false);
		test3.start();

		GNSSReader test4 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\singleStream\\test.xml",100000L,200000L,1000000L,5,NULL,0);
		test4.makeDecStreams();
		test4.setPrintOptions(true,false);
		test4.start();

		GNSSReader test5 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\multiStream\\test.xml",100000L,200000L,1000000L);
		test5.makeDecStreams();
		test5.setPrintOptions(true,false);
		test5.start();

		char** paths = new char*[3];
		paths[0] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\";
		paths[1] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\superstorage\\";
		paths[2] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\storage\\superstorage\\extremestorage\\";

		GNSSReader test6 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPaths\\test.xml",100000L,200000L,1000000L,-1,(const char**)paths,3);
		test6.makeDecStreams();
		test6.setPrintOptions(false,true);
		test6.start();

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

