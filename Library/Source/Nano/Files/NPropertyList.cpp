/*	NAME:
		NPropertyList.cpp

	DESCRIPTION:
		Property list file.

		The Mac XML property list format is documented at:

			http://www.apple.com/DTDs/PropertyList-1.0.dtd

		The Mac Binary property list format is documented at:

			http://opensource.apple.com/source/CF/CF-550/CFBinaryPList.c.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NPropertyList.h"

// Nano
#include "NArray.h"
#include "NByteSwap.h"
#include "NData.h"
#include "NDataEncoder.h"
#include "NDateFormatter.h"
#include "NFileHandle.h"
#include "NFormat.h"
#include "NStdAlgorithm.h"
#include "NTimeUtils.h"
#include "NXMLEncoder.h"
#include "NXMLNode.h"
#include "NanoTypes.h"

// System
#include <math.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Magic
inline constexpr uint8_t kNMagicXML[]                       = {0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20};
inline constexpr uint8_t kNMagicBinary[]                    = {0x62, 0x70, 0x6C, 0x69, 0x73, 0x74, 0x30, 0x30};


// XML
inline constexpr const char* kNPListSystemID1               = "http://www.apple.com/DTDs/PropertyList-1.0.dtd";
inline constexpr const char* kNPListPublicID1               = "-//Apple Computer//DTD PLIST 1.0//EN";

inline constexpr const char* kNXMLTokenArray                = "array";
inline constexpr const char* kNXMLTokenData                 = "data";
inline constexpr const char* kNXMLTokenTime                 = "date";
inline constexpr const char* kNXMLTokenDictionary           = "dict";
inline constexpr const char* kNXMLTokenFalse                = "false";
inline constexpr const char* kNXMLTokenInteger              = "integer";
inline constexpr const char* kNXMLTokenPList                = "plist";
inline constexpr const char* kNXMLTokenReal                 = "real";
inline constexpr const char* kNXMLTokenString               = "string";
inline constexpr const char* kNXMLTokenTrue                 = "true";

inline constexpr const char* kNXMLTokenKey                  = "key";
inline constexpr const char* kNXMLTokenVersion              = "version";


// Binary
inline constexpr uint8_t kNBinaryTokenNull                  = 0x00;
inline constexpr uint8_t kNBinaryTokenInteger               = 0x01;
inline constexpr uint8_t kNBinaryTokenReal                  = 0x02;
inline constexpr uint8_t kNBinaryTokenTime                  = 0x03;
inline constexpr uint8_t kNBinaryTokenData                  = 0x04;
inline constexpr uint8_t kNBinaryTokenStringASCII           = 0x05;
inline constexpr uint8_t kNBinaryTokenStringUTF16           = 0x06;
inline constexpr uint8_t kNBinaryTokenArray                 = 0x0A;
inline constexpr uint8_t kNBinaryTokenDictionary            = 0x0D;

inline constexpr uint8_t kNBinaryValueNull                  = 0x00;
inline constexpr uint8_t kNBinaryValueFill                  = 0x0F;
inline constexpr uint8_t kNBinaryValueFalse                 = 0x08;
inline constexpr uint8_t kNBinaryValueTrue                  = 0x09;
inline constexpr uint8_t kNBinaryValueSizeInteger           = 0x0F;

inline constexpr uint8_t kNBinaryValueInt8                  = 0x00;
inline constexpr uint8_t kNBinaryValueInt16                 = 0x01;
inline constexpr uint8_t kNBinaryValueInt32                 = 0x02;
inline constexpr uint8_t kNBinaryValueInt64                 = 0x03;

inline constexpr uint8_t kNBinaryValueFloat32               = 0x02;
inline constexpr uint8_t kNBinaryValueFloat64               = 0x03;





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
// Binary types
struct NPListBinaryTag
{
	uint8_t theToken;
	uint8_t objectInfo;
};

struct NPListBinaryEncodeInfo
{
	NVectorUInt64 theOffsets;
	NData         objectsTable;
	uint8_t       objectsSize;
};

struct NPListBinaryDecodeInfo
{
	NVectorUInt64  theOffsets;
	const uint8_t* objectsTable;
	uint8_t        objectsSize;
};

NN_STRUCT_PACK_1(NPListBinaryTrailer
				 {
					 uint8_t  reserved[5];
					 uint8_t  sortVersion;
					 uint8_t  offsetsSize;
					 uint8_t  objectsSize;
					 uint64_t numObjects;
					 uint64_t rootObject;
					 uint64_t offsetsTableOffset;
				 });

NBYTESWAP_BEGIN(NPListBinaryTrailer)
NBYTESWAP_B_UInt8_Array(reserved, 5)
NBYTESWAP_B_UInt8(sortVersion)
NBYTESWAP_B_UInt8(offsetsSize)
NBYTESWAP_B_UInt8(objectsSize)
NBYTESWAP_B_UInt64(numObjects)
NBYTESWAP_B_UInt64(rootObject)
NBYTESWAP_B_UInt64(offsetsTableOffset)
NBYTESWAP_END





//=============================================================================
//		NPropertyList::EncodeXML : Encode to XML.
//-----------------------------------------------------------------------------
NString NPropertyList::EncodeXML(const NDictionary& theState)
{
	// Encode to XML
	NData theData = Encode(theState, NPropertyListFormat::XML);

	return NString(NStringEncoding::UTF8, theData);
}





//=============================================================================
//		NPropertyList::DecodeXML : Decode from XML.
//-----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeXML(const NString& theXML)
{


	// Decode from XML
	return Decode(theXML.GetData(NStringEncoding::UTF8));
}





//=============================================================================
//		NPropertyList::Load : Load a property list.
//-----------------------------------------------------------------------------
NDictionary NPropertyList::Load(const NFile& theFile)
{


	// Load the file
	return Decode(NFileHandle::ReadData(theFile));
}





//=============================================================================
//		NPropertyList::Save : Save a property list.
//-----------------------------------------------------------------------------
NStatus NPropertyList::Save(const NFile&        theFile,
							const NDictionary&  theState,
							NPropertyListFormat theFormat)
{


	// Save the file
	NStatus theErr = NFileHandle::WriteData(theFile, Encode(theState, theFormat));
	NN_REQUIRE_NOT_ERR(theErr);

	return theErr;
}





//=============================================================================
//		NPropertyList::Encode : Encode a property list.
//-----------------------------------------------------------------------------
NData NPropertyList::Encode(const NDictionary& theState, NPropertyListFormat theFormat)
{


	// Encode the state
	switch (theFormat)
	{
		case NPropertyListFormat::XML:
			return EncodeToXML(theState);
			break;

		case NPropertyListFormat::Binary:
			return EncodeToBinary(theState);
			break;

		case NPropertyListFormat::Unknown:
			NN_LOG_ERROR("Unsupported format!");
			break;
	}

	return {};
}





//=============================================================================
//		NPropertyList::Decode : Decode a property list.
//-----------------------------------------------------------------------------
NDictionary NPropertyList::Decode(const NData& theData)
{


	// Get the state we need
	NPropertyListFormat theFormat = GetFormat(theData);
	NDictionary         theState;



	// Decode the state
	switch (theFormat)
	{
		case NPropertyListFormat::XML:
			theState = DecodeFromXML(theData);
			break;

		case NPropertyListFormat::Binary:
			theState = DecodeFromBinary(theData);
			break;

		case NPropertyListFormat::Unknown:
			NN_LOG_ERROR("Unsupported format!");
			break;
	}

	return theState;
}





//=============================================================================
//		NPropertyList::GetFormat : Identify a property list format.
//-----------------------------------------------------------------------------
NPropertyListFormat NPropertyList::GetFormat(const NData& theData)
{


	// Get the state we need
	size_t         dataSize = theData.GetSize();
	const uint8_t* dataPtr  = theData.GetData();



	// Check for XML
	if (dataSize >= sizeof(kNMagicXML))
	{
		if (memcmp(dataPtr, kNMagicXML, sizeof(kNMagicXML)) == 0)
		{
			return NPropertyListFormat::XML;
		}
	}



	// Check for binary
	if (dataSize >= sizeof(kNMagicBinary))
	{
		if (memcmp(dataPtr, kNMagicBinary, sizeof(kNMagicBinary)) == 0)
		{
			return NPropertyListFormat::Binary;
		}
	}

	return NPropertyListFormat::Unknown;
}





#pragma mark private
//=============================================================================
//		NPropertyList::EncodeToXML : Encode to NPropertyListFormat::XML.
//-----------------------------------------------------------------------------
NData NPropertyList::EncodeToXML(const NDictionary& theState)
{


	// Create the nodes
	NSharedXMLNode nodeDict = EncodeXML_Dictionary(theState);

	NSharedXMLNode nodePList = std::make_shared<NXMLNode>(NXMLNodeType::Element, kNXMLTokenPList);

	NSharedXMLNode nodeDocType = std::make_shared<NXMLNode>(NXMLNodeType::DocType, kNXMLTokenPList);

	NSharedXMLNode nodeDoc = std::make_shared<NXMLNode>(NXMLNodeType::Document, "");

	nodePList->SetElementAttribute(kNXMLTokenVersion, "1.0");
	nodePList->AddChild(nodeDict);

	nodeDocType->SetDocTypeSystemID(kNPListSystemID1);
	nodeDocType->SetDocTypePublicID(kNPListPublicID1);

	nodeDoc->AddChild(nodeDocType);
	nodeDoc->AddChild(nodePList);



	// Encode the XML
	NXMLEncoder theEncoder;

	NString theXML = theEncoder.Encode(nodeDoc);
	theXML.ReplaceAll("^\t", "", NStringFind::Pattern | NStringFind::MultiLine);

	return theXML.GetData(NStringEncoding::UTF8);
}





//=============================================================================
//		NPropertyList::EncodeXML_Any : Encode an NAny.
//-----------------------------------------------------------------------------
NSharedXMLNode NPropertyList::EncodeXML_Any(const NAny& theValue)
{


	// Encode the value
	NSharedXMLNode theNode;
	NNumber        theNumber;

	if (theValue.IsBool())
	{
		theNode = EncodeXML_Boolean(theValue.GetBool());
	}

	else if (theNumber.SetValue(theValue))
	{
		theNode = EncodeXML_Number(theNumber);
	}

	else if (theValue.IsString())
	{
		theNode = EncodeXML_String(theValue.GetString());
	}

	else if (theValue.IsData())
	{
		theNode = EncodeXML_Data(theValue.GetData());
	}

	else if (theValue.IsTime())
	{
		theNode = EncodeXML_Time(theValue.GetTime());
	}

	else if (theValue.IsArray())
	{
		theNode = EncodeXML_Array(theValue.GetArray());
	}

	else if (theValue.IsDictionary())
	{
		theNode = EncodeXML_Dictionary(theValue.GetDictionary());
	}

	else
	{
		NN_LOG_ERROR("Unknown property list value: {}", theValue.GetType().name());
	}

	return theNode;
}





//=============================================================================
//		NPropertyList::EncodeXML_Array : Encode an array.
//-----------------------------------------------------------------------------
NSharedXMLNode NPropertyList::EncodeXML_Array(const NArray& theValue)
{


	// Get the state we need
	NSharedXMLNode theNode = std::make_shared<NXMLNode>(NXMLNodeType::Element, kNXMLTokenArray);


	// Encode the values
	for (const auto& theItem : theValue)
	{
		NSharedXMLNode nodeItem = EncodeXML_Any(theItem);

		if (nodeItem)
		{
			theNode->AddChild(nodeItem);
		}
	}

	return theNode;
}





//=============================================================================
//		NPropertyList::EncodeXML_Boolean : Encode a bool.
//-----------------------------------------------------------------------------
NSharedXMLNode NPropertyList::EncodeXML_Boolean(bool theValue)
{


	// Encode the value
	NString        theToken = theValue ? kNXMLTokenTrue : kNXMLTokenFalse;
	NSharedXMLNode theNode  = std::make_shared<NXMLNode>(NXMLNodeType::Element, theToken);

	theNode->SetElementUnpaired(true);

	return theNode;
}





//=============================================================================
//		NPropertyList::EncodeXML_Data : Encode binary data.
//-----------------------------------------------------------------------------
NSharedXMLNode NPropertyList::EncodeXML_Data(const NData& theValue)
{


	// Get the state we need
	NString theText = NDataEncoder::Encode(NDataEncoding::Base64, theValue);



	// Encode the value
	NSharedXMLNode theNode = std::make_shared<NXMLNode>(NXMLNodeType::Element, kNXMLTokenData);

	theNode->SetElementContents(theText);

	return theNode;
}





//=============================================================================
//		NPropertyList::EncodeXML_Dictionary : Encode a dictionary.
//-----------------------------------------------------------------------------
NSharedXMLNode NPropertyList::EncodeXML_Dictionary(const NDictionary& theValue)
{


	// Get the state we need
	NSharedXMLNode theNode =
		std::make_shared<NXMLNode>(NXMLNodeType::Element, kNXMLTokenDictionary);



	// Get the keys
	NVectorString theKeys = theValue.GetKeys();

	nstd::sort(theKeys);



	// Encode the values
	for (const auto& theKey : theKeys)
	{
		const auto     iterItem = theValue.find(theKey);
		NSharedXMLNode nodeItem = EncodeXML_Any(iterItem->second);

		if (nodeItem)
		{
			NSharedXMLNode nodeKey =
				std::make_shared<NXMLNode>(NXMLNodeType::Element, kNXMLTokenKey);

			nodeKey->SetElementContents(theKey);

			theNode->AddChild(nodeKey);
			theNode->AddChild(nodeItem);
		}
	}

	return theNode;
}





//=============================================================================
//		NPropertyList::EncodeXML_Number : Encode a number.
//-----------------------------------------------------------------------------
NSharedXMLNode NPropertyList::EncodeXML_Number(const NNumber& theValue)
{


	// Get the state we need
	//
	// The XML property list format requires specific values for nan/info.
	NString theToken, theText;

	if (theValue.IsInteger())
	{
		theToken = kNXMLTokenInteger;
		theText  = NFormat("{}", theValue);
	}
	else
	{
		theToken               = kNXMLTokenReal;
		float64_t valueFloat64 = theValue.GetFloat64();

		if (isinf(valueFloat64))
		{
			if (valueFloat64 < 0.0)
			{
				theText = "-infinity";
			}
			else
			{
				theText = "+infinity";
			}
		}
		else if (isnan(valueFloat64))
		{
			theText = "nan";
		}
		else
		{
			theText = NFormat("{}", valueFloat64);
		}
	}



	// Encode the value
	NSharedXMLNode theNode = std::make_shared<NXMLNode>(NXMLNodeType::Element, theToken);

	theNode->SetElementContents(theText);

	return theNode;
}





//=============================================================================
//		NPropertyList::EncodeXML_String : Encode a string.
//-----------------------------------------------------------------------------
NSharedXMLNode NPropertyList::EncodeXML_String(const NString& theValue)
{


	// Encode the value
	NSharedXMLNode theNode = std::make_shared<NXMLNode>(NXMLNodeType::Element, kNXMLTokenString);

	theNode->SetElementContents(theValue);

	return theNode;
}





//=============================================================================
//		NPropertyList::EncodeXML_Time : Encode a time.
//-----------------------------------------------------------------------------
NSharedXMLNode NPropertyList::EncodeXML_Time(const NTime& theValue)
{


	// Get the state we ned
	NDateFormatter theFormatter;

	theFormatter.SetDate(theValue);
	NString theText = theFormatter.Format("yyyy-MM-dd'T'HH:mm:ss'Z'");



	// Encode the value
	NSharedXMLNode theNode = std::make_shared<NXMLNode>(NXMLNodeType::Element, kNXMLTokenTime);

	theNode->SetElementContents(theText);

	return theNode;
}





//=============================================================================
//		NPropertyList::DecodeFromXML : Decode from NPropertyListFormat::XML.
//-----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeFromXML(const NData& theValue)
{


	// Decode the XML
	NXMLEncoder theEncoder;

	NString        theXML(NStringEncoding::UTF8, theValue);
	NSharedXMLNode nodeDoc = theEncoder.Decode(theXML);



	// Locate the root dictionary
	NSharedXMLNode nodeDict;

	if (nodeDoc)
	{
		for (const auto& theChild : nodeDoc->GetChildren())
		{
			if (theChild->IsElement(kNXMLTokenPList))
			{
				nodeDict = theChild->GetChild(0);

				NN_REQUIRE(theChild->GetChildren().size() == 1);
				NN_REQUIRE(nodeDict->IsElement(kNXMLTokenDictionary));
				break;
			}
		}
	}



	// Decode the XML
	if (nodeDict)
	{
		return DecodeXML_Dictionary(nodeDict);
	}

	return {};
}





//=============================================================================
//		NPropertyList::DecodeXML_Any : Decode a NAny.
//-----------------------------------------------------------------------------
NAny NPropertyList::DecodeXML_Any(const NSharedXMLNode& theNode)
{


	// Decode the value
	NAny theValue;

	if (theNode->IsElement(kNXMLTokenTrue) || theNode->IsElement(kNXMLTokenFalse))
	{
		theValue = DecodeXML_Boolean(theNode);
	}

	else if (theNode->IsElement(kNXMLTokenInteger))
	{
		theValue = DecodeXML_Integer(theNode);
	}

	else if (theNode->IsElement(kNXMLTokenReal))
	{
		theValue = DecodeXML_Real(theNode);
	}

	else if (theNode->IsElement(kNXMLTokenString))
	{
		theValue = DecodeXML_String(theNode);
	}

	else if (theNode->IsElement(kNXMLTokenData))
	{
		theValue = DecodeXML_Data(theNode);
	}

	else if (theNode->IsElement(kNXMLTokenTime))
	{
		theValue = DecodeXML_Time(theNode);
	}

	else if (theNode->IsElement(kNXMLTokenArray))
	{
		theValue = DecodeXML_Array(theNode);
	}

	else if (theNode->IsElement(kNXMLTokenDictionary))
	{
		theValue = DecodeXML_Dictionary(theNode);
	}

	else
	{
		NN_LOG_ERROR("Unknown property list value: {}", theNode->GetTextValue());
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeXML_Array : Decode an array.
//-----------------------------------------------------------------------------
NArray NPropertyList::DecodeXML_Array(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->IsElement(kNXMLTokenArray));



	// Decode the value
	NArray theValue;

	for (const auto& theChild : theNode->GetChildren())
	{
		NAny theItem = DecodeXML_Any(theChild);

		if (!theItem.IsEmpty())
		{
			theValue.emplace_back(theItem);
		}
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeXML_Boolean : Decode a bool.
//-----------------------------------------------------------------------------
bool NPropertyList::DecodeXML_Boolean(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->IsElement(kNXMLTokenTrue) || theNode->IsElement(kNXMLTokenFalse));



	// Decode the value
	return theNode->IsElement(kNXMLTokenTrue);
}





//=============================================================================
//		NPropertyList::DecodeXML_Data : Decode binary data.
//-----------------------------------------------------------------------------
NData NPropertyList::DecodeXML_Data(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->IsElement(kNXMLTokenData));



	// Decode the value
	return NDataEncoder::Decode(NDataEncoding::Base64, theNode->GetElementContents());
}





//=============================================================================
//		NPropertyList::DecodeXML_Dictionary : Decode a dictionary.
//-----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeXML_Dictionary(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->IsElement(kNXMLTokenDictionary));



	// Get the state we need
	NVectorSharedXMLNode theChildren = theNode->GetChildren();
	NN_REQUIRE(NMathUtils::IsEven(theChildren.size()));



	// Decode the value
	NDictionary theValue;

	for (auto theIter = theChildren.begin(); theIter != theChildren.end();)
	{
		// Get the key-value nodes
		NSharedXMLNode nodeKey   = *theIter++;
		NSharedXMLNode nodeValue = *theIter++;

		NN_REQUIRE(nodeKey->IsElement(kNXMLTokenKey));



		// Decode the key-value pair
		NString itemKey   = nodeKey->GetElementContents();
		NAny    itemValue = DecodeXML_Any(nodeValue);

		NN_REQUIRE(!itemKey.IsEmpty());
		NN_REQUIRE(!itemValue.IsEmpty());

		theValue[itemKey] = itemValue;
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeXML_Integer : Decode an integer.
//-----------------------------------------------------------------------------
int64_t NPropertyList::DecodeXML_Integer(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->IsElement(kNXMLTokenInteger));



	// Decode the value
	int64_t theValue = 0;
	NNumber theNumber;

	if (theNumber.SetValue(theNode->GetElementContents()) && theNumber.IsInteger())
	{
		theValue = theNumber.GetInt64();
	}
	else
	{
		NN_LOG_WARNING("Unable to extract integer: [{}]", theNode->GetElementContents());
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeXML_Real : Decode a floating point value.
//-----------------------------------------------------------------------------
float64_t NPropertyList::DecodeXML_Real(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->IsElement(kNXMLTokenReal));



	// Decode the value
	float64_t theValue = 0.0;
	NNumber   theNumber;

	if (theNumber.SetValue(theNode->GetElementContents()))
	{
		theValue = theNumber.GetFloat64();
	}
	else
	{
		NN_LOG_WARNING("Unable to extract real: [{}]", theNode->GetElementContents());
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeXML_String : Decode a string.
//-----------------------------------------------------------------------------
NString NPropertyList::DecodeXML_String(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->IsElement(kNXMLTokenString));



	// Decode the value
	return theNode->GetElementContents();
}





//=============================================================================
//		NPropertyList::DecodeXML_Time : Decode a time.
//-----------------------------------------------------------------------------
NTime NPropertyList::DecodeXML_Time(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->IsElement(kNXMLTokenTime));



	// Decode the value
	int valueYear  = 0;
	int valueMonth = 0;
	int valueDay   = 0;

	int valueHour   = 0;
	int valueMinute = 0;
	int valueSecond = 0;

	NString theText = theNode->GetElementContents();

	int numValues = sscanf(theText.GetUTF8(),
						   "%04d-%02d-%02dT%02d:%02d:%02dZ",
						   &valueYear,
						   &valueMonth,
						   &valueDay,
						   &valueHour,
						   &valueMinute,
						   &valueSecond);

	NN_EXPECT(numValues == 6);
	NN_EXPECT(valueMonth >= 1 && valueMonth <= 12);
	NN_EXPECT(valueDay >= 1 && valueDay <= 31);
	NN_EXPECT(valueHour >= 0 && valueHour <= 23);
	NN_EXPECT(valueMinute >= 0 && valueMinute <= 59);
	NN_EXPECT(valueSecond >= 0 && valueSecond <= 59);



	// Convert to time
	NTime theValue = 0.0;

	if (numValues == 6)
	{
		struct tm timeUTC
		{
		};

		timeUTC.tm_year = valueYear - 1900;
		timeUTC.tm_mon  = valueMonth - 1;
		timeUTC.tm_mday = valueDay;
		timeUTC.tm_hour = valueHour;
		timeUTC.tm_min  = valueMinute;
		timeUTC.tm_sec  = valueSecond;

		theValue = NTimeUtils::ToTime(timeUTC);
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::EncodeToBinary : Encode to NPropertyListFormat::Binary.
//-----------------------------------------------------------------------------
NData NPropertyList::EncodeToBinary(const NDictionary& theState)
{


	// Encode the object table
	//
	// Binary plists do not attempt to share references - two strings with the same
	// underlying shared data will be written out twice.
	//
	// This could be improved by performing a first pass through the values to identify
	// any objects that had the same value (ideally comparing by value, not the underlying
	// shared data: two strings with the same text could be combined into the same object
	// even if they are two separate objects before encoding).
	//
	// Our current approach means that our pre-flight just Is to count the number of
	// objects we'll encode, as this gives us the size for encoding object references.
	NPListBinaryEncodeInfo encodeInfo{};

	uint64_t numObjects = EncodeBinary_GetObjectCount(theState);

	encodeInfo.objectsTable = NData(sizeof(kNMagicBinary), kNMagicBinary);
	encodeInfo.objectsSize  = EncodeBinary_GetIntegerSize(numObjects);

	uint64_t rootObject = EncodeBinary_Any(encodeInfo, theState);



	// Encode the offset table
	uint64_t objectsTableSize = encodeInfo.objectsTable.GetSize();
	uint8_t  offsetsSize      = EncodeBinary_GetIntegerSize(objectsTableSize);
	NData    offsetTable      = EncodeBinary_GetIntegerList(encodeInfo.theOffsets, offsetsSize);



	// Encode the trailer
	NPListBinaryTrailer theTrailer{};

	theTrailer.offsetsSize        = offsetsSize;
	theTrailer.objectsSize        = encodeInfo.objectsSize;
	theTrailer.numObjects         = encodeInfo.theOffsets.size();
	theTrailer.rootObject         = rootObject;
	theTrailer.offsetsTableOffset = objectsTableSize;

	NBYTESWAP_ENCODE(1, NPListBinaryTrailer, &theTrailer);



	// Construct the property list
	NData theData;

	theData.Append(encodeInfo.objectsTable);
	theData.Append(offsetTable);
	theData.Append(sizeof(theTrailer), &theTrailer);

	return theData;
}





//=============================================================================
//		NPropertyList::EncodeBinary_GetIntegerList : Get an integer list.
//-----------------------------------------------------------------------------
NData NPropertyList::EncodeBinary_GetIntegerList(const NVectorUInt64& theObjects,
												 uint8_t              integerSize)
{


	// Get the object references
	NData theData;

	theData.SetCapacity(theObjects.size() * sizeof(uint64_t));

	switch (integerSize)
	{
		case 1:
			for (uint64_t theObject : theObjects)
			{
				NN_REQUIRE(theObject <= kNUInt8Max);
				uint8_t value8 = uint8_t(theObject);
				theData.Append(sizeof(value8), &value8);
			}
			break;

		case 2:
			for (uint64_t theObject : theObjects)
			{
				NN_REQUIRE(theObject <= kNUInt16Max);
				uint16_t value16 = NByteSwap::SwapNtoB(uint16_t(theObject));
				theData.Append(sizeof(value16), &value16);
			}
			break;

		case 4:
			for (uint64_t theObject : theObjects)
			{
				NN_REQUIRE(theObject <= kNUInt32Max);
				uint32_t value32 = NByteSwap::SwapNtoB(uint32_t(theObject));
				theData.Append(sizeof(value32), &value32);
			}
			break;

		case 8:
			for (uint64_t theObject : theObjects)
			{
				uint64_t value64 = NByteSwap::SwapNtoB(uint64_t(theObject));
				theData.Append(sizeof(value64), &value64);
			}
			break;

		default:
			NN_LOG_ERROR("Unknown integer size {}", integerSize);
			NN_UNREACHABLE();
			break;
	}

	return theData;
}





//=============================================================================
//		NPropertyList::EncodeBinary_GetIntegerSize : Get an integer's size.
//-----------------------------------------------------------------------------
uint8_t NPropertyList::EncodeBinary_GetIntegerSize(uint64_t maxValue)
{


	// Get the required size
	if (maxValue <= kNUInt8Max)
	{
		return 1;
	}

	else if (maxValue <= kNUInt16Max)
	{
		return 2;
	}

	else if (maxValue <= kNUInt32Max)
	{
		return 4;
	}

	return 8;
}





//=============================================================================
//		NPropertyList::EncodeBinary_GetObjectCount : Count the objects.
//-----------------------------------------------------------------------------
uint64_t NPropertyList::EncodeBinary_GetObjectCount(const NAny& theValue)
{


	// Count the objects
	//
	// Each value generates one object.
	//
	// Arrays and dictionaries also generate an object for each of their values,
	// with dictionaries also generating objects for each of their keys.
	uint64_t numObjects = 1;

	if (theValue.IsArray())
	{
		for (const auto& childValue : theValue.GetArray())
		{
			numObjects += EncodeBinary_GetObjectCount(childValue);
		}
	}

	else if (theValue.IsDictionary())
	{
		for (const auto& keyValue : theValue.GetDictionary())
		{
			numObjects += 1;
			numObjects += EncodeBinary_GetObjectCount(keyValue.second);
		}
	}

	return numObjects;
}





//=============================================================================
//		NPropertyList::EncodeBinary_WriteObject : Write an object.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_WriteObject(NPListBinaryEncodeInfo& encodeInfo)
{


	// Add the object
	//
	// Object offsets are byte offsets, relative to the start of the object table.
	uint64_t theOffset = encodeInfo.objectsTable.GetSize();

	encodeInfo.theOffsets.push_back(theOffset);
}





//=============================================================================
//		NPropertyList::EncodeBinary_WriteObjectData : Write object data.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_WriteObjectData(NPListBinaryEncodeInfo& encodeInfo,
												 size_t                  theSize,
												 const void*             thePtr)
{


	// Add the data
	encodeInfo.objectsTable.Append(theSize, thePtr);
}





//=============================================================================
//		NPropertyList::EncodeBinary_WriteObjectTag : Write an object tag.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_WriteObjectTag(NPListBinaryEncodeInfo& encodeInfo,
												uint8_t                 theToken,
												uint8_t                 objectInfo,
												uint64_t                objectSize)
{


	// Validate our parameters
	NN_REQUIRE(theToken <= 0x0F);
	NN_REQUIRE(objectInfo <= 0x0F);



	// Get the state we need
	if (objectSize != 0)
	{
		NN_REQUIRE(objectInfo == 0);

		if (objectSize >= kNBinaryValueSizeInteger)
		{
			objectInfo = kNBinaryValueSizeInteger;
		}
		else
		{
			objectInfo = uint8_t(objectSize);
		}
	}



	// Add the tag
	//
	// Each object starts with a tag byte, which is divided into an identifying
	// token (high nibble) and 4 bits of object-specific state (low nibble).
	uint8_t theTag = uint8_t((theToken << 4) | objectInfo);

	encodeInfo.objectsTable.Append(sizeof(theTag), &theTag);



	// Add the size
	//
	// If a size is provided then it will be stored in the object-specific state.
	//
	// if the size exceeds 4 bits then it is appended as an encoded integer.
	if (objectSize != 0 && objectInfo == kNBinaryValueSizeInteger)
	{
		EncodeBinary_Integer(encodeInfo, objectSize, false);
	}
}





//=============================================================================
//		NPropertyList::EncodeBinary_Any : Encode an NAny.
//-----------------------------------------------------------------------------
uint64_t NPropertyList::EncodeBinary_Any(NPListBinaryEncodeInfo& encodeInfo, const NAny& theValue)
{


	// Encode the value
	NNumber theNumber;

	if (theValue.IsBool())
	{
		EncodeBinary_Boolean(encodeInfo, theValue.GetBool());
	}

	else if (theNumber.SetValue(theValue))
	{
		if (theNumber.IsInteger())
		{
			EncodeBinary_Integer(encodeInfo, theNumber);
		}
		else
		{
			EncodeBinary_Real(encodeInfo, theNumber);
		}
	}

	else if (theValue.IsString())
	{
		EncodeBinary_String(encodeInfo, theValue.GetString());
	}

	else if (theValue.IsData())
	{
		EncodeBinary_Data(encodeInfo, theValue.GetData());
	}

	else if (theValue.IsTime())
	{
		EncodeBinary_Time(encodeInfo, theValue.GetTime());
	}

	else if (theValue.IsArray())
	{
		EncodeBinary_Array(encodeInfo, theValue.GetArray());
	}

	else if (theValue.IsDictionary())
	{
		EncodeBinary_Dictionary(encodeInfo, theValue.GetDictionary());
	}

	else
	{
		NN_LOG_ERROR("Unknown property list value: {}", theValue.GetType().name());
	}



	// Get the object
	uint64_t objectRef = uint64_t(encodeInfo.theOffsets.size() - 1);

	return objectRef;
}





//=============================================================================
//		NPropertyList::EncodeBinary_Array : Encode an array.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_Array(NPListBinaryEncodeInfo& encodeInfo, const NArray& theValue)
{


	// Encode the objects
	NVectorUInt64 valueRefs;

	for (const auto& theItem : theValue)
	{
		valueRefs.push_back(EncodeBinary_Any(encodeInfo, theItem));
	}



	// Encode the value
	NData valueData = EncodeBinary_GetIntegerList(valueRefs, encodeInfo.objectsSize);

	EncodeBinary_WriteObject(encodeInfo);
	EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenArray, 0, theValue.size());
	EncodeBinary_WriteObjectData(encodeInfo, valueData.GetSize(), valueData.GetData());
}





//=============================================================================
//		NPropertyList::EncodeBinary_Boolean : Encode a boolean.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_Boolean(NPListBinaryEncodeInfo& encodeInfo, bool theValue)
{


	// Encode the value
	EncodeBinary_WriteObject(encodeInfo);

	if (theValue)
	{
		EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenNull, kNBinaryValueTrue);
	}
	else
	{
		EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenNull, kNBinaryValueFalse);
	}
}





//=============================================================================
//		NPropertyList::EncodeBinary_Data : Encode data.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_Data(NPListBinaryEncodeInfo& encodeInfo, const NData& theValue)
{


	// Encode the value
	EncodeBinary_WriteObject(encodeInfo);
	EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenData, 0, theValue.GetSize());
	EncodeBinary_WriteObjectData(encodeInfo, theValue.GetSize(), theValue.GetData());
}





//=============================================================================
//		NPropertyList::EncodeBinary_Dictionary : Encode a dictionary.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_Dictionary(NPListBinaryEncodeInfo& encodeInfo,
											const NDictionary&      theValue)
{


	// Encode the objects
	NVectorUInt64 keyRefs;
	NVectorUInt64 valueRefs;

	for (const auto& keyValue : theValue)
	{
		keyRefs.push_back(EncodeBinary_Any(encodeInfo, keyValue.first));
	}

	for (const auto& keyValue : theValue)
	{
		valueRefs.push_back(EncodeBinary_Any(encodeInfo, keyValue.second));
	}



	// Encode the value
	NData keyData   = EncodeBinary_GetIntegerList(keyRefs, encodeInfo.objectsSize);
	NData valueData = EncodeBinary_GetIntegerList(valueRefs, encodeInfo.objectsSize);

	EncodeBinary_WriteObject(encodeInfo);
	EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenDictionary, 0, theValue.size());
	EncodeBinary_WriteObjectData(encodeInfo, keyData.GetSize(), keyData.GetData());
	EncodeBinary_WriteObjectData(encodeInfo, valueData.GetSize(), valueData.GetData());
}





//=============================================================================
//		NPropertyList::EncodeBinary_Integer : Encode an integer.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_Integer(NPListBinaryEncodeInfo& encodeInfo,
										 const NNumber&          theValue,
										 bool                    addObject)
{


	// Validate our parameters
	NN_REQUIRE(theValue.IsInteger());


	// Add the object
	//
	// Integers are used to store variable-sized sizes for other objects, so
	// we may not need to start a new object.
	if (addObject)
	{
		EncodeBinary_WriteObject(encodeInfo);
	}



	// Encode the value
	//
	// Signed values are stored as 8 byte big-endian integers.
	//
	// Unsigned values are stored in the smallest big-endian integer
	// that can hold their value.
	int64_t value64 = theValue.GetInt64();

	if (value64 >= 0 && value64 <= kNUInt8Max)
	{
		uint8_t value8 = uint8_t(value64);

		EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenInteger, kNBinaryValueInt8);
		EncodeBinary_WriteObjectData(encodeInfo, sizeof(value8), &value8);
	}

	else if (value64 >= 0 && value64 <= kNUInt16Max)
	{
		uint16_t value16 = NByteSwap::SwapNtoB(uint16_t(value64));

		EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenInteger, kNBinaryValueInt16);
		EncodeBinary_WriteObjectData(encodeInfo, sizeof(value16), &value16);
	}

	else if (value64 >= 0 && value64 <= kNUInt32Max)
	{
		uint32_t value32 = NByteSwap::SwapNtoB(uint32_t(value64));

		EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenInteger, kNBinaryValueInt32);
		EncodeBinary_WriteObjectData(encodeInfo, sizeof(value32), &value32);
	}

	else
	{
		value64 = NByteSwap::SwapNtoB(value64);

		EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenInteger, kNBinaryValueInt64);
		EncodeBinary_WriteObjectData(encodeInfo, sizeof(value64), &value64);
	}
}





//=============================================================================
//		NPropertyList::EncodeBinary_Real : Encode a floating point value.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_Real(NPListBinaryEncodeInfo& encodeInfo, const NNumber& theValue)
{


	// Validate our parameters
	NN_REQUIRE(theValue.IsReal());


	// Encode the value
	EncodeBinary_WriteObject(encodeInfo);

	float64_t value64 = theValue.GetFloat64();
	NByteSwap::SwapNtoB(&value64);

	EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenReal, kNBinaryValueFloat64);
	EncodeBinary_WriteObjectData(encodeInfo, sizeof(value64), &value64);
}





//=============================================================================
//		NPropertyList::EncodeBinary_String : Encode a string.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_String(NPListBinaryEncodeInfo& encodeInfo, const NString& theValue)
{


	// Get the state we need
	//
	// Strings can be encoded as ASCII or as UTF16, depending on content.
	uint8_t theToken = kNBinaryTokenStringASCII;
	NData   theData  = theValue.GetData(NStringEncoding::UTF8);

	size_t         theSize = theData.GetSize();
	const uint8_t* thePtr  = theData.GetData();

	for (size_t n = 0; n < theSize; n++)
	{
		if (thePtr[n] >= 0x80)
		{
			theToken = kNBinaryTokenStringUTF16;
			theData  = theValue.GetData(NStringEncoding::UTF16);
			theSize  = theData.GetSize() / sizeof(utf16_t);

			NByteSwap::Swap(theSize, sizeof(uint16_t), theData.GetMutableData());
			break;
		}
	}



	// Encode the value
	EncodeBinary_WriteObject(encodeInfo);
	EncodeBinary_WriteObjectTag(encodeInfo, theToken, 0, theSize);
	EncodeBinary_WriteObjectData(encodeInfo, theData.GetSize(), theData.GetData());
}





//=============================================================================
//		NPropertyList::EncodeBinary_Time : Encode a time.
//-----------------------------------------------------------------------------
void NPropertyList::EncodeBinary_Time(NPListBinaryEncodeInfo& encodeInfo, const NTime& theValue)
{


	// Get the state we need
	float64_t value64 = theValue;
	NByteSwap::SwapNtoB(&value64);



	// Encode the value
	EncodeBinary_WriteObject(encodeInfo);
	EncodeBinary_WriteObjectTag(encodeInfo, kNBinaryTokenTime, kNBinaryValueFloat64);
	EncodeBinary_WriteObjectData(encodeInfo, sizeof(value64), &value64);
}





//=============================================================================
//		NPropertyList::DecodeFromBinary : Decode from NPropertyListFormat::Binary.
//-----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeFromBinary(const NData& theData)
{


	// Validate our state
	static_assert(sizeof(NPListBinaryTrailer) == 32);



	// Decode the trailer
	NPListBinaryTrailer theTrailer;

	if (theData.GetSize() < sizeof(theTrailer))
	{
		return {};
	}

	memcpy(&theTrailer,
		   theData.GetData(theData.GetSize() - sizeof(theTrailer)),
		   sizeof(theTrailer));
	NBYTESWAP_DECODE(1, NPListBinaryTrailer, &theTrailer);



	// Decode the object table
	NPListBinaryDecodeInfo decodeInfo;

	if constexpr (NN_ARCH_32)
	{
		static_assert(NN_ARCH_64 || sizeof(size_t) == sizeof(uint32_t));
		NN_REQUIRE(theTrailer.offsetsTableOffset <= uint64_t(kNUInt32Max));
	}

	const uint8_t* offsetsTable = theData.GetData(size_t(theTrailer.offsetsTableOffset));

	decodeInfo.theOffsets =
		DecodeBinary_GetObjectOffsets(theTrailer.numObjects, theTrailer.offsetsSize, offsetsTable);
	decodeInfo.objectsSize  = theTrailer.objectsSize;
	decodeInfo.objectsTable = theData.GetData();



	// Decode the root object
	uint64_t rootOffset = DecodeBinary_GetObjectOffset(decodeInfo, theTrailer.rootObject);
	NAny     rootObject = DecodeBinary_Any(decodeInfo, rootOffset);

	NN_REQUIRE(rootObject.Is<NDictionary>());

	return rootObject.GetDictionary();
}





//=============================================================================
//		NPropertyList::DecodeBinary_GetObjectOffset : Get an object offset.
//-----------------------------------------------------------------------------
uint64_t NPropertyList::DecodeBinary_GetObjectOffset(const NPListBinaryDecodeInfo& decodeInfo,
													 uint64_t                      objectRef)
{


	// Get the offset
	uint64_t theOffset = 0;

	if (objectRef < decodeInfo.theOffsets.size())
	{
		theOffset = decodeInfo.theOffsets[size_t(objectRef)];
	}
	else
	{
		NN_LOG_ERROR("Invalid object reference: {}", objectRef);
	}

	return theOffset;
}





//=============================================================================
//		NPropertyList::DecodeBinary_GetObjectOffsets : Get the object offsets.
//-----------------------------------------------------------------------------
NVectorUInt64 NPropertyList::DecodeBinary_GetObjectOffsets(uint64_t       numObjects,
														   uint32_t       offsetsSize,
														   const uint8_t* offsetsTable)
{


	// Validate our parameters
	if constexpr (NN_ARCH_32)
	{
		static_assert(NN_ARCH_64 || sizeof(size_t) == sizeof(uint32_t));
		NN_REQUIRE(numObjects <= uint64_t(kNUInt32Max));
	}



	// Decode the object references table
	NVectorUInt64 theOffsets;

	theOffsets.reserve(size_t(numObjects));

	for (size_t n = 0; n < numObjects; n++)
	{
		theOffsets.push_back(DecodeBinary_GetUIntX(offsetsSize, offsetsTable));
		offsetsTable += offsetsSize;
	}

	return theOffsets;
}





//=============================================================================
//		NPropertyList::DecodeBinary_GetUIntX : Get an integer.
//-----------------------------------------------------------------------------
uint64_t NPropertyList::DecodeBinary_GetUIntX(uint32_t integerSize, const uint8_t* thePtr)
{


	// Read the value
	uint16_t value16  = 0;
	uint32_t value32  = 0;
	uint64_t value64  = 0;
	uint64_t theValue = 0;

	switch (integerSize)
	{
		case 1:
			theValue = thePtr[0];
			break;

		case 2:
			memcpy(&value16, thePtr, sizeof(value16));
			theValue = NByteSwap::SwapBtoN(value16);
			break;

		case 3:
			theValue = (uint32_t(thePtr[0]) << 16) | (uint32_t(thePtr[1]) << 8) |
					   (uint32_t(thePtr[2]) << 0);
			break;

		case 4:
			memcpy(&value32, thePtr, sizeof(value32));
			theValue = NByteSwap::SwapBtoN(value32);
			break;

		case 8:
			memcpy(&value64, thePtr, sizeof(value64));
			theValue = NByteSwap::SwapBtoN(value64);
			break;

		default:
			NN_LOG_ERROR("Unknown integer size {}", integerSize);
			NN_UNREACHABLE();
			break;
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeBinary_ReadObjectRef : Read an object reference.
//-----------------------------------------------------------------------------
uint64_t NPropertyList::DecodeBinary_ReadObjectRef(const NPListBinaryDecodeInfo& decodeInfo,
												   uint64_t&                     byteOffset)
{


	// Get the state we need



	// Get the value
	const uint8_t* thePtr    = decodeInfo.objectsTable + byteOffset;
	uint64_t       objectRef = DecodeBinary_GetUIntX(decodeInfo.objectsSize, thePtr);

	byteOffset += decodeInfo.objectsSize;

	return objectRef;
}





//=============================================================================
//		NPropertyList::DecodeBinary_ReadObjectSize : Read an object size.
//-----------------------------------------------------------------------------
uint64_t NPropertyList::DecodeBinary_ReadObjectSize(const NPListBinaryDecodeInfo& decodeInfo,
													uint64_t&                     byteOffset,
													const NPListBinaryTag&        theTag)
{


	// Get the size
	//
	// Variable-sized fields store their size in their objectInfo, which
	// requires a subsequent integer if more than 4 bits are required.
	uint64_t theSize = 0;

	if (theTag.objectInfo == kNBinaryValueSizeInteger)
	{
		theSize = uint64_t(DecodeBinary_Integer(decodeInfo, byteOffset));
	}
	else
	{
		theSize = theTag.objectInfo;
	}

	return theSize;
}





//=============================================================================
//		NPropertyList::DecodeBinary_ReadObjectTag : Read an object tag.
//-----------------------------------------------------------------------------
NPListBinaryTag NPropertyList::DecodeBinary_ReadObjectTag(const NPListBinaryDecodeInfo& decodeInfo,
														  uint64_t&                     byteOffset)
{


	// Get the state we need
	const uint8_t* thePtr = decodeInfo.objectsTable + byteOffset;



	// Get the value
	//
	// Each object starts with a tag byte, which is divided into an identifying
	// token (high nibble) and 4 bits of object-specific state (low nibble).
	NPListBinaryTag theTag{};

	uint8_t theByte   = thePtr[0];
	theTag.theToken   = uint8_t((theByte >> 4) & 0x0F);
	theTag.objectInfo = uint8_t((theByte >> 0) & 0x0F);

	byteOffset += 1;

	return theTag;
}





//=============================================================================
//		NPropertyList::DecodeBinary_Any : Decode an NAny.
//-----------------------------------------------------------------------------
NAny NPropertyList::DecodeBinary_Any(const NPListBinaryDecodeInfo& decodeInfo, uint64_t& byteOffset)
{


	// Read the tag
	NPListBinaryTag theTag = DecodeBinary_ReadObjectTag(decodeInfo, byteOffset);
	byteOffset--;



	// Decode the value
	NAny theValue;

	switch (theTag.theToken)
	{
		case kNBinaryTokenNull:
			switch (theTag.objectInfo)
			{
				case kNBinaryValueNull:
				case kNBinaryValueFill:
					// Null object, ignore
					byteOffset++;
					break;

				case kNBinaryValueFalse:
				case kNBinaryValueTrue:
					theValue = DecodeBinary_Boolean(decodeInfo, byteOffset);
					break;

				default:
					NN_LOG_ERROR("Unknown null token: {}", theTag.objectInfo);
					break;
			}
			break;

		case kNBinaryTokenInteger:
			theValue = DecodeBinary_Integer(decodeInfo, byteOffset);
			break;

		case kNBinaryTokenReal:
			theValue = DecodeBinary_Real(decodeInfo, byteOffset);
			break;

		case kNBinaryTokenStringASCII:
		case kNBinaryTokenStringUTF16:
			theValue = DecodeBinary_String(decodeInfo, byteOffset);
			break;

		case kNBinaryTokenData:
			theValue = DecodeBinary_Data(decodeInfo, byteOffset);
			break;

		case kNBinaryTokenTime:
			theValue = DecodeBinary_Time(decodeInfo, byteOffset);
			break;

		case kNBinaryTokenArray:
			theValue = DecodeBinary_Array(decodeInfo, byteOffset);
			break;

		case kNBinaryTokenDictionary:
			theValue = DecodeBinary_Dictionary(decodeInfo, byteOffset);
			break;

		default:
			NN_LOG_ERROR("Unknown token: {}", theTag.theToken);
			break;
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeBinary_Array : Decode an array.
//-----------------------------------------------------------------------------
NArray NPropertyList::DecodeBinary_Array(const NPListBinaryDecodeInfo& decodeInfo,
										 uint64_t&                     byteOffset)
{


	// Get the state we need
	NPListBinaryTag theTag  = DecodeBinary_ReadObjectTag(decodeInfo, byteOffset);
	uint64_t        theSize = DecodeBinary_ReadObjectSize(decodeInfo, byteOffset, theTag);

	NN_REQUIRE(theTag.theToken == kNBinaryTokenArray);



	// Read the references
	NVectorUInt64 valueRefs;

	for (uint64_t n = 0; n < theSize; n++)
	{
		valueRefs.push_back(DecodeBinary_ReadObjectRef(decodeInfo, byteOffset));
	}



	// Decode the value
	NArray theValue;

	for (uint64_t n = 0; n < theSize; n++)
	{
		uint64_t valueOffset = DecodeBinary_GetObjectOffset(decodeInfo, valueRefs[size_t(n)]);
		NAny     valueObject = DecodeBinary_Any(decodeInfo, valueOffset);

		theValue.push_back(valueObject);
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeBinary_Boolean : Decode a boolean.
//-----------------------------------------------------------------------------
bool NPropertyList::DecodeBinary_Boolean(const NPListBinaryDecodeInfo& decodeInfo,
										 uint64_t&                     byteOffset)
{


	// Get the state we need
	NPListBinaryTag theTag = DecodeBinary_ReadObjectTag(decodeInfo, byteOffset);

	NN_REQUIRE(theTag.theToken == kNBinaryTokenNull);



	// Decode the value
	bool theValue = false;

	switch (theTag.objectInfo)
	{
		case kNBinaryValueFalse:
			theValue = false;
			break;

		case kNBinaryValueTrue:
			theValue = true;
			break;

		default:
			NN_LOG_ERROR("Unknown boolean value: {}", theTag.objectInfo);
			break;
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeBinary_Data : Decode binary data.
//-----------------------------------------------------------------------------
NData NPropertyList::DecodeBinary_Data(const NPListBinaryDecodeInfo& decodeInfo,
									   uint64_t&                     byteOffset)
{


	// Get the state we need
	NPListBinaryTag theTag  = DecodeBinary_ReadObjectTag(decodeInfo, byteOffset);
	uint64_t        theSize = DecodeBinary_ReadObjectSize(decodeInfo, byteOffset, theTag);
	const uint8_t*  thePtr  = decodeInfo.objectsTable + byteOffset;

	NN_REQUIRE(theTag.theToken == kNBinaryTokenData);

	if constexpr (NN_ARCH_32)
	{
		static_assert(NN_ARCH_64 || sizeof(size_t) == sizeof(uint32_t));
		NN_REQUIRE(theSize <= uint64_t(kNUInt32Max));
	}



	// Decode the value
	NData theValue = NData(size_t(theSize), thePtr);

	byteOffset += theSize;

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeBinary_Dictionary : Decode a dictionary.
//-----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeBinary_Dictionary(const NPListBinaryDecodeInfo& decodeInfo,
												   uint64_t&                     byteOffset)
{


	// Get the state we need
	NPListBinaryTag theTag  = DecodeBinary_ReadObjectTag(decodeInfo, byteOffset);
	uint64_t        theSize = DecodeBinary_ReadObjectSize(decodeInfo, byteOffset, theTag);

	NN_REQUIRE(theTag.theToken == kNBinaryTokenDictionary);



	// Read the references
	NVectorUInt64 keyRefs, valueRefs;

	for (uint64_t n = 0; n < theSize; n++)
	{
		keyRefs.push_back(DecodeBinary_ReadObjectRef(decodeInfo, byteOffset));
	}

	for (uint64_t n = 0; n < theSize; n++)
	{
		valueRefs.push_back(DecodeBinary_ReadObjectRef(decodeInfo, byteOffset));
	}



	// Decode the value
	NDictionary theValue;

	for (uint64_t n = 0; n < theSize; n++)
	{
		uint64_t keyOffset   = DecodeBinary_GetObjectOffset(decodeInfo, keyRefs[size_t(n)]);
		uint64_t valueOffset = DecodeBinary_GetObjectOffset(decodeInfo, valueRefs[size_t(n)]);

		NAny keyObject   = DecodeBinary_Any(decodeInfo, keyOffset);
		NAny valueObject = DecodeBinary_Any(decodeInfo, valueOffset);

		if (keyObject.IsString())
		{
			theValue[keyObject.GetString()] = valueObject;
		}
		else
		{
			NN_LOG_ERROR("Found non-string dictionary key!");
		}
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeBinary_Integer : Decode an integer.
//-----------------------------------------------------------------------------
int64_t NPropertyList::DecodeBinary_Integer(const NPListBinaryDecodeInfo& decodeInfo,
											uint64_t&                     byteOffset)
{


	// Get the state we need
	NPListBinaryTag theTag  = DecodeBinary_ReadObjectTag(decodeInfo, byteOffset);
	size_t          theSize = size_t(1) << theTag.objectInfo;
	const uint8_t*  thePtr  = decodeInfo.objectsTable + byteOffset;

	NN_REQUIRE(theTag.theToken == kNBinaryTokenInteger);



	// Decode the value
	//
	// Signed values are stored as 8 byte big-endian integers. Unsigned values
	// are stored in the smallest big-endian integer that can hold their value.
	uint16_t value16  = 0;
	uint32_t value32  = 0;
	int64_t  value64  = 0;
	int64_t  theValue = 0;

	byteOffset += theSize;

	switch (theSize)
	{
		case 1:
			theValue = thePtr[0];
			break;

		case 2:
			memcpy(&value16, thePtr, sizeof(value16));
			theValue = NByteSwap::SwapBtoN(value16);
			break;

		case 4:
			memcpy(&value32, thePtr, sizeof(value32));
			theValue = NByteSwap::SwapBtoN(value32);
			break;

		case 8:
			memcpy(&value64, thePtr, sizeof(value64));
			theValue = NByteSwap::SwapBtoN(value64);
			break;

		default:
			NN_LOG_ERROR("Unknown integer size: {}", theSize);
			break;
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeBinary_Real : Decode a floating point value.
//-----------------------------------------------------------------------------
float64_t NPropertyList::DecodeBinary_Real(const NPListBinaryDecodeInfo& decodeInfo,
										   uint64_t&                     byteOffset)
{


	// Get the state we need
	NPListBinaryTag theTag = DecodeBinary_ReadObjectTag(decodeInfo, byteOffset);
	const uint8_t*  thePtr = decodeInfo.objectsTable + byteOffset;

	NN_REQUIRE(theTag.theToken == kNBinaryTokenReal);



	// Decode the value
	float32_t value32  = 0.0f;
	float64_t value64  = 0.0;
	float64_t theValue = 0.0;

	switch (theTag.objectInfo)
	{
		case kNBinaryValueFloat32:
			memcpy(&value32, thePtr, sizeof(value32));
			theValue = float64_t(NByteSwap::SwapBtoN(value32));
			byteOffset += sizeof(float32_t);
			break;

		case kNBinaryValueFloat64:
			memcpy(&value64, thePtr, sizeof(value64));
			theValue = float64_t(NByteSwap::SwapBtoN(value64));
			byteOffset += sizeof(float64_t);
			break;

		default:
			NN_LOG_ERROR("Unknown real type: {}", theTag.objectInfo);
			break;
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeBinary_String : Decode a string.
//-----------------------------------------------------------------------------
NString NPropertyList::DecodeBinary_String(const NPListBinaryDecodeInfo& decodeInfo,
										   uint64_t&                     byteOffset)
{


	// Get the state we need
	NPListBinaryTag theTag  = DecodeBinary_ReadObjectTag(decodeInfo, byteOffset);
	uint64_t        theSize = DecodeBinary_ReadObjectSize(decodeInfo, byteOffset, theTag);
	const uint8_t*  thePtr  = decodeInfo.objectsTable + byteOffset;

	if constexpr (NN_ARCH_32)
	{
		static_assert(NN_ARCH_64 || sizeof(size_t) == sizeof(uint32_t));
		NN_REQUIRE((theSize * sizeof(utf16_t)) <= uint64_t(kNUInt32Max));
	}



	// Decode the value
	NString theValue;

	switch (theTag.theToken)
	{
		case kNBinaryTokenStringASCII:
			theSize  = theSize * sizeof(utf8_t);
			theValue = NString(NStringEncoding::ASCII, size_t(theSize), thePtr);
			byteOffset += theSize;
			break;

		case kNBinaryTokenStringUTF16:
			theSize  = theSize * sizeof(utf16_t);
			theValue = NString(NStringEncoding::UTF16BE, size_t(theSize), thePtr);
			byteOffset += theSize;
			break;

		default:
			NN_LOG_ERROR("Unknown string type: {}", theTag.theToken);
			break;
	}

	return theValue;
}





//=============================================================================
//		NPropertyList::DecodeBinary_Time : Decode a time.
//-----------------------------------------------------------------------------
NTime NPropertyList::DecodeBinary_Time(const NPListBinaryDecodeInfo& decodeInfo,
									   uint64_t&                     byteOffset)
{


	// Get the state we need
	NPListBinaryTag theTag = DecodeBinary_ReadObjectTag(decodeInfo, byteOffset);
	const uint8_t*  thePtr = decodeInfo.objectsTable + byteOffset;

	NN_REQUIRE(theTag.theToken == kNBinaryTokenTime);
	NN_REQUIRE(theTag.objectInfo == kNBinaryValueFloat64);



	// Decode the value
	float64_t value64;
	memcpy(&value64, thePtr, sizeof(value64));

	NTime theValue = NByteSwap::SwapBtoN(value64);

	byteOffset += sizeof(float64_t);

	return theValue;
}
