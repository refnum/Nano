/*	NAME:
		NByteSwap.cpp

	DESCRIPTION:
		Byte-swap support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NByteSwap.h"





//============================================================================
//		NByteSwap::Swap16 : Swap a 16-bit value.
//----------------------------------------------------------------------------
void NByteSwap::Swap16(uint16_t *theValue)
{


	// Swap the value
	*theValue =	(uint16_t) ((*theValue & 0xFF00) >> 8) |
				(uint16_t) ((*theValue & 0x00FF) << 8);
}





//============================================================================
//		NByteSwap::Swap32 : Swap a 32-bit value.
//----------------------------------------------------------------------------
void NByteSwap::Swap32(uint32_t *theValue)
{


	// Swap the value
	*theValue =	(uint32_t) ((*theValue & 0xFF000000) >> 24) |
				(uint32_t) ((*theValue & 0x00FF0000) >>  8) |
				(uint32_t) ((*theValue & 0x0000FF00) <<  8) |
				(uint32_t) ((*theValue & 0x000000FF) << 24);
}





//============================================================================
//		NByteSwap::Swap64 : Swap a 64-bit value.
//----------------------------------------------------------------------------
void NByteSwap::Swap64(uint64_t *theValue)
{


	// Swap the value
	*theValue =	(uint64_t) ((*theValue & 0xFF00000000000000ULL) >> 56) |
				(uint64_t) ((*theValue & 0x00FF000000000000ULL) >> 40) |
				(uint64_t) ((*theValue & 0x0000FF0000000000ULL) >> 24) |
				(uint64_t) ((*theValue & 0x000000FF00000000ULL) >>  8) |
				(uint64_t) ((*theValue & 0x00000000FF000000ULL) <<  8) |
				(uint64_t) ((*theValue & 0x0000000000FF0000ULL) << 24) |
				(uint64_t) ((*theValue & 0x000000000000FF00ULL) << 40) |
				(uint64_t) ((*theValue & 0x00000000000000FFULL) << 56);
}





//============================================================================
//		NByteSwap::SwapBlock : Swap a block of values.
//----------------------------------------------------------------------------
void NByteSwap::SwapBlock(NIndex numValues, NIndex valueSize, void *valuePtr)
{	NIndex		n;



	// Swap the block
	switch (valueSize) {
		case 1:
			break;

		case 2:
			for (n = 0; n < numValues; n++)
				NByteSwap::Swap16(((uint16_t *) valuePtr) + n);
			break;

		case 4:
			for (n = 0; n < numValues; n++)
				NByteSwap::Swap32(((uint32_t *) valuePtr) + n);
			break;

		case 8:
			for (n = 0; n < numValues; n++)
				NByteSwap::Swap64(((uint64_t *) valuePtr) + n);
			break;
		
		default:
			NN_LOG("Unknown value size: %d", valueSize);
			break;
		}
}


