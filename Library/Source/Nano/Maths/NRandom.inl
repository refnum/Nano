/*	NAME:
		NRandom.inl

	DESCRIPTION:
		Random numbers.

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
// Nano
#include "NMathUtils.h"





//=============================================================================
//		NRandom::NextBool : Get the next bool.
//-----------------------------------------------------------------------------
inline bool NRandom::NextBool()
{


	// Get the result
	return (RandomUInt32() & 0x1) != 0;
}





//=============================================================================
//		NRandom::NextUInt8 : Get the next uint8_t.
//-----------------------------------------------------------------------------
inline uint8_t NRandom::NextUInt8()
{


	// Get the value
	return uint8_t(RandomUInt32());
}





//=============================================================================
//		NRandom::NextUInt16 : Get the next uint16_t.
//-----------------------------------------------------------------------------
inline uint16_t NRandom::NextUInt16()
{


	// Get the value
	return uint16_t(RandomUInt32());
}





//=============================================================================
//		NRandom::NextUInt32 : Get the next uint32_t.
//-----------------------------------------------------------------------------
inline uint32_t NRandom::NextUInt32()
{


	// Get the value
	return uint32_t(RandomUInt32());
}





//=============================================================================
//		NRandom::NextUInt64 : Get the next uint64_t.
//-----------------------------------------------------------------------------
inline uint64_t NRandom::NextUInt64()
{


	// Get the value
	uint32_t values32[2];
	uint64_t value64;

	values32[0] = RandomUInt32();
	values32[1] = RandomUInt32();

	static_assert(sizeof(value64) == sizeof(values32));
	memcpy(&value64, &values32[0], sizeof(value64));

	return value64;
}





//=============================================================================
//		NRandom::NextFloat32 : Get the next float32_t.
//-----------------------------------------------------------------------------
inline float32_t NRandom::NextFloat32()
{


	// Get the value
	return float32_t(NextFloat64());
}





//=============================================================================
//		NRandom::NextFloat64 : Get the next float64_t.
//-----------------------------------------------------------------------------
inline float64_t NRandom::NextFloat64()
{


	// Get the value
	return ldexp(double(NextUInt64()), -64);
}
