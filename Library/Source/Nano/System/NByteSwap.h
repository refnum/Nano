/*	NAME:
		NByteSwap.h

	DESCRIPTION:
		Byte-swap support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
	static void							Swap16(uint16_t *theValue);
	static void							Swap32(uint32_t *theValue);
	static void							Swap64(uint64_t *theValue);
	static void							SwapBlock(NIndex numValues, NIndex valueSize, void *valuePtr);
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
//		Swap and return
//----------------------------------------------------------------------------
// Always swap
inline uint16_t NSwapUInt16(uint16_t theValue)											{ NByteSwap::Swap16(&theValue);							return(theValue); }
inline uint32_t NSwapUInt32(uint32_t theValue)											{ NByteSwap::Swap32(&theValue);							return(theValue); }
inline uint64_t NSwapUInt64(uint64_t theValue)											{ NByteSwap::Swap64(&theValue);							return(theValue); }

inline int16_t NSwapInt16(int16_t theValue)												{ NByteSwap::Swap16((uint16_t *) &theValue);			return(theValue); }
inline int32_t NSwapInt32(int32_t theValue)												{ NByteSwap::Swap32((uint32_t *) &theValue);			return(theValue); }
inline int64_t NSwapInt64(int64_t theValue)												{ NByteSwap::Swap64((uint64_t *) &theValue);			return(theValue); }

inline float32_t NSwapFloat32(float32_t theValue)										{ NByteSwap::Swap32((uint32_t *) &theValue);			return(theValue); }
inline float64_t NSwapFloat64(float64_t theValue)										{ NByteSwap::Swap64((uint64_t *) &theValue);			return(theValue); }

inline void *NSwapBlock(NIndex numValues, NIndex valueSize, void *valuePtr)				{ NByteSwap::SwapBlock(numValues, valueSize, valuePtr);	return(valuePtr); }



// Native to big
#if NN_TARGET_ENDIAN_BIG
	inline uint16_t NSwapUInt16_NtoB(uint16_t theValue)									{ return(theValue); }
	inline uint32_t NSwapUInt32_NtoB(uint32_t theValue)									{ return(theValue); }
	inline uint64_t NSwapUInt64_NtoB(uint64_t theValue)									{ return(theValue); }

	inline int16_t NSwapInt16_NtoB(int16_t theValue)									{ return(theValue); }
	inline int32_t NSwapInt32_NtoB(int32_t theValue)									{ return(theValue); }
	inline int64_t NSwapInt64_NtoB(int64_t theValue)									{ return(theValue); }

	inline float32_t NSwapFloat32_NtoB(float32_t theValue)								{ return(theValue); }
	inline float64_t NSwapFloat64_NtoB(float64_t theValue)								{ return(theValue); }
	
	inline void *NSwapBlock_NtoB(NIndex numValues, NIndex valueSize, void *valuePtr)	{ NN_UNUSED(numValues); NN_UNUSED(valueSize); return(valuePtr); }
#else
	inline uint16_t NSwapUInt16_NtoB(uint16_t theValue)									{ return(NSwapUInt16(theValue)); }
	inline uint32_t NSwapUInt32_NtoB(uint32_t theValue)									{ return(NSwapUInt32(theValue)); }
	inline uint64_t NSwapUInt64_NtoB(uint64_t theValue)									{ return(NSwapUInt64(theValue)); }

	inline int16_t NSwapInt16_NtoB(int16_t theValue)									{ return(NSwapInt16(theValue)); }
	inline int32_t NSwapInt32_NtoB(int32_t theValue)									{ return(NSwapInt32(theValue)); }
	inline int64_t NSwapInt64_NtoB(int64_t theValue)									{ return(NSwapInt64(theValue)); }

	inline float32_t NSwapFloat32_NtoB(float32_t theValue)								{ return(NSwapFloat32(theValue)); }
	inline float64_t NSwapFloat64_NtoB(float64_t theValue)								{ return(NSwapFloat64(theValue)); }
	
	inline void *NSwapBlock_NtoB(NIndex numValues, NIndex valueSize, void *valuePtr)	{ return(NSwapBlock(numValues, valueSize, valuePtr)); }
#endif



// Native to little
#if NN_TARGET_ENDIAN_BIG
	inline uint16_t NSwapUInt16_NtoL(uint16_t theValue)									{ return(NSwapUInt16(theValue)); }
	inline uint32_t NSwapUInt32_NtoL(uint32_t theValue)									{ return(NSwapUInt32(theValue)); }
	inline uint64_t NSwapUInt64_NtoL(uint64_t theValue)									{ return(NSwapUInt64(theValue)); }

	inline int16_t NSwapInt16_NtoL(int16_t theValue)									{ return(NSwapInt16(theValue)); }
	inline int32_t NSwapInt32_NtoL(int32_t theValue)									{ return(NSwapInt32(theValue)); }
	inline int64_t NSwapInt64_NtoL(int64_t theValue)									{ return(NSwapInt64(theValue)); }

	inline float32_t NSwapFloat32_NtoL(float32_t theValue)								{ return(NSwapFloat32(theValue)); }
	inline float64_t NSwapFloat64_NtoL(float64_t theValue)								{ return(NSwapFloat64(theValue)); }
	
	inline void *NSwapBlock_NtoL(NIndex numValues, NIndex valueSize, void *valuePtr)	{ return(NSwapBlock(numValues, valueSize, valuePtr)); }
#else
	inline uint16_t NSwapUInt16_NtoL(uint16_t theValue)									{ return(theValue); }
	inline uint32_t NSwapUInt32_NtoL(uint32_t theValue)									{ return(theValue); }
	inline uint64_t NSwapUInt64_NtoL(uint64_t theValue)									{ return(theValue); }

	inline int16_t NSwapInt16_NtoL(int16_t theValue)									{ return(theValue); }
	inline int32_t NSwapInt32_NtoL(int32_t theValue)									{ return(theValue); }
	inline int64_t NSwapInt64_NtoL(int64_t theValue)									{ return(theValue); }

	inline float32_t NSwapFloat32_NtoL(float32_t theValue)								{ return(theValue); }
	inline float64_t NSwapFloat64_NtoL(float64_t theValue)								{ return(theValue); }
	
	inline void *NSwapBlock_NtoL(NIndex numValues, NIndex valueSize, void *valuePtr)	{ NN_UNUSED(numValues); NN_UNUSED(valueSize); return(valuePtr); }
#endif



// Big to native
#if NN_TARGET_ENDIAN_BIG
	inline uint16_t NSwapUInt16_BtoN(uint16_t theValue)									{ return(theValue); }
	inline uint32_t NSwapUInt32_BtoN(uint32_t theValue)									{ return(theValue); }
	inline uint64_t NSwapUInt64_BtoN(uint64_t theValue)									{ return(theValue); }

	inline int16_t NSwapInt16_BtoN(int16_t theValue)									{ return(theValue); }
	inline int32_t NSwapInt32_BtoN(int32_t theValue)									{ return(theValue); }
	inline int64_t NSwapInt64_BtoN(int64_t theValue)									{ return(theValue); }

	inline float32_t NSwapFloat32_BtoN(float32_t theValue)								{ return(theValue); }
	inline float64_t NSwapFloat64_BtoN(float64_t theValue)								{ return(theValue); }
	
	inline void *NSwapBlock_BtoN(NIndex numValues, NIndex valueSize, void *valuePtr)	{ NN_UNUSED(numValues); NN_UNUSED(valueSize); return(valuePtr); }
#else
	inline uint16_t NSwapUInt16_BtoN(uint16_t theValue)									{ return(NSwapUInt16(theValue)); }
	inline uint32_t NSwapUInt32_BtoN(uint32_t theValue)									{ return(NSwapUInt32(theValue)); }
	inline uint64_t NSwapUInt64_BtoN(uint64_t theValue)									{ return(NSwapUInt64(theValue)); }

	inline int16_t NSwapInt16_BtoN(int16_t theValue)									{ return(NSwapInt16(theValue)); }
	inline int32_t NSwapInt32_BtoN(int32_t theValue)									{ return(NSwapInt32(theValue)); }
	inline int64_t NSwapInt64_BtoN(int64_t theValue)									{ return(NSwapInt64(theValue)); }

	inline float32_t NSwapFloat32_BtoN(float32_t theValue)								{ return(NSwapFloat32(theValue)); }
	inline float64_t NSwapFloat64_BtoN(float64_t theValue)								{ return(NSwapFloat64(theValue)); }
	
	inline void *NSwapBlock_BtoN(NIndex numValues, NIndex valueSize, void *valuePtr)	{ return(NSwapBlock(numValues, valueSize, valuePtr)); }
#endif



// Little to native
#if NN_TARGET_ENDIAN_BIG
	inline uint16_t NSwapUInt16_LtoN(uint16_t theValue)									{ return(NSwapUInt16(theValue)); }
	inline uint32_t NSwapUInt32_LtoN(uint32_t theValue)									{ return(NSwapUInt32(theValue)); }
	inline uint64_t NSwapUInt64_LtoN(uint64_t theValue)									{ return(NSwapUInt64(theValue)); }

	inline int16_t NSwapInt16_LtoN(int16_t theValue)									{ return(NSwapInt16(theValue)); }
	inline int32_t NSwapInt32_LtoN(int32_t theValue)									{ return(NSwapInt32(theValue)); }
	inline int64_t NSwapInt64_LtoN(int64_t theValue)									{ return(NSwapInt64(theValue)); }

	inline float32_t NSwapFloat32_LtoN(float32_t theValue)								{ return(NSwapFloat32(theValue)); }
	inline float64_t NSwapFloat64_LtoN(float64_t theValue)								{ return(NSwapFloat64(theValue)); }
	
	inline void *NSwapBlock_LtoN(NIndex numValues, NIndex valueSize, void *valuePtr)	{ return(NSwapBlock(numValues, valueSize, valuePtr)); }
#else
	inline uint16_t NSwapUInt16_LtoN(uint16_t theValue)									{ return(theValue); }
	inline uint32_t NSwapUInt32_LtoN(uint32_t theValue)									{ return(theValue); }
	inline uint64_t NSwapUInt64_LtoN(uint64_t theValue)									{ return(theValue); }

	inline int16_t NSwapInt16_LtoN(int16_t theValue)									{ return(theValue); }
	inline int32_t NSwapInt32_LtoN(int32_t theValue)									{ return(theValue); }
	inline int64_t NSwapInt64_LtoN(int64_t theValue)									{ return(theValue); }

	inline float32_t NSwapFloat32_LtoN(float32_t theValue)								{ return(theValue); }
	inline float64_t NSwapFloat64_LtoN(float64_t theValue)								{ return(theValue); }
	
	inline void *NSwapBlock_LtoN(NIndex numValues, NIndex valueSize, void *valuePtr)	{ NN_UNUSED(numValues); NN_UNUSED(valueSize); return(valuePtr); }
#endif





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
//		Swap in place
//----------------------------------------------------------------------------
// Always swap
inline void NSwapUInt16(uint16_t *theValue)							{ *theValue = NSwapUInt16(*theValue); }
inline void NSwapUInt32(uint32_t *theValue)							{ *theValue = NSwapUInt32(*theValue); }
inline void NSwapUInt64(uint64_t *theValue)							{ *theValue = NSwapUInt64(*theValue); }

inline void NSwapInt16(int16_t *theValue)							{ *theValue = NSwapInt16(*theValue); }
inline void NSwapInt32(int32_t *theValue)							{ *theValue = NSwapInt32(*theValue); }
inline void NSwapInt64(int64_t *theValue)							{ *theValue = NSwapInt64(*theValue); }

inline void NSwapFloat32(float32_t *theValue)						{ *theValue = NSwapFloat32(*theValue); }
inline void NSwapFloat64(float64_t *theValue)						{ *theValue = NSwapFloat64(*theValue); }



// Native to big
inline void NSwapUInt16_NtoB(uint16_t *theValue)					{ *theValue = NSwapUInt16_NtoB(*theValue); }
inline void NSwapUInt32_NtoB(uint32_t *theValue)					{ *theValue = NSwapUInt32_NtoB(*theValue); }
inline void NSwapUInt64_NtoB(uint64_t *theValue)					{ *theValue = NSwapUInt64_NtoB(*theValue); }

inline void NSwapInt16_NtoB(int16_t *theValue)						{ *theValue = NSwapInt16_NtoB(*theValue); }
inline void NSwapInt32_NtoB(int32_t *theValue)						{ *theValue = NSwapInt32_NtoB(*theValue); }
inline void NSwapInt64_NtoB(int64_t *theValue)						{ *theValue = NSwapInt64_NtoB(*theValue); }

inline void NSwapFloat32_NtoB(float32_t *theValue)					{ *theValue = NSwapFloat32_NtoB(*theValue); }
inline void NSwapFloat64_NtoB(float64_t *theValue)					{ *theValue = NSwapFloat64_NtoB(*theValue); }



// Native to little
inline void NSwapUInt16_NtoL(uint16_t *theValue)					{ *theValue = NSwapUInt16_NtoL(*theValue); }
inline void NSwapUInt32_NtoL(uint32_t *theValue)					{ *theValue = NSwapUInt32_NtoL(*theValue); }
inline void NSwapUInt64_NtoL(uint64_t *theValue)					{ *theValue = NSwapUInt64_NtoL(*theValue); }

inline void NSwapInt16_NtoL(int16_t *theValue)						{ *theValue = NSwapInt16_NtoL(*theValue); }
inline void NSwapInt32_NtoL(int32_t *theValue)						{ *theValue = NSwapInt32_NtoL(*theValue); }
inline void NSwapInt64_NtoL(int64_t *theValue)						{ *theValue = NSwapInt64_NtoL(*theValue); }

inline void NSwapFloat32_NtoL(float32_t *theValue)					{ *theValue = NSwapFloat32_NtoL(*theValue); }
inline void NSwapFloat64_NtoL(float64_t *theValue)					{ *theValue = NSwapFloat64_NtoL(*theValue); }



// Big to native
inline void NSwapUInt16_BtoN(uint16_t *theValue)					{ *theValue = NSwapUInt16_BtoN(*theValue); }
inline void NSwapUInt32_BtoN(uint32_t *theValue)					{ *theValue = NSwapUInt32_BtoN(*theValue); }
inline void NSwapUInt64_BtoN(uint64_t *theValue)					{ *theValue = NSwapUInt64_BtoN(*theValue); }

inline void NSwapInt16_BtoN(int16_t *theValue)						{ *theValue = NSwapInt16_BtoN(*theValue); }
inline void NSwapInt32_BtoN(int32_t *theValue)						{ *theValue = NSwapInt32_BtoN(*theValue); }
inline void NSwapInt64_BtoN(int64_t *theValue)						{ *theValue = NSwapInt64_BtoN(*theValue); }

inline void NSwapFloat32_BtoN(float32_t *theValue)					{ *theValue = NSwapFloat32_BtoN(*theValue); }
inline void NSwapFloat64_BtoN(float64_t *theValue)					{ *theValue = NSwapFloat64_BtoN(*theValue); }



// Little to native
inline void NSwapUInt16_LtoN(uint16_t *theValue)					{ *theValue = NSwapUInt16_LtoN(*theValue); }
inline void NSwapUInt32_LtoN(uint32_t *theValue)					{ *theValue = NSwapUInt32_LtoN(*theValue); }
inline void NSwapUInt64_LtoN(uint64_t *theValue)					{ *theValue = NSwapUInt64_LtoN(*theValue); }

inline void NSwapInt16_LtoN(int16_t *theValue)						{ *theValue = NSwapInt16_LtoN(*theValue); }
inline void NSwapInt32_LtoN(int32_t *theValue)						{ *theValue = NSwapInt32_LtoN(*theValue); }
inline void NSwapInt64_LtoN(int64_t *theValue)						{ *theValue = NSwapInt64_LtoN(*theValue); }

inline void NSwapFloat32_LtoN(float32_t *theValue)					{ *theValue = NSwapFloat32_LtoN(*theValue); }
inline void NSwapFloat64_LtoN(float64_t *theValue)					{ *theValue = NSwapFloat64_LtoN(*theValue); }





//============================================================================
//		Structures
//----------------------------------------------------------------------------
// A structure swap can be declared with:
//
//		typedef struct {
//			uint16_t				numLoops;
//			uint16_t				numPoints;
//			uint32_t				sizeLoops;
//			uint32_t				sizePoints;
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
//		NBYTESWAP_BEGIN(MyBigStructure)
//			/* Primitive types */
//			NBYTESWAP_L_UInt16			(numLoops)
//			NBYTESWAP_L_UInt16			(numPoints)
//			NBYTESWAP_B_UInt32			(sizeLoops)
//			NBYTESWAP_B_UInt32			(sizePoints)
//
//			/* Embedded structure */
//			NBYTESWAP_Type				(otherStruct, MyOtherStructure)
//
//			/* Array of fixed size */
//			NBYTESWAP_B_UInt32_Array	(fourInts, 4)
//
//			/* Array of variable size */
//			NBYTESWAP_L_UInt32			(numFloats)
//			NBYTESWAP_B_Float32_Array	(someFloats, NBYTESWAP_Fetch(NSwapUInt32_LtoN, numFloats))
//
//			/* Align to 4-byte boundary */
//			NBYTESWAP_Skip(4 - (NBYTESWAP_Offset() % 4))
//		NBYTESWAP_END
//
// Swapping proceeds from top to bottom within a structure, and can be combined
// with custom code.
//
// The 'numItems', 'firstItem', 'currentItem', and 'currentField' variables are
// available to code within the swapping block.
#define NBYTESWAP_DECLARE(_type)															\
	extern void NByteSwap_ ## _type(NIndex numItems, _type *firstItem, bool toNative);

#define NBYTESWAP_BEGIN_NO_DECLARE(_type)													\
	void NByteSwap_ ## _type(NIndex numItems, _type *firstItem, bool toNative)				\
	{	uint8_t     *currentField;                                                          \
		_type		*currentItem;															\
		NIndex		n;																		\
																							\
		currentField = (uint8_t *) firstItem;												\
		for (n = 0; n < numItems; n++)														\
			{																				\
			currentItem = (_type *) currentField;

#define NBYTESWAP_BEGIN(_type)																\
	NBYTESWAP_DECLARE(_type)																\
	NBYTESWAP_BEGIN_NO_DECLARE(_type)

#define NBYTESWAP_END																		\
			}																				\
																							\
        NN_UNUSED(toNative);                                                                \
		NN_UNUSED(currentItem);																\
	}



// Encode/decode
#define NBYTESWAP_ENCODE(_numItems, _type, _firstItem)										\
	NByteSwap_ ## _type(_numItems, (_type *) _firstItem, false)

#define NBYTESWAP_DECODE(_numItems, _type, _firstItem)										\
	NByteSwap_ ## _type(_numItems, (_type *) _firstItem, true)



// Swap fields
inline void NByteSwap_Field(NIndex numValues, uint8_t *&valuePtr, NIndex valueSize, NEndianFormat valueFormat)
{
	if (valueFormat != kNEndianNative)
		NByteSwap::SwapBlock(numValues, valueSize, valuePtr);
	
	valuePtr += (numValues * valueSize);
}

#define NBYTESWAP_Offset()											(currentField - ((uint8_t *) currentItem))
#define NBYTESWAP_Skip(_numBytes)									currentField += _numBytes;
#define NBYTESWAP_Fetch(_swapWith, _fieldName)						(toNative ? currentItem->_fieldName : _swapWith(currentItem->_fieldName))


// Arrays
#define NBYTESWAP_Type_Array(_fieldName, _fieldType, _fieldCount)							\
		NByteSwap_ ## _fieldType(_fieldCount, ( _fieldType *) currentField, toNative);		\
		currentField += (_fieldCount * sizeof(_fieldType));

#define NBYTESWAP_B_UInt8_Array(  _fieldName, _fieldCount)			currentField += (_fieldCount * sizeof(uint8_t));
#define NBYTESWAP_B_UInt16_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(uint16_t),  kNEndianBig);
#define NBYTESWAP_B_UInt32_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(uint32_t),  kNEndianBig);
#define NBYTESWAP_B_UInt64_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(uint64_t),  kNEndianBig);

#define NBYTESWAP_B_Int8_Array(  _fieldName, _fieldCount)			currentField += (_fieldCount * sizeof(uint8_t));
#define NBYTESWAP_B_Int16_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(int16_t),   kNEndianBig);
#define NBYTESWAP_B_Int32_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(int32_t),   kNEndianBig);
#define NBYTESWAP_B_Int64_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(int64_t),   kNEndianBig);

#define NBYTESWAP_B_Float32_Array(_fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(float32_t), kNEndianBig);
#define NBYTESWAP_B_Float64_Array(_fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(float64_t), kNEndianBig);


#define NBYTESWAP_L_UInt8_Array(  _fieldName, _fieldCount)			currentField += (_fieldCount * sizeof(uint8_t));
#define NBYTESWAP_L_UInt16_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(uint16_t),  kNEndianLittle);
#define NBYTESWAP_L_UInt32_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(uint32_t),  kNEndianLittle);
#define NBYTESWAP_L_UInt64_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(uint64_t),  kNEndianLittle);

#define NBYTESWAP_L_Int8_Array(  _fieldName, _fieldCount)			currentField += (_fieldCount * sizeof(uint8_t));
#define NBYTESWAP_L_Int16_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(int16_t),   kNEndianLittle);
#define NBYTESWAP_L_Int32_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(int32_t),   kNEndianLittle);
#define NBYTESWAP_L_Int64_Array( _fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(int64_t),   kNEndianLittle);

#define NBYTESWAP_L_Float32_Array(_fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(float32_t), kNEndianLittle);
#define NBYTESWAP_L_Float64_Array(_fieldName, _fieldCount)			NByteSwap_Field(_fieldCount, currentField, sizeof(float64_t), kNEndianLittle);


// Fields
#define NBYTESWAP_Type(_fieldName, _fieldType)						NBYTESWAP_Type_Array(_fieldName, _fieldType, 1)


#define NBYTESWAP_B_UInt8(  _fieldName)								NBYTESWAP_B_UInt8_Array(  _fieldName, 1)
#define NBYTESWAP_B_UInt16( _fieldName)								NBYTESWAP_B_UInt16_Array( _fieldName, 1)
#define NBYTESWAP_B_UInt32( _fieldName)								NBYTESWAP_B_UInt32_Array( _fieldName, 1)
#define NBYTESWAP_B_UInt64( _fieldName)								NBYTESWAP_B_UInt64_Array( _fieldName, 1)

#define NBYTESWAP_B_Int8(  _fieldName)								NBYTESWAP_B_Int8_Array(  _fieldName, 1)
#define NBYTESWAP_B_Int16( _fieldName)								NBYTESWAP_B_Int16_Array( _fieldName, 1)
#define NBYTESWAP_B_Int32( _fieldName)								NBYTESWAP_B_Int32_Array( _fieldName, 1)
#define NBYTESWAP_B_Int64( _fieldName)								NBYTESWAP_B_Int64_Array( _fieldName, 1)

#define NBYTESWAP_B_Float32(_fieldName)								NBYTESWAP_B_Float32_Array(_fieldName, 1)
#define NBYTESWAP_B_Float64(_fieldName)								NBYTESWAP_B_Float64_Array(_fieldName, 1)


#define NBYTESWAP_L_UInt8(  _fieldName)								NBYTESWAP_L_UInt8_Array(  _fieldName, 1)
#define NBYTESWAP_L_UInt16( _fieldName)								NBYTESWAP_L_UInt16_Array( _fieldName, 1)
#define NBYTESWAP_L_UInt32( _fieldName)								NBYTESWAP_L_UInt32_Array( _fieldName, 1)
#define NBYTESWAP_L_UInt64( _fieldName)								NBYTESWAP_L_UInt64_Array( _fieldName, 1)

#define NBYTESWAP_L_Int8(  _fieldName)								NBYTESWAP_L_Int8_Array(  _fieldName, 1)
#define NBYTESWAP_L_Int16( _fieldName)								NBYTESWAP_L_Int16_Array( _fieldName, 1)
#define NBYTESWAP_L_Int32( _fieldName)								NBYTESWAP_L_Int32_Array( _fieldName, 1)
#define NBYTESWAP_L_Int64( _fieldName)								NBYTESWAP_L_Int64_Array( _fieldName, 1)

#define NBYTESWAP_L_Float32(_fieldName)								NBYTESWAP_L_Float32_Array(_fieldName, 1)
#define NBYTESWAP_L_Float64(_fieldName)								NBYTESWAP_L_Float64_Array(_fieldName, 1)







#endif // NBYTESWAP_HDR


