#include <tinyxml2.h>
#include <GnssMetadata/Metadata.h>
#include <GnssMetadata/XML/XmlProcessor.h>
#include <iostream>

using namespace tinyxml2;
using namespace GnssMetadata;

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
	Metadata md;
	XmlProcessor proc;
	md.Lanes().push_back(lane);
	md.Files().push_back( df);
	md.Systems().push_back(sys);
	md.Streams().push_back(sm2);

	try
	{
		proc.Save( sfilemd.c_str(),  md);
	}
	catch( ApiException& e)
	{
		printf("An error occurred while saving the xml file: %s\n", e.what() );
	}
}
void main()
{

	const char * fileName = "C:\\Users\\ANTadmin\\Desktop\\convertMe\\Trigger\\TRIGRDATA_56320kHz_04bit_Ch0123_2014-06-09-13-01-43-546.tgx";

	tinyxml2::XMLDocument doc;
	doc.LoadFile( fileName );
	
	//doc.ErrorID();

	XMLElement* titleElement = doc.FirstChildElement();

	const char* numStreams = titleElement->FirstChildElement("NUMSTREAMS")->GetText();

	const char* bitsPerSample = titleElement->FirstChildElement("BITSPERSAMPLE")->GetText();
	
	const char* blockOffsetDWords = titleElement->FirstChildElement("BLOCKOFFSETDWORDS")->GetText();
	
	const char* footerMask = titleElement->FirstChildElement("FOOTERMASK")->GetText();

	const char* sampleRateHz = titleElement->FirstChildElement("SAMPLERATEHZ")->GetText();
	
	const char* nextFile = titleElement->FirstChildElement("NEXTFILENAME")->GetText();

	std::cout << "---FILE SUMMARY---\n";
	std::cout << " Streams: " << numStreams <<"\n";
	std::cout << " Bits/Sample: " << bitsPerSample <<"\n";
	std::cout << " BlockOffset(DWORD): " << blockOffsetDWords <<"\n";
	std::cout << " FooterMask: " << footerMask <<"\n";
	std::cout << " SampleRate (Hz): " << sampleRateHz <<"\n";
	std::cout << " Next File: " << nextFile <<"\n";

	//for each stream
	const char* index = titleElement->FirstChildElement("STREAM")->Attribute("INDEX");
	const char* name = titleElement->FirstChildElement("STREAM")->Attribute("NAME");
	const char* carrier = titleElement->FirstChildElement("STREAM")->Attribute("FCAR");
	const char* intermediate = titleElement->FirstChildElement("STREAM")->Attribute("FIF");
	std::cout << " Stream 1: " << index <<"\n";
	std::cout << " Stream 1: " << name <<"\n";
	std::cout << " Stream 1: " << carrier <<"\n";
	std::cout << " Stream 1: " << intermediate <<"\n";


	std::cin.get();


}