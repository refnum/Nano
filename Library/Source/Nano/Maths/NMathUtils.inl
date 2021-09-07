/*	NAME:
		NMathUtils.inl

	DESCRIPTION:
		Math utilities.

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
#include "NDebug.h"
#include "NanoTargets.h"





//=============================================================================
//		NMathUtils::IsOdd : Is this an odd number?
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NMathUtils::IsOdd(T theValue)
{


	// Test the value
	return (theValue & 0x01) != 0;
}





//=============================================================================
//		NMathUtils::IsEven : Is this an even number?
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NMathUtils::IsEven(T theValue)
{


	// Test the value
	return (theValue & 0x01) == 0;
}





//=============================================================================
//		NMathUtils::IsPowerOf2 : Is this a power of 2?
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr bool NMathUtils::IsPowerOf2(T theValue)
{


	// Test the value
	return theValue != 0 && (theValue & (theValue - 1)) == 0;
}





//=============================================================================
//		NMathUtils::NextPowerOf2 : Get the next power of 2.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr T NMathUtils::NextPowerOf2(T theValue)
{


	// Get the value
	//
	// https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
	if (theValue <= 1)
	{
		theValue = 2;
	}
	else
	{
		theValue--;

		if constexpr (sizeof(theValue) >= sizeof(uint8_t))
		{
			theValue |= theValue >> 1;
			theValue |= theValue >> 2;
			theValue |= theValue >> 4;
		}

		if constexpr (sizeof(theValue) >= sizeof(uint16_t))
		{
			theValue |= theValue >> 8;
		}

		if constexpr (sizeof(theValue) >= sizeof(uint32_t))
		{
			theValue |= theValue >> 16;
		}

		if constexpr (sizeof(theValue) >= sizeof(uint64_t))
		{
			theValue |= theValue >> 32;
		}

		theValue++;
	}

	return theValue;
}





//=============================================================================
//		NMathUtils::CountBits : Count the number of set bits.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr size_t NMathUtils::CountBits(T theValue)
{


	// Validate our state
	static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);



#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	// Count with an intrinsic
	switch (sizeof(theValue))
	{
		case 1:
		case 2:
		case 4:
			return size_t(__builtin_popcount(uint32_t(theValue)));
			break;

		case 8:
			return size_t(__builtin_popcountll(uint64_t(theValue)));
			break;
	}

#elif NN_COMPILER_MSVC
	// Count with an intrinsic
	switch (sizeof(theValue))
	{
		case 1:
		case 2:
			return size_t(__popcnt16(uint16_t(theValue)));
			break;

		case 4:
			return size_t(__popcnt(uint32_t(theValue)));
			break;

		case 8:
			return size_t(__popcnt64(uint64_t(theValue)));
			break;
	}

#else

	// Count in parallel
	//
	// This is typically the same approach used by the intrinsics when the
	// popcnt instruction is unavailable.
	//
	// http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
	constexpr T kMagic55555555 = T(~T(0)) / 0x03;
	constexpr T kMagic33333333 = T(~T(0)) / 0x05;
	constexpr T kMagic0F0F0F0F = T(~T(0)) / 0x11;
	constexpr T kMagic01010101 = T(~T(0)) / 0xFF;
	constexpr T kShiftDown     = (sizeof(T) - 1) * 8;

	theValue = theValue - ((theValue >> 1) & kMagic55555555);
	theValue = (theValue & kMagic33333333) + ((theValue >> 2) & kMagic33333333);
	theValue = (theValue + (theValue >> 4)) & kMagic0F0F0F0F;

	return T(theValue * kMagic01010101) >> kShiftDown;
#endif

	NN_UNREACHABLE();
}





//=============================================================================
//		NMathUtils::CountLeadingZeros : Count the number of leading zeros.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr size_t NMathUtils::CountLeadingZeros(T theValue)
{


	// Validate our state
	static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);


#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	// Count with an intrinsic
	switch (sizeof(theValue))
	{
		case 1:
			return size_t(__builtin_clz(uint32_t(theValue)) - 24);
			break;

		case 2:
			return size_t(__builtin_clz(uint32_t(theValue)) - 16);
			break;

		case 4:
			return size_t(__builtin_clz(uint32_t(theValue)));
			break;

		case 8:
			return size_t(__builtin_clzll(uint64_t(theValue)));
			break;
	}

#elif NN_COMPILER_MSVC
	// Count with an intrinsic
	switch (sizeof(theValue))
	{
		case 1:
			return size_t(__lzcnt16(uint16_t(theValue)) - 8);
			break;

		case 2:
			return size_t(__lzcnt16(uint16_t(theValue)));
			break;

		case 4:
			return size_t(__lzcnt(uint32_t(theValue)));
			break;

		case 8:
			return size_t(__lzcnt64(uint64_t(theValue)));
			break;
	}

#else

	// Count the bits
	switch (sizeof(theValue))
	{
		case 1:
			theValue |= (theValue >> 1);
			theValue |= (theValue >> 2);
			theValue |= (theValue >> 4);
			break;

		case 2:
			theValue |= (theValue >> 1);
			theValue |= (theValue >> 2);
			theValue |= (theValue >> 4);
			theValue |= (theValue >> 8);
			break;

		case 4:
			theValue |= (theValue >> 1);
			theValue |= (theValue >> 2);
			theValue |= (theValue >> 4);
			theValue |= (theValue >> 8);
			theValue |= (theValue >> 16);
			break;

		case 8:
			theValue |= (theValue >> 1);
			theValue |= (theValue >> 2);
			theValue |= (theValue >> 4);
			theValue |= (theValue >> 8);
			theValue |= (theValue >> 16);
			theValue |= (theValue >> 32);
			break;
	}

	return CountBits(T(~theValue));
#endif
}





//=============================================================================
//		NMathUtils::CountTrailingZeros : Count the number of trailing zeros.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
constexpr size_t NMathUtils::CountTrailingZeros(T theValue)
{


	// Validate our state
	static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);


#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	// Count with an intrinsic
	switch (sizeof(theValue))
	{
		case 1:
		case 2:
		case 4:
			return size_t(__builtin_ctzl(uint32_t(theValue)));
			break;

		case 8:
			return size_t(__builtin_ctzll(uint64_t(theValue)));
			break;
	}

#elif NN_COMPILER_MSVC
	// Count with an intrinsic
	unsigned long bitIndex = 0;

	switch (sizeof(theValue))
	{
		case 1:
		case 2:
		case 4:
			_BitScanForward(&bitIndex, uint32_t(theValue));
			break;

		case 8:
			_BitScanForward64(&bitIndex, uint64_t(theValue));
			break;
	}

	return size_t(bitIndex);

#else

	// Count the bits
	switch (sizeof(theValue))
	{
		case 1:
			theValue |= (theValue << 1);
			theValue |= (theValue << 2);
			theValue |= (theValue << 4);
			break;

		case 2:
			theValue |= (theValue << 1);
			theValue |= (theValue << 2);
			theValue |= (theValue << 4);
			theValue |= (theValue << 8);
			break;

		case 4:
			theValue |= (theValue << 1);
			theValue |= (theValue << 2);
			theValue |= (theValue << 4);
			theValue |= (theValue << 8);
			theValue |= (theValue << 16);
			break;

		case 8:
			theValue |= (theValue << 1);
			theValue |= (theValue << 2);
			theValue |= (theValue << 4);
			theValue |= (theValue << 8);
			theValue |= (theValue << 16);
			theValue |= (theValue << 32);
			break;
	}

	return CountBits(T(~theValue));
#endif
}





//=============================================================================
//		NMathUtils::RotateLeft : Rotate left.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
inline T NMathUtils::RotateLeft(T theValue, size_t rotateBy)
{


	// Get the state we need
	constexpr T kNumBits = sizeof(T) * 8;

	static_assert(kNumBits == 8 || kNumBits == 16 || kNumBits == 32 || kNumBits == 64);
	NN_REQUIRE(rotateBy > 0 && rotateBy < kNumBits);


	// Rotate the value
	return T(T(theValue >> (kNumBits - rotateBy)) | T(theValue << rotateBy));
}





//=============================================================================
//		NMathUtils::RotateRight : Rotate right.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
inline T NMathUtils::RotateRight(T theValue, size_t rotateBy)
{


	// Get the state we need
	constexpr T kNumBits = sizeof(T) * 8;

	static_assert(kNumBits == 8 || kNumBits == 16 || kNumBits == 32 || kNumBits == 64);
	NN_REQUIRE(rotateBy > 0);
	NN_REQUIRE(rotateBy < kNumBits);



	// Rotate the value
	return T(T(theValue << (kNumBits - rotateBy)) | T(theValue >> rotateBy));
}





//=============================================================================
//		NMathUtils::ToDegrees : Convert from radians to degrees.
//-----------------------------------------------------------------------------
constexpr NDegrees NMathUtils::ToDegrees(NRadians theValue)
{


	// Convert to degrees
	return theValue * (180.0 / kNPi);
}





//=============================================================================
//		NMathUtils::ToRadians : Convert from degrees to radians.
//-----------------------------------------------------------------------------
constexpr NRadians NMathUtils::ToRadians(NDegrees theValue)
{


	// Convert to radians
	return theValue * (kNPi / 180.0);
}
