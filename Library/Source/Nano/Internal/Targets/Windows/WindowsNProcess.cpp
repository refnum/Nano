/*	NAME:
		WindowsNProcess.cpp

	DESCRIPTION:
		Windows process support.

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
#include "NProcess.h"

// Nano
#include "NFilePath.h"
#include "NSharedWindows.h"

// System
#include <Windows.h>
#include <psapi.h>





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetAddressSpaceMax : Get the maximum address space.
//-----------------------------------------------------------------------------
static size_t GetAddressSpaceMax()
{


	// Determine if the app supports large addresses
	uintptr_t          baseAddr  = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
	IMAGE_DOS_HEADER*  headerDOS = reinterpret_cast<IMAGE_DOS_HEADER*>(baseAddr);
	IMAGE_FILE_HEADER* headerPE =
		reinterpret_cast<IMAGE_FILE_HEADER*>(baseAddr + headerDOS->e_lfanew + 4);

	bool isAppLargeAddress = (headerPE->Characteristics & IMAGE_FILE_LARGE_ADDRESS_AWARE);



	// Determine if the OS is 64-bit
	BOOL isWindows64Bit = NN_ARCH_64 ? TRUE : FALSE;

	if (!isWindows64Bit)
	{
		isWindows64Bit = IsWow64Process(GetCurrentProcess(), &isWindows64Bit) && isWindows64Bit;
	}



	// Get the address space
	//
	// We hard-code the standard sizes for 64-bit and 32-bit processes, based
	// on large address support in the app and 64-bit support in the OS.
	//
	// These sizes are are documented at:
	//
	//	https://docs.microsoft.com/ru-ru/windows/win32/memory/memory-limits-for-windows-releases
	//
	size_t theSize = 0;

	if (NN_ARCH_64)
	{
		theSize = isAppLargeAddress ? (128 * kNTebibyte) : (2 * kNGibibyte);
	}
	else
	{
		if (isWindows64Bit)
		{
			theSize = isAppLargeAddress ? (4 * kNGibibyte) : (2 * kNGibibyte);
		}
		else
		{
			theSize = isAppLargeAddress ? (3 * kNGibibyte) : (2 * kNGibibyte);
		}
	}

	return theSize;
}





//=============================================================================
//		NProcess::GetName : Get the name.
//-----------------------------------------------------------------------------
NString NProcess::GetName()
{


	// Get the name
	TCHAR   theBuffer[MAX_PATH]{};
	NString theName;

	if (GetModuleFileName(NULL, theBuffer, MAX_PATH))
	{
		NFilePath thePath(NSharedWindows::ToString(theBuffer));

		if (thePath.IsValid())
		{
			theName = thePath.GetStem();
			NN_EXPECT(!theName.IsEmpty());
		}
	}

	if (theName.IsEmpty())
	{
		theName = "Unknown";
	}

	return theName;
}





//=============================================================================
//		NProcess::GetMemory : Get memory usage.
//-----------------------------------------------------------------------------
NMemoryInfo NProcess::GetMemory()
{


	// Get the state we need
	PROCESS_MEMORY_COUNTERS memCounters{};

	BOOL wasOK = GetProcessMemoryInfo(GetCurrentProcess(), &memCounters, sizeof(memCounters));
	NN_EXPECT(wasOK);



	// Get the memory usage
	//
	// addressSpaceUsed is set to allocated memory, although we could
	// obtain a more accurate result by iterating with VirtualQuery.
	//
	// addressSpaceMax is hard-coded to the standard Windows limits.
	NMemoryInfo theInfo{};

	if (wasOK)
	{
		theInfo.memoryResident   = memCounters.WorkingSetSize;
		theInfo.memoryAllocated  = memCounters.PagefileUsage;
		theInfo.addressSpaceUsed = memCounters.PagefileUsage;
		theInfo.addressSpaceMax  = GetAddressSpaceMax();
	}

	return theInfo;
}
