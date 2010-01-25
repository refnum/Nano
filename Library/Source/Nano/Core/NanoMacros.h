/*	NAME:
		NanoMacros.h

	DESCRIPTION:
		Nano macros.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_MACROS_HDR
#define NANO_MACROS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Mark unused parameters
#define NN_UNUSED(_x)												(void) _x


// Get the size of an array
#define NN_ARRAY_SIZE(_a)											((NIndex) (sizeof((_a)) / sizeof((_a)[0])))


// Get a clamped value
#define NN_CLAMP_VALUE(_v, _min, _max)								std::min((_max), std::max((_min), (_v)))


// Is an address aligned?
#define NN_ALIGNED_TO(_value, _size)								((((UIntPtr) _value) % _size) == 0)


// Declare a binary constant
//
//		 B8(01010101)									= 85
//		B16(10101010, 01010101)							= 43605
//		B32(10000000, 11111111, 10101010, 01010101)		= 2164238933
//
// Based on a PD implementation by Tom Torfs ("Binary constant macros").
#define _NN_HEX(_n)	0x ## _n ## LU

#define _NN_B8(_x)		(UInt8) (((_x & 0x0000000FLU) ?   1 : 0) | \
								 ((_x & 0x000000F0LU) ?   2 : 0) | \
								 ((_x & 0x00000F00LU) ?   4 : 0) | \
								 ((_x & 0x0000F000LU) ?   8 : 0) | \
								 ((_x & 0x000F0000LU) ?  16 : 0) | \
								 ((_x & 0x00F00000LU) ?  32 : 0) | \
								 ((_x & 0x0F000000LU) ?  64 : 0) | \
								 ((_x & 0xF0000000LU) ? 128 : 0))

#define NN_B8(_byte1)											(	(UInt8) _NN_B8(_NN_HEX(_byte1)) )

#define NN_B16(_byte1, _byte2)									(	(((UInt16) NN_B8(_byte1)) <<  8) | \
																	(((UInt16) NN_B8(_byte2)) <<  0) )

#define NN_B32(_byte1, _byte2, _byte3, _byte4)					(	(((UInt32) NN_B8(_byte1)) << 24) | \
																	(((UInt32) NN_B8(_byte2)) << 16) | \
																	(((UInt32) NN_B8(_byte3)) <<  8) | \
																	(((UInt32) NN_B8(_byte4)) <<  0) )

	
	
	
#endif // NANO_MACROS_HDR



