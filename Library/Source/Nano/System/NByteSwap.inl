/*	NAME:
		NByteSwap.inl

	DESCRIPTION:
		Byte-swap support.

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
#include "NanoMacros.h"





//=============================================================================
//		NByteSwap::SwapNtoB : Swap native to big-endian.
//-----------------------------------------------------------------------------
template<typename T>
constexpr T NByteSwap::SwapNtoB(T theValue)
{
	return SwapXtoY(NEndian::Native, NEndian::Big, theValue);
}





//=============================================================================
//		NByteSwap::SwapNtoB : Swap native to big-endian.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::SwapNtoB(T* theValue)
{
	SwapXtoY(NEndian::Native, NEndian::Big, theValue);
}





//=============================================================================
//		NByteSwap::SwapNtoB : Swap native to big-endian.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::SwapNtoB(size_t numValues, T* theValues)
{
	SwapXtoY(NEndian::Native, NEndian::Big, numValues, theValues);
}





//=============================================================================
//		NByteSwap::SwapBtoN : Swap big-endian to native.
//-----------------------------------------------------------------------------
template<typename T>
constexpr T NByteSwap::SwapBtoN(T theValue)
{
	return SwapXtoY(NEndian::Big, NEndian::Native, theValue);
}





//=============================================================================
//		NByteSwap::SwapBtoN : Swap big-endian to native.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::SwapBtoN(T* theValue)
{
	SwapXtoY(NEndian::Big, NEndian::Native, theValue);
}





//=============================================================================
//		NByteSwap::SwapBtoN : Swap big-endian to native.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::SwapBtoN(size_t numValues, T* theValues)
{
	SwapXtoY(NEndian::Big, NEndian::Native, numValues, theValues);
}





//=============================================================================
//		NByteSwap::SwapNtoL : Swap native to little-endian.
//-----------------------------------------------------------------------------
template<typename T>
constexpr T NByteSwap::SwapNtoL(T theValue)
{
	return SwapXtoY(NEndian::Native, NEndian::Little, theValue);
}





//=============================================================================
//		NByteSwap::SwapNtoL : Swap native to little-endian.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::SwapNtoL(T* theValue)
{
	SwapXtoY(NEndian::Native, NEndian::Little, theValue);
}





//=============================================================================
//		NByteSwap::SwapNtoL : Swap native to little-endian.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::SwapNtoL(size_t numValues, T* theValues)
{
	SwapXtoY(NEndian::Native, NEndian::Little, numValues, theValues);
}





//=============================================================================
//		NByteSwap::SwapLtoN : Swap little-endian to native.
//-----------------------------------------------------------------------------
template<typename T>
constexpr T NByteSwap::SwapLtoN(T theValue)
{
	return SwapXtoY(NEndian::Little, NEndian::Native, theValue);
}





//=============================================================================
//		NByteSwap::SwapLtoN : Swap little-endian to native.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::SwapLtoN(T* theValue)
{
	SwapXtoY(NEndian::Little, NEndian::Native, theValue);
}





//=============================================================================
//		NByteSwap::SwapLtoN : Swap little-endian to native.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::SwapLtoN(size_t numValues, T* theValues)
{
	SwapXtoY(NEndian::Little, NEndian::Native, numValues, theValues);
}





#pragma mark private
//=============================================================================
//		NByteSwap::SwapXtoY : Swap X-endian to Y-endian.
//-----------------------------------------------------------------------------
template<typename T>
constexpr T NByteSwap::SwapXtoY(NEndian orderX, NEndian orderY, T theValue)
{
	if (orderX != orderY)
	{
		return Swap(theValue);
	}

	return theValue;
}





//=============================================================================
//		NByteSwap::SwapXtoY : Swap X-endian to Y-endian.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::SwapXtoY(NEndian orderX, NEndian orderY, T* theValue)
{
	if (orderX != orderY)
	{
		Swap(theValue);
	}
}





//=============================================================================
//		NByteSwap::SwapXtoY : Swap X-endian to Y-endian.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::SwapXtoY(NEndian orderX, NEndian orderY, size_t numValues, T* theValues)
{
	if (orderX != orderY)
	{
		Swap(numValues, sizeof(T), theValues);
	}
}





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template<>
constexpr uint16_t NByteSwap::Swap<uint16_t>(uint16_t theValue)
{


	// Swap the value
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	return __builtin_bswap16(theValue);

#else
	return uint16_t(((theValue >> 8) & 0x00FF) | ((theValue << 8) & 0xFF00));
#endif
}





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template<>
constexpr uint32_t NByteSwap::Swap<uint32_t>(uint32_t theValue)
{


	// Swap the value
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	return __builtin_bswap32(theValue);

#else
	return uint32_t(((theValue >> 24) & 0x000000FF) | ((theValue >> 8) & 0x0000FF00) |
					((theValue << 8) & 0x00FF0000) | ((theValue << 24) & 0xFF000000));
#endif
}





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template<>
constexpr uint64_t NByteSwap::Swap<uint64_t>(uint64_t theValue)
{


	// Swap the value
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	return __builtin_bswap64(theValue);

#else
	return uint64_t(
		((theValue >> 56) & 0x00000000000000FFULL) | ((theValue >> 40) & 0x000000000000FF00ULL) |
		((theValue >> 24) & 0x0000000000FF0000ULL) | ((theValue >> 8) & 0x00000000FF000000ULL) |
		((theValue << 8) & 0x000000FF00000000ULL) | ((theValue << 24) & 0x0000FF0000000000ULL) |
		((theValue << 40) & 0x00FF000000000000ULL) | ((theValue << 56) & 0xFF00000000000000ULL));
#endif
}





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template<>
inline float NByteSwap::Swap<float>(float theValue)
{
	Swap(reinterpret_cast<uint32_t*>(&theValue));

	return theValue;
}





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template<>
inline double NByteSwap::Swap<double>(double theValue)
{
	Swap(reinterpret_cast<uint64_t*>(&theValue));

	return theValue;
}





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template<typename T>
constexpr T NByteSwap::Swap(T theValue)
{
	if constexpr (std::is_integral<T>::value)
	{
		if constexpr (sizeof(T) == sizeof(uint16_t))
		{
			theValue = T(Swap(uint16_t(theValue)));
		}
		else if constexpr (sizeof(T) == sizeof(uint32_t))
		{
			theValue = T(Swap(uint32_t(theValue)));
		}
		else if constexpr (sizeof(T) == sizeof(uint64_t))
		{
			theValue = T(Swap(uint64_t(theValue)));
		}
	}
	else
	{
		Swap(1, sizeof(theValue), &theValue);
	}

	return theValue;
}





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::Swap(T* theValue)
{
	*theValue = Swap(*theValue);
}





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template<typename T>
void NByteSwap::Swap(size_t numValues, T* theValues)
{
	Swap(numValues, sizeof(T), theValues);
}
