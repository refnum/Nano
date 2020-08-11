/*	NAME:
		NDebug.cpp

	DESCRIPTION:
		Debug logging and assrtions.

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
#include "NDebug.h"


// Nano
#include "NLog.h"





//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
//		NanoLogFormatArgs : Log a message with std::format formatting.
//-----------------------------------------------------------------------------
void NanoLogFormatArgs(NLogLevel                           logLevel,
					   const char*                         filePath,
					   int                                 lineNum,
					   const fmt::basic_string_view<char>& formatStr,
					   fmt::format_args                    theArgs)
{


	// Log the message
	fmt::memory_buffer theBuffer;
	fmt::detail::vformat_to(theBuffer, formatStr, theArgs);

	NanoLogPrintf(logLevel, filePath, lineNum, "%.*s", int(theBuffer.size()), theBuffer.data());
}





//=============================================================================
//		NanoLogPrintf : Log a message with printf-style formatting.
//-----------------------------------------------------------------------------
void NanoLogPrintf(NLogLevel logLevel, const char* filePath, int lineNum, const char* theMsg, ...)
{


	// Log the message
	va_list argList;

	va_start(argList, theMsg);
	NLog::Get()->Log(logLevel, filePath, lineNum, theMsg, argList);
	va_end(argList);
}
