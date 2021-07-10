/*	NAME:
		NUsageLog.h

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
#ifndef NUSAGE_LOG_H
#define NUSAGE_LOG_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NUsageBucket.h"





//=============================================================================
//		Configuration
//-----------------------------------------------------------------------------
// Usage logging
//
// Usage logging is off by default.
#define NN_ENABLE_USAGE_LOGGING                             0





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Record usage
//
// Record usage of a given tag:
//
//	Example:
//
//		MyObject::MyObject(uint32 theSize)
//			: mVector(theSize)
//		{
//			NN_USAGE("MyObject::MyObject", theSize);
//		}
//
// This accumulates a value against the "MyObject::MyObject:" tag,
// the overall stats for which will be displayed at shutdown.
//
// Tag and value are optional. If both are omitted the function
// name will be used as the tag.
//
#if NN_ENABLE_USAGE_LOGGING

	#define _nn_usage(_tag, _value)                         \
		do                                                  \
		{                                                   \
			static thread_local NUsageBucket sBucket(_tag); \
															\
			sBucket.Add(int64_t(_value));                   \
		} while (0)

	#define _nn_usage_0(...)                                _nn_usage(NN_FUNCTION, 0)

	#define _nn_usage_1(_tag)                               _nn_usage(_tag, 0)

	#define _nn_usage_2(_tag, _value)                       _nn_usage(_tag, _value)

	#define _nn_usage_0_to_2(_0, _1, _2, ...)               _2

	#define NN_USAGE(...)                                   \
		NN_EXPAND(_nn_usage_0_to_2(__VA_ARGS__, _nn_usage_2, _nn_usage_1, _nn_usage_0)(__VA_ARGS__))

#else

	#define NN_USAGE(...)                                   \
		do                                                  \
		{                                                   \
		} while (0)

#endif // NN_ENABLE_USAGE_LOGGING





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NUsageLog
{
public:
										NUsageLog()  = default;
									   ~NUsageLog()  = default;

										NUsageLog(const NUsageLog& otherLog) = delete;
	NUsageLog&                          operator=(const NUsageLog& otherLog) = delete;

										NUsageLog(NUsageLog&& otherLog) = delete;
	NUsageLog&                          operator=(NUsageLog&& otherLog) = delete;


	// Add / remove a bucket
	void                                AddBucket(   const NUsageBucket* theBucket);
	void                                RemoveBucket(const NUsageBucket* theBucket);


	// Get the instance
	static NUsageLog*                   Get();


private:
	void                                RecordStats(const std::string& theTag, const NUsageStats& theStats);
	void                                ReportStats();


private:
	NSetUsageBucket                     mBuckets;
	NMapUsageStats                      mStats;
};



#endif // NUSAGE_LOG_H
