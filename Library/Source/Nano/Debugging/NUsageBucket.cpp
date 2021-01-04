/*	NAME:
		NUsageBucket.cpp

	DESCRIPTION:
		Log system.

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
#include "NUsageBucket.h"

// Nano
#include "NDebug.h"
#include "NUsageLog.h"

// System
#include <algorithm>





//=============================================================================
//		NUsageBucket::NUsageBucket : Constructor.
//-----------------------------------------------------------------------------
NUsageBucket::NUsageBucket(const std::string& theTag)
	: mTag(theTag)
	, mStats{}
{


	// Validate our parameters
	NN_REQUIRE(!theTag.empty());


	// Initialise ourselves
	NUsageLog::Get()->AddBucket(this);
}





//=============================================================================
//		NUsageBucket::~NUsageBucket : Destructor.
//-----------------------------------------------------------------------------
NUsageBucket::~NUsageBucket()
{


	// Clean up
	NUsageLog::Get()->RemoveBucket(this);
}





//=============================================================================
//		NUsageBucket::Add : Add an entry.
//-----------------------------------------------------------------------------
void NUsageBucket::Add(int64_t theValue)
{


	// Add the entry
	if (mStats.theCount == 0)
	{
		mStats.valueMin = theValue;
		mStats.valueMax = theValue;
	}

	mStats.theCount++;
	mStats.valueTotal += theValue;
	mStats.valueMin = std::min(mStats.valueMin, theValue);
	mStats.valueMax = std::max(mStats.valueMax, theValue);
}





//=============================================================================
//		NUsageBucket::GetTag : Get the tag.
//-----------------------------------------------------------------------------
std::string NUsageBucket::GetTag() const
{


	// Get the tag
	return mTag;
}





//=============================================================================
//		NUsageBucket::GetStats : Get the stats.
//-----------------------------------------------------------------------------
NUsageStats NUsageBucket::GetStats() const
{


	// Get the stats
	return mStats;
}
