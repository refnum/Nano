/*	NAME:
		WindowsNMachine.cpp

	DESCRIPTION:
		Windows machine support.

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
#include "NMachine.h"

// Nano
#include "NCommonWindows.h"
#include "NData.h"
#include "NMathUtils.h"

// System
#include <Windows.h>
#include <intrin.h>
#include <sysinfoapi.h>





//=============================================================================
//		NMachine::GetCores : Get the number of cores.
//-----------------------------------------------------------------------------
size_t NMachine::GetCores(NCoreType theType)
{


	// Get the state we need
	DWORD theSize = 0;
	BOOL  isValid = GetLogicalProcessorInformation(nullptr, &theSize);

	NN_REQUIRE(!isValid);
	NN_REQUIRE(theSize > 0);


	NData theData(size_t(theSize), nullptr, NDataSource::None);
	auto* procInfo =
		reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION*>(theData.GetMutableData());

	isValid = GetLogicalProcessorInformation(procInfo, &theSize);
	NN_REQUIRE(isValid);



	// Get the cores
	size_t numCores   = 0;
	size_t numRecords = theSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);

	for (size_t n = 0; n < numRecords; n++)
	{
		if (procInfo[n].Relationship == RelationProcessorCore)
		{
			if (theType == NCoreType::Logical)
			{
				numCores += NMathUtils::CountBits(procInfo[n].ProcessorMask);
			}
			else
			{
				numCores += 1;
			}
		}
	}

	NN_REQUIRE(numCores >= 1);
	return numCores;
}





//=============================================================================
//		NMachine::GetMemoryBytes : Get the memory.
//-----------------------------------------------------------------------------
uint64_t NMachine::GetMemoryBytes()
{


	// Get the memory
	MEMORYSTATUSEX memInfo{};
	memInfo.dwLength = sizeof(memInfo);

	if (!GlobalMemoryStatusEx(&memInfo))
	{
		memInfo.ullTotalPhys = 0;
	}

	return memInfo.ullTotalPhys;
}





//=============================================================================
//		NMachine::GetCPUName : Get the CPU name.
//-----------------------------------------------------------------------------
NString NMachine::GetCPUName()
{


	// Get the name
	NString theName =
		NCommonWindows::RegistryGetString(HKEY_LOCAL_MACHINE,
										  "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
										  "ProcessorNameString");
	NN_EXPECT(!theName.IsEmpty());

	if (theName.IsEmpty())
	{
		theName = "Unknown";
	}

	return theName;
}





//=============================================================================
//		NMachine::GetCPUVendor : Get the CPU vendor.
//-----------------------------------------------------------------------------
NString NMachine::GetCPUVendor()
{


	// Get the name
	NString theName =
		NCommonWindows::RegistryGetString(HKEY_LOCAL_MACHINE,
										  "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
										  "VendorIdentifier");
	NN_EXPECT(!theName.IsEmpty());

	if (theName.IsEmpty())
	{
		theName = "Unknown";
	}

	return theName;
}





//=============================================================================
//		NMachine::GetCPUHertz : Get the CPU speed.
//-----------------------------------------------------------------------------
uint64_t NMachine::GetCPUHertz()
{


	// Get the speed
	int32_t speedMhz =
		NCommonWindows::RegistryGetInt32(HKEY_LOCAL_MACHINE,
										 "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
										 "~MHz");
	NN_REQUIRE(speedMhz > 0);

	return uint64_t(speedMhz) * kNMegahertz;
}
