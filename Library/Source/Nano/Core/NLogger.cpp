/*	NAME:
		NLogger.cpp

	DESCRIPTION:
		Logger.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NLogger.h"

// System
#include <stdio.h>
#include <string.h>





//=============================================================================
//		NLogger::NLogger : Constructor.
//-----------------------------------------------------------------------------
NLogger::NLogger()
{
}





//=============================================================================
//		NLogger::Log : Log a message.
//-----------------------------------------------------------------------------
void NLogger::Log(NLogLevel   logLevel,
				  const char* fileName,
				  size_t      lineNum,
				  const char* theMsg,
				  va_list     theArgs)
{
	// Log the message
	fprintf(stderr, "%s %s:%d ", GetTokenLevel(logLevel), GetTokenSource(fileName), int(lineNum));
	vfprintf(stderr, theMsg, theArgs);
	fprintf(stderr, "\n");
}





//=============================================================================
//		NLogger::Get : Get the instance.
//-----------------------------------------------------------------------------
NLogger* NLogger::Get()
{
	// Get the instance
	//
	// Allowed to leak to support logging during static destruction.
	static NLogger* sInstance = new NLogger;

	return sInstance;
}





//=============================================================================
//		NLogger::GetTokenLevel : Get a log level token.
//-----------------------------------------------------------------------------
const char* NLogger::GetTokenLevel(NLogLevel logLevel) const
{
	// Get the token
	switch (logLevel)
	{
		case NLogLevel::Info:
			return "INFO ";
			break;
		case NLogLevel::Warning:
			return "WARN ";
			break;
		case NLogLevel::Error:
			return "ERR  ";
			break;
	}
}





//=============================================================================
//		NLogger::GetTokenSource : Get a file source token.
//-----------------------------------------------------------------------------
const char* NLogger::GetTokenSource(const char* fileName) const
{
	// Get the token
	const char* theToken = strrchr(fileName, '/');

	if (theToken != nullptr)
	{
		theToken += 1;
	}

	return theToken;
}
