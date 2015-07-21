/**
 * File: Session.h
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

#ifndef SESSION_H_H
#define SESSION_H_H

#include "Position.h"
#include "Date.h"
#include "System.h"
#include "AttributedObject.h"
#include "BaseTypes.h"
#include <list>
namespace GnssMetadata
{
	class Session : public AttributedObject
	{
		
	public:
		Session( const String& id) : AttributedObject( id){}
		Session() {}
		Session(const Session& rhs) 
			: AttributedObject( rhs),
			_toa( rhs._toa), _position(rhs._position), 
			_poc(rhs._poc), _contact(rhs._contact),
			_campaign(rhs._campaign), _scenario(rhs._scenario),
			_systemlist( rhs._systemlist)
		{}	
		const Session& operator=( const Session& rhs)
		{
			if( &rhs == this)
				return *this;
			AttributedObject::operator =(rhs);
			_toa  = rhs._toa;
			_position = rhs._position; 
			_poc = rhs._poc;
			_contact = rhs._contact;
			_campaign = rhs._campaign; 
			_scenario = rhs._scenario;
			_systemlist = rhs._systemlist;
			return *this;
		}

		Date Toa( ) const
		{
			return _toa;
		}
		void Toa( const Date& toa)
		{
			_toa = toa;
		}

		String Campaign( ) const
		{
			return _campaign;
		}
		void Campaign( const String& campaign )
		{
			_campaign = campaign;
		}
		String Contact( ) const
		{
			return _contact;
		}
		void Contact( const String& contact )
		{
			_contact = contact;
		}


		const GnssMetadata::Position& Position( ) const
		{
			return _position;
		}
		GnssMetadata::Position& Position( ) 
		{
			return _position;
		}
		void Position( const GnssMetadata::Position& location )
		{
			_position = location;
		}

		String Poc( ) const
		{
			return _poc;
		}
		void Poc( const String& poc )
		{
			_poc = poc;
		}
		
		void Scenario( const String& scenario )
		{
			_scenario = scenario;
		}
		String Scenario( ) const
		{
			return _scenario;
		}

		const GnssMetadata::SystemList& Systems( ) const
		{
			return _systemlist;
		}
		GnssMetadata::SystemList& Systems( )
		{
			return _systemlist;
		}
		
		/**
		 *  Returns a string representation of the object.
		 *  
		 */
		virtual String toString( const String & sFormat = DefaultFormat ) const;
		
		/**
		 * Virtual function traverses collections of attributed objects looking for object with the
		 * specified id.  Returns the count of objects found.
		 */
		virtual size_t FindObject( SearchItem::List& listResults, 
			const String& sid, const AttributedObject& rparent, bool bExcludeReference=true, int nDepth =-1) const;

	private:
		GnssMetadata::Date _toa;

		GnssMetadata::Position _position;
		//TODO Add Attitude Information.
		String _poc;
		String _contact;
		String _campaign;
		String _scenario;
		GnssMetadata::SystemList _systemlist;
	};

	typedef std::list<Session> SessionList;
}


#endif
