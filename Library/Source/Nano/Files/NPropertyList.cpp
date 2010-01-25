/*	NAME:
		NPropertyList.cpp

	DESCRIPTION:
		Property list file.
	
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
#include "NPropertyList.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Magic
static const UInt8 kMagicMacXML_1_0[]								= { 0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20  };
static const UInt8 kMagicMacBinary_1_0[]							= { 0x62, 0x70, 0x6C, 0x69, 0x73, 0x74, 0x30, 0x30 };


// Tokens
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


// Misc
static const NString kPListSystemID_1_0								= "http://www.apple.com/DTDs/PropertyList-1.0.dtd";
static const NString kPListPublicID_1_0								= "-//Apple Computer//DTD PLIST 1.0//EN";
static const NString kFormatISO8601									= "%04d-%02d-%02dT%02d:%02d:%02dZ";





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
		case kNPropertyListMacXML_1_0:
			theData = EncodeMacXML_1_0(theState);
			break;

		case kNPropertyListMacBinary_1_0:
			theData = EncodeMacBinary_1_0(theState);
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
		case kNPropertyListMacXML_1_0:
			theState = DecodeMacXML_1_0(theData);
			break;

		case kNPropertyListMacBinary_1_0:
			theState = DecodeMacBinary_1_0(theData);
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

	MATCH_FORMAT(kMagicMacXML_1_0,    kNPropertyListMacXML_1_0);
	MATCH_FORMAT(kMagicMacBinary_1_0, kNPropertyListMacBinary_1_0);
	
	#undef MATCH_FORMAT
	
	return(theFormat);
}





//============================================================================
//		NPropertyList::EncodeMacXML_1_0 : Encode kNPropertyListMacXML_1_0.
//----------------------------------------------------------------------------
NData NPropertyList::EncodeMacXML_1_0(const NDictionary &theState)
{	NXMLNode		*nodeDoc, *nodeDocType, *nodePList, *nodeDict;
	NXMLEncoder		theEncoder;
	NString			theXML;



	// Create the nodes
	nodeDict = EncodeMacXML_1_0_Dictionary(theState);

	nodePList = new NXMLNode(kNXMLNodeElement, kTokenPList);
	nodePList->SetElementAttribute(kTokenVersion, "1.0");
	nodePList->AddChild(nodeDict);

	nodeDocType = new NXMLNode(kNXMLNodeDocType, kTokenPList);
	nodeDocType->SetDocTypeSystemID(kPListSystemID_1_0);
	nodeDocType->SetDocTypePublicID(kPListPublicID_1_0);

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
//		NPropertyList::DecodeMacXML_1_0 : Decode kNPropertyListMacXML_1_0.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacXML_1_0(const NData &theData)
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
		theState = DecodeMacXML_1_0_Dictionary(nodeDict);

	delete nodeDoc;

	return(theState);
}




//============================================================================
//		NPropertyList::EncodeMacBinary_1_0 : Encode kNPropertyListMacBinary_1_0.
//----------------------------------------------------------------------------
NData NPropertyList::EncodeMacBinary_1_0(const NDictionary &/*theState*/)
{


	// To do
	NN_LOG("Encoding kNPropertyListMacBinary_1_0 is not supported");
	
	return(NData());
}





//============================================================================
//		NPropertyList::DecodeMacBinary_1_0 : Decode kNPropertyListMacBinary_1_0.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacBinary_1_0(const NData &/*theData*/)
{


	// To do
	NN_LOG("Decoding kNPropertyListMacBinary_1_0 is not supported");
	
	return(NDictionary());
}





//============================================================================
//		NPropertyList::EncodeMacXML_1_0_Boolean : Encode a bool.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Boolean(bool theValue)
{	NXMLNode	*theNode;



	// Encode the value
	theNode = new NXMLNode(kNXMLNodeElement, theValue ? kTokenTrue : kTokenFalse);
	theNode->SetElementUnpaired(true);
	
	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML_1_0_Number : Encode a number.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Number(const NNumber &theValue)
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
//		NPropertyList::EncodeMacXML_1_0_String : Encode a string.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_String(const NString &theValue)
{	NXMLNode	*theNode;



	// Encode the value
	theNode = new NXMLNode(kNXMLNodeElement, kTokenString);
	theNode->SetElementContents(theValue);
	
	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML_1_0_Data : Encode binary data.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Data(const NData &theValue)
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
//		NPropertyList::EncodeMacXML_1_0_Date : Encode a date.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Date(const NDate &theValue)
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
//		NPropertyList::EncodeMacXML_1_0_Array : Encode an array.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Array(const NArray &theValue)
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
			nodeValue = EncodeMacXML_1_0_Boolean(theValue.GetValueBoolean(n));

		else if (theItem.IsNumeric())
			nodeValue = EncodeMacXML_1_0_Number(theValue.GetValue(n));

		else if (theItem.IsType(typeid(NString)))
			nodeValue = EncodeMacXML_1_0_String(theValue.GetValueString(n));

		else if (theItem.IsType(typeid(NData)))
			nodeValue = EncodeMacXML_1_0_Data(theValue.GetValueData(n));

		else if (theItem.IsType(typeid(NDate)))
			nodeValue = EncodeMacXML_1_0_Date(theValue.GetValueDate(n));

		else if (theItem.IsType(typeid(NArray)))
			nodeValue = EncodeMacXML_1_0_Array(theValue.GetValueArray(n));

		else if (theItem.IsType(typeid(NDictionary)))
			nodeValue = EncodeMacXML_1_0_Dictionary(theValue.GetValueDictionary(n));

		else
			{
			theData = theEncoder.Encode(theItem);
			if (!theData.IsEmpty())
				nodeValue = EncodeMacXML_1_0_Data(theData);
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
//		NPropertyList::EncodeMacXML_1_0_Dictionary : Encode a dictionary.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Dictionary(const NDictionary &theValue)
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
			nodeValue = EncodeMacXML_1_0_Boolean(theValue.GetValueBoolean(theKey));

		else if (theItem.IsNumeric())
			nodeValue = EncodeMacXML_1_0_Number(theValue.GetValue(theKey));

		else if (theItem.IsType(typeid(NString)))
			nodeValue = EncodeMacXML_1_0_String(theValue.GetValueString(theKey));

		else if (theItem.IsType(typeid(NData)))
			nodeValue = EncodeMacXML_1_0_Data(theValue.GetValueData(theKey));

		else if (theItem.IsType(typeid(NDate)))
			nodeValue = EncodeMacXML_1_0_Date(theValue.GetValueDate(theKey));

		else if (theItem.IsType(typeid(NArray)))
			nodeValue = EncodeMacXML_1_0_Array(theValue.GetValueArray(theKey));

		else if (theItem.IsType(typeid(NDictionary)))
			nodeValue = EncodeMacXML_1_0_Dictionary(theValue.GetValueDictionary(theKey));

		else
			{
			theData = theEncoder.Encode(theItem);
			if (!theData.IsEmpty())
				nodeValue = EncodeMacXML_1_0_Data(theData);
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
//		NPropertyList::DecodeMacXML_1_0_Boolean : Decode a bool.
//----------------------------------------------------------------------------
bool NPropertyList::DecodeMacXML_1_0_Boolean(const NXMLNode *theNode)
{	bool	theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenTrue) || theNode->IsElement(kTokenFalse));



	// Decode the value
	theValue = theNode->IsElement(kTokenTrue);
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML_1_0_Integer : Decode an integer.
//----------------------------------------------------------------------------
SInt64 NPropertyList::DecodeMacXML_1_0_Integer(const NXMLNode *theNode)
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
//		NPropertyList::DecodeMacXML_1_0_Real : Decode a real.
//----------------------------------------------------------------------------
Float64 NPropertyList::DecodeMacXML_1_0_Real(const NXMLNode *theNode)
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
//		NPropertyList::DecodeMacXML_1_0_String : Decode a string.
//----------------------------------------------------------------------------
NString NPropertyList::DecodeMacXML_1_0_String(const NXMLNode *theNode)
{	NString		theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenString));



	// Decode the value
	theValue = theNode->GetElementContents();
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML_1_0_Data : Decode binary data.
//----------------------------------------------------------------------------
NData NPropertyList::DecodeMacXML_1_0_Data(const NXMLNode *theNode)
{	NDataEncoder	theEncoder;
	NData			theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenData));



	// Decode the value
	theValue = theEncoder.Decode(theNode->GetElementContents(), kNDataEncodingB64);

	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML_1_0_Date : Decode a date.
//----------------------------------------------------------------------------
NDate NPropertyList::DecodeMacXML_1_0_Date(const NXMLNode *theNode)
{	SInt32				numSecs, numItems;
	NString				valueText;
	NGregorianDate		gregDate;
	NDate				theValue;



	// Validate our parameters
	NN_ASSERT(theNode->IsElement(kTokenDate));



	// Get the state we need
	valueText = theNode->GetElementContents();
	numItems  = sscanf(valueText.GetUTF8(), kFormatISO8601.GetUTF8(),
							&gregDate.year, &gregDate.month,  &gregDate.day,
							&gregDate.hour, &gregDate.minute, &numSecs);

	NN_ASSERT(numItems == 6);
	
	
	
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
//		NPropertyList::DecodeMacXML_1_0_Array : Decode an array.
//----------------------------------------------------------------------------
NArray NPropertyList::DecodeMacXML_1_0_Array(const NXMLNode *theNode)
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
			theValue.AppendValue(DecodeMacXML_1_0_Boolean(nodeValue));

		else if (nodeValue->IsElement(kTokenInteger))
			theValue.AppendValue(DecodeMacXML_1_0_Integer(nodeValue));
		
		else if (nodeValue->IsElement(kTokenReal))
			theValue.AppendValue(DecodeMacXML_1_0_Real(nodeValue));
		
		else if (nodeValue->IsElement(kTokenString))
			theValue.AppendValue(DecodeMacXML_1_0_String(nodeValue));
		
		else if (nodeValue->IsElement(kTokenData))
			{
			theData   = DecodeMacXML_1_0_Data(nodeValue);
			theObject = theEncoder.Decode(theData);

			if (theObject.IsValid())
				theValue.AppendValue(theObject);
			else
				theValue.AppendValue(theData);
			}
		
		else if (nodeValue->IsElement(kTokenDate))
			theValue.AppendValue(DecodeMacXML_1_0_Date(nodeValue));
		
		else if (nodeValue->IsElement(kTokenArray))
			theValue.AppendValue(DecodeMacXML_1_0_Array(nodeValue));

		else if (nodeValue->IsElement(kTokenDictionary))
			theValue.AppendValue(DecodeMacXML_1_0_Dictionary(nodeValue));
		
		else
			NN_LOG("Unknown property list value [%@]!", nodeValue->GetTextValue());
		}
	
	return(theValue);
}





//============================================================================
//		NPropertyList::DecodeMacXML_1_0_Dictionary : Decode a dictionary.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacXML_1_0_Dictionary(const NXMLNode *theNode)
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
			theValue.SetValue(theKey, DecodeMacXML_1_0_Boolean(nodeValue));
		
		else if (nodeValue->IsElement(kTokenInteger))
			theValue.SetValue(theKey, DecodeMacXML_1_0_Integer(nodeValue));
		
		else if (nodeValue->IsElement(kTokenReal))
			theValue.SetValue(theKey, DecodeMacXML_1_0_Real(nodeValue));

		else if (nodeValue->IsElement(kTokenString))
			theValue.SetValue(theKey, DecodeMacXML_1_0_String(nodeValue));
		
		else if (nodeValue->IsElement(kTokenData))
			{
			theData   = DecodeMacXML_1_0_Data(nodeValue);
			theObject = theEncoder.Decode(theData);

			if (theObject.IsValid())
				theValue.SetValue(theKey, theObject);
			else
				theValue.SetValue(theKey, theData);
			}
		
		else if (nodeValue->IsElement(kTokenDate))
			theValue.SetValue(theKey, DecodeMacXML_1_0_Date(nodeValue));
		
		else if (nodeValue->IsElement(kTokenArray))
			theValue.SetValue(theKey, DecodeMacXML_1_0_Array(nodeValue));
		
		else if (nodeValue->IsElement(kTokenDictionary))
			theValue.SetValue(theKey, DecodeMacXML_1_0_Dictionary(nodeValue));
		
		else
			NN_LOG("Unknown property list value [%@] [%@]!", theKey, nodeValue->GetTextValue());
		}

	return(theValue);
}




