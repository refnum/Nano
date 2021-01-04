/*	NAME:
		macOSNThread.cpp

	DESCRIPTION:
		macOS thread.

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
#include "NThread.h"

// Nano
#include "NSharedDarwin.h"
#include "NSharedPOSIX.h"





//=============================================================================
//		NThread::GetStackSize : Get the current thread's stack size.
//-----------------------------------------------------------------------------
size_t NThread::GetStackSize()
{


	// Get the size
	return NSharedDarwin::ThreadStackSize();
}





//=============================================================================
//		NThread::GetName : Get the current thread's name.
//-----------------------------------------------------------------------------
NString NThread::GetName()
{


	// Get the name
	return NSharedPOSIX::ThreadGetName();
}





//=============================================================================
//		NThread::SetName : Set the current thread's name.
//-----------------------------------------------------------------------------
void NThread::SetName(const NString& theName)
{


	// Set the name
	NSharedPOSIX::ThreadSetName(theName);
}





//=============================================================================
//		NThread::GetPriority : Get the current thread's priority.
//-----------------------------------------------------------------------------
float NThread::GetPriority()
{


	// Get the priority
	return NSharedPOSIX::ThreadGetPriority();
}





//=============================================================================
//		NThread::SetPriority : Set the current thread's priority.
//-----------------------------------------------------------------------------
void NThread::SetPriority(float thePriority)
{


	// Set the priority
	NSharedPOSIX::ThreadSetPriority(thePriority);
}





//=============================================================================
//		NThread::GetCores : Get the current thread's preferred cores.
//-----------------------------------------------------------------------------
NVectorUInt8 NThread::GetCores()
{


	// Get the cores
	return NSharedDarwin::ThreadGetCores();
}





//=============================================================================
//		NThread::SetCores : Set the current thread's preferred cores.
//-----------------------------------------------------------------------------
void NThread::SetCores(const NVectorUInt8& theCores)
{


	// Set the cores
	NSharedDarwin::ThreadSetCores(theCores);
}





#pragma mark private
//=============================================================================
//		NThread::ThreadCreate : Create a native thread.
//-----------------------------------------------------------------------------
NThreadHandle NThread::ThreadCreate(NThreadContext* theContext)
{


	// Create the thread
	return NSharedPOSIX::ThreadCreate(theContext);
}





//=============================================================================
//		NThread::ThreadJoin : Join a native thread.
//-----------------------------------------------------------------------------
void NThread::ThreadJoin(NThreadHandle theThread)
{


	// Join the thread
	NSharedPOSIX::ThreadJoin(theThread);
}





//=============================================================================
//		NThread::ThreadIsMain : Is this the main thread?
//-----------------------------------------------------------------------------
bool NThread::ThreadIsMain()
{


	// Check the thread
	return NSharedDarwin::ThreadIsMain();
}
