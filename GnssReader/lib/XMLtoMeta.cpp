#include<GnssMetadata/Xml/XmlProcessor.h>
#include "XMLtoMeta.h"
#include<iostream>
//This is more of a static class-> clean it up
using namespace GnssMetadata;
//If there are zero declarations of an XML AttributedObject, this error is to be thrown.
class BadID: public std::exception
{
  char* badIDName;
  virtual const char* what() const throw()
  {
	return badIDName;
  }

public:
	void setBadIDName(char * name)
	{
		badIDName = name;
	}
};


	template<typename T, typename PT> void XMLtoMeta::fixRefdObjs(Metadata* md,std::list<T, std::allocator <T> >* objList, T** objArray)
	{

		int i = 0;
		//for every object in the list.
	
		for(typename std::list<T>::iterator iter = objList->begin();
			iter !=objList->end();iter++)
		{
			//The object is a reference, so we are going to need to search to find the original object.
			if(iter->IsReference())
			{
				AttributedObject::SearchItem::List returnList;
				int objectsFound = md->FindObject(returnList,iter->Id(),*md);
				if(objectsFound == 0)
				{
					//no matching object found!
					BadID e ;
					std::string badIDName = iter->Id().c_str();
					char * badIdc = new char[badIDName.size() + 1];
					std::copy(badIDName.begin(), badIDName.end(), badIdc);
					badIdc[badIDName.size()] = '\0'; 
					e.setBadIDName(badIdc);
					throw e;
				}
				//Got an non-referenced object! now, put it in the array.
				const AttributedObject* foundObject = (returnList.front().pObject);
				AttributedObject* foundObjectNoConst = const_cast<AttributedObject*>(foundObject);	
				objArray[i] = dynamic_cast<PT>(foundObjectNoConst);
			} else {
				objArray[i] = &*iter;
			}
			i++;
		}
	};

	//Retrives a single XML Link (not for lists).
	template<typename T> T XMLtoMeta::findNonRefObj(Metadata* md, AttributedObject* obj)
	{
		if(obj->IsReference())
		{
			//The object is a reference, so we are going to need to search to find the original object.
			AttributedObject::SearchItem::List returnList;
			int objectsFound = md->FindObject(returnList,obj->Id(),*md);
			if(objectsFound == 0)
			{
				BadID e ;
				std::string badIDName = obj->Id().c_str();
				char * badIdc = new char[badIDName.size() + 1];
				std::copy(badIDName.begin(), badIDName.end(), badIdc);
				badIdc[badIDName.size()] = '\0'; 
				e.setBadIDName(badIdc);
				throw e;
			}
		
			const AttributedObject* foundObject = (returnList.front().pObject);
			AttributedObject* foundObjectNoConst = const_cast<AttributedObject*>(foundObject);
			return dynamic_cast<T>(foundObjectNoConst);
		} else {
			//The object wasn't a reference, meaning it was the original.
			return dynamic_cast<T>(obj);
		}
	};

	//gets rid of referenced objects for all XML and converts to arrays.
	void XMLtoMeta::fixAllRefdObjs(){

		File* singleFile = &md.Files().front();
		singleFile->Lane(*findNonRefObj<Lane*>(&md,&singleFile->Lane()));
		//first thing: init block array.
		Lane* singleLane = &singleFile->Lane();
		singleFile->nLane = singleLane;
		singleLane->blockCount = singleLane->Blocks().size();
		


		singleLane->blockArray = new Block*[singleLane->blockCount];
		fixRefdObjs<Block,Block*>(&md,&singleLane->Blocks(),singleLane->blockArray);

		for(int i = 0; i != singleLane->blockCount; i++)
		{
			Block* block = singleLane->blockArray[i];
			//init this block's chunks.
			block->chunkCount = block->Chunks().size();
			block->chunkArray = new Chunk*[block->chunkCount];
			fixRefdObjs<Chunk,Chunk*>(&md,&block->Chunks(),block->chunkArray); 

			for(int i = 0; i != block->chunkCount; i++){
				Chunk* chunk = block->chunkArray[i];
				//init this chunk's lumps.
				chunk->lumpCount = chunk->Lumps().size();
				chunk->lumpArray = new Lump*[chunk->lumpCount];
				//then lump array...
				fixRefdObjs<Lump,Lump*>(&md,&chunk->Lumps(),chunk->lumpArray); 

					for(int i = 0; i != chunk->lumpCount; i++)
					{
					Lump* lump = chunk->lumpArray[i];
					lump->streamCount = lump->Streams().size();
					lump->streamArray = new Stream*[lump->streamCount];
					//finally, streams
					fixRefdObjs<Stream,Stream*>(&md,&lump->Streams(),lump->streamArray); 
				}
			}
		}
	};

	XMLtoMeta::XMLtoMeta(const char* metadataFile){
		XmlProcessor xproc;
		//checkAPI to see if this can cause trouble
		xproc.Load(metadataFile, false, md); 
		fixAllRefdObjs();		
	}


	Metadata* XMLtoMeta::getNonRefdMetadata(){

		return &md;
	}

	XMLtoMeta::XMLtoMeta(){
	
	}

		
