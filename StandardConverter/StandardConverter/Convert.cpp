#include <tinyxml2.h>
#include <GnssMetadata/Metadata.h>
#include <GnssMetadata/XML/XmlProcessor.h>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <direct.h>

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

	//critical info for decoding
	const char* numStreams;
	const char* bitsPerSample;
	const char* blockOffsetDWords;
	const char* footerMask;
	const char* sampleRateHz;
	const char* nextFile;
	vector <XStream * > * streams;

	//other info
	const char* creationTime;
	const char* scenario;
	const char* campaign;
	const char* antenna;
	const char* location;
	const char* engineer;
	const char* company;
	const char* copyright;

};

//Current set of Metadata that has been pulled
struct XMetadata toConvert;
vector <String> paths;
int convertCountParsed;
bool writeAtHomeFlag;

//arguments. f1 is the inital file. 
String f1;
//all paths, but as CSV.
String pathsCSV;
String convertCount;
String writeAtHome;

//Given an old XML document, pulls metadata to be put in toConvert.
bool pullXMetadata(XMLDocument*);

//Given a string and extension, changes the file extension.
string * changeExt(string*,const char *);

void extractPaths()
{
	std::string delimiter = ",";
	size_t pos = 0;
	std::string token;
	while ((pos = pathsCSV.find(delimiter)) != std::string::npos) {
		token = pathsCSV.substr(0, pos);
		paths.push_back(token);
		pathsCSV.erase(0, pos + delimiter.length());	
	}
	paths.push_back(pathsCSV);
}

void changeWD(const char* pathToFile)
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

bool fileExists(const char* file) {
    struct stat buf;
    return (stat(file, &buf) == 0);
}

long getFileSize(const char* file)
{
	struct stat stat_buf;
	int rc = stat(file,&stat_buf);
	return stat_buf.st_size;
}
	
void noArgDialogue()
{
	//First tgx file name
	cout << "Enter the name of the first tgx file\n";
	getline (cin, f1);

	//quick and dirty test
	if(f1.size() == 0)
	{
		std::string def ("C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Tests\\trigrPaths\\folder2\\TRIGRDATA_56320kHz_04bit_Ch0123_2014-06-09-13-01-43-546.tgx");
		f1 = def;
	}

	//PathsCsv (where to find the files, paths separated by commas � similar to the decoder)
	cout << "Enter the list of paths to search, seperated by commas. \n";
	getline (cin, pathsCSV);

	
	//quick and dirty test
	if(pathsCSV.size() == 0)
	{
		//why... doesn't this get popped off the stack? shouldn't defs constructor be called?
		std::string def ("C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Tests\\trigrPaths\\folder2\\,C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Tests\\trigrPaths\\folder\\,C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Tests\\trigrPaths\\folder\\folder\\,C:\\Users\\ANTadmin\\Desktop\\SDR_STANDARD\\Tests\\trigrPaths\\");
		pathsCSV = def;
	}

	//Splice Mode (int_32): 0: convert this file only, >0: attempt to find this many concurrent files and convert, <0: keep converting files until concurrent sequence ends
	cout << "Enter the count of files to convert. '0' means convert all that are found. \n";
	getline (cin, convertCount);

	//Write Mode (bool): 0: write converted files to source dir and rename old file.tgx file to file.tgx_old; 1: write all converted files to app home directory
	cout << "Would you like to write the converted files to the source directory? Otherwise they will be written to this app directory. (Type 'Y' for yes, otherwise press enter) \n";
	getline (cin, writeAtHome);

}

void main()
{
	//pass nothing? Get this dialogue.
	//pass 1 arg? Well, that's an XML file holding all these parameters.
	//pass 5 args? That's all the arguments we need.
	noArgDialogue();

	extractPaths();

	convertCountParsed = atoi(convertCount.c_str());

	writeAtHomeFlag = (writeAtHome.c_str()[0] == 'Y');


	//Output log file of converter activity (session report in text format).
	//TODO

	string * XFile = &f1; 
	
	//TODO: add 'convertcount'
	while(XFile != NULL)
	{
		//Pulls all data from X-XML file.
		XMLDocument XDoc;

		//fails? check a different path.
		//change-wd and file/exists.
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
		File mFile ("Converted Trig File");
		std::string * SDRName = changeExt(XFile,"tgd");
		mFile.Url(*SDRName);
		//we also shall tell it the blockoffset. A Dword is 4bytes.
		long blockOffsetInBytes = 4 * atoi(toConvert.blockOffsetDWords);
		mFile.Offset(blockOffsetInBytes);
		std::string nextFile (toConvert.nextFile);
		nextFile = *changeExt(&nextFile,"tgx");
		std::string nextFileSDRX = *changeExt(&nextFile,"sdrx");
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
		copy->IsReference(true);

		//this can't be right. But how else can I designate a post-block-stream footer? time to read the draft.
		//too many, nerfing to 100
		//need a fix to this
		for(int i = 0 + 1; i != 100; i++)
		{
			mLane.Blocks().push_back(*copy);
		}
		
		mLane.Systems().push_back(mSystem);
		mLane.Sessions().push_back(mSession);
		mFile.Lane(mLane);
		md.Files().push_back(mFile);

		std::string * SDRXName = changeExt(XFile,"sdrx");

		try
		{
			proc.Save( SDRXName->c_str(),  md);
		}
		catch( ApiException& e)
		{
			printf("An error occurred while saving the xml file: %s\n", e.what() );
		}

		
		bool foundFile = false;
		//naivgate to a valid directory
		for(std::vector<String>::iterator fileit = paths.begin(); fileit != paths.end(); ++fileit) {
			changeWD((*fileit).c_str());
			if(fileExists(XFile->c_str()))
			{
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
			printf("Finish!");
			printf(toConvert.nextFile);
			XFile = NULL;
		}
	}

	
		cin.get();cin.get();
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
