/*	NAME:
		NByteSwap.h

	DESCRIPTION:
		Byte-swap support.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
#ifndef NBYTE_SWAP_H
#define NBYTE_SWAP_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoTargets.h"
#include "NanoTypes.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Endian-ness
enum class NEndian
{
	Big                                                     = 1,
	Little                                                  = 2,
	Native                                                  = NN_ENDIAN_BIG ? Big : Little
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NByteSwap
{
public:
	// Swap native to big-endian
	template<typename T>
	static constexpr T                  SwapNtoB(T theValue);

	template<typename T>
	static void                         SwapNtoB(T* theValue);

	template<typename T>
	static void                         SwapNtoB(size_t numValues, T* theValues);


	// Swap big-endian to native
	template<typename T>
	static constexpr T                  SwapBtoN(T theValue);

	template<typename T>
	static void                         SwapBtoN(T* theValue);

	template<typename T>
	static void                         SwapBtoN(size_t numValues, T* theValues);


	// Swap native to little-endian
	template<typename T>
	static constexpr T                  SwapNtoL(T theValue);

	template<typename T>
	static void                         SwapNtoL(T* theValue);

	template<typename T>
	static void                         SwapNtoL(size_t numValues, T* theValues);


	// Swap little-endian to native
	template<typename T>
	static constexpr T                  SwapLtoN(T theValue);

	template<typename T>
	static void                         SwapLtoN(T* theValue);

	template<typename T>
	static void                         SwapLtoN(size_t numValues, T* theValues);


	// Always swap
	template<typename T>
	static constexpr T                  Swap(T theValue);

	template<typename T>
	static void                         Swap(T* theValue);

	template<typename T>
	static void                         Swap(size_t numValues, T* theValues);

	static void                         Swap(size_t numValues, size_t valueSize, void* valuePtr);



private:
	template<typename T>
	static constexpr T                  SwapXtoY(NEndian orderX, NEndian orderY, T theValue);

	template<typename T>
	static void                         SwapXtoY(NEndian orderX, NEndian orderY, T* theValue);

	template<typename T>
	static void                         SwapXtoY(NEndian orderX, NEndian orderY, size_t numValues, T* theValues);
};





//=============================================================================
//		Structures
//-----------------------------------------------------------------------------
// A structure swap can be declared with:
//
//		struct MyStructure {
//			uint16_t				numLoops;
//			uint16_t				numPoints;
//			uint32_t				sizeLoops;
//			uint32_t				sizePoints;
//		};
//
//		NBYTESWAP_BEGIN(MyStructure)
//			NBYTESWAP_L_UInt16		(numLoops)
//			NBYTESWAP_L_UInt16		(numPoints)
//			NBYTESWAP_B_UInt32		(sizeLoops)
//			NBYTESWAP_B_UInt32		(sizePoints)
//		NBYTESWAP_END
//
// The NBYTESWAP_xxx macros declare a function that can swap a MyStructure
// from its native-endian in-memory representation to its on-disk form.
//
// In this case the on-disk representation stores the first two fields
// as little-endian and the last two as big-endian.
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
// More complex definitions can include other types, arrays of values,
// or arrays whose size depends on other values:
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
//
#define NBYTESWAP_DECLARE(_type)                            \
	extern void NByteSwap_##_type(size_t numItems, _type* firstItem, bool toNative);

#define NBYTESWAP_BEGIN_NO_DECLARE(         _type)                                       \
	void NByteSwap_##_type(size_t numItems, _type* firstItem, bool toNative)    \
	{                                                                           \
		uint8_t* currentField;                                                  \
		_type*   currentItem;                                                   \
		size_t   n;                                                             \
																				\
		currentField = (uint8_t*) firstItem;                                    \
		for (n = 0; n < numItems; n++)                                          \
		{                                                                       \
			currentItem = (_type*) currentField;

#define NBYTESWAP_BEGIN(_type)                              \
										NBYTESWAP_DECLARE(         _type)                                \
										NBYTESWAP_BEGIN_NO_DECLARE(_type)

#define NBYTESWAP_END                                       \
	}                                                       \
															\
										NN_UNUSED(toNative);                                    \
										NN_UNUSED(currentItem);                                 \
	}



// Encode/decode
#define NBYTESWAP_ENCODE(_numItems, _type, _firstItem)      \
	NByteSwap_##_type(_numItems, (  _type*) _firstItem, false)

#define NBYTESWAP_DECODE(_numItems, _type, _firstItem)      \
	NByteSwap_##_type(_numItems, (  _type*) _firstItem, true)



// Swap fields
inline void NByteSwap_Field(size_t    numValues,
							uint8_t*& valuePtr,
							size_t    valueSize,
							NEndian   valueOrder)
{
	if (valueOrder != NEndian::Native)
	{
		NByteSwap::Swap(numValues, valueSize, valuePtr);
	}

	valuePtr += (numValues * valueSize);
}

#define NBYTESWAP_Offset()                                  (currentField - ((uint8_t*) currentItem))
#define NBYTESWAP_Skip(_numBytes)                           currentField += _numBytes;
#define NBYTESWAP_Fetch(_swapWith, _fieldName)              \
	(toNative ? currentItem->_fieldName : _swapWith(currentItem->_fieldName))


// Arrays
#define NBYTESWAP_Type_Array(_fieldName, _fieldType, _fieldCount)               \
	NByteSwap_##_fieldType(_fieldCount, (_fieldType*) currentField, toNative);  \
	currentField += (_fieldCount * sizeof(_fieldType));

#define NBYTESWAP_B_UInt8_Array(_fieldName, _fieldCount)    \
	currentField += (_fieldCount * sizeof(uint8_t));

#define NBYTESWAP_B_UInt16_Array(_fieldName, _fieldCount)   \
	NByteSwap_Field(_fieldCount, currentField, sizeof(uint16_t), kNEndianBig);

#define NBYTESWAP_B_UInt32_Array(_fieldName, _fieldCount)   \
	NByteSwap_Field(_fieldCount, currentField, sizeof(uint32_t), kNEndianBig);

#define NBYTESWAP_B_UInt64_Array(_fieldName, _fieldCount)   \
	NByteSwap_Field(_fieldCount, currentField, sizeof(uint64_t), kNEndianBig);

#define NBYTESWAP_B_Int8_Array(_fieldName, _fieldCount)     \
	currentField += (_fieldCount * sizeof(uint8_t));

#define NBYTESWAP_B_Int16_Array(_fieldName, _fieldCount)    \
	NByteSwap_Field(_fieldCount, currentField, sizeof(int16_t), kNEndianBig);

#define NBYTESWAP_B_Int32_Array(_fieldName, _fieldCount)    \
	NByteSwap_Field(_fieldCount, currentField, sizeof(int32_t), kNEndianBig);

#define NBYTESWAP_B_Int64_Array(_fieldName, _fieldCount)    \
	NByteSwap_Field(_fieldCount, currentField, sizeof(int64_t), kNEndianBig);

#define NBYTESWAP_B_Float32_Array(_fieldName, _fieldCount)  \
	NByteSwap_Field(_fieldCount, currentField, sizeof(float32_t), kNEndianBig);

#define NBYTESWAP_B_Float64_Array(_fieldName, _fieldCount)  \
	NByteSwap_Field(_fieldCount, currentField, sizeof(float64_t), kNEndianBig);


#define NBYTESWAP_L_UInt8_Array(_fieldName, _fieldCount)    \
	currentField += (_fieldCount * sizeof(uint8_t));

#define NBYTESWAP_L_UInt16_Array(_fieldName, _fieldCount)   \
	NByteSwap_Field(_fieldCount, currentField, sizeof(uint16_t), kNEndianLittle);

#define NBYTESWAP_L_UInt32_Array(_fieldName, _fieldCount)   \
	NByteSwap_Field(_fieldCount, currentField, sizeof(uint32_t), kNEndianLittle);

#define NBYTESWAP_L_UInt64_Array(_fieldName, _fieldCount)   \
	NByteSwap_Field(_fieldCount, currentField, sizeof(uint64_t), kNEndianLittle);

#define NBYTESWAP_L_Int8_Array(_fieldName, _fieldCount)     \
	currentField += (_fieldCount * sizeof(uint8_t));

#define NBYTESWAP_L_Int16_Array(_fieldName, _fieldCount)    \
	NByteSwap_Field(_fieldCount, currentField, sizeof(int16_t), kNEndianLittle);

#define NBYTESWAP_L_Int32_Array(_fieldName, _fieldCount)    \
	NByteSwap_Field(_fieldCount, currentField, sizeof(int32_t), kNEndianLittle);

#define NBYTESWAP_L_Int64_Array(_fieldName, _fieldCount)    \
	NByteSwap_Field(_fieldCount, currentField, sizeof(int64_t), kNEndianLittle);

#define NBYTESWAP_L_Float32_Array(_fieldName, _fieldCount)  \
	NByteSwap_Field(_fieldCount, currentField, sizeof(float32_t), kNEndianLittle);

#define NBYTESWAP_L_Float64_Array(_fieldName, _fieldCount)  \
	NByteSwap_Field(_fieldCount, currentField, sizeof(float64_t), kNEndianLittle);


// Fields
#define NBYTESWAP_Type(_fieldName, _fieldType)              NBYTESWAP_Type_Array(_fieldName, _fieldType, 1)


#define NBYTESWAP_B_UInt8(_fieldName)                       NBYTESWAP_B_UInt8_Array(_fieldName, 1)
#define NBYTESWAP_B_UInt16(_fieldName)                      NBYTESWAP_B_UInt16_Array(_fieldName, 1)
#define NBYTESWAP_B_UInt32(_fieldName)                      NBYTESWAP_B_UInt32_Array(_fieldName, 1)
#define NBYTESWAP_B_UInt64(_fieldName)                      NBYTESWAP_B_UInt64_Array(_fieldName, 1)

#define NBYTESWAP_B_Int8(_fieldName)                        NBYTESWAP_B_Int8_Array(_fieldName, 1)
#define NBYTESWAP_B_Int16(_fieldName)                       NBYTESWAP_B_Int16_Array(_fieldName, 1)
#define NBYTESWAP_B_Int32(_fieldName)                       NBYTESWAP_B_Int32_Array(_fieldName, 1)
#define NBYTESWAP_B_Int64(_fieldName)                       NBYTESWAP_B_Int64_Array(_fieldName, 1)

#define NBYTESWAP_B_Float32(_fieldName)                     NBYTESWAP_B_Float32_Array(_fieldName, 1)
#define NBYTESWAP_B_Float64(_fieldName)                     NBYTESWAP_B_Float64_Array(_fieldName, 1)


#define NBYTESWAP_L_UInt8(_fieldName)                       NBYTESWAP_L_UInt8_Array(_fieldName, 1)
#define NBYTESWAP_L_UInt16(_fieldName)                      NBYTESWAP_L_UInt16_Array(_fieldName, 1)
#define NBYTESWAP_L_UInt32(_fieldName)                      NBYTESWAP_L_UInt32_Array(_fieldName, 1)
#define NBYTESWAP_L_UInt64(_fieldName)                      NBYTESWAP_L_UInt64_Array(_fieldName, 1)

#define NBYTESWAP_L_Int8(_fieldName)                        NBYTESWAP_L_Int8_Array(_fieldName, 1)
#define NBYTESWAP_L_Int16(_fieldName)                       NBYTESWAP_L_Int16_Array(_fieldName, 1)
#define NBYTESWAP_L_Int32(_fieldName)                       NBYTESWAP_L_Int32_Array(_fieldName, 1)
#define NBYTESWAP_L_Int64(_fieldName)                       NBYTESWAP_L_Int64_Array(_fieldName, 1)

#define NBYTESWAP_L_Float32(_fieldName)                     NBYTESWAP_L_Float32_Array(_fieldName, 1)
#define NBYTESWAP_L_Float64(_fieldName)                     NBYTESWAP_L_Float64_Array(_fieldName, 1)





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NByteSwap.inl"



#endif // NBYTE_SWAP_H
