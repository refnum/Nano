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
#include "NUInt128.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// PCG constants
#define NN_PCG_CONSTANTS(_output, _work, _multiplier, _increment, _shift1, _shift2, _rotate)    \
	template<>                                                                                  \
	struct kPCG_XSH_RR<_output>                                                                 \
	{                                                                                           \
		using Work = _work;                                                                     \
																								\
		static constexpr _work multiplier = _multiplier;                                        \
		static constexpr _work increment  = _increment;                                         \
		static constexpr _work shift1     = _shift1;                                            \
		static constexpr _work shift2     = _shift2;                                            \
		static constexpr _work rotate     = _rotate;                                            \
	};

template<typename T>
struct kPCG_XSH_RR
{
};

NN_PCG_CONSTANTS(uint8_t, uint16_t, 12829, 47989, 5, 5, 13);

NN_PCG_CONSTANTS(uint16_t, uint32_t, 747796405, 2891336453, 10, 12, 28);

NN_PCG_CONSTANTS(uint32_t, uint64_t, 6364136223846793005ULL, 1442695040888963407ULL, 18, 27, 59);

NN_PCG_CONSTANTS(uint64_t,
				 uint128_t,
				 NMakeUInt128(2549297995355413924ULL, 4865540595714422341ULL),
				 NMakeUInt128(6364136223846793005ULL, 1442695040888963407ULL),
				 29,
				 58,
				 122);





//=============================================================================
//		NRandom::NextBool : Get the next bool.
//-----------------------------------------------------------------------------
inline bool NRandom::NextBool()
{
	// Get the value
	return NMathUtils::IsOdd(NextUInt32());
}





//=============================================================================
//		NRandom::NextUInt8 : Get the next uint8_t.
//-----------------------------------------------------------------------------
inline uint8_t NRandom::NextUInt8()
{


	// Get the value
	return PCG_XSH_RR<uint8_t>();
}





//=============================================================================
//		NRandom::NextUInt16 : Get the next uint16_t.
//-----------------------------------------------------------------------------
inline uint16_t NRandom::NextUInt16()
{


	// Get the value
	return PCG_XSH_RR<uint16_t>();
}





//=============================================================================
//		NRandom::NextUInt32 : Get the next uint32_t.
//-----------------------------------------------------------------------------
inline uint32_t NRandom::NextUInt32()
{


	// Get the value
	return PCG_XSH_RR<uint32_t>();
}





//=============================================================================
//		NRandom::NextUInt64 : Get the next uint64_t.
//-----------------------------------------------------------------------------
inline uint64_t NRandom::NextUInt64()
{


	// Get the value
	return PCG_XSH_RR<uint64_t>();
}





//=============================================================================
//		NRandom::GetBool : Get the next bool.
//-----------------------------------------------------------------------------
inline bool NRandom::GetBool()
{


	// Get the value
	return GetStream().NextBool();
}





//=============================================================================
//		NRandom::GetUInt8 : Get the next uint8_t.
//-----------------------------------------------------------------------------
inline uint8_t NRandom::GetUInt8()
{


	// Get the value
	return GetStream().NextUInt8();
}





//=============================================================================
//		NRandom::GetUInt16 : Get the next uint16_t.
//-----------------------------------------------------------------------------
inline uint16_t NRandom::GetUInt16()
{


	// Get the value
	return GetStream().NextUInt16();
}





//=============================================================================
//		NRandom::GetUInt32 : Get the next uint32_t.
//-----------------------------------------------------------------------------
inline uint32_t NRandom::GetUInt32()
{


	// Get the value
	return GetStream().NextUInt32();
}





//=============================================================================
//		NRandom::GetUInt64 : Get the next uint64_t.
//-----------------------------------------------------------------------------
inline uint64_t NRandom::GetUInt64()
{


	// Get the value
	return GetStream().NextUInt64();
}





//=============================================================================
//		NRandom::GetFloat32 : Get the next float32_t.
//-----------------------------------------------------------------------------
inline float32_t NRandom::GetFloat32()
{


	// Get the value
	return GetStream().NextFloat32();
}





//=============================================================================
//		NRandom::GetFloat64 : Get the next float64_t.
//-----------------------------------------------------------------------------
inline float64_t NRandom::GetFloat64()
{


	// Get the value
	return GetStream().NextFloat64();
}





//=============================================================================
//		NRandom::GetUInt8 : Get the next uint8_t.
//-----------------------------------------------------------------------------
inline uint8_t NRandom::GetUInt8(uint8_t minValue, uint8_t maxValue)
{


	// Get the value
	return GetStream().NextUInt8(minValue, maxValue);
}





//=============================================================================
//		NRandom::GetUInt16 : Get the next uint16_t.
//-----------------------------------------------------------------------------
inline uint16_t NRandom::GetUInt16(uint16_t minValue, uint16_t maxValue)
{


	// Get the value
	return GetStream().NextUInt16(minValue, maxValue);
}





//=============================================================================
//		NRandom::GetUInt32 : Get the next uint32_t.
//-----------------------------------------------------------------------------
inline uint32_t NRandom::GetUInt32(uint32_t minValue, uint32_t maxValue)
{


	// Get the value
	return GetStream().NextUInt32(minValue, maxValue);
}





//=============================================================================
//		NRandom::GetUInt64 : Get the next uint64_t.
//-----------------------------------------------------------------------------
inline uint64_t NRandom::GetUInt64(uint64_t minValue, uint64_t maxValue)
{


	// Get the value
	return GetStream().NextUInt64(minValue, maxValue);
}





//=============================================================================
//		NRandom::GetData : Get the next random data.
//-----------------------------------------------------------------------------
inline void NRandom::GetData(size_t theSize, void* thePtr)
{


	// Get the data
	GetStream().NextData(theSize, thePtr);
}





#pragma mark private
//=============================================================================
//		NRandom::PCG_XSH_RR : Calculate a random integer.
//-----------------------------------------------------------------------------
template<typename Output>
Output NRandom::PCG_XSH_RR()
{


	// Get the state we need
	using Work = typename kPCG_XSH_RR<Output>::Work;

	constexpr Work kMultiplier = kPCG_XSH_RR<Output>::multiplier;
	constexpr Work kIncrement  = kPCG_XSH_RR<Output>::increment;
	constexpr Work kShift1     = kPCG_XSH_RR<Output>::shift1;
	constexpr Work kShift2     = kPCG_XSH_RR<Output>::shift2;
	constexpr Work kRotate     = kPCG_XSH_RR<Output>::rotate;

	static_assert(sizeof(Work) <= sizeof(mState));
	static_assert(NMathUtils::IsOdd(kIncrement));



	// Advance the state
	Work oldState;
	memcpy(&oldState, mState, sizeof(Work));

	Work newState = Work(oldState * kMultiplier + kIncrement);
	memcpy(mState, &newState, sizeof(Work));



	// Generate the result
	Output rotateBy  = Output(oldState >> kRotate);
	Output theResult = Output(((oldState >> kShift1) ^ oldState) >> kShift2);

	if (rotateBy != 0)
	{
		theResult = NMathUtils::RotateRight(theResult, rotateBy);
	}

	return theResult;
}





//=============================================================================
//		NRandom::PCG_XSH_RR : Calculate a random integer.
//-----------------------------------------------------------------------------
template<typename Output>
Output NRandom::PCG_XSH_RR(Output minValue, Output maxValue)
{


	// Get the state we need
	using Work = typename kPCG_XSH_RR<Output>::Work;



	// Apply the range
	//
	// We generate unbiased numbers with Lemire's method:
	//
	//	https://www.pcg-random.org/posts/bounded-rands.html
	//
	// The result is calculated relative to 0, then moved to the desired range.
	Output maxRange = Output(maxValue - minValue);
	Output r        = PCG_XSH_RR<Output>();
	Work   m        = Work(Work(r) * Work(maxRange));
	Output l        = Output(m);

	if (l < maxRange)
	{
		NN_DIAGNOSTIC_PUSH();
		NN_DIAGNOSTIC_IGNORE_MSVC(C4146_unary_minus_applied_to_unsigned);
		Output t = Output(-maxRange);
		NN_DIAGNOSTIC_POP();

		if (t >= maxRange)
		{
			t = t - maxRange;
			if (t >= maxRange)
			{
				t %= maxRange;
			}
		}

		while (l < t)
		{
			r = PCG_XSH_RR<Output>();
			m = Work(Work(r) * Work(maxRange));
			l = Output(m);
		}
	}

	return Output(minValue + Output(m >> (sizeof(Output) * 8)));
}
