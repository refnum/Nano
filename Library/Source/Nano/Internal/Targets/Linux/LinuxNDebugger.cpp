/*	NAME:
		LinuxNDebugger.cpp

	DESCRIPTION:
		Linux debugger support.

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
#include "NCommonLinux.h"

// System
#include <cxxabi.h>
#include <execinfo.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
inline const char*      kNDebuggerSymbolName                = "\\((\\w*)\\+";
inline constexpr size_t kNDebuggerMaxFrames                 = 512;





//=============================================================================
//		NDebugger::IsActive : Is a debugger active?
//-----------------------------------------------------------------------------
bool NDebugger::IsActive()
{


	// Check the state
	return NCommonLinux::DebuggerIsActive();
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
	numFrames = size_t(backtrace(theFrames, int(numFrames)));

	char** theSymbols = backtrace_symbols(theFrames, int(numFrames));
	if (theSymbols == nullptr)
	{
		numFrames = 0;
	}



	// Process the symbols
	//
	// Example symbols:
	//
	//		"NanoTest/Project/NanoTest(_ZN9NDebugger12GetBacktraceEmm+0xa7) [0x55694f2a90a5]"
	//		"NanoTest/Project/NanoTest(+0x2224fb) [0x55694f0654fb]"
	//		"NanoTest/Project/NanoTest(+0x22249f) [0x55694f06549f]"
	//		"NanoTest/Project/NanoTest(+0x223820) [0x55694f066820]"
	//		"NanoTest/Project/NanoTest(_ZNK5Catch8TestCase6invokeEv+0x2d) [0x55694f1bbc9b]"
	//		"NanoTest/Project/NanoTest(_ZN5Catch10RunContext20invokeActiveTestCaseEv+0x3c) [0x55694f1b60a8]"
	//		"NanoTest/Project/NanoTest(_ZN5Catch10RunContext14runCurrentTestERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_+0x273) [0x55694f1b5ddf]"
	//		"NanoTest/Project/NanoTest(_ZN5Catch10RunContext7runTestERKNS_8TestCaseE+0x244) [0x55694f1b4810]"
	//		"NanoTest/Project/NanoTest(+0x374a26) [0x55694f1b7a26]"
	//		"NanoTest/Project/NanoTest(_ZN5Catch7Session11runInternalEv+0x16b) [0x55694f1b8e35]"
	//		"NanoTest/Project/NanoTest(_ZN5Catch7Session3runEv+0x5b) [0x55694f1b8b2b]"
	//		"NanoTest/Project/NanoTest(_ZN5Catch7Session3runIcEEiiPKPKT_+0x57) [0x55694f1f6a19]"
	//		"NanoTest/Project/NanoTest(main+0x59) [0x55694f1cef6e]"
	//		"/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf3) [0x7f16bb14f1e3]"
	//		"NanoTest/Project/NanoTest(_start+0x2e) [0x55694f064bce]"
	//
	NVectorString theTrace;

	for (size_t n = 0; n < numFrames; n++)
	{
		if (n >= skipFrames)
		{
			// Get the raw name (if any)
			NString theSymbol(theSymbols[n]);
			NString theName = theSymbol.GetMatch(kNDebuggerSymbolName);


			// Demangle the name
			//
			// C symbols can't be demangled so are used as-is.
			int   sysErr  = 0;
			char* cppName = abi::__cxa_demangle(theName.GetUTF8(), nullptr, nullptr, &sysErr);

			if (cppName != nullptr && sysErr == 0)
			{
				theName = cppName;
				free(cppName);
			}


			theTrace.emplace_back(theName);
		}
	}



	// Clean up
	if (theSymbols != nullptr)
	{
		free(theSymbols);
	}

	return theTrace;
}
