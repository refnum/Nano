/*	NAME:
		NLog.h

	DESCRIPTION:
		Log system.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
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
#include "NLogOutput.h"
#include "NanoMacros.h"

// System
#include <stdarg.h>
#include <stdlib.h>





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Logging
//
// Logging is enabled in debug builds.
//
#define NN_LOGGING                                          NN_DEBUG


// Log a message
//
// Example:
//
//		NN_LOG_INFO("This is just information.");
//
//		NN_LOG_WARNING("This is a warning!");
//
//		NN_LOG_ERROR("Something has gone wrong!");
//
#if NN_LOGGING

	#define NN_LOG_INFO(...)                                            \
		do                                                              \
		{                                                               \
			NanoLog(NLogLevel::Info, __FILE__, __LINE__, __VA_ARGS);    \
		} while (false)

	#define NN_LOG_WARNING(...)                                         \
		do                                                              \
		{                                                               \
			NanoLog(NLogLevel::Warning, __FILE__, __LINE__, __VA_ARGS); \
		} while (false)

	#define NN_LOG_ERROR(...)                                           \
		do                                                              \
		{                                                               \
			NanoLog(NLogLevel::Error, __FILE__, __LINE__, __VA_ARGS);   \
		} while (false)

#else

	#define NN_LOG_INFO(...)
	#define NN_LOG_WARNING(...)
	#define NN_LOG_ERROR(...)

#endif





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Log levels
//
// The log level indicates the priority of the message.
enum class NLogLevel
{
	Info,
	Warning,
	Error
};



// Misc
static constexpr size_t kNLogMessageMax                     = 8 * 1024;
static constexpr size_t kNLogTokenMax                       = 128;





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
struct NLogMessage
{
	const char* filePath;
	size_t      lineNum;

	NLogLevel logLevel;
	char      logMsg[kNLogMessageMax];

	char tokenLevel[kNLogTokenMax];
	char tokenTime[kNLogTokenMax];
	char tokenThread[kNLogTokenMax];
	char tokenSource[kNLogTokenMax];
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NLog
{
public:
										NLog()  = default;
									   ~NLog()  = default;

										NLog(     const NLog&) = delete;
	NLog&                               operator=(const NLog&) = delete;

										NLog(     NLog&&)  = delete;
	NLog&                               operator=(NLog&&)  = delete;


	// Log a message
	void                                Log(NLogLevel   logLevel,
											const char* filePath,
											size_t      lineNum,
											const char* theMsg,
											va_list     theArgs);


	// Get the instance
	static NLog*                        Get();


private:
	void                                OutputMessage(const NLogMessage& logMsg);

	void                                FormatMessage(NLogMessage& logMsg,
													  NLogLevel    logLevel,
													  const char*  filePath,
													  size_t       lineNum,
													  const char*  theMsg,
													  va_list      theArgs) const;

	void                                FormatLevel( NLogMessage& logMsg) const;
	void                                FormatTime(  NLogMessage& logMsg) const;
	void                                FormatThread(NLogMessage& logMsg) const;
	void                                FormatSource(NLogMessage& logMsg) const;


private:
	NLogOutputConsole                   mOutput;
};





//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
NN_VALIDATE_PRINTF(4, 5)
void NanoLog(NLogLevel logLevel, const char* filePath, size_t lineNum, const char* theMsg, ...);



#endif // NLOG_H
