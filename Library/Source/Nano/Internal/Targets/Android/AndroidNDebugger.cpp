/*	NAME:
		AndroidNDebugger.cpp

	DESCRIPTION:
		Android debugger support.

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
#include "NDebugger.h"

// Nano
#include "NSharedLinux.h"

// System
#include <cxxabi.h>
#include <dlfcn.h>
#include <unwind.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr size_t kNDebuggerMaxFrames                 = 512;





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
struct NUnwindFrames
{
	void** frameCurrent;
	void** frameEnd;
};





//=============================================================================
//		UnwindCallback : Stack unwinding callback.
//-----------------------------------------------------------------------------
static _Unwind_Reason_Code UnwindCallback(struct _Unwind_Context* theContext, void* userData)
{
	// Get the state we need
	NUnwindFrames* theFrames = reinterpret_cast<NUnwindFrames*>(userData);
	uintptr_t      currentPC = _Unwind_GetIP(theContext);



	// Record the frame
	if (currentPC != 0)
	{
		if (theFrames->frameCurrent != theFrames->frameEnd)
		{
			*theFrames->frameCurrent++ = reinterpret_cast<void*>(currentPC);
		}
		else
		{
			return _URC_END_OF_STACK;
		}
	}

	return _URC_NO_REASON;
}





#pragma mark NDebugger
//=============================================================================
//		NDebugger::IsActive : Is a debugger active?
//-----------------------------------------------------------------------------
bool NDebugger::IsActive()
{


	// Check for a tracer
	NString theText  = NSharedLinux::GetProcFile(NFilePath("/proc/self/status"));
	bool    isActive = !theText.Contains("TracerPid:\t0\n");

	if (isActive)
	{
		isActive = theText.Contains("TracerPid:\t");
	}

	return isActive;
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
	void*         theFrames[kNDebuggerMaxFrames];
	NUnwindFrames unwindFrames;

	unwindFrames.frameCurrent = theFrames;
	unwindFrames.frameEnd     = theFrames + +kNDebuggerMaxFrames;

	_Unwind_Backtrace(UnwindCallback, &unwindFrames);
	numFrames = std::min(numFrames, size_t(unwindFrames.frameCurrent - theFrames));



	// Process the symbols
	NVectorString theTrace;

	for (size_t n = 0; n < numFrames; n++)
	{
		if (n >= skipFrames)
		{
			// Get the symbol
			NString theSymbol;

			Dl_info info;
			if (dladdr(theFrames[n], &info) && info.dli_sname != nullptr)
			{
				theSymbol = info.dli_sname;
			}


			// Demangle the name
			//
			// C symbols can't be demangled so are used as-is.
			int     sysErr  = 0;
			NString theName = theSymbol;
			char*   cppName =
				__cxxabiv1::__cxa_demangle(theName.GetUTF8(), nullptr, nullptr, &sysErr);

			if (cppName != nullptr && sysErr == 0)
			{
				theName = cppName;
				free(cppName);
			}

			theTrace.emplace_back(theName);
		}
	}

	return theTrace;
}
