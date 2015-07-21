/**
 * File: Lump.h
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

#ifndef LUMP_H_H
#define LUMP_H_H

#include "BaseTypes.h"
#include "Stream.h"
#include "AttributedObject.h"

namespace GnssMetadata
{
	/**
	 * Class defines a collection of sample words organized into a contiguous block. This is known as a Lump in the 
	 * currente draft specfication.
	 */
	class Lump: public AttributedObject
	{
	public:
		/**
		 * Default constructor for the lump.
		 */
		Lump(  const String& id = "", bool bIsReference = false)
			: AttributedObject( id, bIsReference)
		{
			
		}

		Lump( const Lump& rhs) : AttributedObject(rhs), 
			_streamlist(rhs._streamlist)
		{
			
		}

		const Lump& operator=( const Lump& rhs)
		{
			if( &rhs == this)
				return *this;
			AttributedObject::operator =(rhs);
			_streamlist = rhs._streamlist;
			return *this;
		}

		const GnssMetadata::StreamList& Streams( ) const
		{
			return _streamlist;
		}
		GnssMetadata::StreamList& Streams( )
		{
			return _streamlist;
		}

		/**
		 * Returns a string representation of the object.
		 */
		virtual String toString( const String & sFormat = DefaultFormat ) const;

		/**
		 * Virtual function traverses collections of attributed objects looking for object with the
		 * specified id.  Returns the count of objects found.
		 */
		virtual size_t FindObject( SearchItem::List& listResults, 
			const String& sid, const AttributedObject& rparent, bool bExcludeReference=true, int nDepth =-1) const;

	private:
		GnssMetadata::StreamList _streamlist;		
	};

	typedef std::list<Lump> LumpList;	
	
}


#endif
