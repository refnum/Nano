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
//		Constants
//----------------------------------------------------------------------------
// Endian
typedef enum {
	kEndianBig,
	kEndianLittle
} EndianFormat;

static const EndianFormat kEndianNative							= (NN_TARGET_ENDIAN_BIG ? kEndianBig : kEndianLittle);





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Get the size of an array
#define GET_ARRAY_SIZE(_a)							(sizeof((_a)) / sizeof((_a)[0]))


// Clamp a value
#define GET_CLAMPED_VALUE(_v, _min, _max)			std::min((_max), std::max((_min), (_v)))





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSystemUtilities {
public:


};






#endif // NSYSTEMUTILITIES_HDR



