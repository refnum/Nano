/*	NAME:
		NLog.h

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
#ifndef NLOG_H
#define NLOG_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NDebug.h"
#include "NLogOutput.h"
#include "NSpinLock.h"
#include "NanoConstants.h"

// System
#include <stdarg.h>
#include <stdlib.h>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Misc
inline constexpr size_t kNLogMessageMax                     = 8 * kNKibibyte;
inline constexpr size_t kNLogSourceMax                      = 128;





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
struct NLogMessage
{
	const char* filePath;
	int         lineNum;

	NLogLevel   logLevel;
	const char* logMsg;

	char tagLevel[13];
	char tagDate[37];
	char tagTime[16];
	char tagThread[9];
	char tagSource[kNLogSourceMax];

	char msgBuffer[kNLogMessageMax];
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NLog
{
public:
										NLog()  = default;
									   ~NLog()  = default;

										NLog(     const NLog& otherLog) = delete;
	NLog&                               operator=(const NLog& otherLog) = delete;

										NLog(     NLog&& otherLog) = delete;
	NLog&                               operator=(NLog&& otherLog) = delete;


	// Log a message
	void                                Log(NLogLevel   logLevel,
											const char* filePath,
											int         lineNum,
											const char* logMsg,
											va_list     theArgs);


	// Get the instance
	static NLog*                        Get();


private:
	void                                OutputMessage(const NLogMessage& theMsg);

	void                                FormatMessage(NLogMessage& theMsg,
													  NLogLevel    logLevel,
													  const char*  filePath,
													  int          lineNum,
													  const char*  logMsg,
													  va_list      theArgs) const;

	void                                FormatTagLevel( NLogMessage& theMsg) const;
	void                                FormatTagTime(  NLogMessage& theMsg) const;
	void                                FormatTagThread(NLogMessage& theMsg) const;
	void                                FormatTagSource(NLogMessage& theMsg) const;


private:
	NSpinLock                           mLock;
	NLogOutputConsole                   mOutput;
};



#endif // NLOG_H
