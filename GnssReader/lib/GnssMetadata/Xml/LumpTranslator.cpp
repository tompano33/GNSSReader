/**
 * File: LumpTranslator.cpp
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


#include "LumpTranslator.h"
#include <GnssMetadata/Lump.h>
#include "XmlDefs.h"
#include <string.h>
#include <stdlib.h>

using namespace GnssMetadata;
using namespace tinyxml2;

NODELIST_BEGIN(_LumpNodes)
	NODELIST_ENTRY( "stream", TE_STREAM)
NODELIST_END


LumpTranslator::LumpTranslator() 
: Translator( (NodeEntry*) _LumpNodes)
{
}

/**
 * Reads a node from the document and parses into metadata.
 */
bool LumpTranslator::OnRead( Context & ctxt, const XMLElement & elem, AccessorAdaptorBase* pAdaptor )
{
	if( pAdaptor == NULL)
		return false;
	Lump lump;

	bool bRetVal = true;

	//Parse the AttributedObject Elements.
	if( !ReadAttributedObject( lump, ctxt, elem, false))
		return false;

	//Done processing element, if no children, meaning this is 
	//an element referencing another element.
	if( elem.NoChildren())
		lump.IsReference(true);
	else
	{
		//Parse Channel.
		bRetVal &= ReadList<Stream>(lump.Streams(), "stream", ctxt, elem);
	}

	//Lastly set the lump on the specified object.
	if( bRetVal)
		pAdaptor->set( &lump);
	return bRetVal;
}
/**
 * Write the current object 
 */
void LumpTranslator::OnWrite( const Object * pObject, pcstr pszName, Context & ctxt, tinyxml2::XMLNode & elem )
{
	const Lump* plump = dynamic_cast< const Lump*>(pObject);
	if( plump == NULL) 
		throw TranslationException("LumpTranslator cannot cast Lump object");

	XMLElement* pelemc = elem.GetDocument()->NewElement( pszName);

	//Fill out id, artifacts, and comments last in accordance
	//with schema.
	WriteAttributedObject( *plump, ctxt, *pelemc);

	if( !plump->IsReference())
	{
		//Write band
		WriteList<Stream>( plump->Streams(), "stream", ctxt,*pelemc);
	}
	

	elem.InsertEndChild( pelemc);
}
