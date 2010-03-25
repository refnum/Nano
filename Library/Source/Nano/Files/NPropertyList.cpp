/*	NAME:
		NPropertyList.cpp

	DESCRIPTION:
		Property list file.

		The Mac XML1 property list format is documented at:
		
			http://www.apple.com/DTDs/PropertyList-1.0.dtd

		The Mac Binary1 property list format is documented at:
		
			http://opensource.apple.com/source/CF/CF-550/CFBinaryPList.c
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"
#include "NMathUtilities.h"
#include "NDataEncoder.h"
#include "NXMLEncoder.h"
#include "NEncoder.h"
#include "NByteSwap.h"
#include "NPropertyList.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Magic
static const UInt8 kMagicMacXML1[]									= { 0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20  };
static const UInt8 kMagicMacBinary1[]								= { 0x62, 0x70, 0x6C, 0x69, 0x73, 0x74, 0x30, 0x30 };


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
static const UInt8 kBinary1_Token_Null								= 0x00;
static const UInt8 kBinary1_Token_Integer							= 0x01;
static const UInt8 kBinary1_Token_Real								= 0x02;
static const UInt8 kBinary1_Token_Date								= 0x03;
static const UInt8 kBinary1_Token_Data								= 0x04;
static const UInt8 kBinary1_Token_StringUTF8						= 0x05;
static const UInt8 kBinary1_Token_StringUTF16						= 0x06;
static const UInt8 kBinary1_Token_Array								= 0x0A;
static const UInt8 kBinary1_Token_Dictionary						= 0x0D;

static const UInt8 kBinary1_Value_Null								= 0x00;
static const UInt8 kBinary1_Value_False								= 0x08;
static const UInt8 kBinary1_Value_True								= 0x09;
static const UInt8 kBinary1_Value_IntegerSize						= 0x0F;
static const UInt8 kBinary1_Value_Float32							= 0x02;
static const UInt8 kBinary1_Value_Float64							= 0x03;





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
// Mac Binary 1.0
typedef struct {
	UInt8		reserved[6];
	UInt8		objectTableRefSize;
	UInt8		objectRefSize;
	UInt64		numObjects;
	UInt64		topObjectRef;
	UInt64		objectTableOffset;
} MacBinary1_Trailer;	

NBYTESWAP_BEGIN(MacBinary1_Trailer)
	NBYTESWAP_B_UInt8_Array		(reserved, 6)
	NBYTESWAP_B_UInt8			(objectTableRefSize)
	NBYTESWAP_B_UInt8			(objectRefSize)
	NBYTESWAP_B_UInt64			(numObjects)
	NBYTESWAP_B_UInt64			(topObjectRef)
	NBYTESWAP_B_UInt64			(objectTableOffset)
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





//============================================================================
//		NPropertyList::GetFormat : Get the format of an encoded property list.
//----------------------------------------------------------------------------
#pragma mark -
NPropertyListFormat NPropertyList::GetFormat(const NData &theData)
{	NPropertyListFormat		theFormat;
	const UInt8				*dataPtr;
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
NData NPropertyList::EncodeMacBinary1(const NDictionary &/*theState*/)
{


	// To do
	NN_LOG("Encoding kNPropertyListMacBinary1 is not supported");
	
	return(NData());
}





//============================================================================
//		NPropertyList::DecodeMacBinary1 : Decode kNPropertyListMacBinary1.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacBinary1(const NData &theData)
{	MacBinary1_Trailer			theTrailer;
	NVariant					topObject;
	UInt32						topOffset;
	NDictionary					theState;
	MacBinary1_Info				theInfo;



	// Validate our state
	NN_ASSERT(sizeof(theTrailer) == 32);



	// Decode the trailer
	if (theData.GetSize() < (NIndex) sizeof(theTrailer))
		return(theState);

	memcpy(&theTrailer, theData.GetData(theData.GetSize() - sizeof(theTrailer)), sizeof(theTrailer));
	NBYTESWAP_DECODE(1, MacBinary1_Trailer, &theTrailer);

	if (theTrailer.numObjects        >= kUInt32Max ||
		theTrailer.topObjectRef      >= kUInt32Max ||
		theTrailer.objectTableOffset >= kUInt32Max)
		{
		NN_LOG("NPropertyList does not support 4Gb plists");
		return(theState);
		}



	// Prepare to decode
	theInfo.basePtr       = theData.GetData();
	theInfo.theObjects    = DecodeMacBinary1_GetObjectRefs(theTrailer.numObjects, theTrailer.objectTableRefSize, theData.GetData(theTrailer.objectTableOffset));
	theInfo.objectRefSize = theTrailer.objectRefSize;



	// Decode the top object
	topOffset = DecodeMacBinary1_GetObjectOffset(theInfo, theTrailer.topObjectRef);
	topObject = DecodeMacBinary1_Value(          theInfo, topOffset);
	
	if (!topObject.GetValue(theState))
		NN_LOG("Unknown top object - not a dictionary");

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
	gregDate = theValue.GetGregorianDate(kNTimeZoneUTC);

	valueText.Format(kFormatISO8601, gregDate.year, gregDate.month,  gregDate.day,
									 gregDate.hour, gregDate.minute, (UInt32) gregDate.second);



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
				NN_LOG("Unknown property list value!");
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
SInt64 NPropertyList::DecodeMacXML1_Integer(const NXMLNode *theNode)
{	NNumber		rawValue;
	SInt64		theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenInteger));



	// Decode the value
	if (!rawValue.SetValue(theNode->GetElementContents()) || !rawValue.IsInteger())
		NN_LOG("Unable to extract integer: [%@]", theNode->GetElementContents());
	
	theValue = rawValue.GetSInt64();
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML1_Real : Decode a real.
//----------------------------------------------------------------------------
Float64 NPropertyList::DecodeMacXML1_Real(const NXMLNode *theNode)
{	NNumber		rawValue;
	Float64		theValue;



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
{	int					valMonth, valDay,valHour, valMinute;
	SInt32				numSecs, numItems;
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

	NN_ASSERT(numItems == 6);

	gregDate.month  = valMonth;
	gregDate.day    = valDay;
	gregDate.hour   = valHour;
	gregDate.minute = valMinute;



	// Decode the value
	if (numItems == 6)
		{
		gregDate.second = numSecs;
		theValue.SetGregorianDate(gregDate, kNTimeZoneUTC);
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
//		NPropertyList::DecodeMacBinary1_GetUIntX : Get an integer.
//----------------------------------------------------------------------------
UInt32 NPropertyList::DecodeMacBinary1_GetUIntX(UInt32 theSize, const UInt8 *thePtr)
{	UInt32		theValue;



	// Read the value
	theValue = 0;

	switch (theSize) {
		case 1:
			theValue = thePtr[0];
			break;
			
		case 2:
			theValue = NSwapUInt16_BtoN( *((UInt16 *) thePtr) );
			break;
			
		case 3:
			theValue =	(((UInt32) thePtr[0]) << 16) |
						(((UInt32) thePtr[1]) <<  8) |
						(((UInt32) thePtr[2]) <<  0);
			break;
			
		case 4:
			theValue = NSwapUInt32_BtoN( *((UInt32 *) thePtr) );
			break;
			
		default:
			NN_LOG("Unknown size: %d", theSize);
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_GetObjectRefs : Get the object references.
//----------------------------------------------------------------------------
UInt32List NPropertyList::DecodeMacBinary1_GetObjectRefs(NIndex numObjects, UInt32 theSize, const UInt8 *thePtr)
{	UInt32List		theObjects;
	UInt32			objectRef;
	NIndex			n;



	// Get the state we need
	theObjects.reserve(numObjects);



	// Decode the object references table
	for (n = 0; n < numObjects; n++)
		{
		objectRef = DecodeMacBinary1_GetUIntX(theSize, thePtr);
		thePtr += theSize;

		theObjects.push_back(objectRef);
		}

	return(theObjects);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_GetObjectOffset : Get an object offset.
//----------------------------------------------------------------------------
UInt32 NPropertyList::DecodeMacBinary1_GetObjectOffset(const MacBinary1_Info &theInfo, UInt32 objectRef)
{	UInt32		theOffset;



	// Get the offset
	theOffset = 0;

	if (objectRef < theInfo.theObjects.size())
		theOffset = theInfo.theObjects[objectRef];
	else
		NN_LOG("Invalid object reference: %d", objectRef);
	
	return(theOffset);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_ReadObjectTag : Read an object tag.
//----------------------------------------------------------------------------
MacBinary1_Tag NPropertyList::DecodeMacBinary1_ReadObjectTag(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
{	const UInt8			*thePtr;
	UInt8				theByte;
	MacBinary1_Tag		theTag;



	// Get the state we need
	thePtr = theInfo.basePtr + byteOffset;



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
UInt32 NPropertyList::DecodeMacBinary1_ReadObjectSize(const MacBinary1_Info &theInfo, UInt32 &byteOffset, UInt32 theSize)
{


	// Get the size
	//
	// Variable-sized fields typically store the size in their objectInfo,
	// which requires a subsequent integer if more than 4 bits are required.
	if (theSize == kBinary1_Value_IntegerSize)
		theSize = DecodeMacBinary1_Integer(theInfo, byteOffset);

	return(theSize);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_ReadObjectRef : Read an object reference.
//----------------------------------------------------------------------------
UInt32 NPropertyList::DecodeMacBinary1_ReadObjectRef(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
{	UInt32			objectRef;
	const UInt8		*thePtr;



	// Get the state we need
	thePtr = theInfo.basePtr + byteOffset;



	// Get the value
	objectRef   = DecodeMacBinary1_GetUIntX(theInfo.objectRefSize, thePtr);
	byteOffset += theInfo.objectRefSize;

	return(objectRef);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Value : Decode a value.
//----------------------------------------------------------------------------
NVariant NPropertyList::DecodeMacBinary1_Value(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
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

		case kBinary1_Token_StringUTF8:
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
bool NPropertyList::DecodeMacBinary1_Boolean(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
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
SInt64 NPropertyList::DecodeMacBinary1_Integer(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
{	SInt64				theValue;
	const UInt8			*thePtr;
	NIndex				theSize;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag  = DecodeMacBinary1_ReadObjectTag(theInfo, byteOffset);
	theSize = 1 << theTag.objectInfo;
	thePtr  = theInfo.basePtr + byteOffset;

	NN_ASSERT(theTag.theToken == kBinary1_Token_Integer);



	// Decode the value
	//
	// 1, 2, and 4 byte integers are stored as unsigned. 8 byte integers are
	// stored as signed, and negative values are always stored as 8 bytes.
	theValue    = 0;
	byteOffset += theSize;

	switch (theSize) {
		case 1:
			theValue =                   *((UInt8  *) thePtr);
			break;

		case 2:
			theValue = NSwapUInt16_BtoN( *((UInt16 *) thePtr) );
			break;

		case 4:
			theValue = NSwapUInt32_BtoN( *((UInt32 *) thePtr) );
			break;

		case 8:
			theValue = NSwapSInt64_BtoN( *((SInt64 *) thePtr) );
			break;

		default:
			NN_LOG("Unknown integer size: %d", theSize);
			break;
		}

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Real : Decode a real.
//----------------------------------------------------------------------------
Float64 NPropertyList::DecodeMacBinary1_Real(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
{	Float64				value64, theValue;
	Float32				value32;
	const UInt8			*thePtr;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag = DecodeMacBinary1_ReadObjectTag(theInfo, byteOffset);
	thePtr = theInfo.basePtr + byteOffset;

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
NString NPropertyList::DecodeMacBinary1_String(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
{	NString				theValue;
	const UInt8			*thePtr;
	NIndex				theSize;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag  = DecodeMacBinary1_ReadObjectTag( theInfo, byteOffset);
	theSize = DecodeMacBinary1_ReadObjectSize(theInfo, byteOffset, theTag.objectInfo);
	thePtr  = theInfo.basePtr + byteOffset;



	// Decode the value
	switch (theTag.theToken) {
		case kBinary1_Token_StringUTF8:
			theSize     = theSize * sizeof(UTF8Char);
			theValue    = NString(thePtr, theSize, kNStringEncodingUTF8);
			byteOffset += theSize;
			break;

		case kBinary1_Token_StringUTF16:
			theSize     = theSize * sizeof(UTF16Char);
			theValue    = NString(thePtr, theSize, kNStringEncodingUTF16BE);
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
NData NPropertyList::DecodeMacBinary1_Data(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
{	NData				theValue;
	const UInt8			*thePtr;
	NIndex				theSize;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag  = DecodeMacBinary1_ReadObjectTag( theInfo, byteOffset);
	theSize = DecodeMacBinary1_ReadObjectSize(theInfo, byteOffset, theTag.objectInfo);
	thePtr  = theInfo.basePtr + byteOffset;

	NN_ASSERT(theTag.theToken == kBinary1_Token_Data);



	// Decode the value
	theValue    = NData(theSize, thePtr);
	byteOffset += theSize;

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Date : Decode a date.
//----------------------------------------------------------------------------
NDate NPropertyList::DecodeMacBinary1_Date(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
{	NDate				theValue;
	Float64				value64;
	const UInt8			*thePtr;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag = DecodeMacBinary1_ReadObjectTag(theInfo, byteOffset);
	thePtr = theInfo.basePtr + byteOffset;

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
NArray NPropertyList::DecodeMacBinary1_Array(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
{	UInt32				valueOffset;
	NVariant			valueObject;
	NIndex				n, theSize;
	UInt32List			valueRefs;
	NArray				theValue;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag  = DecodeMacBinary1_ReadObjectTag( theInfo, byteOffset);
	theSize = DecodeMacBinary1_ReadObjectSize(theInfo, byteOffset, theTag.objectInfo);

	NN_ASSERT(theTag.theToken == kBinary1_Token_Array);



	// Read the references
	for (n = 0; n < theSize; n++)
		valueRefs.push_back(DecodeMacBinary1_ReadObjectRef(theInfo, byteOffset));



	// Decode the value
	for (n = 0; n < theSize; n++)
		{
		valueOffset = DecodeMacBinary1_GetObjectOffset(theInfo, valueRefs[n]);
		valueObject = DecodeMacBinary1_Value(          theInfo, valueOffset);
		
		theValue.AppendValue(valueObject);
		}

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacBinary1_Dictionary : Decode a dictionary.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacBinary1_Dictionary(const MacBinary1_Info &theInfo, UInt32 &byteOffset)
{	UInt32				keyOffset, valueOffset;
	NVariant			keyObject, valueObject;
	UInt32List			keyRefs, valueRefs;
	NIndex				n, theSize;
	NDictionary			theValue;
	NString				theKey;
	MacBinary1_Tag		theTag;



	// Get the state we need
	theTag  = DecodeMacBinary1_ReadObjectTag( theInfo, byteOffset);
	theSize = DecodeMacBinary1_ReadObjectSize(theInfo, byteOffset, theTag.objectInfo);

	NN_ASSERT(theTag.theToken == kBinary1_Token_Dictionary);



	// Read the references
	for (n = 0; n < theSize; n++)
		keyRefs.push_back(  DecodeMacBinary1_ReadObjectRef(theInfo, byteOffset));

	for (n = 0; n < theSize; n++)
		valueRefs.push_back(DecodeMacBinary1_ReadObjectRef(theInfo, byteOffset));



	// Decode the value
	for (n = 0; n < theSize; n++)
		{
		keyOffset   = DecodeMacBinary1_GetObjectOffset(theInfo,   keyRefs[n]);
		valueOffset = DecodeMacBinary1_GetObjectOffset(theInfo, valueRefs[n]);

		keyObject   = DecodeMacBinary1_Value(theInfo,   keyOffset);
		valueObject = DecodeMacBinary1_Value(theInfo, valueOffset);
		
		if (keyObject.GetValue(theKey))
			theValue.SetValue( theKey, valueObject);
		else
			NN_LOG("Found non-string dictionary key!");
		}

	return(theValue);
}





