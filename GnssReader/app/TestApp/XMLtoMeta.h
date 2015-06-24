#ifndef XML_TO_META_H_H
#define XML_TO_META_H_H

#include<GnssMetadata/Xml/XmlProcessor.h>

using namespace GnssMetadata;
//If there are zero declarations of an XML AttributedObject, this error is to be thrown.


class XMLtoMeta
{
	Metadata md;

	//Converts lists contaning referenced objects to an array without referenced objects. Run this before doing any XML operations!
	template<typename T, typename PT> void fixRefdObjs(Metadata* md,std::list<T, std::allocator<T>>* objList, T** objArray);
	//Retrives a single XML Link (not for lists).
	template<typename T> T findNonRefObj(Metadata* md, AttributedObject* obj);
	void fixAllRefdObjs();
	void deallocAllRefdObjs();

public:

	XMLtoMeta::XMLtoMeta(const char* metadataFile);
	XMLtoMeta::XMLtoMeta();
	Metadata* getNonRefdMetadata();
		
};
#endif