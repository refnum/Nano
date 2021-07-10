/*	NAME:
		NUsageBucket.h

	DESCRIPTION:
		Usage bucket.

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
#ifndef NUSAGE_BUCKET_H
#define NUSAGE_BUCKET_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoTypes.h"


// System
#include <set>
#include <string>
#include <unordered_map>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NUsageBucket;


// Stats
struct NUsageStats
{
	size_t  theCount   = 0;
	int64_t valueTotal = 0;
	int64_t valueMin   = 0;
	int64_t valueMax   = 0;
};


// Containers
using NMapUsageStats  = std::unordered_map<std::string, NUsageStats>;
using NSetUsageBucket = std::set<const NUsageBucket*>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NUsageBucket
{
public:
										NUsageBucket(const std::string& theTag);
									   ~NUsageBucket();

										NUsageBucket(const NUsageBucket& otherBucket) = delete;
	NUsageBucket&                       operator=(   const NUsageBucket& otherBucket) = delete;

										NUsageBucket(NUsageBucket&& otherBucket) = delete;
	NUsageBucket&                       operator=(   NUsageBucket&& otherBucket) = delete;


	// Add an entry
	void                                Add(int64_t theValue);


	// Get the tag
	std::string                         GetTag() const;


	// Get the stats
	NUsageStats                         GetStats() const;

private:
	std::string                         mTag;
	NUsageStats                         mStats;
};



#endif // NUSAGE_BUCKET_H
