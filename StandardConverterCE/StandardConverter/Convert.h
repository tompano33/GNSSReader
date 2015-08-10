//need header guards

#include <vector>
#include <tinyxml2.h>
#include <GnssMetadata/Metadata.h>
#include <GnssMetadata/XML/XmlProcessor.h>

class Convert
{
	//Directory of this application
	const char* appDir;
	std::vector <std::string> paths;
	bool writeAtHomeFlag;

	bool convertAll;
	size_t filesToConvert;

	//current file to decode
	std::string * XFile;

	//Stream information pulled from old XML - Packaged Inside Metadata
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
		std::vector <XStream * > * streams;

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

	//Given an old XML document, pulls metadata to be put in toConvert.
	bool pullXMetadata(tinyxml2::XMLDocument*);

	//Given a string and extension, returns new string with the new file extension.
	std::string * changeExt(std::string*,const char *);

	//Extracts PathsCsv to a vector of strings.
	void extractPaths(const char *);

	//Changes Working Directory
	static void changeWD(const char*);

	//returns size of file
	long getFileSize(const char* file);

	//returns 1 if it found the file and sets the working dir to it. Otherwise, returnd false.
	bool findFile(const char*);

public:

	void start();
	//returns true if file exists
	static bool fileExists(const char* file);

	Convert::Convert(const char*, const char*, std::vector<std::string>, int, bool);

};