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
#include "NThreadID.h"
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
#if NN_TARGET_WINDOWS
static constexpr const char* kEmojiInfo                     = "--";
static constexpr const char* kEmojiWarning                  = "++";
static constexpr const char* kEmojiError                    = "!!";
#else
static constexpr const char* kEmojiInfo                     = "\xF0\x9F\x92\xAC";
static constexpr const char* kEmojiWarning                  = "\xF0\x9F\x94\x86";
static constexpr const char* kEmojiError                    = "\xE2\x9B\x94\xEF\xB8\x8F";
#endif // NN_TARGET_WINDOWS





//=============================================================================
//		NLog::Log : Log a message.
//-----------------------------------------------------------------------------
void NLog::Log(NLogLevel   logLevel,
			   const char* filePath,
			   int         lineNum,
			   const char* logMsg,
			   va_list     theArgs)
{


	// Log the message
	static thread_local NLogMessage sLogMsg;

	FormatMessage(sLogMsg, logLevel, filePath, lineNum, logMsg, theArgs);
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
void NLog::OutputMessage(const NLogMessage& theMsg)
{
	NScopedLock acquireLock(mLock);



	// Output the message
	mOutput.LogMessage(theMsg);
}





//=============================================================================
//		NLog::FormatMessage : Format the message.
//-----------------------------------------------------------------------------
void NLog::FormatMessage(NLogMessage& theMsg,
						 NLogLevel    logLevel,
						 const char*  filePath,
						 int          lineNum,
						 const char*  logMsg,
						 va_list      theArgs) const
{


	// Format the message
	theMsg.filePath = filePath;
	theMsg.lineNum  = lineNum;
	theMsg.logLevel = logLevel;
	theMsg.logMsg   = logMsg;

	FormatTagLevel(theMsg);
	FormatTagTime(theMsg);
	FormatTagThread(theMsg);
	FormatTagSource(theMsg);

	vsnprintf(theMsg.msgBuffer, sizeof(theMsg.msgBuffer), logMsg, theArgs);
}





//=============================================================================
//		NLog::FormatTagLevel : Format the level tag.
//-----------------------------------------------------------------------------
void NLog::FormatTagLevel(NLogMessage& theMsg) const
{


	// Format the level
	const char* levelEmoji = "??";
	const char* levelLabel = "????";

	switch (theMsg.logLevel)
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

	snprintf(theMsg.tagLevel, sizeof(theMsg.tagLevel), "%s %s", levelEmoji, levelLabel);
}





//=============================================================================
//		NLog::FormatTagTime : Format the date and time tags.
//-----------------------------------------------------------------------------
void NLog::FormatTagTime(NLogMessage& theMsg) const
{


	// Get the state we need
	NInterval timeNow   = NTimeUtils::GetTime();
	struct tm timeLocal = NTimeUtils::ToTmLocal(timeNow);
	uint64_t  timeUS    = uint64_t((timeNow - floor(timeNow)) / kNTimeMicrosecond);



	// Format the time
	snprintf(theMsg.tagDate,
			 sizeof(theMsg.tagDate),
			 "%04d-%02d-%02d ",
			 timeLocal.tm_year + 1900,
			 timeLocal.tm_mon + 1,
			 timeLocal.tm_mday);

	snprintf(&theMsg.tagTime[0],
			 10,
			 "%02d:%02d:%02d.",
			 timeLocal.tm_hour,
			 timeLocal.tm_min,
			 timeLocal.tm_sec);

	snprintf(&theMsg.tagTime[9], 7, "%06d", int(timeUS));
}





//=============================================================================
//		NLog::FormatTagThread : Format the thread ID tag.
//-----------------------------------------------------------------------------
void NLog::FormatTagThread(NLogMessage& theMsg) const
{


	// Format the thread
	snprintf(theMsg.tagThread,
			 sizeof(theMsg.tagThread),
			 "%08" PRIX32,
			 uint32_t(NThreadID::Get().GetValue()));
}





//=============================================================================
//		NLog::FormatTagSource : Format the source.
//-----------------------------------------------------------------------------
void NLog::FormatTagSource(NLogMessage& theMsg) const
{


	// Locate the file name
	const char* fileName = strrchr(theMsg.filePath, '/');

	if (fileName == nullptr)
	{
		fileName = strrchr(theMsg.filePath, '\\');
	}

	if (fileName == nullptr)
	{
		fileName = theMsg.filePath;
	}
	else
	{
		fileName += 1;
	}



	// Format the source
	snprintf(theMsg.tagSource, sizeof(theMsg.tagSource), "%s:%d", fileName, theMsg.lineNum);
}
