/*	NAME:
		NByteSwap.cpp

	DESCRIPTION:
		Nano byte-swap support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NByteSwap.h"





//============================================================================
//		NByteSwap::SwapInt16 : Swap a 16-bit value.
//----------------------------------------------------------------------------
void NByteSwap::SwapInt16(UInt16 *theValue)
{


	// Swap the value
	*theValue =	(((UInt16) ((*theValue) & 0xFF00)) >> 8) |
				(((UInt16) ((*theValue) & 0x00FF)) << 8);
}





//============================================================================
//		NByteSwap::SwapInt32 : Swap a 32-bit value.
//----------------------------------------------------------------------------
void NByteSwap::SwapInt32(UInt32 *theValue)
{


	// Swap the value
	*theValue =	(((UInt32) ((*theValue) & 0xFF000000)) >> 24) |
				(((UInt32) ((*theValue) & 0x00FF0000)) >>  8) |
				(((UInt32) ((*theValue) & 0x0000FF00)) <<  8) |
				(((UInt32) ((*theValue) & 0x000000FF)) << 24);
}





//============================================================================
//		NByteSwap::SwapInt64 : Swap a 64-bit value.
//----------------------------------------------------------------------------
void NByteSwap::SwapInt64(UInt64 *theValue)
{


	// Swap the value
	*theValue =	(((UInt64) ((*theValue) & 0xFF00000000000000ULL)) >> 56) |
				(((UInt64) ((*theValue) & 0x00FF000000000000ULL)) >> 40) |
				(((UInt64) ((*theValue) & 0x0000FF0000000000ULL)) >> 24) |
				(((UInt64) ((*theValue) & 0x000000FF00000000ULL)) >>  8) |
				(((UInt64) ((*theValue) & 0x00000000FF000000ULL)) <<  8) |
				(((UInt64) ((*theValue) & 0x0000000000FF0000ULL)) << 24) |
				(((UInt64) ((*theValue) & 0x000000000000FF00ULL)) << 40) |
				(((UInt64) ((*theValue) & 0x00000000000000FFULL)) << 56);
}

