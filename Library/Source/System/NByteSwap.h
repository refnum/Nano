/*	NAME:
		NByteSwap.h

	DESCRIPTION:
		Nano byte-swap support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NBYTESWAP_HDR
#define NBYTESWAP_HDR
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

static const EndianFormat kEndianNative								= (NN_TARGET_ENDIAN_BIG ? kEndianBig : kEndianLittle);





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NByteSwap {
public:


};





#endif // NBYTESWAP_HDR


