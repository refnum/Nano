/*	NAME:
		tvOSNMachine.cpp

	DESCRIPTION:
		tvOS machine support.

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
#include "NCommonDarwin.h"





//=============================================================================
//		NMachine::GetCores : Get the number of cores.
//-----------------------------------------------------------------------------
size_t NMachine::GetCores(NCoreType theType)
{


	// Get the cores
	return NCommonDarwin::MachineCores(theType);
}





//=============================================================================
//		NMachine::GetMemoryBytes : Get the memory.
//-----------------------------------------------------------------------------
uint64_t NMachine::GetMemoryBytes()
{


	// Get the memory
	return NCommonDarwin::MachineMemory();
}





//=============================================================================
//		NMachine::GetCPUName : Get the CPU name.
//-----------------------------------------------------------------------------
NString NMachine::GetCPUName()
{


	// Get the name
	return NCommonDarwin::MachineCPUName();
}





//=============================================================================
//		NMachine::GetCPUVendor : Get the CPU vendor.
//-----------------------------------------------------------------------------
NString NMachine::GetCPUVendor()
{


	// Get the vendor
	return NCommonDarwin::MachineCPUVendor();
}





//=============================================================================
//		NMachine::GetCPUHertz : Get the CPU speed.
//-----------------------------------------------------------------------------
uint64_t NMachine::GetCPUHertz()
{


	// Get the speed
	return NCommonDarwin::MachineCPUHertz();
}
