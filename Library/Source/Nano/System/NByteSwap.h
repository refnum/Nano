/*	NAME:
		NByteSwap.h

	DESCRIPTION:
		Byte-swap support.
	
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
	static void							SwapBlock(NIndex numValues, NIndex valueSize, void *theValues);
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
//		Swap and return
//----------------------------------------------------------------------------
// Always swap
inline UInt16 NSwapUInt16(UInt16 theValue)						{ NByteSwap::SwapInt16(&theValue);				return(theValue); }
inline UInt32 NSwapUInt32(UInt32 theValue)						{ NByteSwap::SwapInt32(&theValue);				return(theValue); }
inline UInt64 NSwapUInt64(UInt64 theValue)						{ NByteSwap::SwapInt64(&theValue);				return(theValue); }

inline SInt16 NSwapSInt16(SInt16 theValue)						{ NByteSwap::SwapInt16((UInt16 *) &theValue);	return(theValue); }
inline SInt32 NSwapSInt32(SInt32 theValue)						{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
inline SInt64 NSwapSInt64(SInt64 theValue)						{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }

inline Float32 NSwapFloat32(Float32 theValue)					{ NByteSwap::SwapInt32((UInt32 *) &theValue);	return(theValue); }
inline Float64 NSwapFloat64(Float64 theValue)					{ NByteSwap::SwapInt64((UInt64 *) &theValue);	return(theValue); }



// Native to big
#if NN_TARGET_ENDIAN_BIG
	inline UInt16 NSwapUInt16_NtoB(UInt16 theValue)				{ return(theValue); }
	inline UInt32 NSwapUInt32_NtoB(UInt32 theValue)				{ return(theValue); }
	inline UInt64 NSwapUInt64_NtoB(UInt64 theValue)				{ return(theValue); }

	inline SInt16 NSwapSInt16_NtoB(SInt16 theValue)				{ return(theValue); }
	inline SInt32 NSwapSInt32_NtoB(SInt32 theValue)				{ return(theValue); }
	inline SInt64 NSwapSInt64_NtoB(SInt64 theValue)				{ return(theValue); }

	inline Float32 SwapFloat32_NtoB(Float32 theValue)			{ return(theValue); }
	inline Float64 SwapFloat64_NtoB(Float64 theValue)			{ return(theValue); }
#else
	inline UInt16 NSwapUInt16_NtoB(UInt16 theValue)				{ return(NSwapUInt16(theValue)); }
	inline UInt32 NSwapUInt32_NtoB(UInt32 theValue)				{ return(NSwapUInt32(theValue)); }
	inline UInt64 NSwapUInt64_NtoB(UInt64 theValue)				{ return(NSwapUInt64(theValue)); }

	inline SInt16 NSwapSInt16_NtoB(SInt16 theValue)				{ return(NSwapSInt16(theValue)); }
	inline SInt32 NSwapSInt32_NtoB(SInt32 theValue)				{ return(NSwapSInt32(theValue)); }
	inline SInt64 NSwapSInt64_NtoB(SInt64 theValue)				{ return(NSwapSInt64(theValue)); }

	inline Float32 NSwapFloat32_NtoB(Float32 theValue)			{ return(NSwapFloat32(theValue)); }
	inline Float64 NSwapFloat64_NtoB(Float64 theValue)			{ return(NSwapFloat64(theValue)); }
#endif



// Native to little
#if NN_TARGET_ENDIAN_BIG
	inline UInt16 NSwapUInt16_NtoL(UInt16 theValue)				{ return(NSwapUInt16(theValue)); }
	inline UInt32 NSwapUInt32_NtoL(UInt32 theValue)				{ return(NSwapUInt32(theValue)); }
	inline UInt64 NSwapUInt64_NtoL(UInt64 theValue)				{ return(NSwapUInt64(theValue)); }

	inline SInt16 NSwapSInt16_NtoL(SInt16 theValue)				{ return(NSwapSInt16(theValue)); }
	inline SInt32 NSwapSInt32_NtoL(SInt32 theValue)				{ return(NSwapSInt32(theValue)); }
	inline SInt64 NSwapSInt64_NtoL(SInt64 theValue)				{ return(NSwapSInt64(theValue)); }

	inline Float32 NSwapFloat32_NtoL(Float32 theValue)			{ return(NSwapFloat32(theValue)); }
	inline Float64 NSwapFloat64_NtoL(Float64 theValue)			{ return(NSwapFloat64(theValue)); }
#else
	inline UInt16 NSwapUInt16_NtoL(UInt16 theValue)				{ return(theValue); }
	inline UInt32 NSwapUInt32_NtoL(UInt32 theValue)				{ return(theValue); }
	inline UInt64 NSwapUInt64_NtoL(UInt64 theValue)				{ return(theValue); }

	inline SInt16 NSwapSInt16_NtoL(SInt16 theValue)				{ return(theValue); }
	inline SInt32 NSwapSInt32_NtoL(SInt32 theValue)				{ return(theValue); }
	inline SInt64 NSwapSInt64_NtoL(SInt64 theValue)				{ return(theValue); }

	inline Float32 NSwapFloat32_NtoL(Float32 theValue)			{ return(theValue); }
	inline Float64 NSwapFloat64_NtoL(Float64 theValue)			{ return(theValue); }
#endif



// Big to native
#if NN_TARGET_ENDIAN_BIG
	inline UInt16 NSwapUInt16_BtoN(UInt16 theValue)				{ return(theValue); }
	inline UInt32 NSwapUInt32_BtoN(UInt32 theValue)				{ return(theValue); }
	inline UInt64 NSwapUInt64_BtoN(UInt64 theValue)				{ return(theValue); }

	inline SInt16 NSwapSInt16_BtoN(SInt16 theValue)				{ return(theValue); }
	inline SInt32 NSwapSInt32_BtoN(SInt32 theValue)				{ return(theValue); }
	inline SInt64 NSwapSInt64_BtoN(SInt64 theValue)				{ return(theValue); }

	inline Float32 NSwapFloat32_BtoN(Float32 theValue)			{ return(theValue); }
	inline Float64 NSwapFloat64_BtoN(Float64 theValue)			{ return(theValue); }
#else
	inline UInt16 NSwapUInt16_BtoN(UInt16 theValue)				{ return(NSwapUInt16(theValue)); }
	inline UInt32 NSwapUInt32_BtoN(UInt32 theValue)				{ return(NSwapUInt32(theValue)); }
	inline UInt64 NSwapUInt64_BtoN(UInt64 theValue)				{ return(NSwapUInt64(theValue)); }

	inline SInt16 NSwapSInt16_BtoN(SInt16 theValue)				{ return(NSwapSInt16(theValue)); }
	inline SInt32 NSwapSInt32_BtoN(SInt32 theValue)				{ return(NSwapSInt32(theValue)); }
	inline SInt64 NSwapSInt64_BtoN(SInt64 theValue)				{ return(NSwapSInt64(theValue)); }

	inline Float32 NSwapFloat32_BtoN(Float32 theValue)			{ return(NSwapFloat32(theValue)); }
	inline Float64 NSwapFloat64_BtoN(Float64 theValue)			{ return(NSwapFloat64(theValue)); }
#endif



// Little to native
#if NN_TARGET_ENDIAN_BIG
	inline UInt16 NSwapUInt16_LtoN(UInt16 theValue)				{ return(NSwapUInt16(theValue)); }
	inline UInt32 NSwapUInt32_LtoN(UInt32 theValue)				{ return(NSwapUInt32(theValue)); }
	inline UInt64 NSwapUInt64_LtoN(UInt64 theValue)				{ return(NSwapUInt64(theValue)); }

	inline SInt16 NSwapSInt16_LtoN(SInt16 theValue)				{ return(NSwapSInt16(theValue)); }
	inline SInt32 NSwapSInt32_LtoN(SInt32 theValue)				{ return(NSwapSInt32(theValue)); }
	inline SInt64 NSwapSInt64_LtoN(SInt64 theValue)				{ return(NSwapSInt32(theValue)); }

	inline Float32 NSwapFloat32_LtoN(Float32 theValue)			{ return(NSwapFloat32(theValue)); }
	inline Float64 NSwapFloat64_LtoN(Float64 theValue)			{ return(NSwapFloat64(theValue)); }
#else
	inline UInt16 NSwapUInt16_LtoN(UInt16 theValue)				{ return(theValue); }
	inline UInt32 NSwapUInt32_LtoN(UInt32 theValue)				{ return(theValue); }
	inline UInt64 NSwapUInt64_LtoN(UInt64 theValue)				{ return(theValue); }

	inline SInt16 NSwapSInt16_LtoN(SInt16 theValue)				{ return(theValue); }
	inline SInt32 NSwapSInt32_LtoN(SInt32 theValue)				{ return(theValue); }
	inline SInt64 NSwapSInt64_LtoN(SInt64 theValue)				{ return(theValue); }

	inline Float32 NSwapFloat32_LtoN(Float32 theValue)			{ return(theValue); }
	inline Float64 NSwapFloat64_LtoN(Float64 theValue)			{ return(theValue); }
#endif





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
//		Swap in place
//----------------------------------------------------------------------------
// Always swap
inline void NSwapUInt16(UInt16 *theValue)						{ *theValue = NSwapUInt16(*theValue); }
inline void NSwapUInt32(UInt32 *theValue)						{ *theValue = NSwapUInt32(*theValue); }
inline void NSwapUInt64(UInt64 *theValue)						{ *theValue = NSwapUInt64(*theValue); }

inline void NSwapSInt16(SInt16 *theValue)						{ *theValue = NSwapSInt16(*theValue); }
inline void NSwapSInt32(SInt32 *theValue)						{ *theValue = NSwapSInt32(*theValue); }
inline void NSwapSInt64(SInt64 *theValue)						{ *theValue = NSwapSInt64(*theValue); }

inline void NSwapFloat32(Float32 *theValue)						{ *theValue = NSwapFloat32(*theValue); }
inline void NSwapFloat64(Float64 *theValue)						{ *theValue = NSwapFloat64(*theValue); }



// Native to big
inline void NSwapUInt16_NtoB(UInt16 *theValue)					{ *theValue = NSwapUInt16_NtoB(*theValue); }
inline void NSwapUInt32_NtoB(UInt32 *theValue)					{ *theValue = NSwapUInt32_NtoB(*theValue); }
inline void NSwapUInt64_NtoB(UInt64 *theValue)					{ *theValue = NSwapUInt64_NtoB(*theValue); }

inline void NSwapSInt16_NtoB(SInt16 *theValue)					{ *theValue = NSwapSInt16_NtoB(*theValue); }
inline void NSwapSInt32_NtoB(SInt32 *theValue)					{ *theValue = NSwapSInt32_NtoB(*theValue); }
inline void NSwapSInt64_NtoB(SInt64 *theValue)					{ *theValue = NSwapSInt64_NtoB(*theValue); }

inline void NSwapFloat32_NtoB(Float32 *theValue)				{ *theValue = NSwapFloat32_NtoB(*theValue); }
inline void NSwapFloat64_NtoB(Float64 *theValue)				{ *theValue = NSwapFloat64_NtoB(*theValue); }



// Native to little
inline void NSwapUInt16_NtoL(UInt16 *theValue)					{ *theValue = NSwapUInt16_NtoL(*theValue); }
inline void NSwapUInt32_NtoL(UInt32 *theValue)					{ *theValue = NSwapUInt32_NtoL(*theValue); }
inline void NSwapUInt64_NtoL(UInt64 *theValue)					{ *theValue = NSwapUInt64_NtoL(*theValue); }

inline void NSwapSInt16_NtoL(SInt16 *theValue)					{ *theValue = NSwapSInt16_NtoL(*theValue); }
inline void NSwapSInt32_NtoL(SInt32 *theValue)					{ *theValue = NSwapSInt32_NtoL(*theValue); }
inline void NSwapSInt64_NtoL(SInt64 *theValue)					{ *theValue = NSwapSInt64_NtoL(*theValue); }

inline void NSwapFloat32_NtoL(Float32 *theValue)				{ *theValue = NSwapFloat32_NtoL(*theValue); }
inline void NSwapFloat64_NtoL(Float64 *theValue)				{ *theValue = NSwapFloat64_NtoL(*theValue); }



// Big to native
inline void NSwapUInt16_BtoN(UInt16 *theValue)					{ *theValue = NSwapUInt16_BtoN(*theValue); }
inline void NSwapUInt32_BtoN(UInt32 *theValue)					{ *theValue = NSwapUInt32_BtoN(*theValue); }
inline void NSwapUInt64_BtoN(UInt64 *theValue)					{ *theValue = NSwapUInt64_BtoN(*theValue); }

inline void NSwapSInt16_BtoN(SInt16 *theValue)					{ *theValue = NSwapSInt16_BtoN(*theValue); }
inline void NSwapSInt32_BtoN(SInt32 *theValue)					{ *theValue = NSwapSInt32_BtoN(*theValue); }
inline void NSwapSInt64_BtoN(SInt64 *theValue)					{ *theValue = NSwapSInt64_BtoN(*theValue); }

inline void NSwapFloat32_BtoN(Float32 *theValue)				{ *theValue = NSwapFloat32_BtoN(*theValue); }
inline void NSwapFloat64_BtoN(Float64 *theValue)				{ *theValue = NSwapFloat64_BtoN(*theValue); }



// Little to native
inline void NSwapUInt16_LtoN(UInt16 *theValue)					{ *theValue = NSwapUInt16_LtoN(*theValue); }
inline void NSwapUInt32_LtoN(UInt32 *theValue)					{ *theValue = NSwapUInt32_LtoN(*theValue); }
inline void NSwapUInt64_LtoN(UInt64 *theValue)					{ *theValue = NSwapUInt64_LtoN(*theValue); }

inline void NSwapSInt16_LtoN(SInt16 *theValue)					{ *theValue = NSwapSInt16_LtoN(*theValue); }
inline void NSwapSInt32_LtoN(SInt32 *theValue)					{ *theValue = NSwapSInt32_LtoN(*theValue); }
inline void NSwapSInt64_LtoN(SInt64 *theValue)					{ *theValue = NSwapSInt64_LtoN(*theValue); }

inline void NSwapFloat32_LtoN(Float32 *theValue)				{ *theValue = NSwapFloat32_LtoN(*theValue); }
inline void NSwapFloat64_LtoN(Float64 *theValue)				{ *theValue = NSwapFloat64_LtoN(*theValue); }





//============================================================================
//		Structures
//----------------------------------------------------------------------------
// A structure swap can be declared with:
//
//		typedef struct {
//			UInt16		numLoops;
//			UInt16		numPoints;
//			UInt32		sizeLoops;
//			UInt32		sizePoints;
//		} MyStructure;
//
//		NBYTESWAP_BEGIN(MyStructure)
//			NBYTESWAP_L_UInt16		(numLoops)
//			NBYTESWAP_L_UInt16		(numPoints)
//			NBYTESWAP_B_UInt32		(sizeLoops)
//			NBYTESWAP_B_UInt32		(sizePoints)
//		NBYTESWAP_END
//
// The NBYTESWAP_xxx macros declare a function capable of swapping a MyStructure
// from its in-memory representation (using native endian-ness) to its on-disk
// representation (e.g., where the first two fields are little-endian, but the
// last two might be big-endian).
//
//
// A structure can be encoded to/from the on-disk representation with:
//
//		MyStructure		theValue;
//
//		// After reading from disk, swap to native
//		NBYTESWAP_DECODE(1, MyStructure, &theValue);
//
//
//		// Manipulate the structure
//		theValue.numLoops  += 1;
//		theVlaue.sizeLoops += 2;
//
//
//		// Encode for saving to disk
//		NBYTESWAP_ENCODE(1, MyStructure, &theValue);
//
//
// More complex definitions can include other types, arrays of values, or
// arrays whose size depends on other values:
//
//		NBYTESWAP_DECLARE(MyOtherStructure)
//
//		NBYTESWAP_BEGIN(MyStructure)
//			NBYTESWAP_L_UInt16			(numLoops)
//			NBYTESWAP_L_UInt16			(numPoints)
//			NBYTESWAP_B_UInt32			(sizeLoops)
//			NBYTESWAP_B_UInt32			(sizePoints)
//			NBYTESWAP_Type				(otherStruct)

//			/* This array is fixed size */
//			NBYTESWAP_B_UInt32_Array	(fourInts, 4)
//
//			/* Size of this array specified by numFloats */
//			NBYTESWAP_L_UInt32			(numFloats)
//			NBYTESWAP_B_Float32_Array	(someFloats, toNative ? currentItem->numFloats :
//																NSwapUInt32_LtoN(currentItem->numFloats))
//		NBYTESWAP_END
//
// Swapping proceeds from top to bottom within a structure, and can be combined
// with custom code that processes fields within 'currentItem' directly.
//
// The 'numItems', 'firstItem', and 'currentItem' variables are available to
// code within the swapping block.
#define NBYTESWAP_DECLARE(_type)															\
	extern void NByteSwap_ ## _type(NIndex numItems, _type *firstItem);

#define NBYTESWAP_BEGIN_NO_DECLARE(_type)													\
	void NByteSwap_ ## _type(NIndex numItems, _type *firstItem, bool toNative)				\
	{	_type		*currentItem;															\
		NIndex		n;																		\
																							\
		for (n = 0; n < numItems; n++)														\
			{																				\
			currentItem = &firstItem[n];

#define NBYTESWAP_BEGIN(_type)																\
	NBYTESWAP_DECLARE         (_type)														\
	NBYTESWAP_BEGIN_NO_DECLARE(_type)

#define NBYTESWAP_END																		\
			}																				\
	}



// Encode/decode
#define NBYTESWAP_ENCODE(_numItems, _type, _firstItem)										\
	NByteSwap_ ## _type(_numItems, _firstItem, true)

#define NBYTESWAP_DECODE(_numItems, _type, _firstItem)										\
	NByteSwap_ ## _type(_numItems, _firstItem, false)



// Swap fields
inline void NByteSwap_Field(NIndex numValues, void *theValue, NIndex valueSize, NEndianFormat valueFormat)
{
	if (valueFormat != kNEndianNative)
		NByteSwap::SwapBlock(numValues, valueSize, theValue);
}


#define NBYTESWAP_Type(_fieldName)								NByteSwap_ ## _fieldType(1, &currentItem->_fieldName);


#define NBYTESWAP_B_UInt8 (_fieldName)
#define NBYTESWAP_B_UInt16(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(UInt16), kNEndianBig);
#define NBYTESWAP_B_UInt32(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(UInt32), kNEndianBig);
#define NBYTESWAP_B_UInt64(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(UInt64), kNEndianBig);

#define NBYTESWAP_B_SInt8 (_fieldName)
#define NBYTESWAP_B_SInt16(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(SInt16), kNEndianBig);
#define NBYTESWAP_B_SInt32(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(SInt32), kNEndianBig);
#define NBYTESWAP_B_SInt64(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(SInt64), kNEndianBig);

#define NBYTESWAP_B_Float32(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(Float32), kNEndianBig);
#define NBYTESWAP_B_Float64(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(Float64), kNEndianBig);


#define NBYTESWAP_L_UInt8 (_fieldName)
#define NBYTESWAP_L_UInt16(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(UInt16), kNEndianLittle);
#define NBYTESWAP_L_UInt32(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(UInt32), kNEndianLittle);
#define NBYTESWAP_L_UInt64(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(UInt64), kNEndianLittle);

#define NBYTESWAP_L_SInt8 (_fieldName)
#define NBYTESWAP_L_SInt16(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(SInt16), kNEndianLittle);
#define NBYTESWAP_L_SInt32(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(SInt32), kNEndianLittle);
#define NBYTESWAP_L_SInt64(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(SInt64), kNEndianLittle);

#define NBYTESWAP_L_Float32(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(Float32), kNEndianLittle);
#define NBYTESWAP_L_Float64(_fieldName)							NByteSwap_Field(1, &currentItem->_fieldName, sizeof(Float64), kNEndianLittle);



// Swap arrays
#define NBYTESWAP_Type_Array(_fieldName, _fieldCount)			NByteSwap_ ## _fieldType(_fieldCount, &currentItem->_fieldName);


#define NBYTESWAP_B_UInt8_Array (_fieldName, _fieldCount)
#define NBYTESWAP_B_UInt16_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(UInt16), kNEndianBig);
#define NBYTESWAP_B_UInt32_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(UInt32), kNEndianBig);
#define NBYTESWAP_B_UInt64_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(UInt64), kNEndianBig);

#define NBYTESWAP_B_SInt8_Array(_fieldName, _fieldCount)
#define NBYTESWAP_B_SInt16_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(SInt16), kNEndianBig);
#define NBYTESWAP_B_SInt32_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(SInt32), kNEndianBig);
#define NBYTESWAP_B_SInt64_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(SInt64), kNEndianBig);

#define NBYTESWAP_B_Float32_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(Float32), kNEndianBig);
#define NBYTESWAP_B_Float64_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(Float64), kNEndianBig);


#define NBYTESWAP_L_UInt8_Array(_fieldName, _fieldCount)
#define NBYTESWAP_L_UInt16_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(UInt16), kNEndianLittle);
#define NBYTESWAP_L_UInt32_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(UInt32), kNEndianLittle);
#define NBYTESWAP_L_UInt64_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(UInt64), kNEndianLittle);

#define NBYTESWAP_L_SInt8_Array(_fieldName, _fieldCount)
#define NBYTESWAP_L_SInt16_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(SInt16), kNEndianLittle);
#define NBYTESWAP_L_SInt32_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(SInt32), kNEndianLittle);
#define NBYTESWAP_L_SInt64_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(SInt64), kNEndianLittle);

#define NBYTESWAP_L_Float32_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(Float32), kNEndianLittle);
#define NBYTESWAP_L_Float64_Array(_fieldName, _fieldCount)		NByteSwap_Field(_fieldCount, &currentItem->_fieldName, sizeof(Float64), kNEndianLittle);





#endif // NBYTESWAP_HDR


