/*	NAME:
		NSystemUtilities.h

	DESCRIPTION:
		System utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSYSTEMUTILITIES_HDR
#define NSYSTEMUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Get the size of an array
#define GET_ARRAY_SIZE(_a)											((NIndex) (sizeof((_a)) / sizeof((_a)[0])))


// Clamp a value
#define GET_CLAMPED_VALUE(_v, _min, _max)							std::min((_max), std::max((_min), (_v)))


// Declare a binary constant
//
//		 B8(01010101)									= 85
//		B16(10101010, 01010101)							= 43605
//		B32(10000000, 11111111, 10101010, 01010101)		= 2164238933
//
// Original implementation by Tom Torfs.
#define _HEX(_n)		0x ## _n ## LU

#define _B8(_x)			(UInt8) (((_x & 0x0000000FLU) ?   1 : 0) | \
								 ((_x & 0x000000F0LU) ?   2 : 0) | \
								 ((_x & 0x00000F00LU) ?   4 : 0) | \
								 ((_x & 0x0000F000LU) ?   8 : 0) | \
								 ((_x & 0x000F0000LU) ?  16 : 0) | \
								 ((_x & 0x00F00000LU) ?  32 : 0) | \
								 ((_x & 0x0F000000LU) ?  64 : 0) | \
								 ((_x & 0xF0000000LU) ? 128 : 0))

#define B8(_byte1)												(	(UInt8) _B8(_HEX(_byte1)) )

#define B16(_byte1, _byte2)										(	(((UInt16) B8(_byte1)) <<  8) | \
																	(((UInt16) B8(_byte2)) <<  0) )

#define B32(_byte1, _byte2, _byte3, _byte4)						(	(((UInt32) B8(_byte1)) << 24) | \
																	(((UInt32) B8(_byte2)) << 16) | \
																	(((UInt32) B8(_byte3)) <<  8) | \
																	(((UInt32) B8(_byte4)) <<  0) )





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSystemUtilities {
public:


};






#endif // NSYSTEMUTILITIES_HDR



