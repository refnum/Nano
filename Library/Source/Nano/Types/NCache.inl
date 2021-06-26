/*	NAME:
		NCache.inl

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NStdAlgorithm.h"
#include "NTimeUtils.h"





//=============================================================================
//		NCache::NCache : Constructor.
//-----------------------------------------------------------------------------
template<typename K, typename V>
NCache<K, V>::NCache()
	: mMaxCost(0)
	, mMaxSize(0)
	, mCost(0)
	, mCache{}
{
}





//=============================================================================
//		NCache::Clear : Clear the cache.
//-----------------------------------------------------------------------------
template<typename K, typename V>
void NCache<K, V>::Clear()
{


	// Clear the cache
	mCost = 0;
	mCache.clear();
}





//=============================================================================
//		NCache::GetCost : Get the cost.
//-----------------------------------------------------------------------------
template<typename K, typename V>
size_t NCache<K, V>::GetCost() const
{


	// Get the cost
	return mCost;
}





//=============================================================================
//		NCache::GetSize : Get the size.
//-----------------------------------------------------------------------------
template<typename K, typename V>
size_t NCache<K, V>::GetSize() const
{


	// Get the size
	return mCache.size();
}





//=============================================================================
//		NCache::HasKey : Does a key exist?
//-----------------------------------------------------------------------------
template<typename K, typename V>
bool NCache<K, V>::HasKey(const K& theKey) const
{


	// Check the cache
	return nstd::contains(mCache, theKey);
}





//=============================================================================
//		NCache::RemoveKey : Remove a key.
//-----------------------------------------------------------------------------
template<typename K, typename V>
void NCache<K, V>::RemoveKey(const K& theKey)
{


	// Validate our parameters
	NN_REQUIRE(HasKey(theKey));



	// Remove the key
	auto theIter = mCache.find(theKey);
	NN_REQUIRE(theIter != mCache.end());

	NN_REQUIRE(mCost >= theIter->second.theCost);
	mCost -= theIter->second.theCost;

	mCache.erase(theIter);
}





//=============================================================================
//		NCache::GetValue : Get a value.
//-----------------------------------------------------------------------------
template<typename K, typename V>
std::shared_ptr<V> NCache<K, V>::GetValue(const K& theKey) const
{


	// Get the value
	auto theIter = mCache.find(theKey);
	if (theIter != mCache.end())
	{
		theIter->second.lastAccess = NTimeUtils::GetTime();
		return theIter->second.theValue;
	}

	return {};
}





//=============================================================================
//		NCache::SetValue : Set a value.
//-----------------------------------------------------------------------------
template<typename K, typename V>
void NCache<K, V>::SetValue(const K& theKey, std::shared_ptr<V> theValue, size_t theCost)
{


	// Remove any existing entry
	if (HasKey(theKey))
	{
		RemoveKey(theKey);
	}



	// Add the entry
	mCache.insert({theKey, {theValue, NTimeUtils::GetTime(), theCost}});
	mCost += theCost;

	Purge();
}





//=============================================================================
//		NCache::SetValue : Set a value.
//-----------------------------------------------------------------------------
template<typename K, typename V>
void NCache<K, V>::SetValue(const K& theKey, const V& theValue, size_t theCost)
{


	// Set the value
	SetValue(theKey, std::make_shared<V>(theValue), theCost);
}





//=============================================================================
//		NCache::GetMaxCost : Get the maximum cost.
//-----------------------------------------------------------------------------
template<typename K, typename V>
size_t NCache<K, V>::GetMaxCost() const
{


	// Get the value
	return mMaxCost;
}





//=============================================================================
//		NCache::SetMaxCost : Set the maximum cost.
//-----------------------------------------------------------------------------
template<typename K, typename V>
void NCache<K, V>::SetMaxCost(size_t maxCost)
{


	// Set the value
	mMaxCost = maxCost;

	Purge();
}





//=============================================================================
//		NCache::GetMaxSize : Get the maximum size.
//-----------------------------------------------------------------------------
template<typename K, typename V>
size_t NCache<K, V>::GetMaxSize() const
{


	// Get the value
	return mMaxSize;
}





//=============================================================================
//		NCache::SetMaxSize : Set the maximum size.
//-----------------------------------------------------------------------------
template<typename K, typename V>
void NCache<K, V>::SetMaxSize(size_t maxSize)
{


	// Set the value
	mMaxSize = maxSize;

	Purge();
}





#pragma mark private
//=============================================================================
//		NCache::Purge : Purge the cache.
//-----------------------------------------------------------------------------
template<typename K, typename V>
void NCache<K, V>::Purge()
{


	// Purge the cache
	//
	// We purge by cost then size to allow a cache to have both a cost
	// nad  asize limit where "expensive" items are more discardable
	// than the quantity of items.
	if (mMaxCost != 0 && mCost > mMaxCost)
	{
		PurgeCost();
	}

	if (mMaxSize != 0 && mCache.size() > mMaxSize)
	{
		PurgeSize();
	}
}





//=============================================================================
//		NCache::PurgeSize : Purge the cache by size.
//-----------------------------------------------------------------------------
template<typename K, typename V>
void NCache<K, V>::PurgeSize()
{


	// Validate our state
	NN_REQUIRE(mMaxSize != 0 && mCache.size() > mMaxSize);



	// Get the state we need
	struct NCacheKeyTime
	{
		K     theKey;
		NTime lastAccess;
	};

	std::vector<NCacheKeyTime> keyTimes;

	for (const auto& keyValue : mCache)
	{
		keyTimes.push_back({keyValue.first, keyValue.second.lastAccess});
	}



	// Purge the cache
	//
	// Sort by least recently used,
	nstd::sort(keyTimes,
			   [](const NCacheKeyTime& valueA, const NCacheKeyTime& valueB)
			   {
				   return valueA.lastAccess < valueB.lastAccess;
			   });

	for (const auto& keyTime : keyTimes)
	{
		RemoveKey(keyTime.theKey);
		if (mCache.size() <= mMaxSize)
		{
			break;
		}
	}
}





//=============================================================================
//		NCache::PurgeCost : Purge the cache by cost.
//-----------------------------------------------------------------------------
template<typename K, typename V>
void NCache<K, V>::PurgeCost()
{


	// Validate our state
	NN_REQUIRE(mMaxCost != 0 && mCost > mMaxCost);



	// Get the state we need
	struct NCacheKeyCost
	{
		K      theKey;
		size_t theCost;
	};

	std::vector<NCacheKeyCost> keyCosts;

	for (const auto& keyValue : mCache)
	{
		keyCosts.push_back({keyValue.first, keyValue.second.theCost});
	}



	// Purge the cache
	//
	// Sort by largest cost.
	nstd::sort(keyCosts,
			   [](const NCacheKeyCost& valueA, const NCacheKeyCost& valueB)
			   {
				   return valueA.theCost > valueB.theCost;
			   });

	for (const auto& keyCost : keyCosts)
	{
		RemoveKey(keyCost.theKey);
		if (mCost <= mMaxCost)
		{
			break;
		}
	}
}
