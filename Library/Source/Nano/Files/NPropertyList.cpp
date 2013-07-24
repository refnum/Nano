/*	NAME:
		NPropertyList.cpp

	DESCRIPTION:
		Property list file.

		The Mac XML1 property list format is documented at:
		
			http://www.apple.com/DTDs/PropertyList-1.0.dtd

		The Mac Binary1 property list format is documented at:
		
			http://opensource.apple.com/source/CF/CF-550/CFBinaryPList.c
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetPOSIX.h"
#include "NFileUtilities.h"
#include "NMathUtilities.h"
#include "NDataEncoder.h"
#include "NJSONEncoder.h"
#include "NXMLEncoder.h"
#include "NEncoder.h"
#include "NByteSwap.h"
#include "NPropertyList.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Magic
static const uint8_t kMagicMacXML1[]								= { 0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20  };
static const uint8_t kMagicMacBinary1[]								= { 0x62, 0x70, 0x6C, 0x69, 0x73, 0x74, 0x30, 0x30 };


// Mac XML 1.0
static const NString kTokenArray									= "array";
static const NString kTokenData										= "data";
static const NString kTokenDate										= "date";
static const NString kTokenDictionary								= "dict";
static const NString kTokenFalse									= "false";
static const NString kTokenInteger									= "integer";
static const NString kTokenPList									= "plist";
static const NString kTokenReal										= "real";
static const NString kTokenString									= "string";
static const NString kTokenTrue										= "true";

static const NString kTokenKey										= "key";
static const NString kTokenUnknown									= "unknown";
static const NString kTokenVersion									= "version";

static const NString kPListSystemID1								= "http://www.apple.com/DTDs/PropertyList-1.0.dtd";
static const NString kPListPublicID1								= "-//Apple Computer//DTD PLIST 1.0//EN";
static const NString kFormatISO8601									= "%04d-%02d-%02dT%02d:%02d:%02dZ";


// Mac Binary 1.0
static const uint8_t kBinary1_Token_Null							= 0x00;
static const uint8_t kBinary1_Token_Integer							= 0x01;
static const uint8_t kBinary1_Token_Real							= 0x02;
static const uint8_t kBinary1_Token_Date							= 0x03;
static const uint8_t kBinary1_Token_Data							= 0x04;
static const uint8_t kBinary1_Token_StringASCII						= 0x05;
static const uint8_t kBinary1_Token_StringUTF16						= 0x06;
static const uint8_t kBinary1_Token_Array							= 0x0A;
static const uint8_t kBinary1_Token_Dictionary						= 0x0D;

static const uint8_t kBinary1_Value_Null							= 0x00;
static const uint8_t kBinary1_Value_Fill							= 0x0F;
static const uint8_t kBinary1_Value_False							= 0x08;
static const uint8_t kBinary1_Value_True							= 0x09;
static const uint8_t kBinary1_Value_SizeInteger						= 0x0F;

static const uint8_t kBinary1_Value_Int8							= 0x00;
static const uint8_t kBinary1_Value_Int16							= 0x01;
static const uint8_t kBinary1_Value_Int32							= 0x02;
static const uint8_t kBinary1_Value_Int64							= 0x03;

static const uint8_t kBinary1_Value_Float32							= 0x02;
static const uint8_t kBinary1_Value_Float64							= 0x03;





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
// Mac Binary 1.0
typedef struct {
	uint8_t		reserved[5];
	uint8_t		sortVersion;
	uint8_t		offsetsSize;
	uint8_t		objectsSize;
	uint64_t	numObjects;
	uint64_t	rootObject;
	uint64_t	offsetsTableOffset;
} MacBinary1_Trailer;

NBYTESWAP_BEGIN(MacBinary1_Trailer)
	NBYTESWAP_B_UInt8_Array		(reserved, 5)
	NBYTESWAP_B_UInt8			(sortVersion)
	NBYTESWAP_B_UInt8			(offsetsSize)
	NBYTESWAP_B_UInt8			(objectsSize)
	NBYTESWAP_B_UInt64			(numObjects)
	NBYTESWAP_B_UInt64			(rootObject)
	NBYTESWAP_B_UInt64			(offsetsTableOffset)
NBYTESWAP_END





//============================================================================
//		NPropertyList::NPropertyList : Constructor.
//----------------------------------------------------------------------------
NPropertyList::NPropertyList(void)
{
}





//============================================================================
//		NPropertyList::~NPropertyList : Destructor.
//----------------------------------------------------------------------------
NPropertyList::~NPropertyList(void)
{
}





//============================================================================
//		NPropertyList::GetFormat : Identify a property list format.
//----------------------------------------------------------------------------
NPropertyListFormat NPropertyList::GetFormat(const NData &theData)
{	NPropertyListFormat		theFormat;
	const uint8_t			*dataPtr;
	NIndex					dataSize;



	// Get the state we need
	theFormat = kNPropertyListInvalid;
	dataSize  = theData.GetSize();
	dataPtr   = theData.GetData();



	// Identify the format
	#define MATCH_FORMAT(_magic, _format)													\
		do																					\
			{																				\
			if (theFormat == kNPropertyListInvalid && dataSize >= NN_ARRAY_SIZE(_magic))	\
				{																			\
				if (memcmp(dataPtr, _magic, NN_ARRAY_SIZE(_magic)) == 0)					\
					theFormat = _format;													\
				}																			\
			}																				\
		while (0)

	MATCH_FORMAT(kMagicMacXML1,    kNPropertyListMacXML1);
	MATCH_FORMAT(kMagicMacBinary1, kNPropertyListMacBinary1);
	
	#undef MATCH_FORMAT
	
	return(theFormat);
}





//============================================================================
//		NPropertyList::Encode : Encode a property list.
//----------------------------------------------------------------------------
NData NPropertyList::Encode(const NDictionary &theState, NPropertyListFormat theFormat)
{	NData		theData;



	// Encode the state
	switch (theFormat) {
		case kNPropertyListMacXML1:
			theData = EncodeMacXML1(theState);
			break;

		case kNPropertyListMacBinary1:
			theData = EncodeMacBinary1(theState);
			break;

		case kNPropertyListJSON:
			theData = EncodeJSON(theState).GetData();
			break;
		
		default:
			NN_LOG("Unknown property list format: %d", theFormat);
			break;
		}
	
	return(theData);
}





//============================================================================
//		NPropertyList::Decode : Decode a property list.
//----------------------------------------------------------------------------
NDictionary NPropertyList::Decode(const NData &theData)
{	NPropertyListFormat		theFormat;
	NDictionary				theState;



	// Get the state we need
	theFormat = GetFormat(theData);



	// Decode the state
	switch (theFormat) {
		case kNPropertyListMacXML1:
			theState = DecodeMacXML1(theData);
			break;

		case kNPropertyListMacBinary1:
			theState = DecodeMacBinary1(theData);
			break;

		case kNPropertyListJSON:
			theState = DecodeJSON(theData);
			break;
		
		default:
			NN_LOG("Unknown property list format: %d", theFormat);
			break;
		}
	
	return(theState);
}





//============================================================================
//		NPropertyList::EncodeXML : Encode to XML.
//----------------------------------------------------------------------------
NString NPropertyList::EncodeXML(const NDictionary &theState)
{	NData		theData;
	NString		theXML;



	// Encode to XML
	theData = Encode(theState, kNPropertyListXML);
	theXML.SetData(theData);
	
	return(theXML);
}





//============================================================================
//		NPropertyList::DecodeXML : Decode from XML.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeXML(const NString &theXML)
{	NDictionary		theState;
	NData			theData;



	// Decode from XML
	theState = Decode(theXML.GetData());
	
	return(theState);
}





//============================================================================
//		NPropertyList::EncodeJSON : Encode to JSON.
//----------------------------------------------------------------------------
NString NPropertyList::EncodeJSON(const NDictionary &theState)
{	NJSONEncoder		theEncoder;
	NString				theJSON;



	// Encode to JSON
	theJSON = theEncoder.Encode(theState);

	return(theJSON);
}





//============================================================================
//		NPropertyList::DecodeJSON : Decode from JSON.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeJSON(const NString &theJSON)
{	NJSONEncoder		theEncoder;
	NDictionary			theState;
	NVariant			theValue;



	// Decode from JSON
	theValue = theEncoder.Decode(theJSON);
	
	if (!theValue.GetValue(theState))
		NN_LOG("Unable to convert JSON to dictionary: [%@]", theJSON);

	return(theState);
}





//============================================================================
//		NPropertyList::Load : Load a property list.
//----------------------------------------------------------------------------
NDictionary NPropertyList::Load(const NFile &theFile)
{	NDictionary		theState;
	NData			theData;



	// Load the file
	theData  = NFileUtilities::GetFileData(theFile);
	theState = Decode(theData);
	
	return(theState);
}





//============================================================================
//		NPropertyList::Save : Save a property list.
//----------------------------------------------------------------------------
NStatus NPropertyList::Save(const NFile &theFile, const NDictionary &theState, NPropertyListFormat theFormat)
{	NData		theData;
	NStatus		theErr;



	// Save the file
	theData = Encode(theState, theFormat);
	theErr  = NFileUtilities::SetFileData(theFile, theData);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





#pragma mark private
//============================================================================
//		NPropertyList::EncodeMacXML1 : Encode kNPropertyListMacXML1.
//----------------------------------------------------------------------------
NData NPropertyList::EncodeMacXML1(const NDictionary &theState)
{	NXMLNode		*nodeDoc, *nodeDocType, *nodePList, *nodeDict;
	NXMLEncoder		theEncoder;
	NString			theXML;



	// Create the nodes
	nodeDict = EncodeMacXML1_Dictionary(theState);

	nodePList = new NXMLNode(kNXMLNodeElement, kTokenPList);
	nodePList->SetElementAttribute(kTokenVersion, "1.0");
	nodePList->AddChild(nodeDict);

	nodeDocType = new NXMLNode(kNXMLNodeDocType, kTokenPList);
	nodeDocType->SetDocTypeSystemID(kPListSystemID1);
	nodeDocType->SetDocTypePublicID(kPListPublicID1);

	nodeDoc = new NXMLNode(kNXMLNodeDocument, "");
	nodeDoc->AddChild(nodeDocType);
	nodeDoc->AddChild(nodePList);



	// Encode the XML
	theXML = theEncoder.Encode(nodeDoc);
	theXML.ReplaceAll("^\t", "", kNStringPattern);

	delete nodeDoc;
	
	return(theXML.GetData());
}





//============================================================================
//		NPropertyList::DecodeMacXML1 : Decode kNPropertyListMacXML1.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacXML1(const NData &theData)
{	NXMLNode						*nodeDoc, *nodeDict;
	const NXMLNodeList				*theChildren;
	NXMLEncoder						theEncoder;
	NDictionary						theState;
	NXMLNodeListConstIterator		theIter;



	// Get the state we need
	nodeDoc  = theEncoder.Decode(theData);
	nodeDict = NULL;
	
	if (nodeDoc != NULL)
		{
		theChildren = nodeDoc->GetChildren();
		for (theIter = theChildren->begin(); theIter != theChildren->end(); theIter++)
			{
			if ((*theIter)->IsElement(kTokenPList))
				{
				theChildren = (*theIter)->GetChildren();
				nodeDict    = theChildren->at(0);
				
				NN_ASSERT(theChildren->size() == 1);
				NN_ASSERT(nodeDict->IsElement(kTokenDictionary));
				break;
				}
			}
		}



	// Decode the XML
	if (nodeDict != NULL)
		theState = DecodeMacXML1_Dictionary(nodeDict);

	delete nodeDoc;

	return(theState);
}





//============================================================================
//		NPropertyList::EncodeMacBinary1 : Encode kNPropertyListMacBinary1.
//----------------------------------------------------------------------------
NData NPropertyList::EncodeMacBinary1(const NDictionary &theState)
{	uint32_t					numObjects, objectsTableSize, offsetsSize;
	NData						theData, offsetTable;
	uint64_t					rootObject;
	MacBinary1_Trailer			theTrailer;
	MacBinary1_EncodeInfo		theInfo;



	// Encode the object table
	//
	// Binary plists do not attempt to share references - two strings with the same
	// underlying shared data pointer will be written out twice.
	//
	// This could be improved by performing a first pass through the values to identify
	// any objects that had the same value (ideally comparing by value, not the underlying
	// shared data: two strings with the same text could be combined into the same object
	// even if they are two separate objects before encoding).
	//
	// Our current approach means that our pre-flight just has to count the number of
	// objects we'll encode, as this gives us the size for encoding object references.
	numObjects = 0;
	EncodeMacBinary1_GetObjectCount(theState, &numObjects);

	theInfo.objectsSize = EncodeMacBinary1_GetIntegerSize(numObjects);
	theInfo.objectsTable.AppendData(sizeof(kMagicMacBinary1), kMagicMacBinary1);

	rootObject = EncodeMacBinary1_Value(theInfo, theState);



	// Encode the offset table
	objectsTableSize = theInfo.objectsTable.GetSize();
	offsetsSize      = EncodeMacBinary1_GetIntegerSize(objectsTableSize);
	offsetTable      = EncodeMacBinary1_GetIntegerList(theInfo.theOffsets, offsetsSize);



	// Encode the trailer
	memset(&theTrailer, 0x00, sizeof(theTrailer));
	
	theTrailer.offsetsSize        = (uint8_t) offsetsSize;
	theTrailer.objectsSize        = (uint8_t) theInfo.objectsSize;
	theTrailer.numObjects         = theInfo.theOffsets.size();
	theTrailer.rootObject         = rootObject;
	theTrailer.offsetsTableOffset = objectsTableSize;

	NBYTESWAP_ENCODE(1, MacBinary1_Trailer, &theTrailer);



	// Construct the property list
	theData.AppendData(theInfo.objectsTable);
	theData.AppendData(offsetTable);
	theData.AppendData(sizeof(theTrailer), &theTrailer);

	return(theData);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1 : Decode kNPropertyListMacBinary1.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacBinary1(const NData &theData)
{	const uint8_t				*offsetsTable;
	MacBinary1_Trailer			theTrailer;
	NVariant					rootObject;
	uint64_t					rootOffset;
	NDictionary					theState;
	MacBinary1_DecodeInfo		theInfo;



	// Validate our state
	NN_ASSERT(sizeof(theTrailer) == 32);



	// Decode the trailer
	if (theData.GetSize() < (NIndex) sizeof(theTrailer))
		return(theState);

	memcpy(&theTrailer, theData.GetData(theData.GetSize() - sizeof(theTrailer)), sizeof(theTrailer));
	NBYTESWAP_DECODE(1, MacBinary1_Trailer, &theTrailer);



	// Prepare to decode
	NN_ASSERT((NIndex) theTrailer.offsetsTableOffset < kNIndexMax);
	offsetsTable = theData.GetData((NIndex) theTrailer.offsetsTableOffset);

	theInfo.theOffsets   = DecodeMacBinary1_GetObjectOffsets(theTrailer.numObjects, theTrailer.offsetsSize, offsetsTable);
	theInfo.objectsSize  = theTrailer.objectsSize;
	theInfo.objectsTable = theData.GetData();



	// Decode the top object
	rootOffset = DecodeMacBinary1_GetObjectOffset(theInfo, theTrailer.rootObject);
	rootObject = DecodeMacBinary1_Value(          theInfo, rootOffset);
	
	if (!rootObject.GetValue(theState))
		NN_LOG("Unknown root object - not a dictionary");

	return(theState);
}





//============================================================================
//		NPropertyList::EncodeMacXML1_Boolean : Encode a bool.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML1_Boolean(bool theValue)
{	NXMLNode	*theNode;



	// Encode the value
	theNode = new NXMLNode(kNXMLNodeElement, theValue ? kTokenTrue : kTokenFalse);
	theNode->SetElementUnpaired(true);
	
	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML1_Number : Encode a number.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML1_Number(const NNumber &theValue)
{	NString			textType, valueText;
	NXMLNode		*theNode;



	// Validate our state
	//
	// The XML property list format requires specific values for nan/info.
	NN_ASSERT(kNStringInfinityNeg == "-infinity");
	NN_ASSERT(kNStringInfinityPos == "+infinity");
	NN_ASSERT(kNStringNaN         == "nan");



	// Get the state we need
	textType  = theValue.IsInteger() ? kTokenInteger : kTokenReal;
	valueText = theValue.GetString();



	// Encode the value
	theNode = new NXMLNode(kNXMLNodeElement, textType);
	theNode->SetElementContents(valueText);

	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML1_String : Encode a string.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML1_String(const NString &theValue)
{	NXMLNode	*theNode;



	// Encode the value
	theNode = new NXMLNode(kNXMLNodeElement, kTokenString);
	theNode->SetElementContents(theValue);
	
	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML1_Data : Encode binary data.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML1_Data(const NData &theValue)
{	NDataEncoder	theEncoder;
	NString			valueText;
	NXMLNode		*theNode;



	// Get the state we need
	valueText = theEncoder.Encode(theValue, kNDataEncodingB64);



	// Encode the value
	theNode = new NXMLNode(kNXMLNodeElement, kTokenData);
	theNode->SetElementContents(valueText);

	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML1_Date : Encode a date.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML1_Date(const NDate &theValue)
{	NString				valueText;
	NGregorianDate		gregDate;
	NXMLNode			*theNode;



	// Get the state we ned
	gregDate = theValue.GetDate(kNTimeZoneUTC);

	valueText.Format(kFormatISO8601, gregDate.year, gregDate.month,  gregDate.day,
									 gregDate.hour, gregDate.minute, (uint32_t) gregDate.second);



	// Encode the value
	theNode = new NXMLNode(kNXMLNodeElement, kTokenDate);
	theNode->SetElementContents(valueText);
	
	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML1_Array : Encode an array.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML1_Array(const NArray &theValue)
{	NXMLNode		*theNode, *nodeValue;
	NIndex			n, theSize;
	NEncoder		theEncoder;
	NData			theData;
	NVariant		theItem;



	// Get the state we need
	theSize = theValue.GetSize();
	theNode = new NXMLNode(kNXMLNodeElement, kTokenArray);



	// Encode the value
	for (n = 0; n < theSize; n++)
		{
		// Get the state we need
		theItem = theValue.GetValue(n);



		// Create the value
		if (theItem.IsType(typeid(bool)))
			nodeValue = EncodeMacXML1_Boolean(theValue.GetValueBoolean(n));

		else if (theItem.IsNumeric())
			nodeValue = EncodeMacXML1_Number(theValue.GetValue(n));

		else if (theItem.IsType(typeid(NString)))
			nodeValue = EncodeMacXML1_String(theValue.GetValueString(n));

		else if (theItem.IsType(typeid(NData)))
			nodeValue = EncodeMacXML1_Data(theValue.GetValueData(n));

		else if (theItem.IsType(typeid(NDate)))
			nodeValue = EncodeMacXML1_Date(theValue.GetValueDate(n));

		else if (theItem.IsType(typeid(NArray)))
			nodeValue = EncodeMacXML1_Array(theValue.GetValueArray(n));

		else if (theItem.IsType(typeid(NDictionary)))
			nodeValue = EncodeMacXML1_Dictionary(theValue.GetValueDictionary(n));

		else
			{
			theData = theEncoder.Encode(theItem);
			if (!theData.IsEmpty())
				nodeValue = EncodeMacXML1_Data(theData);
			else
				{
				NN_LOG("Unknown property list value!");
				nodeValue = new NXMLNode(kNXMLNodeElement, kTokenUnknown);
				}
			}



		// Add the value
		theNode->AddChild(nodeValue);
		}
	 
	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML1_Dictionary : Encode a dictionary.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML1_Dictionary(const NDictionary &theValue)
{	NXMLNode					*theNode, *nodeKey, *nodeValue;
	NEncoder					theEncoder;
	NVariant					theItem;
	NStringList					theKeys;
	NStringListConstIterator	theIter;
	NData						theData;
	NString						theKey;



	// Get the state we need
	theKeys = theValue.GetKeys(true);
	theNode = new NXMLNode(kNXMLNodeElement, kTokenDictionary);



	// Encode the value
	for (theIter = theKeys.begin(); theIter != theKeys.end(); theIter++)
		{
		// Get the state we need
		theKey  = *theIter;
		theItem = theValue.GetValue(theKey);
		
		
		
		// Create the key-value pair
		nodeKey = new NXMLNode(kNXMLNodeElement, kTokenKey);
		nodeKey->SetElementContents(theKey);

		if (theItem.IsType(typeid(bool)))
			nodeValue = EncodeMacXML1_Boolean(theValue.GetValueBoolean(theKey));

		else if (theItem.IsNumeric())
			nodeValue = EncodeMacXML1_Number(theValue.GetValue(theKey));

		else if (theItem.IsType(typeid(NString)))
			nodeValue = EncodeMacXML1_String(theValue.GetValueString(theKey));

		else if (theItem.IsType(typeid(NData)))
			nodeValue = EncodeMacXML1_Data(theValue.GetValueData(theKey));

		else if (theItem.IsType(typeid(NDate)))
			nodeValue = EncodeMacXML1_Date(theValue.GetValueDate(theKey));

		else if (theItem.IsType(typeid(NArray)))
			nodeValue = EncodeMacXML1_Array(theValue.GetValueArray(theKey));

		else if (theItem.IsType(typeid(NDictionary)))
			nodeValue = EncodeMacXML1_Dictionary(theValue.GetValueDictionary(theKey));

		else
			{
			theData = theEncoder.Encode(theItem);
			if (!theData.IsEmpty())
				nodeValue = EncodeMacXML1_Data(theData);
			else
				{
				NN_LOG("Unknown property list value [%s]!", theItem.GetType().name());
				nodeValue = new NXMLNode(kNXMLNodeElement, kTokenUnknown);
				}
			}



		// Add the key-value pair
		theNode->AddChild(nodeKey);
		theNode->AddChild(nodeValue);
		}
	 
	return(theNode);
}





//============================================================================
//		NPropertyList::DecodeMacXML1_Boolean : Decode a bool.
//----------------------------------------------------------------------------
bool NPropertyList::DecodeMacXML1_Boolean(const NXMLNode *theNode)
{	bool	theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenTrue) || theNode->IsElement(kTokenFalse));



	// Decode the value
	theValue = theNode->IsElement(kTokenTrue);
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML1_Integer : Decode an integer.
//----------------------------------------------------------------------------
int64_t NPropertyList::DecodeMacXML1_Integer(const NXMLNode *theNode)
{	NNumber		rawValue;
	int64_t		theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenInteger));



	// Decode the value
	if (!rawValue.SetValue(theNode->GetElementContents()) || !rawValue.IsInteger())
		NN_LOG("Unable to extract integer: [%@]", theNode->GetElementContents());
	
	theValue = rawValue.GetInt64();
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML1_Real : Decode a floating point value.
//----------------------------------------------------------------------------
float64_t NPropertyList::DecodeMacXML1_Real(const NXMLNode *theNode)
{	NNumber		rawValue;
	float64_t	theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenReal));



	// Decode the value
	if (!rawValue.SetValue(theNode->GetElementContents()))
		NN_LOG("Unable to extract real: [%@]", theNode->GetElementContents());
		
	theValue = rawValue.GetFloat64();

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML1_String : Decode a string.
//----------------------------------------------------------------------------
NString NPropertyList::DecodeMacXML1_String(const NXMLNode *theNode)
{	NString		theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenString));



	// Decode the value
	theValue = theNode->GetElementContents();
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML1_Data : Decode binary data.
//----------------------------------------------------------------------------
NData NPropertyList::DecodeMacXML1_Data(const NXMLNode *theNode)
{	NDataEncoder	theEncoder;
	NData			theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenData));



	// Decode the value
	theValue = theEncoder.Decode(theNode->GetElementContents(), kNDataEncodingB64);

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML1_Date : Decode a date.
//----------------------------------------------------------------------------
NDate NPropertyList::DecodeMacXML1_Date(const NXMLNode *theNode)
{	int					valMonth, valDay, valHour, valMinute;
	int32_t				numSecs, numItems;
	NString				valueText;
	NGregorianDate		gregDate;
	NDate				theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenDate));



	// Get the state we need
	valueText = theNode->GetElementContents();
	numItems  = sscanf(valueText.GetUTF8(), kFormatISO8601.GetUTF8(),
							&gregDate.year, &valMonth,  &valDay,
							&valHour, &valMinute, &numSecs);

	NN_ASSERT(numItems  == 6);
	NN_ASSERT(valMonth  <= kInt8Max);
	NN_ASSERT(valDay    <= kInt8Max);
	NN_ASSERT(valHour   <= kInt8Max);
	NN_ASSERT(valMinute <= kInt8Max);

	gregDate.month    = (int8_t) valMonth;
	gregDate.day      = (int8_t) valDay;
	gregDate.hour     = (int8_t) valHour;
	gregDate.minute   = (int8_t) valMinute;
	gregDate.timeZone = kNTimeZoneUTC;



	// Decode the value
	//
	// Conversion from a gregorian date to an NTime can introduce rounding
	// errors from the underlying platform (e.g., Windows gregorian conversion
	// is only accurate to 100 nanoseconds).
	//
	// Since we know our seconds are integers, we round after conversion.
	if (numItems == 6)
		{
		gregDate.second = numSecs;
		theValue.SetDate(gregDate);
		theValue.SetTime(NTargetPOSIX::rint(theValue.GetTime()));
		}
	else
		theValue.SetTime(0.0);
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML1_Array : Decode an array.
//----------------------------------------------------------------------------
NArray NPropertyList::DecodeMacXML1_Array(const NXMLNode *theNode)
{	const NXMLNodeList				*theChildren;
	const NXMLNode					*nodeValue;
	NEncoder						theEncoder;
	NVariant						theObject;
	NArray							theValue;
	NData							theData;
	NXMLNodeListConstIterator		theIter;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenArray));



	// Get the state we need
	theChildren = theNode->GetChildren();



	// Decode the value
	for (theIter = theChildren->begin(); theIter != theChildren->end(); theIter++)
		{
		// Get the state we need
		nodeValue = *theIter;
		
		
		// Get the value
		if (nodeValue->IsElement(kTokenTrue) || nodeValue->IsElement(kTokenFalse))
			theValue.AppendValue(DecodeMacXML1_Boolean(nodeValue));

		else if (nodeValue->IsElement(kTokenInteger))
			theValue.AppendValue(DecodeMacXML1_Integer(nodeValue));
		
		else if (nodeValue->IsElement(kTokenReal))
			theValue.AppendValue(DecodeMacXML1_Real(nodeValue));
		
		else if (nodeValue->IsElement(kTokenString))
			theValue.AppendValue(DecodeMacXML1_String(nodeValue));
		
		else if (nodeValue->IsElement(kTokenData))
			{
			theData   = DecodeMacXML1_Data(nodeValue);
			theObject = theEncoder.Decode(theData);

			if (theObject.IsValid())
				theValue.AppendValue(theObject);
			else
				theValue.AppendValue(theData);
			}
		
		else if (nodeValue->IsElement(kTokenDate))
			theValue.AppendValue(DecodeMacXML1_Date(nodeValue));
		
		else if (nodeValue->IsElement(kTokenArray))
			theValue.AppendValue(DecodeMacXML1_Array(nodeValue));

		else if (nodeValue->IsElement(kTokenDictionary))
			theValue.AppendValue(DecodeMacXML1_Dictionary(nodeValue));
		
		else
			NN_LOG("Unknown property list value [%@]!", nodeValue->GetTextValue());
		}
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML1_Dictionary : Decode a dictionary.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacXML1_Dictionary(const NXMLNode *theNode)
{	const NXMLNode					*nodeKey, *nodeValue;
	const NXMLNodeList				*theChildren;
	NEncoder						theEncoder;
	NVariant						theObject;
	NDictionary						theValue;
	NXMLNodeListConstIterator		theIter;
	NData							theData;
	NString							theKey;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenDictionary));



	// Get the state we need
	theChildren = theNode->GetChildren();
	NN_ASSERT(NMathUtilities::IsEven(theChildren->size()));



	// Decode the value
	for (theIter = theChildren->begin(); theIter != theChildren->end(); )
		{
		// Get the state we need
		nodeKey   = *theIter++;
		nodeValue = *theIter++;


		// Get the key
		NN_ASSERT(nodeKey->IsElement(kTokenKey));
		theKey = nodeKey->GetElementContents();
		
		
		// Get the value
		if (nodeValue->IsElement(kTokenTrue) || nodeValue->IsElement(kTokenFalse))
			theValue.SetValue(theKey, DecodeMacXML1_Boolean(nodeValue));
		
		else if (nodeValue->IsElement(kTokenInteger))
			theValue.SetValue(theKey, DecodeMacXML1_Integer(nodeValue));
		
		else if (nodeValue->IsElement(kTokenReal))
			theValue.SetValue(theKey, DecodeMacXML1_Real(nodeValue));

		else if (nodeValue->IsElement(kTokenString))
			theValue.SetValue(theKey, DecodeMacXML1_String(nodeValue));
		
		else if (nodeValue->IsElement(kTokenData))
			{
			theData   = DecodeMacXML1_Data(nodeValue);
			theObject = theEncoder.Decode(theData);

			if (theObject.IsValid())
				theValue.SetValue(theKey, theObject);
			else
				theValue.SetValue(theKey, theData);
			}
		
		else if (nodeValue->IsElement(kTokenDate))
			theValue.SetValue(theKey, DecodeMacXML1_Date(nodeValue));
		
		else if (nodeValue->IsElement(kTokenArray))
			theValue.SetValue(theKey, DecodeMacXML1_Array(nodeValue));
		
		else if (nodeValue->IsElement(kTokenDictionary))
			theValue.SetValue(theKey, DecodeMacXML1_Dictionary(nodeValue));
		
		else
			NN_LOG("Unknown property list value [%@] [%@]!", theKey, nodeValue->GetTextValue());
		}

	return(theValue);
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_GetObjectCount : Count the objects.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_GetObjectCount(const NVariant &theValue, uint32_t *numObjects)
{	NDictionary		valueDictionary;
	NArray			valueArray;



	// Count the objects
	//
	// Each value generates one object.
	//
	// Arrays and dictionaries also generate an object for each of their values,
	// with dictionaries also generating objects for each of their keys.
	*numObjects += 1;

	if (theValue.GetValue(valueArray))
		valueArray.ForEach(     BindSelf(NPropertyList::EncodeMacBinary1_GetObjectCount, kNArg2, numObjects));

	else if (theValue.GetValue(valueDictionary))
		{
		*numObjects += valueDictionary.GetSize();
		valueDictionary.ForEach(BindSelf(NPropertyList::EncodeMacBinary1_GetObjectCount, kNArg2, numObjects));
		}
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_GetIntegerSize : Get an integer's size.
//----------------------------------------------------------------------------
uint32_t NPropertyList::EncodeMacBinary1_GetIntegerSize(uint64_t theSize)
{


	// Get the required size
	if (theSize <= kUInt8Max)
		return(1);

	else if (theSize <= kUInt16Max)
		return(2);

	else if (theSize <= kUInt32Max)
		return(4);
		
	return(8);
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_GetIntegerList : Get an integer list.
//----------------------------------------------------------------------------
NData NPropertyList::EncodeMacBinary1_GetIntegerList(const UInt64List &theObjects, uint32_t objectsSize)
{	uint64_t	theObject, value64;
	NIndex		n, numObjects;
	NData		theData;
	uint32_t	value32;
	uint16_t	value16;
	uint8_t		value8;



	// Get the state we need
	numObjects = theObjects.size();

	theData.Reserve(numObjects * sizeof(uint64_t));



	// Get the object references
	for (n = 0; n < numObjects; n++)
		{
		theObject = theObjects[n];
		
		switch (objectsSize) {
			case 1:
				NN_ASSERT(theObject <= kUInt8Max);
				value8 = (uint8_t) theObject;
				theData.AppendData(sizeof(value8), &value8);
				break;

			case 2:
				NN_ASSERT(theObject <= kUInt16Max);
				value16 = NSwapUInt16_NtoB((uint16_t) theObject);
				theData.AppendData(sizeof(value16), &value16);
				break;

			case 4:
				NN_ASSERT(theObject <= kUInt32Max);
				value32 = NSwapUInt32_NtoB((uint32_t) theObject);
				theData.AppendData(sizeof(value32), &value32);
				break;

			case 8:
				NN_ASSERT(theObject <= kUInt64Max);
				value64 = NSwapUInt64_NtoB((uint64_t) theObject);
				theData.AppendData(sizeof(value64), &value64);
				break;

			default:
				NN_LOG("Unknown object reference size: %ld", objectsSize);
				break;
			}
		}
	
	return(theData);
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_WriteObject : Write an object.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_WriteObject(MacBinary1_EncodeInfo &theInfo)
{	uint64_t	theOffset;



	// Add the object
	//
	// Object offsets are byte offsets, relative to the start of the object table.
	theOffset = theInfo.objectsTable.GetSize();

	theInfo.theOffsets.push_back(theOffset);
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_WriteObjectTag : Write an object tag.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_WriteObjectTag(MacBinary1_EncodeInfo &theInfo, uint8_t theToken, uint8_t objectInfo, uint64_t objectSize)
{	uint8_t	theTag;



	// Validate our parameters
	NN_ASSERT(theToken   <= 0x0F);
	NN_ASSERT(objectInfo <= 0x0F);



	// Get the state we need
	if (objectSize != 0)
		{
		NN_ASSERT(objectInfo == 0);

		if (objectSize >= kBinary1_Value_SizeInteger)
			objectInfo =  kBinary1_Value_SizeInteger;
		else
			objectInfo = (uint8_t) objectSize;
		}



	// Add the tag
	//
	// Each object starts with a tag byte, which is divided into an identifying
	// token (high nibble) and 4 bits of object-specific state (low nibble).
	theTag = ((uint8_t) (theToken << 4)) | objectInfo;

	theInfo.objectsTable.AppendData(sizeof(theTag), &theTag);



	// Add the size
	//
	// If a size is provided then it will be stored in the object-specific state.
	//
	// if the size exceeds 4 bits then it is appended as an encoded integer.
	if (objectSize != 0 && objectInfo == kBinary1_Value_SizeInteger)
		EncodeMacBinary1_Integer(theInfo, (int64_t) objectSize, false);
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_WriteObjectData : Write object data.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_WriteObjectData(MacBinary1_EncodeInfo &theInfo, NIndex theSize, const void *thePtr)
{


	// Add the data
	theInfo.objectsTable.AppendData(theSize, thePtr);
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_Value : Encode a value.
//----------------------------------------------------------------------------
uint64_t NPropertyList::EncodeMacBinary1_Value(MacBinary1_EncodeInfo &theInfo, const NVariant &theValue)
{	NDictionary			valueDictionary;
	bool				valueBoolean;
	NNumber				valueNumber;
	NString				valueString;
	NArray				valueArray;
	NEncoder			theEncoder;
	NData				valueData;
	uint64_t			objectRef;
	NDate				valueDate;



	// Encode the value
	if (theValue.GetValue(valueBoolean))
		EncodeMacBinary1_Boolean(theInfo, valueBoolean);

	else if (theValue.IsNumeric() && valueNumber.SetValue(theValue))
		{
		if (valueNumber.IsInteger())
			EncodeMacBinary1_Integer(theInfo, valueNumber);
		else
			EncodeMacBinary1_Real(   theInfo, valueNumber);
		}

	else if (theValue.GetValue(valueString))
		EncodeMacBinary1_String(theInfo, valueString);

	else if (theValue.GetValue(valueData))
		EncodeMacBinary1_Data(theInfo, valueData);

	else if (theValue.GetValue(valueDate))
		EncodeMacBinary1_Date(theInfo, valueDate);

	else if (theValue.GetValue(valueArray))
		EncodeMacBinary1_Array(theInfo, valueArray);

	else if (theValue.GetValue(valueDictionary))
		EncodeMacBinary1_Dictionary(theInfo, valueDictionary);
	
	else
		{
		valueData = theEncoder.Encode(theValue);
		if (!valueData.IsEmpty())
			EncodeMacBinary1_Data(theInfo, valueData);
		else
			NN_LOG("Unknown object: %s", theValue.GetType().name());
		}



	// Get the object
	objectRef = (uint64_t) (theInfo.theOffsets.size() - 1);

	return(objectRef);
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_Boolean : Encode a boolean.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_Boolean(MacBinary1_EncodeInfo &theInfo, bool theValue)
{


	// Encode the value
	EncodeMacBinary1_WriteObject(theInfo);

	if (theValue)
		EncodeMacBinary1_WriteObjectTag(theInfo, kBinary1_Token_Null, kBinary1_Value_True);
	else
		EncodeMacBinary1_WriteObjectTag(theInfo, kBinary1_Token_Null, kBinary1_Value_False);
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_Integer : Encode an integer.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_Integer(MacBinary1_EncodeInfo &theInfo, const NNumber &theValue, bool addObject)
{	int64_t		value64;
	uint32_t	value32;
	uint16_t	value16;
	uint8_t		value8;



	// Get the state we need
	value64 = theValue.GetInt64();



	// Add the object
	//
	// Integers are used to store variable-sized sizes for other objects, so
	// we may not need to start a new object.
	if (addObject)
		EncodeMacBinary1_WriteObject(theInfo);



	// Encode the value
	//
	// Signed values are stored as 8 byte big-endian integers. Unsigned values
	// are stored in the smallest big-endian integer that can hold their value.
	if (value64 >= 0 && value64 <= kUInt8Max)
		{
		value8 = (uint8_t) value64;

		EncodeMacBinary1_WriteObjectTag( theInfo, kBinary1_Token_Integer, kBinary1_Value_Int8);
		EncodeMacBinary1_WriteObjectData(theInfo, sizeof(value8), &value8);
		}

	else if (value64 >= 0 && value64 <= kUInt16Max)
		{
		value16 = NSwapUInt16_NtoB((uint16_t) value64);

		EncodeMacBinary1_WriteObjectTag( theInfo, kBinary1_Token_Integer, kBinary1_Value_Int16);
		EncodeMacBinary1_WriteObjectData(theInfo, sizeof(value16), &value16);
		}

	else if (value64 >= 0 && value64 <= kUInt32Max)
		{
		value32 = NSwapUInt32_NtoB((uint32_t) value64);

		EncodeMacBinary1_WriteObjectTag( theInfo, kBinary1_Token_Integer, kBinary1_Value_Int32);
		EncodeMacBinary1_WriteObjectData(theInfo, sizeof(value32), &value32);
		}

	else
		{
		value64 = NSwapInt64_NtoB(value64);

		EncodeMacBinary1_WriteObjectTag( theInfo, kBinary1_Token_Integer, kBinary1_Value_Int64);
		EncodeMacBinary1_WriteObjectData(theInfo, sizeof(value64), &value64);
		}
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_Real : Encode a floating point value.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_Real(MacBinary1_EncodeInfo &theInfo, const NNumber &theValue)
{	float64_t		value64;
	float32_t		value32;



	// Encode the value
	EncodeMacBinary1_WriteObject(theInfo);
	
	switch (theValue.GetPrecision()) {
		case kNPrecisionFloat32:
			value32 = theValue.GetFloat32();
			NSwapFloat32_NtoB(&value32);

			EncodeMacBinary1_WriteObjectTag( theInfo, kBinary1_Token_Real, kBinary1_Value_Float32);
			EncodeMacBinary1_WriteObjectData(theInfo, sizeof(value32), &value32);
			break;

		case kNPrecisionFloat64:
			value64 = theValue.GetFloat64();
			NSwapFloat64_NtoB(&value64);

			EncodeMacBinary1_WriteObjectTag( theInfo, kBinary1_Token_Real, kBinary1_Value_Float64);
			EncodeMacBinary1_WriteObjectData(theInfo, sizeof(value64), &value64);
			break;

		default:
			NN_LOG("Unknown real precision: %d (%@)", theValue.GetPrecision(), theValue.GetString());
			break;
		}
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_String : Encode a string.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_String(MacBinary1_EncodeInfo &theInfo, const NString &theValue)
{	NIndex			n, theSize;
	uint8_t			theToken;
	NData			theData;
	const uint8_t	*thePtr;



	// Get the state we need
	//
	// Strings can be encoded as ASCII or as UTF16, depending on content.
	theToken = kBinary1_Token_StringASCII;
	theData  = theValue.GetData(kNStringEncodingUTF8, kNStringRenderNone);
	theSize  = theData.GetSize();
	thePtr   = theData.GetData();

	for (n = 0; n < theSize; n++)
		{
		if (thePtr[n] >= 0x80)
			{
			theToken = kBinary1_Token_StringUTF16;
			theData  = theValue.GetData(kNStringEncodingUTF16, kNStringRenderNone);
			theSize  = theData.GetSize() / sizeof(utf16_t);
			
			NSwapBlock_NtoB(theSize, sizeof(utf16_t), theData.GetData());
			break;
			}
		}



	// Encode the value
	EncodeMacBinary1_WriteObject(    theInfo);
	EncodeMacBinary1_WriteObjectTag( theInfo, theToken, 0, theSize);
	EncodeMacBinary1_WriteObjectData(theInfo, theData.GetSize(), theData.GetData());
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_Data : Encode data.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_Data(MacBinary1_EncodeInfo &theInfo, const NData &theValue)
{


	// Encode the value
	EncodeMacBinary1_WriteObject(    theInfo);
	EncodeMacBinary1_WriteObjectTag( theInfo, kBinary1_Token_Data, 0, theValue.GetSize());
	EncodeMacBinary1_WriteObjectData(theInfo, theValue.GetSize(),     theValue.GetData());
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_Date : Encode a date.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_Date(MacBinary1_EncodeInfo &theInfo, const NDate &theValue)
{	float64_t		value64;



	// Get the state we need
	value64 = theValue.GetTime();
	NSwapFloat64_NtoB(&value64);



	// Encode the value
	EncodeMacBinary1_WriteObject(    theInfo);
	EncodeMacBinary1_WriteObjectTag( theInfo, kBinary1_Token_Date, kBinary1_Value_Float64);
	EncodeMacBinary1_WriteObjectData(theInfo, sizeof(value64), &value64);
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_Array : Encode an array.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_Array(MacBinary1_EncodeInfo &theInfo, const NArray &theValue)
{	NIndex			n, theSize;
	UInt64List		valueRefs;
	NData			valueData;



	// Encode the objects
	theSize = theValue.GetSize();

	for (n = 0; n < theSize; n++)
		valueRefs.push_back(EncodeMacBinary1_Value(theInfo, theValue.GetValue(n)));



	// Encode the value
	valueData = EncodeMacBinary1_GetIntegerList(valueRefs, theInfo.objectsSize);

	EncodeMacBinary1_WriteObject(    theInfo);
	EncodeMacBinary1_WriteObjectTag( theInfo, kBinary1_Token_Array, 0, theSize);
	EncodeMacBinary1_WriteObjectData(theInfo, valueData.GetSize(), valueData.GetData());
}





//============================================================================
//		NPropertyList::EncodeMacBinary1_Dictionary : Encode a dictionary.
//----------------------------------------------------------------------------
void NPropertyList::EncodeMacBinary1_Dictionary(MacBinary1_EncodeInfo &theInfo, const NDictionary &theValue)
{	UInt64List		keyRefs, valueRefs;
	NData			keyData, valueData;
	NIndex			n, theSize;
	NStringList		theKeys;



	// Encode the objects
	theKeys = theValue.GetKeys();
	theSize = theKeys.size();

	for (n = 0; n < theSize; n++)
		keyRefs.push_back(EncodeMacBinary1_Value(theInfo, theKeys[n]));

	for (n = 0; n < theSize; n++)
		valueRefs.push_back(EncodeMacBinary1_Value(theInfo, theValue.GetValue(theKeys[n])));



	// Encode the value
	keyData   = EncodeMacBinary1_GetIntegerList(keyRefs,   theInfo.objectsSize);
	valueData = EncodeMacBinary1_GetIntegerList(valueRefs, theInfo.objectsSize);

	EncodeMacBinary1_WriteObject(    theInfo);
	EncodeMacBinary1_WriteObjectTag( theInfo, kBinary1_Token_Dictionary, 0, theSize);
	EncodeMacBinary1_WriteObjectData(theInfo,   keyData.GetSize(),   keyData.GetData());
	EncodeMacBinary1_WriteObjectData(theInfo, valueData.GetSize(), valueData.GetData());
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_GetUIntX : Get an integer.
//----------------------------------------------------------------------------
uint64_t NPropertyList::DecodeMacBinary1_GetUIntX(uint32_t theSize, const uint8_t *thePtr)
{	uint64_t		theValue;



	// Read the value
	theValue = 0;

	switch (theSize) {
		case 1:
			theValue = thePtr[0];
			break;
			
		case 2:
			theValue = NSwapUInt16_BtoN( *((uint16_t *) thePtr) );
			break;
			
		case 3:
			theValue =	(((uint32_t) thePtr[0]) << 16) |
						(((uint32_t) thePtr[1]) <<  8) |
						(((uint32_t) thePtr[2]) <<  0);
			break;
			
		case 4:
			theValue = NSwapUInt32_BtoN( *((uint32_t *) thePtr) );
			break;
			
		case 8:
			theValue = NSwapUInt64_BtoN( *((uint64_t *) thePtr) );
			break;
			
		default:
			NN_LOG("Unknown size: %d", theSize);
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_GetObjectOffsets : Get the object offsets.
//----------------------------------------------------------------------------
UInt64List NPropertyList::DecodeMacBinary1_GetObjectOffsets(uint64_t numObjects, uint32_t offsetsSize, const uint8_t *offsetsTable)
{	uint64_t		n, theOffset;
	UInt64List		theOffsets;



	// Get the state we need
	theOffsets.reserve((size_t) numObjects);



	// Decode the object references table
	for (n = 0; n < numObjects; n++)
		{
		theOffset     = DecodeMacBinary1_GetUIntX(offsetsSize, offsetsTable);
		offsetsTable += offsetsSize;

		theOffsets.push_back(theOffset);
		}

	return(theOffsets);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_GetObjectOffset : Get an object offset.
//----------------------------------------------------------------------------
uint64_t NPropertyList::DecodeMacBinary1_GetObjectOffset(const MacBinary1_DecodeInfo &theInfo, uint64_t objectRef)
{	uint64_t	theOffset;



	// Get the offset
	theOffset = 0;

	if (objectRef < theInfo.theOffsets.size())
		theOffset = theInfo.theOffsets[(size_t) objectRef];
	else
		NN_LOG("Invalid object reference: %lld", objectRef);
	
	return(theOffset);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_ReadObjectTag : Read an object tag.
//----------------------------------------------------------------------------
MacBinary1_Tag NPropertyList::DecodeMacBinary1_ReadObjectTag(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	const uint8_t		*thePtr;
	uint8_t				theByte;
	MacBinary1_Tag		theTag;



	// Get the state we need
	thePtr = theInfo.objectsTable + byteOffset;



	// Get the value
	//
	// Each object starts with a tag byte, which is divided into an identifying
	// token (high nibble) and 4 bits of object-specific state (low nibble).
	theByte     = thePtr[0];
	byteOffset += 1;

	theTag.theToken   = (theByte >> 4) & 0x0F;
	theTag.objectInfo = (theByte >> 0) & 0x0F;

	return(theTag);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_ReadObjectSize : Read an object size.
//----------------------------------------------------------------------------
uint64_t NPropertyList::DecodeMacBinary1_ReadObjectSize(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset, const MacBinary1_Tag &theTag)
{	uint64_t		theSize;



	// Get the size
	//
	// Variable-sized fields store their size in their objectInfo, which
	// requires a subsequent integer if more than 4 bits are required.
	if (theTag.objectInfo == kBinary1_Value_SizeInteger)
		theSize = DecodeMacBinary1_Integer(theInfo, byteOffset);
	else
		theSize = theTag.objectInfo;

	return(theSize);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_ReadObjectRef : Read an object reference.
//----------------------------------------------------------------------------
uint64_t NPropertyList::DecodeMacBinary1_ReadObjectRef(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	uint64_t		objectRef;
	const uint8_t	*thePtr;



	// Get the state we need
	thePtr = theInfo.objectsTable + byteOffset;



	// Get the value
	objectRef   = DecodeMacBinary1_GetUIntX(theInfo.objectsSize, thePtr);
	byteOffset += theInfo.objectsSize;

	return(objectRef);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Value : Decode a value.
//----------------------------------------------------------------------------
NVariant NPropertyList::DecodeMacBinary1_Value(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	NVariant			theValue;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag = DecodeMacBinary1_ReadObjectTag(theInfo, byteOffset);
	byteOffset--;



	// Decode the value
	switch (theTag.theToken) {
		case kBinary1_Token_Null:
			switch (theTag.objectInfo) {
				case kBinary1_Value_Null:
				case kBinary1_Value_Fill:
					// Null object, ignore
					byteOffset++;
					break;

				case kBinary1_Value_False:
				case kBinary1_Value_True:
					theValue = DecodeMacBinary1_Boolean(theInfo, byteOffset);
					break;

				default:
					NN_LOG("Unknown null token: %d", theTag.objectInfo);
					break;
				}
			break;

		case kBinary1_Token_Integer:
			theValue = DecodeMacBinary1_Integer(theInfo, byteOffset);
			break;

		case kBinary1_Token_Real:
			theValue = DecodeMacBinary1_Real(theInfo, byteOffset);
			break;

		case kBinary1_Token_StringASCII:
		case kBinary1_Token_StringUTF16:
			theValue = DecodeMacBinary1_String(theInfo, byteOffset);
			break;

		case kBinary1_Token_Data:
			theValue = DecodeMacBinary1_Data(theInfo, byteOffset);
			break;

		case kBinary1_Token_Date:
			theValue = DecodeMacBinary1_Date(theInfo, byteOffset);
			break;

		case kBinary1_Token_Array:
			theValue = DecodeMacBinary1_Array(theInfo, byteOffset);
			break;

		case kBinary1_Token_Dictionary:
			theValue = DecodeMacBinary1_Dictionary(theInfo, byteOffset);
			break;
		
		default:
			NN_LOG("Unknown token: %d", theTag.theToken);
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Boolean : Decode a boolean.
//----------------------------------------------------------------------------
bool NPropertyList::DecodeMacBinary1_Boolean(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	bool				theValue;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag = DecodeMacBinary1_ReadObjectTag(theInfo, byteOffset);

	NN_ASSERT(theTag.theToken == kBinary1_Token_Null);



	// Decode the value
	switch (theTag.objectInfo) {
		case kBinary1_Value_False:
			theValue = false;
			break;
				
		case kBinary1_Value_True:
			theValue = true;
			break;
			
		default:
			NN_LOG("Unknown boolean value: %d", theTag.objectInfo);
			theValue = false;
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Integer : Decode an integer.
//----------------------------------------------------------------------------
int64_t NPropertyList::DecodeMacBinary1_Integer(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	int64_t				theValue;
	const uint8_t		*thePtr;
	NIndex				theSize;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag  = DecodeMacBinary1_ReadObjectTag(theInfo, byteOffset);
	theSize = 1 << theTag.objectInfo;
	thePtr  = theInfo.objectsTable + byteOffset;

	NN_ASSERT(theTag.theToken == kBinary1_Token_Integer);



	// Decode the value
	//
	// Signed values are stored as 8 byte big-endian integers. Unsigned values
	// are stored in the smallest big-endian integer that can hold their value.
	theValue    = 0;
	byteOffset += theSize;

	switch (theSize) {
		case 1:
			theValue =                   *((const uint8_t  *) thePtr);
			break;

		case 2:
			theValue = NSwapUInt16_BtoN( *((const uint16_t *) thePtr));
			break;

		case 4:
			theValue = NSwapUInt32_BtoN( *((const uint32_t *) thePtr));
			break;

		case 8:
			theValue = NSwapInt64_BtoN( *((const int64_t  *) thePtr));
			break;

		default:
			NN_LOG("Unknown integer size: %d", theSize);
			break;
		}

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Real : Decode a floating point value.
//----------------------------------------------------------------------------
float64_t NPropertyList::DecodeMacBinary1_Real(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	float64_t			value64, theValue;
	float32_t			value32;
	const uint8_t		*thePtr;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag = DecodeMacBinary1_ReadObjectTag(theInfo, byteOffset);
	thePtr = theInfo.objectsTable + byteOffset;

	NN_ASSERT(theTag.theToken == kBinary1_Token_Real);



	// Decode the value
	theValue = 0.0;

	switch (theTag.objectInfo) {
		case kBinary1_Value_Float32:
			memcpy(&value32, thePtr, sizeof(value32));
			NSwapFloat32_BtoN(&value32);

			theValue    = value32;
			byteOffset += sizeof(value32);
			break;

		case kBinary1_Value_Float64:
			memcpy(&value64, thePtr, sizeof(value64));
			NSwapFloat64_BtoN(&value64);

			theValue    = value64;
			byteOffset += sizeof(value64);
			break;

		default:
			NN_LOG("Unknown real type: %d", theTag.objectInfo);
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_String : Decode a string.
//----------------------------------------------------------------------------
NString NPropertyList::DecodeMacBinary1_String(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	NString				theValue;
	const uint8_t		*thePtr;
	uint64_t			theSize;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag  = DecodeMacBinary1_ReadObjectTag( theInfo, byteOffset);
	theSize = DecodeMacBinary1_ReadObjectSize(theInfo, byteOffset, theTag);
	thePtr  = theInfo.objectsTable + byteOffset;

	NN_ASSERT((theSize * sizeof(utf16_t)) <= (uint64_t) kNIndexMax);



	// Decode the value
	switch (theTag.theToken) {
		case kBinary1_Token_StringASCII:
			theSize     = theSize * sizeof(utf8_t);
			theValue    = NString(thePtr, (NIndex) theSize, kNStringEncodingASCII);
			byteOffset += theSize;
			break;

		case kBinary1_Token_StringUTF16:
			theSize     = theSize * sizeof(utf16_t);
			theValue    = NString(thePtr, (NIndex) theSize, kNStringEncodingUTF16BE);
			byteOffset += theSize;
			break;
		
		default:
			NN_LOG("Unknown string type: %d", theTag.theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Data : Decode binary data.
//----------------------------------------------------------------------------
NVariant NPropertyList::DecodeMacBinary1_Data(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	NEncoder			theEncoder;
	NVariant			theValue;
	NData				theData;
	const uint8_t		*thePtr;
	uint64_t			theSize;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag  = DecodeMacBinary1_ReadObjectTag( theInfo, byteOffset);
	theSize = DecodeMacBinary1_ReadObjectSize(theInfo, byteOffset, theTag);
	thePtr  = theInfo.objectsTable + byteOffset;

	NN_ASSERT(theTag.theToken == kBinary1_Token_Data);
	NN_ASSERT(theSize         <= (uint64_t) kNIndexMax);



	// Decode the data
	theData     = NData((NIndex) theSize, thePtr);
	byteOffset += theSize;



	// Decode the value
	//
	// NEncoder objects are encoded to binary plists as raw data, so when decoding
	// raw data we must push data through NEncoder (to see if we can reconstruct an
	// object from it, or if it really is just raw data).
	theValue = theEncoder.Decode(theData);

	if (!theValue.IsValid())
		theValue = theData;

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Date : Decode a date.
//----------------------------------------------------------------------------
NDate NPropertyList::DecodeMacBinary1_Date(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	NDate				theValue;
	float64_t			value64;
	const uint8_t		*thePtr;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag = DecodeMacBinary1_ReadObjectTag(theInfo, byteOffset);
	thePtr = theInfo.objectsTable + byteOffset;

	NN_ASSERT(theTag.theToken   == kBinary1_Token_Date);
	NN_ASSERT(theTag.objectInfo == kBinary1_Value_Float64);
	


	// Decode the value
	memcpy(&value64, thePtr, sizeof(value64));
	NSwapFloat64_BtoN(&value64);

	theValue    =  NDate(value64);
	byteOffset += sizeof(value64);

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Array : Decode an array.
//----------------------------------------------------------------------------
NArray NPropertyList::DecodeMacBinary1_Array(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	uint64_t			valueOffset;
	NVariant			valueObject;
	uint64_t			n, theSize;
	UInt64List			valueRefs;
	NArray				theValue;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag  = DecodeMacBinary1_ReadObjectTag( theInfo, byteOffset);
	theSize = DecodeMacBinary1_ReadObjectSize(theInfo, byteOffset, theTag);

	NN_ASSERT(theTag.theToken == kBinary1_Token_Array);



	// Read the references
	for (n = 0; n < theSize; n++)
		valueRefs.push_back(DecodeMacBinary1_ReadObjectRef(theInfo, byteOffset));



	// Decode the value
	for (n = 0; n < theSize; n++)
		{
		valueOffset = DecodeMacBinary1_GetObjectOffset(theInfo, valueRefs[(size_t) n]);
		valueObject = DecodeMacBinary1_Value(          theInfo, valueOffset);
		
		theValue.AppendValue(valueObject);
		}

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Dictionary : Decode a dictionary.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacBinary1_Dictionary(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset)
{	uint64_t			keyOffset, valueOffset;
	NVariant			keyObject, valueObject;
	UInt64List			keyRefs, valueRefs;
	uint64_t			n, theSize;
	NDictionary			theValue;
	NString				theKey;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag  = DecodeMacBinary1_ReadObjectTag( theInfo, byteOffset);
	theSize = DecodeMacBinary1_ReadObjectSize(theInfo, byteOffset, theTag);

	NN_ASSERT(theTag.theToken == kBinary1_Token_Dictionary);



	// Read the references
	for (n = 0; n < theSize; n++)
		keyRefs.push_back(  DecodeMacBinary1_ReadObjectRef(theInfo, byteOffset));

	for (n = 0; n < theSize; n++)
		valueRefs.push_back(DecodeMacBinary1_ReadObjectRef(theInfo, byteOffset));



	// Decode the value
	for (n = 0; n < theSize; n++)
		{
		keyOffset   = DecodeMacBinary1_GetObjectOffset(theInfo,   keyRefs[(size_t) n]);
		valueOffset = DecodeMacBinary1_GetObjectOffset(theInfo, valueRefs[(size_t) n]);

		keyObject   = DecodeMacBinary1_Value(theInfo,   keyOffset);
		valueObject = DecodeMacBinary1_Value(theInfo, valueOffset);
		
		if (keyObject.GetValue(theKey))
			theValue.SetValue( theKey, valueObject);
		else
			NN_LOG("Found non-string dictionary key!");
		}

	return(theValue);
}





