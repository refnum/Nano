/*	NAME:
		NByteSwap.cpp

	DESCRIPTION:
		Byte-swap support.

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
#include "NByteSwap.h"

// Nano
#include "NDebug.h"





//=============================================================================
//		NByteSwap::Swap : Swap N values.
//-----------------------------------------------------------------------------
void NByteSwap::Swap(size_t numValues, size_t valueSize, void* valuePtr)
{


	// Validate our parameters
	NN_REQUIRE(valueSize == 1 || valueSize == 2 || valueSize == 4 || valueSize == 8);



	// Swap the values
	switch (valueSize)
	{
		case 1:
			break;

		case 2:
			for (size_t n = 0; n < numValues; n++)
			{
				uint16_t* value16 = reinterpret_cast<uint16_t*>(valuePtr);
				value16[n]        = Swap(value16[n]);
			}
			break;

		case 4:
			for (size_t n = 0; n < numValues; n++)
			{
				uint32_t* value32 = reinterpret_cast<uint32_t*>(valuePtr);
				value32[n]        = Swap(value32[n]);
			}
			break;

		case 8:
			for (size_t n = 0; n < numValues; n++)
			{
				uint64_t* value64 = reinterpret_cast<uint64_t*>(valuePtr);
				value64[n]        = Swap(value64[n]);
			}
			break;
	}
}
