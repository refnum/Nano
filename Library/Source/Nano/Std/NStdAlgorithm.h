/*	NAME:
		NStdAlgorithm.h

	DESCRIPTION:
		std::algorithm extensions.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#ifndef NSTD_ALGORITHM_H
#define NSTD_ALGORITHM_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NDebug.h"

// System
#include <algorithm>
#include <set>
#include <unordered_set>
#include <vector>

#if NN_COMPILER_CLANG
	#include <experimental/functional>
#else
	#include <functional>
#endif

namespace nstd
{





//=============================================================================
//		Template Declarations
//-----------------------------------------------------------------------------
template<typename T>
void push_back(T& container1, const T& container2);

template<typename T>
void push_back(T& container1, T&& container2);

template<typename T, typename P>
bool erase_if(T& container, const P& predicate);

template<typename H, typename N>
H search(H hayBegin, H hayEnd, N needleBegin, N needleEnd);





//=============================================================================
//		nstd::appended : Return the result of appending two containers.
//-----------------------------------------------------------------------------
template<typename T1, typename T2>
T1 appended(T1 container1, T2&& container2)
{
	nstd::push_back(container1, std::forward<T2>(container2));
	return container1;
}





//=============================================================================
//		nstd::contains : Does an associative container contain a key?
//-----------------------------------------------------------------------------
template<typename T>
bool contains(const T& container, const typename T::key_type& key)
{
	return container.find(key) != container.end();
}





//=============================================================================
//		nstd::contains : Does an std::set contain a key?
//-----------------------------------------------------------------------------
template<typename T>
bool contains(const std::set<T>& container, const T& key)
{
	return container.find(key) != container.end();
}





//=============================================================================
//		nstd::contains : Does an std::unordered_set contain a key?
//-----------------------------------------------------------------------------
template<typename T>
bool contains(const std::unordered_set<T>& container, const T& key)
{
	return container.find(key) != container.end();
}





//=============================================================================
//		nstd::contains : Does a container contain a value?
//-----------------------------------------------------------------------------
template<typename T>
bool contains(const T& container, const typename T::value_type& value)
{
	auto iterBegin = std::begin(container);
	auto iterEnd   = std::end(container);

	return std::find(iterBegin, iterEnd, value) != iterEnd;
}





//=============================================================================
//		nstd::contains : Does a container contain a value?
//-----------------------------------------------------------------------------
template<class T, size_t N>
bool contains(const T (&container)[N], const T& value)
{
	auto iterBegin = std::begin(container);
	auto iterEnd   = std::end(container);

	return std::find(iterBegin, iterEnd, value) != iterEnd;
}





//=============================================================================
//		nstd::equal : Are two containers equal?
//-----------------------------------------------------------------------------
template<typename T>
bool equal(const T& container1, const T& container2)
{
	return std::equal(std::begin(container1),
					  std::end(container1),
					  std::begin(container2),
					  std::end(container2));
}





//=============================================================================
//		nstd::erase : Erase a value from a container.
//-----------------------------------------------------------------------------
template<typename T>
bool erase(T& container, const typename T::value_type& value)
{
	return erase_if(container, [&](const typename T::value_type& element) {
		return element == value;
	});
}





//=============================================================================
//		nstd::erase_if : Erase a value from a container with a predicate.
//-----------------------------------------------------------------------------
template<typename T, typename P>
bool erase_if(T& container, const P& predicate)
{
	auto iterBegin = std::begin(container);
	auto iterEnd   = std::end(container);

	auto iterErase = std::remove_if(iterBegin, iterEnd, predicate);
	bool didErase  = (iterErase != iterEnd);

	if (didErase)
	{
		container.erase(iterErase, iterEnd);
	}

	return didErase;
}





//=============================================================================
//		nstd::extract_back : Extract the last element from a container.
//-----------------------------------------------------------------------------
template<class T>
typename T::value_type extract_back(T& container)
{
	NN_REQUIRE(!container.empty());

	auto iterBack = container.back();
	auto theValue = std::move(iterBack);

	container.pop_back();

	return theValue;
}





//=============================================================================
//		nstd::extract_front : Extract the first element from a container.
//-----------------------------------------------------------------------------
template<class T>
typename T::value_type extract_front(T& container)
{
	NN_REQUIRE(!container.empty());

	auto iterBegin = container.begin();
	auto theValue  = std::move(*iterBegin);

	container.erase(iterBegin);

	return theValue;
}





//=============================================================================
//		nstd::fetch : Fetch a value from an associative container.
//-----------------------------------------------------------------------------
template<class T>
typename T::mapped_type fetch(const T&                       container,
							  const typename T::key_type&    key,
							  const typename T::mapped_type& defaultValue)
{
	auto iterFind = container.find(key);

	if (iterFind != container.end())
	{
		return iterFind->second;
	}
	else
	{
		return defaultValue;
	}
}





//=============================================================================
//		nstd::front : Get a pointer to the first element in a container.
//-----------------------------------------------------------------------------
template<class T>
typename T::value_type* front(T& container)
{
	if (container.empty())
	{
		return nullptr;
	}
	else
	{
		return &container[0];
	}
}





//=============================================================================
//		nstd::keys : Get the keys from an associative container.
//-----------------------------------------------------------------------------
template<class T>
std::vector<typename T::key_type> keys(const T& container)
{
	std::vector<typename T::key_type> result;
	result.reserve(container.size());

	for (const auto& iter : container)
	{
		result.emplace_back(iter.first);
	}

	return result;
}





//=============================================================================
//		nstd::move_back : Move a container's contents to the end of a container.
//-----------------------------------------------------------------------------
template<typename T>
void move_back(T& container1, T& container2)
{
	container1.reserve(container1.size() + container2.size());
	std::move(container2.begin(), container2.end(), std::back_inserter(container1));

	container2.clear();
}





//=============================================================================
//		nstd::pop_front : Remove the first element from a container.
//-----------------------------------------------------------------------------
template<typename T>
void pop_front(T& container)
{
	NN_REQUIRE(!container.empty());

	container.erase(container.begin());
}





//=============================================================================
//		nstd::push_back : Insert a container at the end of a container.
//-----------------------------------------------------------------------------
template<typename T>
void push_back(T& container1, const T& container2)
{
	container1.insert(container1.end(), container2.begin(), container2.end());
}





//=============================================================================
//		nstd::push_back : Insert a container at the end of a container.
//-----------------------------------------------------------------------------
template<typename T>
void push_back(T& container1, T&& container2)
{
	nstd::move_back(container1, container2);
}





//=============================================================================
//		nstd::push_front : Insert an element at the start of a container.
//-----------------------------------------------------------------------------
template<typename T>
void push_front(T& container, const typename T::value_type& value)
{
	container.insert(container.begin(), value);
}





//=============================================================================
//		nstd::push_front : Insert a container at the start of a container.
//-----------------------------------------------------------------------------
template<typename T>
void push_front(T& container1, const T& container2)
{
	container1.insert(container1.begin(), container2.begin(), container2.end());
}





//=============================================================================
//		nstd::push_front : Insert an element at the start of a container.
//-----------------------------------------------------------------------------
template<typename T>
void push_front(T& container, typename T::value_type&& value)
{
	container.insert(container.begin(), std::move(value));
}





//=============================================================================
//		nstd::reverse : Reverse the values within a container.
//-----------------------------------------------------------------------------
template<typename T>
void reverse(T& container)
{
	std::reverse(std::begin(container), std::end(container));
}





//=============================================================================
//		nstd::reversed : Return a reversed container.
//-----------------------------------------------------------------------------
template<typename T>
T reversed(T container)
{
	nstd::reverse(container);
	return container;
}





//=============================================================================
//		nstd::search : Search a container for a container.
//-----------------------------------------------------------------------------
template<typename T>
typename T::const_iterator search(const T& haystack, const T& needle)
{
	return nstd::search(haystack.begin(), haystack.end(), needle.begin(), needle.end());
}





//=============================================================================
//		nstd::search : Search a container for a container.
//-----------------------------------------------------------------------------
template<typename H, typename N>
H search(H hayBegin, H hayEnd, N needleBegin, N needleEnd)
{
#if NN_COMPILER_CLANG

	NN_DIAGNOSTIC_PUSH();
	NN_DIAGNOSTIC_IGNORE_CLANG("-Wunknown-warning-option");
	NN_DIAGNOSTIC_IGNORE_CLANG("-Wctad-maybe-unsupported");

	return std::search(hayBegin,
					   hayEnd,
					   std::experimental::boyer_moore_horspool_searcher(needleBegin, needleEnd));

	NN_DIAGNOSTIC_POP();

#else
	return std::search(hayBegin,
					   hayEnd,
					   std::boyer_moore_horspool_searcher(needleBegin, needleEnd));
#endif
}





//=============================================================================
//		nstd::sort : Sort the values within a container.
//-----------------------------------------------------------------------------
template<typename T>
void sort(T& container)
{
	std::sort(std::begin(container), std::end(container));
}





//=============================================================================
//		nstd::sort : Sort the values within a container with a comparator.
//-----------------------------------------------------------------------------
template<typename T, typename C>
void sort(T& container, const C& comparator)
{
	std::sort(std::begin(container), std::end(container), comparator);
}





//=============================================================================
//		nstd::stable_uniquify : Order-preserving uniquify the values in a container.
//-----------------------------------------------------------------------------
template<class T>
void stable_uniquify(T& container)
{
	typedef typename T::value_type V;


	// Set
	//
	// We use a custom comparator to dereference during comparisons.
	struct UniquifyCompare
	{
		bool operator()(const V* a, const V* b) const
		{
			return *a < *b;
		}
	};

	std::set<const V*, UniquifyCompare> theSet;


	// Result
	//
	// We reserve the maximum required space to ensure that pointers
	// into the result remain valid during processing.
	T theResult;

	theResult.reserve(container.size());


	// Uniquify the container
	for (const V& theValue : container)
	{
		if (theSet.find(&theValue) == theSet.end())
		{
			theResult.emplace_back(std::move(theValue));
			theSet.insert(&theResult.back());
		}
	}

	theResult.shrink_to_fit();
	std::swap(container, theResult);
}





//=============================================================================
//		nstd::uniquify : Uniquify the values in a container.
//-----------------------------------------------------------------------------
template<class T>
void uniquify(T& container)
{
	auto iterBegin = container.begin();
	auto iterEnd   = container.end();

	std::sort(iterBegin, iterEnd);
	container.erase(std::unique(iterBegin, iterEnd), iterEnd);
}





//=============================================================================
//		nstd::values : Get the values from an associative container.
//-----------------------------------------------------------------------------
template<class T>
std::vector<typename T::mapped_type> values(const T& container)
{
	std::vector<typename T::mapped_type> result;
	result.reserve(container.size());

	for (const auto& iter : container)
	{
		result.emplace_back(iter.second);
	}

	return result;
}



}    // namespace nstd



#endif // NSTD_ALGORITHM_H
