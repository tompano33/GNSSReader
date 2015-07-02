/**
 * File: StreamTranslator.cpp
 * Author: M.B. Mathews
 *  
 * Copyright(c) 2014 Institute of Navigation
 * http://www.ion.org
 *  
 * This Metadata API is free software; you can redistribute it and/or
 * modify it under the terms of the Lesser GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with Metadata API.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "StreamTranslator.h"
#include <GnssMetadata/Stream.h>
#include "XmlDefs.h"
#include <string.h>
#include <stdlib.h>

using namespace GnssMetadata;
using namespace tinyxml2;

// Alignment Values
static const char* _szAlignFmts[] = {"Left","Right", "Undefined"};
static Stream::StreamAlignment ToAlignmentFormat( const char* pszFmt)
{
    for( unsigned  int i = 0; i < 3; i++)
	{
		if( strcmp( _szAlignFmts[i], pszFmt) == 0)
			return (Stream::StreamAlignment)i;
	}
	return (Stream::StreamAlignment)2;
}

// Sample Format Values
static const char* _szSampleFmts[] = {"IF","IFn","IQ",
	"IQn","InQ","InQn","QI","QIn","QnI","QnIn"};
static Stream::SampleFormat ToSampleFormat( const char* pszFmt)
{
    for( unsigned int i = 0; i < 10; i++)
	{
		if( strcmp( _szSampleFmts[i], pszFmt) == 0)
			return (Stream::SampleFormat)i;
	}
	return (Stream::SampleFormat)2;
}


NODELIST_BEGIN(_StreamNodes)
	NODELIST_ENTRY( "ratefactor",	TE_SIMPLE_TYPE)
	NODELIST_ENTRY( "quantization",	TE_SIMPLE_TYPE)
	NODELIST_ENTRY( "packedbits",	TE_SIMPLE_TYPE)
	NODELIST_ENTRY( "alignment",	TE_SIMPLE_TYPE)
	NODELIST_ENTRY( "format",		TE_SIMPLE_TYPE)
	NODELIST_ENTRY( "encoding",		TE_SIMPLE_TYPE)
	NODELIST_ENTRY( "band",		TE_BAND)
	NODELIST_ENTRY( "comment",	TE_SIMPLE_TYPE)
	NODELIST_ENTRY( "artifact", TE_SIMPLE_TYPE)
NODELIST_END

StreamTranslator::StreamTranslator() 
: Translator( (NodeEntry*) _StreamNodes)
{
}

/**
 * Reads a node from the document and parses into metadata.
 */
bool StreamTranslator::OnRead( Context & ctxt, const XMLElement & elem, AccessorAdaptorBase* pAdaptor )
{
	const XMLElement* pchild;
	if( pAdaptor == NULL)
		return false;
	Stream stream;

	bool bRetVal = true;

	//Parse the AttributedObject Elements.
	if( !ReadAttributedObject( stream, ctxt, elem))
		return false;

	//Done processing element, if no children, meaning this is 
	//an element referencing another element.
	if( elem.NoChildren())
		stream.IsReference(true);
	else
	{
		//Parse ratefactor
		stream.RateFactor(ReadFirstElement("ratefactor", elem, true, (size_t) 0));

		//Parse quantization
		stream.Quantization(ReadFirstElement("quantization", elem, true, (size_t) 0));

		//Parse packedbits
		stream.Packedbits(ReadFirstElement("packedbits", elem, true, stream.Quantization() ));

		//Parse alignment
		pchild = elem.FirstChildElement("alignment");
		if( pchild != NULL)
		{
			stream.Alignment( ToAlignmentFormat(pchild->GetText()));
		}

		//Parse format
		pchild = elem.FirstChildElement("format");
		stream.Format( ToSampleFormat(pchild->GetText()));

		//Parse encoding
		pchild = elem.FirstChildElement("encoding");
		stream.Encoding( pchild->GetText());

		//Parse Channel.
		bRetVal &= ReadList<Band>(stream.Bands(), "band", ctxt, elem);
	}

	//Lastly set the stream on the specified object.
	if( bRetVal)
		pAdaptor->set( &stream);
	return bRetVal;
}
/**
 * Write the current object 
 */
void StreamTranslator::OnWrite( const Object * pObject, pcstr pszName, Context & ctxt, tinyxml2::XMLNode & elem )
{
	const Stream* pstream = dynamic_cast< const Stream*>(pObject);
	if( pstream == NULL) 
		throw TranslationException("StreamTranslator cannot cast Stream object");

	XMLElement* pelemc = elem.GetDocument()->NewElement( pszName);

	//Fill out id, artifacts, and comments last in accordance
	//with schema.
	WriteAttributedObject( *pstream, ctxt, *pelemc);

	if( !pstream->IsReference())
	{
		XMLElement* pelem;
		char buff[64];

		//Write ratefactor
		pelem = elem.GetDocument()->NewElement( "ratefactor");
		sprintf( buff, "%ld", pstream->RateFactor() );
		pelem->SetText( buff );
		pelemc->InsertEndChild( pelem);

		//Write quantization
		pelem = elem.GetDocument()->NewElement( "quantization");
		sprintf( buff, "%ld", pstream->Quantization() );
		pelem->SetText( buff );
		pelemc->InsertEndChild( pelem);

		//Write packedbits
		pelem = elem.GetDocument()->NewElement( "packedbits");
		sprintf( buff, "%ld", pstream->Packedbits() );
		pelem->SetText( buff );
		pelemc->InsertEndChild( pelem);

		//Write alignment
		pelem = elem.GetDocument()->NewElement( "alignment");
		pelem->SetText( _szAlignFmts[ pstream->Alignment()] );
		pelemc->InsertEndChild( pelem);

		//Write format
		pelem = elem.GetDocument()->NewElement( "format");
		pelem->SetText( _szSampleFmts[pstream->Format()] );
		pelemc->InsertEndChild( pelem);

		//Write encoding
		pelem = elem.GetDocument()->NewElement( "encoding");
		pelem->SetText( pstream->Encoding().c_str() );
		pelemc->InsertEndChild( pelem);

		//Write band
		WriteList<Band>( pstream->Bands(), "band", ctxt,*pelemc);

	}
	
	elem.InsertEndChild( pelemc);
}
