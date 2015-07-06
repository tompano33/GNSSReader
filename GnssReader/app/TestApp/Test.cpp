//Tests the GnssReader using files in the Tests/ directory

#include "GnssReader.h"
#include <iostream>
void testSuite()
{
	try{
		while(true)
		{

			char** paths = new char*[1];
			paths[0] = "C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPathRead\\sine2\\";

			GNSSReader test4 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\altPathRead\\test.xml",1,500000L,1000000L,-1, (const char**)paths,1);
			test4.makeDecStreams();
			test4.setPrintOptions(true,false);
			test4.startAsThread();
			while(!test4.isDone()){;}
		}
	
	

	/**	{
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
		GNSSReader test5 ("C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\singleStream\\test.xml",10000L,20000L,1000000L);
			test5.makeDecStreams();
			test5.setPrintOptions(true,false);
			test5.start();
		}
	
		
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

