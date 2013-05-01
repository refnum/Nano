/*	NAME:
		NCache.h

	DESCRIPTION:
		Cache object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCACHE_HDR
#define NCACHE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCacheValue.h"
#include "NCacheKey.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Items
typedef struct NCacheItem {
	NCacheKey		theKey;
	NCacheValue	   *theValue;
} NCacheItem;


// Lists
typedef std::map<NCacheKey, NCacheValue*>							NCacheMap;
typedef NCacheMap::iterator											NCacheMapIterator;
typedef NCacheMap::const_iterator									NCacheMapConstIterator;

typedef std::vector<NCacheItem>										NCacheItemList;
typedef NCacheItemList::iterator									NCacheItemListIterator;
typedef NCacheItemList::const_iterator								NCacheItemListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCache : public NContainer {
public:
										NCache(void);
	virtual							   ~NCache(void);


	// Clear the cache
	//
	// Releases all objects in the cache.
	void								Clear(void);


	// Purge the cache
	//
	// May release zero or more objects from the cache.
	void								Purge(void);


	// Get the size
	NIndex								GetSize(void) const;


	// Get the cost
	NIndex								GetCost(void) const;


	// Does a key exist?
	bool								HasKey(const NCacheKey &theKey) const;


	// Remove a key
	void								RemoveKey(const NCacheKey &theKey);


	// Get/set a value
	//
	// If the value does not exist, NULL is returned. Returned values should be retained
	// by the caller if they wish to prevent purging.
	//
	// The cache will retain values assigned to it, releasing its reference when purging.
	NCacheValue						   *GetValue(const NCacheKey &theKey) const;
	void								SetValue(const NCacheKey &theKey, NCacheValue *theValue);


	// Get/set the cache name
	NString								GetName(void) const;
	void								SetName(const NString &theName);


	// Get/set the maximum cache size
	//
	// If non-zero, the cache will purge items to stay within the limit.
	NIndex								GetMaxSize(void) const;
	void								SetMaxSize(NIndex theValue);


	// Get/set the maximum cache cost
	//
	// If non-zero, the cache will purge items to stay within the limit.
	NIndex								GetMaxCost(void) const;
	void								SetMaxCost(NIndex theValue);


private:
	bool								NeedsPurge(void);

	NCacheItemList						GetItems(void);


private:
	NString								mName;

	NIndex								mMaxSize;
	NIndex								mMaxCost;

	NCacheMap							mCache;
	NIndex								mCacheCost;
};





#endif // NCACHE_HDR


