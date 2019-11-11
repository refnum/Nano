/*	NAME:
		NLogger.h

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
#ifndef NLOGGER_H
#define NLOGGER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// System
#include <stdarg.h>
#include <stdlib.h>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
enum class NLogLevel
{
	Info,
	Warning,
	Error
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NLogger
{
public:
										NLogger();
									   ~NLogger() = default;

										NLogger(  const NLogger&) = delete;
	NLogger&                            operator=(const NLogger&) = delete;

										NLogger(  NLogger&&) = delete;
	NLogger&                            operator=(NLogger&&) = delete;


	// Log a message
	void                                Log(NLogLevel   logLevel,
											const char* fileName,
											size_t      lineNum,
											const char* theMsg,
											va_list     theArgs);


	// Get the instance
	static NLogger*                     Get();


private:
	const char*                         GetTokenLevel(NLogLevel logLevel)    const;
	const char*                         GetTokenSource(const char* fileName) const;
};


#endif // NLOGGER_H
