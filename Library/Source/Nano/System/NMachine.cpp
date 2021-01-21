/*	NAME:
		NMachine.cpp

	DESCRIPTION:
		Machine support.

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





//=============================================================================
//		NMachine::GetMemory : Get the available memory.
//-----------------------------------------------------------------------------
float64_t NMachine::GetMemory(size_t theScale)
{


	// Get the memory
	uint64_t sizeBytes = GetMemoryBytes();
	NN_REQUIRE(sizeBytes > 0);

	return float64_t(sizeBytes) / float64_t(theScale);
}





//=============================================================================
//		NMachine::GetCPUArchitecture : Get the CPU architecture.
//-----------------------------------------------------------------------------
NString NMachine::GetCPUArchitecture()
{


	// Get the architecture
	if (NN_ARCH_ARM)
	{
		return NN_ARCH_64 ? "arm64" : "arm";
	}

	else if (NN_ARCH_X86)
	{
		return NN_ARCH_64 ? "x86_64" : "x86";
	}

	else
	{
		NN_LOG_WARNING("Unknown architecture!");
		return "UNKNOWN";
	}
}
