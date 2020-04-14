/*	NAME:
		NUsageLog.cpp

	DESCRIPTION:
		Log system.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NUsageLog.h"

// Nano
#include "NDebug.h"
#include "NStdAlgorithm.h"





//=============================================================================
//		NUsageLog::AddBucket : Add a bucket.
//-----------------------------------------------------------------------------
void NUsageLog::AddBucket(const NUsageBucket* theBucket)
{


	// Validate our parameters
	NN_REQUIRE(theBucket != nullptr);
	NN_REQUIRE(!nstd::contains(mBuckets, theBucket));



	// Add the bucket
	mBuckets.insert(theBucket);
}





//=============================================================================
//		NUsageLog::RemoveBucket : Remove a bucket.
//-----------------------------------------------------------------------------
void NUsageLog::RemoveBucket(const NUsageBucket* theBucket)
{


	// Validate our parameters
	NN_REQUIRE(theBucket != nullptr);
	NN_REQUIRE(nstd::contains(mBuckets, theBucket));



	// Remove the bucket
	//
	// Removing the final bucket triggers the stats.
	RecordStats(theBucket->GetTag(), theBucket->GetStats());
	mBuckets.erase(theBucket);

	if (mBuckets.empty())
	{
		ReportStats();
	}
}





//=============================================================================
//		NUsageLog::Get : Get the instance.
//-----------------------------------------------------------------------------
NUsageLog* NUsageLog::Get()
{


	// Get the instance
	//
	// Allowed to leak to support logging during static destruction.
	static NUsageLog* sInstance = new NUsageLog;

	return sInstance;
}





#pragma mark private
//=============================================================================
//		NUsageLog::RecordStats : Record the stats from a bucket.
//-----------------------------------------------------------------------------
void NUsageLog::RecordStats(const std::string& theTag, const NUsageStats& theStats)
{


	// Validate our parameters
	NN_REQUIRE(!theTag.empty());
	NN_UNUSED(theStats);



#if NN_ENABLE_USAGE_LOGGING
	// Record the stats
	auto theIter = mStats.find(theTag);

	if (theIter == mStats.end())
	{
		mStats[theTag] = theStats;
	}
	else
	{
		auto dstStats = theIter->second;

		dstStats.theCount += theStats.theCount;
		dstStats.valueTotal += theStats.valueTotal;
		dstStats.valueMin = std::min(dstStats.valueMin, theStats.valueMin);
		dstStats.valueMax = std::max(dstStats.valueMax, theStats.valueMax);
	}
#endif // NN_ENABLE_USAGE_LOGGING
}





//=============================================================================
//		NUsageLog::ReportStats : Report the stats.
//-----------------------------------------------------------------------------
void NUsageLog::ReportStats()
{
#if NN_ENABLE_USAGE_LOGGING


	// Get the state we need
	auto theKeys = nstd::keys(mStats);
	nstd::sort(theKeys);



	// Report the stats
	NN_LOG_INFO("========================");
	NN_LOG_INFO("||  NUsageLog Report  ||");
	NN_LOG_INFO("========================");

	for (const auto& theKey : theKeys)
	{
		auto& theStats = mStats[theKey];

		NN_LOG_INFO("{}", theKey);
		NN_LOG_INFO("{:=<{}}", "", theKey.size());
		NN_LOG_INFO("Total Usage   : {}", theStats.theCount);

		if (theStats.valueMin != 0 || theStats.valueMax != 0 || theStats.valueTotal != 0)
		{
			NN_LOG_INFO("Total Value   : {}", theStats.valueTotal);
			NN_LOG_INFO("Minimum Value : {}", theStats.valueMin);
			NN_LOG_INFO("Average Value : {}", theStats.valueTotal / int64_t(theStats.theCount));
			NN_LOG_INFO("Maximum Value : {}", theStats.valueMax);
		}
		NN_LOG_INFO("");
	}
#endif // NN_ENABLE_USAGE_LOGGING
}
