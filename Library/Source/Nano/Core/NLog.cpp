/*	NAME:
		NLog.cpp

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
#include "NLog.h"

// Nano
#include "NScopedLock.h"
#include "NThread.h"
#include "NTimeUtils.h"

// System
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr const char* kEmojiInfo                     = "\xF0\x9F\x92\xAC";
static constexpr const char* kEmojiWarning                  = "\xF0\x9F\x94\x86";
static constexpr const char* kEmojiError                    = "\xE2\x9B\x94\xEF\xB8\x8F";





//=============================================================================
//		NLog::Log : Log a message.
//-----------------------------------------------------------------------------
void NLog::Log(NLogLevel   logLevel,
			   const char* filePath,
			   int         lineNum,
			   const char* theMsg,
			   va_list     theArgs)
{


	// Log the message
	static thread_local NLogMessage sLogMsg;

	FormatMessage(sLogMsg, logLevel, filePath, lineNum, theMsg, theArgs);
	OutputMessage(sLogMsg);
}





//=============================================================================
//		NLog::Get : Get the instance.
//-----------------------------------------------------------------------------
NLog* NLog::Get()
{


	// Get the instance
	//
	// Allowed to leak to support logging during static destruction.
	static NLog* sInstance = new NLog;

	return sInstance;
}





#pragma mark private
//=============================================================================
//		NLog::OutputMessage : Output a message.
//-----------------------------------------------------------------------------
void NLog::OutputMessage(const NLogMessage& logMsg)
{
	NScopedLock acquireLock(mLock);



	// Output the message
	mOutput.LogMessage(logMsg);
}





//=============================================================================
//		NLog::FormatMessage : Format the message.
//-----------------------------------------------------------------------------
void NLog::FormatMessage(NLogMessage& logMsg,
						 NLogLevel    logLevel,
						 const char*  filePath,
						 int          lineNum,
						 const char*  theMsg,
						 va_list      theArgs) const
{


	// Format the message
	logMsg.filePath = filePath;
	logMsg.lineNum  = lineNum;
	logMsg.logLevel = logLevel;

	FormatLevel(logMsg);
	FormatTime(logMsg);
	FormatThread(logMsg);
	FormatSource(logMsg);

	vsnprintf(logMsg.logMsg, sizeof(logMsg.logMsg), theMsg, theArgs);
}





//=============================================================================
//		NLog::FormatLevel : Format the log level.
//-----------------------------------------------------------------------------
void NLog::FormatLevel(NLogMessage& logMsg) const
{


	// Format the level
	const char* levelEmoji = "??";
	const char* levelLabel = "????";

	switch (logMsg.logLevel)
	{
		case kNLogLevelInfo:
			levelEmoji = kEmojiInfo;
			levelLabel = "INFO";
			break;
		case kNLogLevelWarning:
			levelEmoji = kEmojiWarning;
			levelLabel = "WARN";
			break;
		case kNLogLevelError:
			levelEmoji = kEmojiError;
			levelLabel = "ERR ";
			break;
	}

	snprintf(logMsg.tokenLevel, sizeof(logMsg.tokenLevel), "%s %s", levelEmoji, levelLabel);
}





//=============================================================================
//		NLog::FormatTime : Format the time.
//-----------------------------------------------------------------------------
void NLog::FormatTime(NLogMessage& logMsg) const
{


	// Get the state we need
	NInterval timeNow = NTimeUtils::GetTime();
	uint64_t  timeUS  = uint64_t((timeNow - floor(timeNow)) / kNTimeMicrosecond);



	// Format the time
	snprintf(logMsg.tokenTime, sizeof(logMsg.tokenTime), "%04d-%02d-%02d ", 2000, 1, 1);

	snprintf(&logMsg.tokenTime[11], 10, "%02d:%02d:%02d.", 0, 0, uint32_t(timeNow) % 60);

	snprintf(&logMsg.tokenTime[20], 7, "%06d", int(timeUS));
}





//=============================================================================
//		NLog::FormatThread : Format the thread ID.
//-----------------------------------------------------------------------------
void NLog::FormatThread(NLogMessage& logMsg) const
{


	// Format the thread
	snprintf(logMsg.tokenThread,
			 sizeof(logMsg.tokenThread),
			 "%08" PRIX32,
			 uint32_t(NThread::GetID()));
}





//=============================================================================
//		NLog::FormatSource : Format the source.
//-----------------------------------------------------------------------------
void NLog::FormatSource(NLogMessage& logMsg) const
{


	// Format the source
	const char* fileName = strrchr(logMsg.filePath, '/');

	if (fileName == nullptr)
	{
		fileName = logMsg.filePath;
	}
	else
	{
		fileName += 1;
	}

	snprintf(logMsg.tokenSource, sizeof(logMsg.tokenSource), "%s:%d", fileName, logMsg.lineNum);
}
