/*	NAME:
		NCache.h

	DESCRIPTION:
		Cache object.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
#ifndef NCACHE_H
#define NCACHE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinContainer.h"
#include "NTime.h"

// System
#include <memory>
#include <unordered_map>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Cache entry
template<typename V>
struct NCacheEntry
{
	std::shared_ptr<V> theValue;
	NTime              lastAccess;
	size_t             theCost;
};


// Containers
template<typename K, typename V>
using NMapCacheEntries = std::unordered_map<K, NCacheEntry<V>>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename K, typename V>
class NN_EMPTY_BASE NCache : public NMixinContainer<NCache<K, V>>
{
public:
										NCache();


	// Clear the cache
	void                                Clear();


	// Get the total cost
	size_t                              GetCost() const;


	// Get the total size
	size_t                              GetSize() const;


	// Is a key present?
	bool                                HasKey(const K& theKey) const;


	// Remove a key
	//
	// The key must exist in the cache.
	void                                RemoveKey(const K& theKey);


	// Get/set a value
	//
	// GetValue will return a null pointer if the key does not exist.
	//
	// SetValue will replace any existing value for this key in the cache.
	//
	// If adding the new value exceeds a cache threshold then items will
	// be purged to meet that threshold, potentially including the newly
	// added item.
	std::shared_ptr<V>                  GetValue(const K& theKey) const;
	void                                SetValue(const K& theKey, const std::shared_ptr<V>& theValue, size_t theCost = 0);
	void                                SetValue(const K& theKey, const V& theValue, size_t theCost = 0);


	// Get/set the maximum cost
	//
	// A non-zero limit will purge the most expensive items by cost to
	// stay within the limit.
	//
	// The default limit is zero.
	size_t                              GetMaxCost() const;
	void                                SetMaxCost(size_t maxCost);


	// Get/set the maximum size
	//
	// A non-zero limit will purge the least recently used items to
	// stay within the limit.
	//
	// The default limit is zero.
	size_t                              GetMaxSize() const;
	void                                SetMaxSize(size_t maxSize);


private:
	void                                Purge();
	void                                PurgeCost();
	void                                PurgeSize();


private:
	size_t                              mMaxCost;
	size_t                              mMaxSize;
	size_t                              mCost;
	mutable NMapCacheEntries<K, V>      mCache;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCache.inl"



#endif // NCACHE_H
