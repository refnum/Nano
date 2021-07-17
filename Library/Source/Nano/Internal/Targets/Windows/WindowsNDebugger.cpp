/*	NAME:
		WindowsNDebugger.cpp

	DESCRIPTION:
		Windows debugger support.

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
#include "NDebugger.h"

// Nano
#include "NCommonWindows.h"

// System
#include <dbghelp.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr size_t kNDebuggerMaxFrames                 = 512;





//=============================================================================
//		NDebugger::IsActive : Is a debugger active?
//-----------------------------------------------------------------------------
bool NDebugger::IsActive()
{


	// Check the state
	return IsDebuggerPresent();
}





//=============================================================================
//		NDebugger::GetBacktrace : Get a backtrace.
//-----------------------------------------------------------------------------
NVectorString NDebugger::GetBacktrace(size_t skipFrames, size_t numFrames)
{


	// Get the state we need
	//
	// We increase skipFrames to skip ourselves.
	skipFrames++;

	if (numFrames != kNSizeMax)
	{
		numFrames += skipFrames;
	}

	numFrames = std::min(numFrames, kNDebuggerMaxFrames);



	// Get the frames
	void* theFrames[kNDebuggerMaxFrames];

	HANDLE hProcess = GetCurrentProcess();
	BOOL   wasOK    = SymInitialize(hProcess, NULL, TRUE);
	NN_EXPECT(wasOK);

	if (wasOK)
	{
		numFrames = CaptureStackBackTrace(DWORD(skipFrames), DWORD(numFrames), theFrames, nullptr);
	}
	else
	{
		numFrames = 0;
	}



	// Process the symbols
	NVectorString theTrace;

	char         symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
	PSYMBOL_INFO symbolInfo   = (PSYMBOL_INFO) symbolBuffer;
	DWORD64      symbolOffset = 0;

	symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbolInfo->MaxNameLen   = MAX_SYM_NAME;

	for (size_t n = 0; n < numFrames; n++)
	{
		NString theName;

		if (SymFromAddr(hProcess, DWORD64(theFrames[n]), &symbolOffset, symbolInfo))
		{
			theName = symbolInfo->Name;
		}

		theTrace.emplace_back(theName);
	}



	// Clean up
	if (wasOK)
	{
		wasOK = SymCleanup(hProcess);
		NN_EXPECT(wasOK);
	}

	return theTrace;
}
