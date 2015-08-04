#include <tinyxml2.h>
#include <GnssMetadata/Metadata.h>
#include <GnssMetadata/XML/XmlProcessor.h>
#include <iostream>
#include <vector>

using namespace tinyxml2;
using namespace GnssMetadata;
using namespace std;

//Stream information pulled from old XML
struct XStream {

	const char* index;
	const char* name;
	const char* carrier;
	const char* intermediate;

};

//Metadata information pulled from old XML
struct XMetadata {

	const char* numStreams;
	const char* bitsPerSample;
	const char* blockOffsetDWords;
	const char* footerMask;
	const char* sampleRateHz;
	const char* nextFile;
	vector <XStream * > * streams;

};

//Current set of Metadata that has been pulled
struct XMetadata toConvert;


//Given an old XML document, pulls metadata to be put in toConvert.
bool pullXMetadata(XMLDocument*);

//Given a string and extension, changes the file extension.
string * changeExt(string*,const char *);

void main()
{
	//pass nothing? Get this dialogue.
	//pass 1 arg? Well, that's an XML file holding all these parameters.
	//pass 5 args? That's all the arguments we need.

	//First tgx file name
	cout << "Enter the name of the first tgx file\n";
	std::string f1;
	getline (cin, f1);

	//quick and dirty test
	if(f1.at(0) == '*')
	{
		std::string def ("C:\\Users\\ANTadmin\\Desktop\\convertMe\\Trigger\\TRIGRDATA_56320kHz_04bit_Ch0123_2014-06-09-13-01-43-546.tgx");
		f1 = def;
	}

	//PathsCsv (where to find the files, paths separated by commas – similar to the decoder)
	cout << "Enter the list of paths to search, seperated by commas. \n";
	String paths;
	cin >> paths;

	//Splice Mode (int_32): 0: convert this file only, >0: attempt to find this many concurrent files and convert, <0: keep converting files until concurrent sequence ends
	cout << "Enter the count of files to convert. '0' means convert all that are found. \n";
	String convertCount;
	cin >> convertCount;

	//Write Mode (bool): 0: write converted files to source dir and rename old file.tgx file to file.tgx_old; 1: write all converted files to app home directory
	cout << "Would you like to write the converted files to the source directory? Otherwise they will be written to this app directory. ( Y / N ) \n";
	String writeAtHome;
	cin >> writeAtHome;

	//Output log file of converter activity (session report in text format).
	//TODO

	string * XFile = &f1; 

	while(XFile != NULL)
	{
		//Pulls all data from X-XML file.
		XMLDocument XDoc;
		XDoc.LoadFile(XFile->c_str());
		pullXMetadata(&XDoc);
	
		std::cout << "---FILE SUMMARY---\n";
		std::cout << " Streams: " <<  toConvert.numStreams <<"\n";
		std::cout << " Bits/Sample: " <<  toConvert.bitsPerSample <<"\n";
		std::cout << " BlockOffset(DWORD): " <<  toConvert.blockOffsetDWords <<"\n";
		std::cout << " FooterMask: " <<  toConvert.footerMask <<"\n";
		std::cout << " SampleRate (Hz): " <<  toConvert.sampleRateHz <<"\n";
		std::cout << " Next File: " <<  toConvert.nextFile <<"\n";

		Metadata md;
		XmlProcessor proc;

		//let us first input the file into the metadata.
		File mFile;
		std::string * SDRName = changeExt(XFile,"tgd");
		mFile.Url(*SDRName);
		//we also shall tell it the blockoffset. A Dword is 4bytes.
		mFile.Offset(4 * atoi(toConvert.blockOffsetDWords));
		//TODO: declare next, and lane.

		//Done with file! now make a lane.
		Lane mLane;

		//Make lane's system. freqbase is the most critical field.
		System mSystem;
		Frequency mFreq (atof(toConvert.sampleRateHz));
		mSystem.BaseFrequency() = mFreq;

		//then, write a block to the lane.
		Block mBlock;
		//how big is a block? one ms?
		mBlock.Cycles(1337);
		Chunk mChunk;
		//Sloppy but works for this case
		mChunk.SizeWord((atoi(toConvert.bitsPerSample) * atoi(toConvert.numStreams)) / 8);
		mChunk.CountWords(1);

		Lump mLump;
		
		//now the fun part: adding streams.
		for(std::vector<XStream * >::iterator stritr = toConvert.streams->begin(); stritr != toConvert.streams->end(); ++stritr) 
		{
			Stream * mStream = new Stream();

			mStream->Id((*stritr)->name);

			mStream->Packedbits(atoi(toConvert.bitsPerSample));
			mStream->Quantization(atoi(toConvert.bitsPerSample));
			
			Band * bs = new Band( ((*stritr)->index) );
			bs->CenterFrequency(*( new Frequency ( atof((*stritr)->carrier) , Frequency::Hz )) );
			bs->TranslatedFrequency(*(new Frequency ( atof((*stritr)->intermediate) , Frequency::Hz) ) );

			mStream->Bands().push_back(*bs);
			mLump.Streams().push_back(*mStream);
			
		}
		

		//construct the hierarchy.
		mChunk.Lumps().push_back(mLump);		
		mBlock.Chunks().push_back(mChunk);
		mLane.Blocks().push_back(mBlock);
		mLane.Systems().push_back(mSystem);
		mFile.Lane(mLane);
		md.Files().push_back(mFile);

		cin.get();cin.get();

		std::string * SDRXName = changeExt(XFile,"sdrx");
		try
		{
			proc.Save( SDRXName->c_str(),  md);
		}
		catch( ApiException& e)
		{
			printf("An error occurred while saving the xml file: %s\n", e.what() );
		}

		XFile = NULL;
	}
}

bool pullXMetadata(tinyxml2::XMLDocument * doc)
{	
	//doc.ErrorID();

	XMLElement* titleElement = doc->FirstChildElement();

	toConvert.numStreams = titleElement->FirstChildElement("NUMSTREAMS")->GetText();
	toConvert.bitsPerSample = titleElement->FirstChildElement("BITSPERSAMPLE")->GetText();
	toConvert.blockOffsetDWords = titleElement->FirstChildElement("BLOCKOFFSETDWORDS")->GetText();
	toConvert.footerMask = titleElement->FirstChildElement("FOOTERMASK")->GetText();
	toConvert.sampleRateHz = titleElement->FirstChildElement("SAMPLERATEHZ")->GetText();
	toConvert.nextFile = titleElement->FirstChildElement("NEXTFILENAME")->GetText();
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
std::string * changeExt(std::string * XFilePtr, const char * ext)
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
