/**
 * File: XMLtoMeta.h
 * Author: WJLIDDY
 * Makes a Metadata object from an XML file
 */
#ifndef XML_TO_META_H_H
#define XML_TO_META_H_H

#include<GnssMetadata/Xml/XmlProcessor.h>

using namespace GnssMetadata;

class XMLtoMeta
{
	Metadata md;

	/** So here's the thing
	*	When I use the API's call to transform an xml to meta it does a few things I need to fix.
	*   First, if I reference something by it's ID in the xml, the metadata object gets two object
	*   One is the actual object and one is a reference
	*   So to fix this, I call the API's search function to get a pointer to the actual object.
	*   But once I find it, what do I do?
	*   Well it turns out that this XML parsing program stored objects in a std::list
	*   Which is WAY too slow for our application
	*   So I put all the references I find into af custom array
	*   This function here does just that. Fixes a referenced object list and puts it into an array.
	*   T is an original object, PT is a pointer of type T.
	*/
	template<typename T, typename PT> void fixRefdObjs(Metadata* md,std::list<T, std::allocator<T> >* objList, T** objArray);
	

	
	//Fixes all objects that are referenced and puts them into my defined array.
	void fixAllRefdObjs();

public:

	XMLtoMeta(const char* metadataFile);
	Metadata* getNonRefdMetadata();
	XMLtoMeta();
	//Retrives a single XML Link (not for lists).
	template<typename T> T findNonRefObj(Metadata* md, AttributedObject* obj);
		
};
#endif
