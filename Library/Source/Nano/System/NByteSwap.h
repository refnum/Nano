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
	// Swap primitives
	static void							SwapInt16(UInt16 *theValue);
	static void							SwapInt32(UInt32 *theValue);
	static void							SwapInt64(UInt64 *theValue);
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Native to big
#if TARGET_RT_BIG_ENDIAN
	inline UInt16 SwapUInt16_NtoB(UInt16 theValue)			{ return(theValue); }
	inline UInt32 SwapUInt32_NtoB(UInt32 theValue)			{ return(theValue); }
	inline UInt64 SwapUInt64_NtoB(UInt64 theValue)			{ return(theValue); }

	inline SInt16 SwapSInt16_NtoB(SInt16 theValue)			{ return(theValue); }
	inline SInt32 SwapSInt32_NtoB(SInt32 theValue)			{ return(theValue); }
	inline SInt64 SwapSInt64_NtoB(SInt64 theValue)			{ return(theValue); }

	inline Float32 SwapFloat32_NtoB(Float32 theValue)		{ return(theValue); }
	inline Float64 SwapFloat64_NtoB(Float64 theValue)		{ return(theValue); }
#else
	inline UInt16 SwapUInt16_NtoB(UInt16 theValue)			{ NByteSwap::SwapInt16(&theValue);				return(theValue); }
	inline UInt32 SwapUInt32_NtoB(UInt32 theValue)			{ NByteSwap::SwapInt32(&theValue);				return(theValue); }
	inline UInt64 SwapUInt64_NtoB(UInt64 theValue)			{ NByteSwap::SwapInt64(&theValue);				return(theValue); }

	inline SInt16 SwapSInt16_NtoB(SInt16 theValue)			{ NByteSwap::SwapInt16((UInt16 *) &theValue);	return(theValue); }
	inline SInt32 SwapSInt32_NtoB(SInt32 theValue)			{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
	inline SInt64 SwapSInt64_NtoB(SInt64 theValue)			{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }

	inline Float32 SwapFloat32_NtoB(Float32 theValue)		{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
	inline Float64 SwapFloat64_NtoB(Float64 theValue)		{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }
#endif



// Native to little
#if TARGET_RT_BIG_ENDIAN
	inline UInt16 SwapUInt16_NtoL(UInt16 theValue)			{ NByteSwap::SwapInt16(&theValue);				return(theValue); }
	inline UInt32 SwapUInt32_NtoL(UInt32 theValue)			{ NByteSwap::SwapInt32(&theValue);				return(theValue); }
	inline UInt64 SwapUInt64_NtoL(UInt64 theValue)			{ NByteSwap::SwapInt64(&theValue);				return(theValue); }

	inline SInt16 SwapSInt16_NtoL(SInt16 theValue)			{ NByteSwap::SwapInt16((UInt16 *) &theValue);	return(theValue); }
	inline SInt32 SwapSInt32_NtoL(SInt32 theValue)			{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
	inline SInt64 SwapSInt64_NtoL(SInt64 theValue)			{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }

	inline Float32 SwapFloat32_NtoL(Float32 theValue)		{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
	inline Float64 SwapFloat64_NtoL(Float64 theValue)		{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }
#else
	inline UInt16 SwapUInt16_NtoL(UInt16 theValue)			{ return(theValue); }
	inline UInt32 SwapUInt32_NtoL(UInt32 theValue)			{ return(theValue); }
	inline UInt64 SwapUInt64_NtoL(UInt64 theValue)			{ return(theValue); }

	inline SInt16 SwapSInt16_NtoL(SInt16 theValue)			{ return(theValue); }
	inline SInt32 SwapSInt32_NtoL(SInt32 theValue)			{ return(theValue); }
	inline SInt64 SwapSInt64_NtoL(SInt64 theValue)			{ return(theValue); }

	inline Float32 SwapFloat32_NtoL(Float32 theValue)		{ return(theValue); }
	inline Float64 SwapFloat64_NtoL(Float64 theValue)		{ return(theValue); }
#endif



// Big to native
#if TARGET_RT_BIG_ENDIAN
	inline UInt16 SwapUInt16_BtoN(UInt16 theValue)			{ return(theValue); }
	inline UInt32 SwapUInt32_BtoN(UInt32 theValue)			{ return(theValue); }
	inline UInt64 SwapUInt64_BtoN(UInt64 theValue)			{ return(theValue); }

	inline SInt16 SwapSInt16_BtoN(SInt16 theValue)			{ return(theValue); }
	inline SInt32 SwapSInt32_BtoN(SInt32 theValue)			{ return(theValue); }
	inline SInt64 SwapSInt64_BtoN(SInt64 theValue)			{ return(theValue); }

	inline Float32 SwapFloat32_BtoN(Float32 theValue)		{ return(theValue); }
	inline Float64 SwapFloat64_BtoN(Float64 theValue)		{ return(theValue); }
#else
	inline UInt16 SwapUInt16_BtoN(UInt16 theValue)			{ NByteSwap::SwapInt16(&theValue);				return(theValue); }
	inline UInt32 SwapUInt32_BtoN(UInt32 theValue)			{ NByteSwap::SwapInt32(&theValue);				return(theValue); }
	inline UInt64 SwapUInt64_BtoN(UInt64 theValue)			{ NByteSwap::SwapInt64(&theValue);				return(theValue); }

	inline SInt16 SwapSInt16_BtoN(SInt16 theValue)			{ NByteSwap::SwapInt16((UInt16 *) &theValue);	return(theValue); }
	inline SInt32 SwapSInt32_BtoN(SInt32 theValue)			{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
	inline SInt64 SwapSInt64_BtoN(SInt64 theValue)			{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }

	inline Float32 SwapFloat32_BtoN(Float32 theValue)		{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
	inline Float64 SwapFloat64_BtoN(Float64 theValue)		{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }
#endif



// Little to native
#if TARGET_RT_BIG_ENDIAN
	inline UInt16 SwapUInt16_LtoN(UInt16 theValue)			{ NByteSwap::SwapInt16(&theValue);				return(theValue); }
	inline UInt32 SwapUInt32_LtoN(UInt32 theValue)			{ NByteSwap::SwapInt32(&theValue);				return(theValue); }
	inline UInt64 SwapUInt64_LtoN(UInt64 theValue)			{ NByteSwap::SwapInt64(&theValue);				return(theValue); }

	inline SInt16 SwapSInt16_LtoN(SInt16 theValue)			{ NByteSwap::SwapInt16((UInt16 *) &theValue);	return(theValue); }
	inline SInt32 SwapSInt32_LtoN(SInt32 theValue)			{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
	inline SInt64 SwapSInt64_LtoN(SInt64 theValue)			{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }

	inline Float32 SwapFloat32_LtoN(Float32 theValue)		{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
	inline Float64 SwapFloat64_LtoN(Float64 theValue)		{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }
#else
	inline UInt16 SwapUInt16_LtoN(UInt16 theValue)			{ return(theValue); }
	inline UInt32 SwapUInt32_LtoN(UInt32 theValue)			{ return(theValue); }
	inline UInt64 SwapUInt64_LtoN(UInt64 theValue)			{ return(theValue); }

	inline SInt16 SwapSInt16_LtoN(SInt16 theValue)			{ return(theValue); }
	inline SInt32 SwapSInt32_LtoN(SInt32 theValue)			{ return(theValue); }
	inline SInt64 SwapSInt64_LtoN(SInt64 theValue)			{ return(theValue); }

	inline Float32 SwapFloat32_LtoN(Float32 theValue)		{ return(theValue); }
	inline Float64 SwapFloat64_LtoN(Float64 theValue)		{ return(theValue); }
#endif





#endif // NBYTESWAP_HDR


