/**
 * File: AttributedObject.h
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

#ifndef ATTRIBUTEDOBJECT_H_H
#define ATTRIBUTEDOBJECT_H_H

#include "BaseTypes.h"
#include "AnyUri.h"
#include "Comment.h"

namespace GnssMetadata
{
	/**
	 * An attributed object can have comments and artifact URLs associated with it.
	 */
	class AttributedObject: public Object
	{
	public:
		/** 
		 * Class provides results for searching for attributed objects.
		 */
		struct SearchItem
		{
			typedef std::list<SearchItem> List;
			const AttributedObject* pParent;
			const AttributedObject* pObject;
		};

	public:
		/**
		 * Standard reference constructor.  Should be implemented by all
		 * derived objects as is. This is reuqired to make ToReference function 
		 * work right.
		 */
		AttributedObject( const String& id, bool bIsReference = false) 
			: _id( id), _bIsReference( bIsReference)
		{}
		AttributedObject() 
			: _bIsReference(false) 
		{}
		AttributedObject(const AttributedObject& rhs) 
			: _id( rhs._id), _bIsReference( rhs._bIsReference),
			_comments( rhs._comments), _artifacts(rhs._artifacts)
		{}

		const AttributedObject& operator=( const AttributedObject& rhs)
		{
			_id = rhs._id;
			_bIsReference = rhs._bIsReference;
			_comments = rhs._comments;
			_artifacts = rhs._artifacts;
			return *this;
		}

		void Id( const String& id )
		{
			_id = id;
		}
		String Id( ) const
		{
			return _id;
		}

		bool IsReference() const 
		{
			return _bIsReference;
		}
		void IsReference( bool bValue)
		{
			_bIsReference = bValue;
		}

		/** 
		 * Constructs a reference object of the current instance.  If already, a reference,
		 * just returns the object.  Note this function only works where the standard reference
		 * constructor is defined.
		 */
		template<typename Type>
		Type ToReference() const 
		{ 
			return ( _bIsReference)
				? *dynamic_cast< const Type*>(this)
				: Type( this->Id(), true);
		}

		const CommentList& Comments( ) const
		{
			return _comments;
		}
		CommentList& Comments( ) 
		{
			return _comments;
		}

		const AnyUriList& Artifacts( ) const
		{
			return _artifacts;
		}

		AnyUriList& Artifacts( ) 
		{
			return _artifacts;
		}

		/**
		 * Adds a comment to the object.
		 */
		void AddComment( const String& sComment, Comment::CommentFormat fmt = Comment::text )
		{
			Comment cmt( sComment, fmt);
			_comments.push_back( cmt);
		}

		/**
		 * Adds an artifact to the object.
		 */
		void AddArtifact( const String& sArtifact)
		{
			AnyUri uri( sArtifact);
			_artifacts.push_back( uri);
		}

		/**
		 *  Returns a string representation of the object.
		 */
		virtual String toString( const String & sFormat = DefaultFormat ) const;

		/**
		 * Virtual function traverses collections of attributed objects looking for object with the
		 * specified id.  Returns the count of objects found.
		 */
		virtual size_t FindObject( SearchItem::List& listResults, 
			const String& sid, const AttributedObject& rparent, bool bExcludeReference=true, int nDepth =-1) const;

	protected:
		/**
		 *Helper function searches a std list with specified search parameters.
		 */
		 template<typename Type> 
		 static size_t SearchList( AttributedObject::SearchItem::List& listResults, 
			 const std::list<typename Type>&, const String& sid, const AttributedObject& rparent, bool bExcludeReference, int nDepth );


	private:
		String		_id;
		bool		_bIsReference;
		CommentList _comments;
		AnyUriList  _artifacts;
	};


	




	/**
	 *Helper function searches a std list with specified search parameters.
	 */
	 template<typename Type> 
	 static size_t AttributedObject::SearchList( AttributedObject::SearchItem::List& listResults, 
		 const std::list<typename Type>& listsrc, const String& sid, const AttributedObject& rparent, bool bExcludeReference, int nDepth )
	 {
		 //Don't search on zero depth.
		 if( nDepth == 0) return 0;
		
		 size_t count =0;
		 std::list<Type>::const_iterator iter = listsrc.begin();
		 for( ; iter != listsrc.end(); iter++)
		 {
			 //Search object for matching ID.
			 count += (*iter).FindObject( listResults, sid, rparent, bExcludeReference, nDepth);
		 }
		 return count;
	 }

	
}


#endif
