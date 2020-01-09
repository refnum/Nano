/*	NAME:
		NByteSwap.inl

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
#include "NanoMacros.h"





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template <typename T>
T NByteSwap::Swap(T theValue)
{
	Swap(1, &theValue);
	return theValue;
}





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template <typename T>
void NByteSwap::Swap(T* theValue)
{
	Swap(1, theValue);
}





//=============================================================================
//		NByteSwap::Swap : Always swap.
//-----------------------------------------------------------------------------
template <typename T>
void NByteSwap::Swap(size_t numValues, T* theValues)
{
	SwapN(numValues, sizeof(T), theValues);
}





//=============================================================================
//		NByteSwap::SwapNtoB : Swap native to big-endian.
//-----------------------------------------------------------------------------
template <typename T>
T NByteSwap::SwapNtoB(T theValue)
{
	return SwapXtoY(NEndian::Native, NEndian::Big, theValue);
}





//=============================================================================
//		NByteSwap::SwapNtoB : Swap native to big-endian.
//-----------------------------------------------------------------------------
template <typename T>
void NByteSwap::SwapNtoB(T* theValue)
{
	return SwapXtoY(NEndian::Native, NEndian::Big, theValue);
}





//=============================================================================
//		NByteSwap::SwapNtoB : Swap native to big-endian.
//-----------------------------------------------------------------------------
template <typename T>
void NByteSwap::SwapNtoB(size_t numValues, T* theValues)
{
	return SwapXtoY(NEndian::Native, NEndian::Big, numValues, theValues);
}





//=============================================================================
//		NByteSwap::SwapBtoN : Swap big-endian to native.
//-----------------------------------------------------------------------------
template <typename T>
T NByteSwap::SwapBtoN(T theValue)
{
	return SwapXtoY(NEndian::Big, NEndian::Native, theValue);
}





//=============================================================================
//		NByteSwap::SwapBtoN : Swap big-endian to native.
//-----------------------------------------------------------------------------
template <typename T>
void NByteSwap::SwapBtoN(T* theValue)
{
	return SwapXtoY(NEndian::Big, NEndian::Native, theValue);
}





//=============================================================================
//		NByteSwap::SwapBtoN : Swap big-endian to native.
//-----------------------------------------------------------------------------
template <typename T>
void NByteSwap::SwapBtoN(size_t numValues, T* theValues)
{
	return SwapXtoY(NEndian::Big, NEndian::Native, numValues, theValues);
}





//=============================================================================
//		NByteSwap::SwapNtoL : Swap native to little-endian.
//-----------------------------------------------------------------------------
template <typename T>
T NByteSwap::SwapNtoL(T theValue)
{
	return SwapXtoY(NEndian::Native, NEndian::Little, theValue);
}





//=============================================================================
//		NByteSwap::SwapNtoL : Swap native to little-endian.
//-----------------------------------------------------------------------------
template <typename T>
void NByteSwap::SwapNtoL(T* theValue)
{
	return SwapXtoY(NEndian::Native, NEndian::Little, theValue);
}





//=============================================================================
//		NByteSwap::SwapNtoL : Swap native to little-endian.
//-----------------------------------------------------------------------------
template <typename T>
void NByteSwap::SwapNtoL(size_t numValues, T* theValues)
{
	return SwapXtoY(NEndian::Native, NEndian::Little, numValues, theValues);
}





//=============================================================================
//		NByteSwap::SwapLtoN : Swap little-endian to native.
//-----------------------------------------------------------------------------
template <typename T>
T NByteSwap::SwapLtoN(T theValue)
{
	return SwapXtoY(NEndian::Little, NEndian::Native, theValue);
}





//=============================================================================
//		NByteSwap::SwapLtoN : Swap little-endian to native.
//-----------------------------------------------------------------------------
template <typename T>
void NByteSwap::SwapLtoN(T* theValue)
{
	return SwapXtoY(NEndian::Little, NEndian::Native, theValue);
}





//=============================================================================
//		NByteSwap::SwapLtoN : Swap little-endian to native.
//-----------------------------------------------------------------------------
template <typename T>
void NByteSwap::SwapLtoN(size_t numValues, T* theValues)
{
	return SwapXtoY(NEndian::Little, NEndian::Native, numValues, theValues);
}





#pragma mark private
//=============================================================================
//		NByteSwap::SwapXtoY : Swap X-endian to Y-endian.
//-----------------------------------------------------------------------------
template <typename T>
T NByteSwap::SwapXtoY(NEndian orderX, NEndian orderY, T theValue)
{
	if (orderX != orderY)
	{
		Swap(theValue);
	}

	return theValue;
}





//=============================================================================
//		NByteSwap::SwapXtoY : Swap X-endian to Y-endian.
//-----------------------------------------------------------------------------
template <typename T>
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
template <typename T>
void NByteSwap::SwapXtoY(NEndian orderX, NEndian orderY, size_t numValues, T* theValues)
{
	if (orderX != orderY)
	{
		Swap(numValues, sizeof(T), theValues);
	}
}





//=============================================================================
//		NByteSwap::Swap16 : Swap a uint16_t.
//-----------------------------------------------------------------------------
void NByteSwap::Swap16(uint16_t* theValue)
{


	// Swap the value
	*theValue = uint16_t((*theValue & 0xFF00) >> 8) | uint16_t((*theValue & 0x00FF) << 8);
}





//=============================================================================
//		NByteSwap::Swap32 : Swap a uint32_t.
//-----------------------------------------------------------------------------
void NByteSwap::Swap32(uint32_t* theValue)
{


	// Swap the value
	*theValue = uint32_t((*theValue & 0xFF000000) >> 24) | uint32_t((*theValue & 0x00FF0000) >> 8) |
				uint32_t((*theValue & 0x0000FF00) << 8) | uint32_t((*theValue & 0x000000FF) << 24);
}





//=============================================================================
//		NByteSwap::Swap64 : Swap a uint64_t.
//-----------------------------------------------------------------------------
void NByteSwap::Swap64(uint64_t* theValue)
{


	// Swap the value
	*theValue = uint64_t((*theValue & 0xFF00000000000000ULL) >> 56) |
				uint64_t((*theValue & 0x00FF000000000000ULL) >> 40) |
				uint64_t((*theValue & 0x0000FF0000000000ULL) >> 24) |
				uint64_t((*theValue & 0x000000FF00000000ULL) >> 8) |
				uint64_t((*theValue & 0x00000000FF000000ULL) << 8) |
				uint64_t((*theValue & 0x0000000000FF0000ULL) << 24) |
				uint64_t((*theValue & 0x000000000000FF00ULL) << 40) |
				uint64_t((*theValue & 0x00000000000000FFULL) << 56);
}
