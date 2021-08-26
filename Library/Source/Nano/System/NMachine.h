/*	NAME:
		NMachine.h

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
#ifndef NMACHINE_H
#define NMACHINE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoConstants.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// CPU core type
enum class NCoreType
{
	Logical,
	Physical
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NString;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NMachine
{
public:
	// Get the number of cores
	static size_t                       GetCores(NCoreType theType = NCoreType::Logical);


	// Get the memory
	//
	// The default scale factor returns physical memory in bytes.
	//
	// Passing a different scale factor will return the size in the
	// appropriate units (e.g., a scale of kNGibibyte will return GiB).
	static float64_t                    GetMemory(size_t theScale = kNByte);


	// Get the maximum CPU speed
	//
	// The default scale factor returns the speed in Hertz.
	//
	// Passing a different scale factor will return the speed in the
	// appropriate units (e.g., a scale of kNGigahertz will return GHz).
	static float64_t                    GetCPUSpeed(size_t theScale = kNHertz);


	// Get the CPU info
	static NString                      GetCPUName();
	static NString                      GetCPUVendor();
	static NString                      GetCPUArchitecture();


private:
	static uint64_t                     GetMemoryBytes();
	static uint64_t                     GetCPUHertz();
};



#endif // NMACHINE_H
