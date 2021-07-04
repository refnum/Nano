/*	NAME:
		NPropertyList.h

	DESCRIPTION:
		Property list file.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
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
#ifndef NPROPERTY_LIST_H
#define NPROPERTY_LIST_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NXMLNode.h"
#include "NanoConstants.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Proeprty list format
enum class NPropertyListFormat
{
	Unknown,
	XML,
	Binary
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NAny;
class NArray;
class NData;
class NDictionary;
class NFile;
class NFile;
class NNumber;
class NString;
class NTime;

struct NPListBinaryTag;
struct NPListBinaryEncodeInfo;
struct NPListBinaryDecodeInfo;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NPropertyList
{
public:
	// Encode/decode a property list to XML
	static NString                      EncodeXML(const NDictionary& theState);
	static NDictionary                  DecodeXML(const NString& theXML);


	// Load/save a property list from a file
	static NDictionary                  Load(const NFile&        theFile);
	static NStatus                      Save(const NFile&        theFile,
											 const NDictionary&  theState,
											 NPropertyListFormat theFormat = NPropertyListFormat::Binary);


	// Encode/decode a property list
	static NData                        Encode(const NDictionary&  theState,
											   NPropertyListFormat theFormat = NPropertyListFormat::Binary);
	static NDictionary                  Decode(const NData& theData);


	// Identify a property list format
	static NPropertyListFormat          GetFormat(const NData& theData);


private:
	static NData                        EncodeToXML(    const NDictionary& theState);
	static NSharedPtrXMLNode            EncodeXML_Any(  const NAny&             theValue);
	static NSharedPtrXMLNode            EncodeXML_Array(const NArray&           theValue);
	static NSharedPtrXMLNode            EncodeXML_Boolean(bool                  theValue);
	static NSharedPtrXMLNode            EncodeXML_Data(      const NData&       theValue);
	static NSharedPtrXMLNode            EncodeXML_Dictionary(const NDictionary& theValue);
	static NSharedPtrXMLNode            EncodeXML_Number(    const NNumber&     theValue);
	static NSharedPtrXMLNode            EncodeXML_String(    const NString&     theValue);
	static NSharedPtrXMLNode            EncodeXML_Time(      const NTime&       theValue);

	static NDictionary                  DecodeFromXML(       const NData& theData);
	static NAny                         DecodeXML_Any(       const NSharedPtrXMLNode& theNode);
	static NArray                       DecodeXML_Array(     const NSharedPtrXMLNode& theNode);
	static bool                         DecodeXML_Boolean(   const NSharedPtrXMLNode& theNode);
	static NData                        DecodeXML_Data(      const NSharedPtrXMLNode& theNode);
	static NDictionary                  DecodeXML_Dictionary(const NSharedPtrXMLNode& theNode);
	static int64_t                      DecodeXML_Integer(   const NSharedPtrXMLNode& theNode);
	static float64_t                    DecodeXML_Real(      const NSharedPtrXMLNode& theNode);
	static NString                      DecodeXML_String(    const NSharedPtrXMLNode& theNode);
	static NTime                        DecodeXML_Time(      const NSharedPtrXMLNode& theNode);


	static NData                        EncodeToBinary(const NDictionary& theState);

	static NData                        EncodeBinary_GetIntegerList(const NVectorUInt64& theObjects, uint8_t integerSize);

	static uint8_t                      EncodeBinary_GetIntegerSize(uint64_t maxValue);

	static uint64_t                     EncodeBinary_GetObjectCount(const NAny& theValue);

	static void                         EncodeBinary_WriteObject(NPListBinaryEncodeInfo& encodeInfo);

	static void                         EncodeBinary_WriteObjectData(NPListBinaryEncodeInfo& encodeInfo,
																	 size_t                  theSize,
																	 const void*             thePtr);

	static void                         EncodeBinary_WriteObjectTag(NPListBinaryEncodeInfo& encodeInfo,
																	uint8_t                 theToken,
																	uint8_t                 objectInfo,
																	uint64_t                objectSize = 0);

	static uint64_t                     EncodeBinary_Any(NPListBinaryEncodeInfo& encodeInfo, const NAny& theValue);

	static void                         EncodeBinary_Array(NPListBinaryEncodeInfo& encodeInfo, const NArray& theValue);

	static void                         EncodeBinary_Boolean(NPListBinaryEncodeInfo& encodeInfo, bool theValue);

	static void                         EncodeBinary_Data(NPListBinaryEncodeInfo& encodeInfo, const NData& theValue);

	static void                         EncodeBinary_Dictionary(NPListBinaryEncodeInfo& encodeInfo,
																const NDictionary&      theValue);

	static void                         EncodeBinary_Integer(NPListBinaryEncodeInfo& encodeInfo,
															 const NNumber&          theValue,
															 bool                    addObject = true);

	static void                         EncodeBinary_Real(NPListBinaryEncodeInfo& encodeInfo, const NNumber& theValue);

	static void                         EncodeBinary_String(NPListBinaryEncodeInfo& encodeInfo, const NString& theValue);

	static void                         EncodeBinary_Time(NPListBinaryEncodeInfo& encodeInfo, const NTime& theValue);


	static NDictionary                  DecodeFromBinary(const NData& theData);

	static uint64_t                     DecodeBinary_GetObjectOffset(const NPListBinaryDecodeInfo& decodeInfo,
																	 uint64_t                      objectRef);

	static NVectorUInt64                DecodeBinary_GetObjectOffsets(uint64_t       numObjects,
																	  uint32_t       offsetsSize,
																	  const uint8_t* offsetsTable);

	static uint64_t                     DecodeBinary_GetUIntX(uint32_t integerSize, const uint8_t* thePtr);

	static uint64_t                     DecodeBinary_ReadObjectRef(const NPListBinaryDecodeInfo& decodeInfo,
																   uint64_t&                     byteOffset);

	static uint64_t                     DecodeBinary_ReadObjectSize(const NPListBinaryDecodeInfo& decodeInfo,
																	uint64_t&                     byteOffset,
																	const NPListBinaryTag&        theTag);

	static NPListBinaryTag              DecodeBinary_ReadObjectTag(const NPListBinaryDecodeInfo& decodeInfo,
																   uint64_t&                     byteOffset);

	static NAny                         DecodeBinary_Any(const NPListBinaryDecodeInfo& decodeInfo, uint64_t& byteOffset);

	static NArray                       DecodeBinary_Array(const NPListBinaryDecodeInfo& decodeInfo,
														   uint64_t&                     byteOffset);

	static bool                         DecodeBinary_Boolean(const NPListBinaryDecodeInfo& decodeInfo,
															 uint64_t&                     byteOffset);

	static NData                        DecodeBinary_Data(const NPListBinaryDecodeInfo& decodeInfo, uint64_t& byteOffset);

	static NDictionary                  DecodeBinary_Dictionary(const NPListBinaryDecodeInfo& decodeInfo,
																uint64_t&                     byteOffset);

	static int64_t                      DecodeBinary_Integer(const NPListBinaryDecodeInfo& decodeInfo,
															 uint64_t&                     byteOffset);

	static float64_t                    DecodeBinary_Real(const NPListBinaryDecodeInfo& decodeInfo,
														  uint64_t&                     byteOffset);

	static NString                      DecodeBinary_String(const NPListBinaryDecodeInfo& decodeInfo,
															uint64_t&                     byteOffset);

	static NTime                        DecodeBinary_Time(const NPListBinaryDecodeInfo& decodeInfo, uint64_t& byteOffset);
};



#endif // NPROPERTY_LIST_H
