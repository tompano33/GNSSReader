#include <string>
#include <vector>
#include <iostream>
#include <tinyxml2.h>
#include "Convert.h"

std::string argf1;
std::string argPathsCSV;
std::string argConvertCount;
std::string argWriteAtHome;
std::vector <std::string> paths;

void noArgDialogue();
void parseXMLConfig(const char*);
void extractPaths();

using namespace tinyxml2;

int main (int argc, const char * argv[])
{
	const char* appDir = argv[0];

	/**
	if(argc == 1)
		noArgDialogue();
	else if (argc == 2)
		parseXMLConfig(argv[2]);
	else if (argc == 5)
	{
		argf1 = *(new std::string(argv[1]));
		argPathsCSV  = *(new std::string(argv[2]));
		argConvertCount = *(new std::string(argv[3]));
		argWriteAtHome = *(new std::string(argv[4]));
	} else
	{
		printf("Error, please supply 0, 1, or 4 args");
	}
	*/
	parseXMLConfig("C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Tests\\trigrtest\\config.xml");
	//parse all the data
	extractPaths();
	//xmmlll
	paths.push_back("C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Tests\\trigrtest\\");
	int convertCountParsed = atoi(argConvertCount.c_str());
	bool writeAtHomeFlag = (argWriteAtHome.c_str()[0] == '1');
	Convert converter (appDir,argf1.c_str(),paths,convertCountParsed,writeAtHomeFlag);
	converter.start();
}

//Extracts PathsCsv to a vector of strings.
void extractPaths()
{
	std::string delimiter = ",";
	size_t pos = 0;
	std::string token;
	while ((pos = argPathsCSV.find(delimiter)) != std::string::npos) {
		token = argPathsCSV.substr(0, pos);
		paths.push_back(token);
		argPathsCSV.erase(0, pos + delimiter.length());	
	}
	paths.push_back(argPathsCSV);
}
	
//Console Dialogue to get args if none were passed
void noArgDialogue()
{
	//First tgx file name
	std::cout << "Enter the name of the first tgx file\n";
	std::getline (std::cin, argf1);

	//PathsCsv (where to find the files, paths separated by commas – similar to the decoder)
	std::cout << "Enter the list of paths to search, seperated by commas. \n";
	std::getline (std::cin, argPathsCSV);

	//Splice Mode (int_32): 0: convert this file only, >0: attempt to find this many concurrent files and convert, <0: keep converting files until concurrent sequence ends
	std::cout << "Enter the count of files to convert. '0' means convert all that are found. \n";
	std::getline (std::cin, argConvertCount);

	//Write Mode (bool): 0: write converted files to source dir and rename old file.tgx file to file.tgx_old; 1: write all converted files to app home directory
	std::cout << "Would you like to write the converted files to this app directory? Otherwise they will be written to the source directory. (Type '1' for yes, otherwise press enter) \n";
	std::getline (std::cin, argWriteAtHome);

}

void parseXMLConfig(const char* pathToConfig){
	
		//Pulls all data from X-XML file.
		XMLDocument config;

		//We know this file exists be at the end of the loop before.
		config.LoadFile(pathToConfig);

		XMLElement* titleElement = config.FirstChildElement();

		argf1 = titleElement->FirstChildElement("FIRSTTGX")->GetText();

		//TODO: special case for argPathsCSV
		if(titleElement->FirstChildElement("PATHSCSV")->GetText() != NULL)
			argPathsCSV = titleElement->FirstChildElement("PATHSCSV")->GetText();
		else
			argPathsCSV = " ";

		argConvertCount = titleElement->FirstChildElement("FILESTOCONVERT")->GetText();
		argWriteAtHome = titleElement->FirstChildElement("WRITEMODE")->GetText();

}


