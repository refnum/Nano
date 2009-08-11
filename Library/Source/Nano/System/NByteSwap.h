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
	kNEndianBig,
	kNEndianLittle
} NEndianFormat;

static const NEndianFormat kNEndianNative = (NN_TARGET_ENDIAN_BIG ? kNEndianBig : kNEndianLittle);





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
//		Swap and return
//----------------------------------------------------------------------------
// Always swap
inline UInt16 SwapUInt16(UInt16 theValue)					{ NByteSwap::SwapInt16(&theValue);				return(theValue); }
inline UInt32 SwapUInt32(UInt32 theValue)					{ NByteSwap::SwapInt32(&theValue);				return(theValue); }
inline UInt64 SwapUInt64(UInt64 theValue)					{ NByteSwap::SwapInt64(&theValue);				return(theValue); }

inline SInt16 SwapSInt16(SInt16 theValue)					{ NByteSwap::SwapInt16((UInt16 *) &theValue);	return(theValue); }
inline SInt32 SwapSInt32(SInt32 theValue)					{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
inline SInt64 SwapSInt64(SInt64 theValue)					{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }

inline Float32 SwapFloat32(Float32 theValue)				{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
inline Float64 SwapFloat64(Float64 theValue)				{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }



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
	inline UInt16 SwapUInt16_NtoB(UInt16 theValue)			{ return(SwapUInt16(theValue)); }
	inline UInt32 SwapUInt32_NtoB(UInt32 theValue)			{ return(SwapUInt32(theValue)); }
	inline UInt64 SwapUInt64_NtoB(UInt64 theValue)			{ return(SwapUInt64(theValue)); }

	inline SInt16 SwapSInt16_NtoB(SInt16 theValue)			{ return(SwapSInt16(theValue)); }
	inline SInt32 SwapSInt32_NtoB(SInt32 theValue)			{ return(SwapSInt32(theValue)); }
	inline SInt64 SwapSInt64_NtoB(SInt64 theValue)			{ return(SwapSInt64(theValue)); }

	inline Float32 SwapFloat32_NtoB(Float32 theValue)		{ return(SwapFloat32(theValue)); }
	inline Float64 SwapFloat64_NtoB(Float64 theValue)		{ return(SwapFloat64(theValue)); }
#endif



// Native to little
#if TARGET_RT_BIG_ENDIAN
	inline UInt16 SwapUInt16_NtoL(UInt16 theValue)			{ return(SwapUInt16(theValue));  }
	inline UInt32 SwapUInt32_NtoL(UInt32 theValue)			{ return(SwapUInt32(theValue)); }
	inline UInt64 SwapUInt64_NtoL(UInt64 theValue)			{ return(SwapUInt64(theValue)); }

	inline SInt16 SwapSInt16_NtoL(SInt16 theValue)			{ return(SwapSInt16(theValue)); }
	inline SInt32 SwapSInt32_NtoL(SInt32 theValue)			{ return(SwapSInt32(theValue)); }
	inline SInt64 SwapSInt64_NtoL(SInt64 theValue)			{ return(SwapSInt64(theValue)); }

	inline Float32 SwapFloat32_NtoL(Float32 theValue)		{ return(SwapFloat32(theValue)); }
	inline Float64 SwapFloat64_NtoL(Float64 theValue)		{ return(SwapFloat64(theValue)); }
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
	inline UInt16 SwapUInt16_BtoN(UInt16 theValue)			{ return(SwapUInt16(theValue)); }
	inline UInt32 SwapUInt32_BtoN(UInt32 theValue)			{ return(SwapUInt32(theValue)); }
	inline UInt64 SwapUInt64_BtoN(UInt64 theValue)			{ return(SwapUInt64(theValue)); }

	inline SInt16 SwapSInt16_BtoN(SInt16 theValue)			{ return(SwapSInt16(theValue)); }
	inline SInt32 SwapSInt32_BtoN(SInt32 theValue)			{ return(SwapSInt32(theValue)); }
	inline SInt64 SwapSInt64_BtoN(SInt64 theValue)			{ return(SwapSInt64(theValue)); }

	inline Float32 SwapFloat32_BtoN(Float32 theValue)		{ return(SwapFloat32(theValue)); }
	inline Float64 SwapFloat64_BtoN(Float64 theValue)		{ return(SwapFloat64(theValue)); }
#endif



// Little to native
#if TARGET_RT_BIG_ENDIAN
	inline UInt16 SwapUInt16_LtoN(UInt16 theValue)			{ return(SwapUInt16(theValue)); }
	inline UInt32 SwapUInt32_LtoN(UInt32 theValue)			{ return(SwapUInt32(theValue)); }
	inline UInt64 SwapUInt64_LtoN(UInt64 theValue)			{ return(SwapUInt64(theValue)); }

	inline SInt16 SwapSInt16_LtoN(SInt16 theValue)			{ return(SwapSInt16(theValue)); }
	inline SInt32 SwapSInt32_LtoN(SInt32 theValue)			{ return(SwapSInt32(theValue)); }
	inline SInt64 SwapSInt64_LtoN(SInt64 theValue)			{ return(SwapSInt32(theValue)); }

	inline Float32 SwapFloat32_LtoN(Float32 theValue)		{ return(SwapFloat32(theValue)); }
	inline Float64 SwapFloat64_LtoN(Float64 theValue)		{ return(SwapFloat64(theValue)); }
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





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
//		Swap in place
//----------------------------------------------------------------------------
// Always swap
inline void SwapUInt16(UInt16 *theValue)					{ *theValue = SwapUInt16(*theValue); }
inline void SwapUInt32(UInt32 *theValue)					{ *theValue = SwapUInt32(*theValue); }
inline void SwapUInt64(UInt64 *theValue)					{ *theValue = SwapUInt64(*theValue); }

inline void SwapSInt16(SInt16 *theValue)					{ *theValue = SwapSInt16(*theValue); }
inline void SwapSInt32(SInt32 *theValue)					{ *theValue = SwapSInt32(*theValue); }
inline void SwapSInt64(SInt64 *theValue)					{ *theValue = SwapSInt64(*theValue); }

inline void SwapFloat32(Float32 *theValue)					{ *theValue = SwapFloat32(*theValue); }
inline void SwapFloat64(Float64 *theValue)					{ *theValue = SwapFloat64(*theValue); }



// Native to big
inline void SwapUInt16_NtoB(UInt16 *theValue)				{ *theValue = SwapUInt16_NtoB(*theValue); }
inline void SwapUInt32_NtoB(UInt32 *theValue)				{ *theValue = SwapUInt32_NtoB(*theValue); }
inline void SwapUInt64_NtoB(UInt64 *theValue)				{ *theValue = SwapUInt64_NtoB(*theValue); }

inline void SwapSInt16_NtoB(SInt16 *theValue)				{ *theValue = SwapSInt16_NtoB(*theValue); }
inline void SwapSInt32_NtoB(SInt32 *theValue)				{ *theValue = SwapSInt32_NtoB(*theValue); }
inline void SwapSInt64_NtoB(SInt64 *theValue)				{ *theValue = SwapSInt64_NtoB(*theValue); }

inline void SwapFloat32_NtoB(Float32 *theValue)				{ *theValue = SwapFloat32_NtoB(*theValue); }
inline void SwapFloat64_NtoB(Float64 *theValue)				{ *theValue = SwapFloat64_NtoB(*theValue); }



// Native to little
inline void SwapUInt16_NtoL(UInt16 *theValue)				{ *theValue = SwapUInt16_NtoL(*theValue); }
inline void SwapUInt32_NtoL(UInt32 *theValue)				{ *theValue = SwapUInt32_NtoL(*theValue); }
inline void SwapUInt64_NtoL(UInt64 *theValue)				{ *theValue = SwapUInt64_NtoL(*theValue); }

inline void SwapSInt16_NtoL(SInt16 *theValue)				{ *theValue = SwapSInt16_NtoL(*theValue); }
inline void SwapSInt32_NtoL(SInt32 *theValue)				{ *theValue = SwapSInt32_NtoL(*theValue); }
inline void SwapSInt64_NtoL(SInt64 *theValue)				{ *theValue = SwapSInt64_NtoL(*theValue); }

inline void SwapFloat32_NtoL(Float32 *theValue)				{ *theValue = SwapFloat32_NtoL(*theValue); }
inline void SwapFloat64_NtoL(Float64 *theValue)				{ *theValue = SwapFloat64_NtoL(*theValue); }



// Big to native
inline void SwapUInt16_BtoN(UInt16 *theValue)				{ *theValue = SwapUInt16_BtoN(*theValue); }
inline void SwapUInt32_BtoN(UInt32 *theValue)				{ *theValue = SwapUInt32_BtoN(*theValue); }
inline void SwapUInt64_BtoN(UInt64 *theValue)				{ *theValue = SwapUInt64_BtoN(*theValue); }

inline void SwapSInt16_BtoN(SInt16 *theValue)				{ *theValue = SwapSInt16_BtoN(*theValue); }
inline void SwapSInt32_BtoN(SInt32 *theValue)				{ *theValue = SwapSInt32_BtoN(*theValue); }
inline void SwapSInt64_BtoN(SInt64 *theValue)				{ *theValue = SwapSInt64_BtoN(*theValue); }

inline void SwapFloat32_BtoN(Float32 *theValue)				{ *theValue = SwapFloat32_BtoN(*theValue); }
inline void SwapFloat64_BtoN(Float64 *theValue)				{ *theValue = SwapFloat64_BtoN(*theValue); }



// Little to native
inline void SwapUInt16_LtoN(UInt16 *theValue)				{ *theValue = SwapUInt16_LtoN(*theValue); }
inline void SwapUInt32_LtoN(UInt32 *theValue)				{ *theValue = SwapUInt32_LtoN(*theValue); }
inline void SwapUInt64_LtoN(UInt64 *theValue)				{ *theValue = SwapUInt64_LtoN(*theValue); }

inline void SwapSInt16_LtoN(SInt16 *theValue)				{ *theValue = SwapSInt16_LtoN(*theValue); }
inline void SwapSInt32_LtoN(SInt32 *theValue)				{ *theValue = SwapSInt32_LtoN(*theValue); }
inline void SwapSInt64_LtoN(SInt64 *theValue)				{ *theValue = SwapSInt64_LtoN(*theValue); }

inline void SwapFloat32_LtoN(Float32 *theValue)				{ *theValue = SwapFloat32_LtoN(*theValue); }
inline void SwapFloat64_LtoN(Float64 *theValue)				{ *theValue = SwapFloat64_LtoN(*theValue); }






#endif // NBYTESWAP_HDR


