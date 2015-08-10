#include <tinyxml2.h>
#include <GnssMetadata/Metadata.h>
#include <GnssMetadata/XML/XmlProcessor.h>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <direct.h>
#include <stdio.h>
#include "Convert.h"

using namespace tinyxml2;
using namespace GnssMetadata;
using namespace std;

Convert::Convert(const char* pappDir, const char* startFile, std::vector<std::string> ppaths, int convertCountParsed, bool pwriteAtHomeFlag)
{
	appDir = pappDir;
	paths = ppaths;
	writeAtHomeFlag = pwriteAtHomeFlag;

	//figure out how many files to convert.
	convertAll = false;

	if(convertCountParsed < 0)
	{
		convertAll = true;
		filesToConvert = 0;

	} else if (convertCountParsed == 0)
	{
		filesToConvert = 1;
	} else filesToConvert = convertCountParsed;

	//TODO: Output log file of converter activity (session report in text format).

	 XFile = new String(startFile); 
	
	 if(!findFile(XFile->c_str()))
	 {
		std::printf("Error: Specified first metadata file does not exist");
		//throw error here
	}

	 //we know the first file exists, now we are set to decode.
};

bool Convert::findFile(const char* file)
{
	for(std::vector<String>::iterator fileit = paths.begin(); fileit != paths.end(); ++fileit) {
		changeWD((*fileit).c_str());
		if(fileExists(file))
		{
			return true;
		}
	}
	return false;
}

void Convert::changeWD(const char* pathToFile)
{
	//Simply change the working directory.
	std::string fname = std::string(pathToFile,strlen(pathToFile));
	std::string dir;
	const size_t last_slash_idx = fname.rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		 dir = fname.substr(0, last_slash_idx);
	}
	chdir(dir.c_str());
}

bool Convert::fileExists(const char* file) {
    struct stat buf;
    return (stat(file, &buf) == 0);
}

long Convert::getFileSize(const char* file)
{
	struct stat stat_buf;
	int rc = stat(file,&stat_buf);
	return stat_buf.st_size;
}

void Convert::start()
{
	changeWD(XFile->c_str());

	while(XFile != NULL && (convertAll || filesToConvert > 0))
	{
		if(filesToConvert > 0)
			filesToConvert--;

		//Pulls all data from X-XML file.
		XMLDocument XDoc;

		//We know this file exists be at the end of the loop before.
		XDoc.LoadFile(XFile->c_str());

		//printf(


		pullXMetadata(&XDoc);

		std::string XFileOld(XFile->c_str());
		XFileOld.append(".old");

		//TODO To be logged
		/**
		std::cout << "---FILE SUMMARY---\n";
		std::cout << " Streams: " <<  toConvert.numStreams <<"\n";
		std::cout << " Bits/Sample: " <<  toConvert.bitsPerSample <<"\n";
		std::cout << " BlockOffset(DWORD): " <<  toConvert.blockOffsetDWords <<"\n";
		std::cout << " FooterMask: " <<  toConvert.footerMask <<"\n";
		std::cout << " SampleRate (Hz): " <<  toConvert.sampleRateHz <<"\n";
		std::cout << " Next File: " <<  toConvert.nextFile <<"\n";
		*/

		Metadata md;
		XmlProcessor proc;

		//let us first input the file into the metadata.
		File mFile ("Converted Trig File");
		std::string * SDRName = changeExt(XFile,"tgd");
		mFile.Url(*SDRName);
		//we also shall tell it the blockoffset. A Dword is 4bytes.
		long blockOffsetInBytes = 4 * atoi(toConvert.blockOffsetDWords);
		mFile.Offset(blockOffsetInBytes);
		std::string nextFile (toConvert.nextFile);
		nextFile = *changeExt(&nextFile,"tgx");
		std::string nextFileSDRX = *changeExt(&nextFile,"tgx");
		mFile.Next(AnyUri(nextFileSDRX));

		//Done with file! now make a lane.
		Lane mLane ("Converted Lane");

		//Make lane's system. freqbase is the most critical field.
		System mSystem("Converted System");
		Frequency mFreq (atof(toConvert.sampleRateHz));
		mSystem.BaseFrequency() = mFreq;

		Session mSession ("Converted Session");
		mSession.Poc(toConvert.engineer);
		mSession.Campaign(toConvert.campaign);

		/**
			//other info
			const char* creationTime;
			const char* scenario;
			const char* antenna;
			const char* location;
			const char* company;
			const char* copyright;
		*/

		//then, write a block to the lane.
		Block mBlock ("Converted Block");

		//how big is a block? one ms? frequency = cycles/sec. So simply divide frequnecy by 1000.
		int blockCycles = atoi(toConvert.sampleRateHz) / 1000;
		mBlock.Cycles(blockCycles);
		//footer is four bytes
		mBlock.SizeFooter(4);
		
		Chunk mChunk ("Converted Chunk");
		//Sloppy but works for this case
		mChunk.SizeWord((atoi(toConvert.bitsPerSample) * atoi(toConvert.numStreams)) / 8);
		mChunk.CountWords(1);

		Lump mLump ("Converted Lump");
		
		//now the fun part: adding streams.
		for(std::vector<XStream * >::iterator stritr = toConvert.streams->begin(); stritr != toConvert.streams->end(); ++stritr) 
		{

			Stream * mStream = new Stream( (*stritr)->name);

			mStream->Packedbits(atoi(toConvert.bitsPerSample));
			mStream->Quantization(atoi(toConvert.bitsPerSample));
			mStream->Encoding("INT8");
			mStream->Format(Stream::IF);

			Band * bs = new Band( ((*stritr)->index) );
			bs->CenterFrequency(*( new Frequency ( atof((*stritr)->carrier) , Frequency::Hz )) );
			bs->TranslatedFrequency(*(new Frequency ( atof((*stritr)->intermediate) , Frequency::Hz) ) );

			mStream->Bands().push_back(*bs);
			mLump.Streams().push_back(*mStream);
		}
		

		//construct the hierarchy.
		mChunk.Lumps().push_back(mLump);		
		mBlock.Chunks().push_back(mChunk);
		//TODO: Add support so GNSS reader can iterate over blocks instead of explicitly defining each.
		mLane.Blocks().push_back(mBlock);

		long fileSizeMinusOffset = getFileSize(SDRName->c_str()) - blockOffsetInBytes;
		//2 bytes per cycle, don't forget the footer
		long totalBlockCount = fileSizeMinusOffset / ((2*blockCycles) + 4);
		long totalBlockMod = fileSizeMinusOffset % ((2*blockCycles) + 4);

		cout << totalBlockCount;
		printf("File size minus Offset: \n");
		cout << fileSizeMinusOffset;
		printf("Block Count \n");
		cout << totalBlockCount;
		printf("LeftOver Bytes \n");
		cout << totalBlockMod;

		
		Block * copy = (new Block("Converted Block"));

		//copy->IsReference(true);
		//this can't be right. But how else can I designate a post-block-stream footer? It turns out this is a special case in the XML.
		//mLane.Blocks().push_back(*copy);

		mLane.Systems().push_back(mSystem);
		mLane.Sessions().push_back(mSession);
		mFile.Lane(mLane);
		md.Files().push_back(mFile);

		std::string * SDRXName = changeExt(XFile,"tgx");
		//If we happen upon an absolute file path, remove it. 
				std::cout << writeAtHomeFlag;
		
		const size_t lastSlashIndex = SDRXName->rfind('\\');
		String relativeName;

		if(lastSlashIndex != String::npos)
		{
			printf(SDRXName->c_str());
			cout << lastSlashIndex;
			relativeName = SDRXName->substr(lastSlashIndex+1,String::npos);
			SDRXName = &relativeName;
			printf(SDRXName->c_str());
		}

		if(!writeAtHomeFlag)
		{
			//Rename the old file.
			rename(XFile->c_str(),XFileOld.c_str());
		}

		try
		{
			if(writeAtHomeFlag)
			{
				//put it in this app directory, don't touch old SDRs.
				cout << "\nAppdiris\n" <<  appDir << "\n";
				changeWD(appDir);
			}

			proc.Save( SDRXName->c_str(),  md);
			cin.get();
		}
		catch( ApiException& e)
		{
			printf("An error occurred while saving the xml file: %s\n", e.what() );
		}

		
		bool foundFile = false;

		//find the next X_FILE

		for(std::vector<String>::iterator fileit = paths.begin(); fileit != paths.end(); ++fileit) {
			changeWD((*fileit).c_str());
			if(fileExists(nextFile.c_str()))
			{
				printf("Found ");
				foundFile = true;
				break;
			}
		}

		if(!foundFile)
		{
			printf(XFile->c_str());
			printf("Could not be found");
		}

		//TODO destroy metadata if I need to
		if(fileExists(nextFile.c_str()))
		{
			printf("Continue!");
			printf(nextFile.c_str());
			XFile = (new String(nextFile.c_str()));
		}
		else
		{
			printf("Finish!: file did not exist");
			printf(nextFile.c_str());
		//	printf(fileExists(nextFile.c_str()));
			XFile = NULL;
		}
	}
}

bool Convert::pullXMetadata(tinyxml2::XMLDocument * doc)
{	
	//doc.ErrorID();

	XMLElement* titleElement = doc->FirstChildElement();
	printf(titleElement->FirstChildElement()->Name());

	//TODO This is the wrong file.

	toConvert.numStreams = titleElement->FirstChildElement("NUMSTREAMS")->GetText();
	toConvert.bitsPerSample = titleElement->FirstChildElement("BITSPERSAMPLE")->GetText();
	toConvert.blockOffsetDWords = titleElement->FirstChildElement("BLOCKOFFSETDWORDS")->GetText();
	toConvert.footerMask = titleElement->FirstChildElement("FOOTERMASK")->GetText();
	toConvert.sampleRateHz = titleElement->FirstChildElement("SAMPLERATEHZ")->GetText();
	toConvert.nextFile = titleElement->FirstChildElement("NEXTFILENAME")->GetText();

	//toConvert.creationTime = titleElemer
	toConvert.scenario = titleElement->FirstChildElement("SCENARIO")->GetText();
	toConvert.campaign = titleElement->FirstChildElement("CAMPAIGN")->GetText();
	//const char* antenna;
	//const char* location;
	toConvert.engineer = titleElement->FirstChildElement("ENGINEER")->GetText();
	toConvert.company = titleElement->FirstChildElement("COMPANY")->GetText();
	toConvert.copyright = titleElement->FirstChildElement("COPYRIGHT")->GetText();
	toConvert.streams = new vector <XStream * > ();

	int streamCount = atoi(toConvert.numStreams);

	XMLElement * streamNode = titleElement->FirstChildElement("STREAM");

	for(int i = 0; i != streamCount; i++)
	{
		//for each stream
		struct XStream * stream = new struct XStream;

		stream->index = streamNode->Attribute("INDEX");
		stream->name = streamNode->Attribute("NAME");
		stream->carrier = streamNode->Attribute("FCAR");
		stream->intermediate = streamNode->Attribute("FIF");

		toConvert.streams->push_back(stream);
		streamNode = streamNode->NextSibling()->ToElement();


	
	}


	return true;
}

//So we know the file to decode.
std::string * Convert::changeExt(std::string * XFilePtr, const char * ext)
{
	std::string * retString = new std::string;
	retString->assign(*XFilePtr);

	while(retString->at(retString->size() - 1) != '.')
	{
		retString->pop_back();
	}

	retString->append(ext);
	return retString;
}
