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
	vector <XStream> streams;

};

//Current set of Metadata that has been pulled
struct XMetadata toConvert;

//temp function: ignore.
void WriteXmlFile(const char* pszFilename)
{
    printf("\nWriting GNSS Metadata to xml file: %s\n", pszFilename);

    ////////////////////////////////
	//Define information about the datafile.

	//UTC:    30-Dec-2014 22:38:54
	//GPS:    1825/254334.906
	size_t offset = 588;
    std::string sfile = "141230-gps-4msps.bds";
    std::string sfilemd = pszFilename;

	////////////////////////////////
	//Define the Session.
	Session sess("0");
	sess.Scenario("Example 2");
	sess.Campaign("GNSS Metadata API Testing");
	sess.Contact("J. Doe");
	sess.Position( Position(47.76471300, -122.15612900, 25.610));
	sess.AddComment("This is an example metadata specification with two interleaved streams.");

	////////////////////////////////
	//Define the System, Sources, and cluster.
	System sys("A2300-1");
	sys.BaseFrequency( Frequency( 4, Frequency::MHz));
	sys.Equipment("ASR-2300");
	sys.AddComment( "ASR-2300 configured with standard firmware and FPGA id=1, version=1.18.");

	Cluster clstr("Antenna");

	Source src1( Source::Patch, Source::RHCP, "L1 C/A");
	src1.IdCluster("Antenna");

	Source src2( Source::Patch, Source::RHCP, "L2 C");
	src2.IdCluster("Antenna");

	sys.AddSource( src1);
	sys.AddSource( src2);
	sys.AddCluster(clstr);

	////////////////////////////////
	//Define Band 1 and L1 C/A Stream.
	Band ch1("L1External");
	ch1.CenterFrequency(Frequency( 1575.42, Frequency::MHz));
	ch1.TranslatedFrequency(Frequency( 38400, Frequency::Hz));

	Stream sm1("L1ca");
	sm1.RateFactor(1);
	sm1.Quantization(8);
	sm1.Packedbits(16);
	sm1.Encoding("INT8");
	sm1.Format(Stream::IQ);
	sm1.Bands().push_back( ch1);

	////////////////////////////////
	//Define Band 2 and L2 C Stream.
	Band ch2("L2cExternal");
	ch2.CenterFrequency(Frequency( 1227.6, Frequency::MHz));
	ch2.TranslatedFrequency(Frequency( 38400, Frequency::Hz));

	//Stream sm2 will be added to the global metadata
	//and as a reference to the lump.
	Stream sm2("L2C");
	sm2.RateFactor(1);
	sm2.Quantization(8);
	sm2.Packedbits(16);
	sm2.Encoding("INT8");
	sm2.Format(Stream::IQ);
	sm2.Bands().push_back(ch2);

	////////////////////////////////
	//Define the lane
	Lump lump;
	lump.Streams().push_back( sm1);
	lump.Streams().push_back( sm2.ToReference<Stream>());	


	Chunk chunk;
	chunk.SizeWord(4);
	chunk.CountWords(1);
	chunk.Lumps().push_back(lump);

	Block blk(256);
	blk.Chunks().push_back(chunk);
	
	Lane lane("GPS SPS Data");
	lane.Sessions().push_back( sess);
	lane.Blocks().push_back(blk);
	lane.AddBandSource(ch1, src1);
	lane.AddBandSource(ch2, src2);
	lane.Systems().push_back( sys.ToReference<System>());

	////////////////////////////////
	//Define the file
	File df;
	df.Url( sfile);
	df.Lane( lane, true);

	//Note: we aren't specifying copyright or owner.


	////////////////////////////////
	//Assemble the Metadata object and write XML 

}

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
		Chunk mChunk;

		//construct the hierarchy.
		
		mBlock.Chunks().push_back(mChunk);
		mLane.Blocks().push_back(mBlock);
		mLane.Systems().push_back(mSystem);
		mFile.Lane(mLane);
		md.Files().push_back(mFile);

		/**
		LUMP
One or more lumps (specified in order) present in this chunk.
Lump
Required
SIZEWORD
Bytes of unsigned integer datatype that data shall be read as
UINT8
1, 2, 4, 8 (Corresponds to UINT8, UINT16, UINT32 and UINT64)
Required
COUNTWORDS
Total number of words to be read in order to read/decode this chunk
UINT8
Required
ENDIAN
Endianness of words stored in chunk
Endian
‘L’ – Little
‘B’ – Big
‘N’ – not applicable
yes
‘N’
PADDING
Padding applied during encoding
Padding
‘H’ – head padding
‘T’ – Tail padding
‘N’ – No padding
yes
‘N’
SHIFT
Word shift direction
WordShift
‘L’ – Left shift
‘R’ – Right shift
Required
		
		*/
		

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

/**
First tgx file name
PathsCsv (where to find the files, paths separated by commas – similar to the decoder)
Splice Mode (int_32): 0: convert this file only, >0: attempt to find this many concurrent files and convert, <0: keep converting files until concurrent sequence ends
Write Mode (bool): 0: write converted files to source dir and rename old file.tgx file to file.tgx_old; 1: write all converted files to app home directory
Output log file of converter activity (session report in text format).
NOTE: care should be taken to ‘do no harm’ to existing files. This app will be used to convert hundreds of thousands of files, some of which are the only copies we have.
*/