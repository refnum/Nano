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
#if NN_TARGET_ENDIAN_BIG
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
#if NN_TARGET_ENDIAN_BIG
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
#if NN_TARGET_ENDIAN_BIG
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
#if NN_TARGET_ENDIAN_BIG
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





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Swap structures
//
// A structure swap can be declared with:
//
//		typedef struct {
//			UInt16		numLoops;
//			UInt16		numPoints;
//			UInt32		sizeLoops;
//			UInt32		sizePoints;
//		} MyStructure;
//
//		NN_BYTESWAP_BEGIN(MyStructure)
//			NN_BYTESWAP_L_UInt16	(numLoops)
//			NN_BYTESWAP_L_UInt16	(numPoints)
//			NN_BYTESWAP_B_UInt32	(sizeLoops)
//			NN_BYTESWAP_B_UInt32	(sizePoints)
//		NN_BYTESWAP_END
//
// The NN_BYTESWAP_xxx macros declare a function capable of swapping a MyStructure
// from its in-memory representation (using native endian-ness) to its on-disk
// representation (where the first two fields are little-endian, and the last
// two are big-endian).
//
// A structure can be encoded to/from the on-disk representation with:
//
//		MyStructure		theValue;
//
//		// After reading from disk, swap to native
//		NN_BYTESWAP_DECODE(1, MyStructure, &theValue);
//
//
//		// Manipulate the structure
//		theValue.numLoops  += 1;
//		theVlaue.sizeLoops += 2;
//
//
//		// Encode for saving to disk
//		NN_BYTESWAP_ENCODE(1, MyStructure, &theValue);
//
#define NN_BYTESWAP_BEGIN(_type)															\
	static void NSwap_ ## _type(NIndex _numItems, _type *_theValue, bool _toNative)			\
	{	_type		*_valuePtr;																\
		NIndex		_n;																		\
																							\
		for (_n = 0; _n < _numItems; _n++)													\
			{																				\
			_valuePtr = &_theValue[_n];

#define NN_BYTESWAP_END																		\
			}																				\
	}

#define NN_BYTESWAP_ENCODE(_numItems, _type, _ptr)											\
	NSwap_ ## _type(_numItems, _ptr, false)													\

#define NN_BYTESWAP_DECODE(_numItems, _type, _ptr)											\
	NSwap_ ## _type(_numItems, _ptr, true)													\



// Big endian
#define NN_BYTESWAP_B_UInt8(_field)															\
	if (_toNative)																			\
		;																					\
	else																					\
		;

#define NN_BYTESWAP_B_UInt16(_field)														\
	if (_toNative)																			\
		SwapUInt16_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapUInt16_NtoL(&_valuePtr->_field);

#define NN_BYTESWAP_B_UInt32(_field)														\
	if (_toNative)																			\
		SwapUInt32_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapUInt32_NtoL(&_valuePtr->_field);

#define NN_BYTESWAP_B_UInt64(_field)														\
	if (_toNative)																			\
		SwapUInt64_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapUInt64_NtoL(&_valuePtr->_field);



#define NN_BYTESWAP_B_SInt8(_field)															\
	if (_toNative)																			\
		;																					\
	else																					\
		;

#define NN_BYTESWAP_B_SInt16(_field)														\
	if (_toNative)																			\
		SwapSInt16_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapSInt16_NtoL(&_valuePtr->_field);

#define NN_BYTESWAP_B_SInt32(_field)														\
	if (_toNative)																			\
		SwapSInt32_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapSInt32_NtoL(&_valuePtr->_field);

#define NN_BYTESWAP_B_SInt64(_field)														\
	if (_toNative)																			\
		SwapSInt64_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapSInt64_NtoL(&_valuePtr->_field);



#define NN_BYTESWAP_B_Float32(_field)														\
	if (_toNative)																			\
		SwapFloat32_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapFloat32_NtoL(&_valuePtr->_field);

#define NN_BYTESWAP_B_Float64(_field)														\
	if (_toNative)																			\
		SwapFloat64_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapFloat64_NtoL(&_valuePtr->_field);



// Little endian
#define NN_BYTESWAP_L_UInt8(_field)															\
	if (_toNative)																			\
		;																					\
	else																					\
		;

#define NN_BYTESWAP_L_UInt16(_field)														\
	if (_toNative)																			\
		SwapUInt16_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapUInt16_NtoL(&_valuePtr->_field);

#define NN_BYTESWAP_L_UInt32(_field)														\
	if (_toNative)																			\
		SwapUInt32_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapUInt32_NtoL(&_valuePtr->_field);

#define NN_BYTESWAP_L_UInt64(_field)														\
	if (_toNative)																			\
		SwapUInt64_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapUInt64_NtoL(&_valuePtr->_field);



#define NN_BYTESWAP_L_SInt8(_field)															\
	if (_toNative)																			\
		;																					\
	else																					\
		;

#define NN_BYTESWAP_L_SInt16(_field)														\
	if (_toNative)																			\
		SwapSInt16_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapSInt16_NtoL(&_valuePtr->_field);

#define NN_BYTESWAP_L_SInt32(_field)														\
	if (_toNative)																			\
		SwapSInt32_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapSInt32_NtoL(&_valuePtr->_field);

#define NN_BYTESWAP_L_SInt64(_field)														\
	if (_toNative)																			\
		SwapSInt64_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapSInt64_NtoL(&_valuePtr->_field);



#define NN_BYTESWAP_L_Float32(_field)														\
	if (_toNative)																			\
		SwapFloat32_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapFloat32_NtoL(&_valuePtr->_field);

#define NN_BYTESWAP_L_Float64(_field)														\
	if (_toNative)																			\
		SwapFloat64_LtoN(&_valuePtr->_field);												\
	else																					\
		SwapFloat64_NtoL(&_valuePtr->_field);




#endif // NBYTESWAP_HDR


