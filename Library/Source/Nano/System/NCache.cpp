/*	NAME:
		NCache.cpp

	DESCRIPTION:
		Cache object.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NTimeUtilities.h"
#include "NCache.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NIndex kDefaultMaxSize									= 100;
static const NIndex kDefaultMaxCost									= 30 * kNMegabyte;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
struct NCompareLRU
{
	inline bool operator()(const NCacheItem &value1, const NCacheItem &value2) const
	{
		return(value1.theValue->GetAccessTime() < value2.theValue->GetAccessTime());
	}
};





//============================================================================
//		NCache::NCache : Constructor.
//----------------------------------------------------------------------------
NCache::NCache(void)
{


	// Initialize ourselves
	mMaxSize = kDefaultMaxSize;
	mMaxCost = kDefaultMaxCost;

	mCacheCost = 0;
}





//============================================================================
//		NCache::~NCache : Destructor.
//----------------------------------------------------------------------------
NCache::~NCache(void)
{


	// Clean up
	Clear();
}





//============================================================================
//		NCache::GetSize : Get the size.
//----------------------------------------------------------------------------
NIndex NCache::GetSize(void) const
{


	// Get the size
	return((NIndex) mCache.size());
}





//============================================================================
//		NCache::GetCost : Get the cost.
//----------------------------------------------------------------------------
NIndex NCache::GetCost(void) const
{


	// Get the cost
	return(mCacheCost);
}





//============================================================================
//		NCache::Clear : Clear the cache.
//----------------------------------------------------------------------------
void NCache::Clear(void)
{


	// Clear the cache
	while (!mCache.empty())
		RemoveKey(mCache.begin()->first);
}





//============================================================================
//		NCache::Purge : Purge the cache.
//----------------------------------------------------------------------------
void NCache::Purge(void)
{	NCacheItemList					theItems;
	NCacheItemListConstIterator		theIter;



	// Check our state
	if (!NeedsPurge())
		return;
	
	
	
	// Get the state we need
	//
	// Items are sorted with least-recently-used to the front.
	theItems = GetItems();
	
	sort(theItems, NCompareLRU());



	// Purge the cache
	for (theIter = theItems.begin(); theIter != theItems.end(); theIter++)
		{
		if (theIter->theValue->IsPurgeable())
			RemoveKey(theIter->theKey);
		
		if (!NeedsPurge())
			break;
		}
}





//============================================================================
//		NCache::HasKey : Does a key exist?
//----------------------------------------------------------------------------
bool NCache::HasKey(const NCacheKey &theKey) const
{	NCacheMapConstIterator		theIter;
	bool						hasKey;



	// Check the cache
	theIter = mCache.find(theKey);
	hasKey  = (theIter != mCache.end());
	
	return(hasKey);
}





//============================================================================
//		NCache::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NCache::RemoveKey(const NCacheKey &theKey)
{	NCacheValue				*theValue;
	NCacheMapIterator		theIter;



	// Validate our state
	NN_ASSERT(HasKey(theKey));



	// Get the state we need
	theIter  = mCache.find(theKey);
	theValue = theIter->second;



	// Remove the key
	mCacheCost -= theValue->GetCost();
	NN_ASSERT(mCacheCost >= 0);

	mCache.erase(theIter);
	theValue->Release();
}





//============================================================================
//		NCache::GetValue : Get a value.
//----------------------------------------------------------------------------
NCacheValue *NCache::GetValue(const NCacheKey &theKey) const
{	NCacheValue				*theValue;
	NCacheMapConstIterator	theIter;



	// Get the value
	theValue = NULL;
	theIter  = mCache.find(theKey);
	
	if (theIter != mCache.end())
		{
		theValue = theIter->second;
		theValue->SetAccessTime();
		}

	return(theValue);
}





//============================================================================
//		NCache::SetValue : Set a value.
//----------------------------------------------------------------------------
void NCache::SetValue(const NCacheKey &theKey, NCacheValue *theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue->GetCost() >= 0);



	// Prepare to set
	if (HasKey(theKey))
		RemoveKey(theKey);

	if (NeedsPurge())
		Purge();



	// Set the value
	mCache[theKey] = theValue;
	mCacheCost    += theValue->GetCost();

	theValue->SetAccessTime();
	theValue->Retain();
}





//============================================================================
//		NCache::GetName : Get the cache name.
//----------------------------------------------------------------------------
NString NCache::GetName(void) const
{


	// Get the name
	return(mName);
}





//============================================================================
//		NCache::SetName : Set the cache name.
//----------------------------------------------------------------------------
void NCache::SetName(const NString &theName)
{


	// Set the name
	mName = theName;
}





//============================================================================
//		NCache::GetMaxSize : Get the maximum size.
//----------------------------------------------------------------------------
NIndex NCache::GetMaxSize(void) const
{


	// Get the value
	return(mMaxSize);
}





//============================================================================
//		NCache::SetMaxSize : Set the maximum size.
//----------------------------------------------------------------------------
void NCache::SetMaxSize(NIndex theValue)
{


	// Set the value
	mMaxSize = theValue;
}





//============================================================================
//		NCache::GetMaxCost : Get the maximum cost.
//----------------------------------------------------------------------------
NIndex NCache::GetMaxCost(void) const
{


	// Get the value
	return(mMaxCost);
}





//============================================================================
//		NCache::SetMaxCost : Set the maximum cost.
//----------------------------------------------------------------------------
void NCache::SetMaxCost(NIndex theValue)
{


	// Set the value
	mMaxCost = theValue;
}





//============================================================================
//		NCache::NeedsPurge : Does the cache need to purge?
//----------------------------------------------------------------------------
#pragma mark -
bool NCache::NeedsPurge(void)
{	bool		needsPurge;



	// Check our state
	needsPurge = false;
	
	if (mMaxSize != 0 && GetSize() > mMaxSize)
		needsPurge = true;

	if (mMaxCost != 0 && GetCost() > mMaxCost)
		needsPurge = true;
	
	return(needsPurge);
}





//============================================================================
//		NCache::GetItems : Get the cache items.
//----------------------------------------------------------------------------
NCacheItemList NCache::GetItems(void)
{	NCacheItemList				theItems;
	NCacheItem					theItem;
	NCacheMapConstIterator		theIter;



	// Get the items
	for (theIter = mCache.begin(); theIter != mCache.end(); theIter++)
		{
		theItem.theKey = theIter->first;
		theItem.theValue = theIter->second;
		
		theItems.push_back(theItem);
		}

	return(theItems);
}

