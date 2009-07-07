/*	NAME:
		NPropertyList.cpp

	DESCRIPTION:
		Property list file.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NFileUtilities.h"
#include "NMathUtilities.h"
#include "NXMLEncoder.h"
#include "NB64Encoder.h"
#include "NPropertyList.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt8 kMagicMacXML_1_0[]								= { 0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20  };
static const UInt8 kMagicMacBinary_1_0[]							= { 0x62, 0x70, 0x6C, 0x69, 0x73, 0x74, 0x30, 0x30 };





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
			if (theFormat == kNPropertyListInvalid && dataSize >= GET_ARRAY_SIZE(_magic))	\
				{																			\
				if (memcmp(dataPtr, _magic, GET_ARRAY_SIZE(_magic)) == 0)					\
					{																		\
					theFormat = _format;													\
					}																		\
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
{	NXMLNode		*thePList, *theDict;
	NXMLEncoder		theEncoder;
	NString			theXML;



	// Create the nodes
	thePList = new NXMLNode(kXMLNodeElement, "plist");
	thePList->SetElementAttribute("version", "1.0");
	
	theDict = EncodeMacXML_1_0_Dictionary(theState);
	thePList->AddChild(theDict);



	// Encode the XML
	theXML = theEncoder.Encode(thePList);
	delete thePList;
	
	return(theXML.GetData());
}





//============================================================================
//		NPropertyList::DecodeMacXML_1_0 : Decode kNPropertyListMacXML_1_0.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacXML_1_0(const NData &theData)
{
}




//============================================================================
//		NPropertyList::EncodeMacBinary_1_0 : Encode kNPropertyListMacBinary_1_0.
//----------------------------------------------------------------------------
NData NPropertyList::EncodeMacBinary_1_0(const NDictionary &theState)
{
}





//============================================================================
//		NPropertyList::DecodeMacBinary_1_0 : Decode kNPropertyListMacBinary_1_0.
//----------------------------------------------------------------------------
NDictionary NPropertyList::DecodeMacBinary_1_0(const NData &theData)
{
}





//============================================================================
//		NPropertyList::EncodeMacXML_1_0_Dictionary : Encode a dictionary.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Dictionary(const NDictionary &theValue)
{	NXMLNode					*theNode, *nodeKey, *nodeValue;
	NVariant					theItem;
	NStringList					theKeys;
	NStringListConstIterator	theIter;
	NString						theKey;



	// Get the state we need
	theKeys = theValue.GetKeys(true);
	theNode = new NXMLNode(kXMLNodeElement, "dict");



	// Encode the value
	for (theIter = theKeys.begin(); theIter != theKeys.end(); theIter++)
		{
		// Get the state we need
		theKey  = *theIter;
		theItem = theValue.GetValue(theKey);
		
		
		
		// Create the key-value pair
		nodeKey = new NXMLNode(kXMLNodeElement, "key");
		nodeKey->SetElementContents(theKey);

		if (theItem.IsType(typeid(NDictionary)))
			nodeValue = EncodeMacXML_1_0_Dictionary(theValue.GetValueDictionary(theKey));

		else if (theItem.IsType(typeid(NArray)))
			nodeValue = EncodeMacXML_1_0_Array(theValue.GetValueArray(theKey));

		else if (theItem.IsType(typeid(NString)))
			nodeValue = EncodeMacXML_1_0_String(theValue.GetValueString(theKey));

		else if (theItem.IsNumeric())
			nodeValue = EncodeMacXML_1_0_Number(theValue.GetValue(theKey));

		else if (theItem.IsType(typeid(NData)))
			nodeValue = EncodeMacXML_1_0_Data(theValue.GetValueData(theKey));

		else if (theItem.IsType(typeid(NDate)))
			nodeValue = EncodeMacXML_1_0_Date(theValue.GetValueDate(theKey));
		
		else
			{
			NN_LOG("Unknown property list value!");
			nodeValue = new NXMLNode(kXMLNodeElement, "unknown");
			}



		// Add the key-value pair
		theNode->AddChild(nodeKey);
		theNode->AddChild(nodeValue);
		}
	 
	return(theNode); 
}





//============================================================================
//		NPropertyList::EncodeMacXML_1_0_Array : Encode an array.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Array(const NArray &theValue)
{	NXMLNode		*theNode, *nodeValue;
	NIndex			n, theSize;
	NVariant		theItem;



	// Get the state we need
	theSize = theValue.GetSize();
	theNode = new NXMLNode(kXMLNodeElement, "array");



	// Encode the value
	for (n = 0; n < theSize; n++)
		{
		// Get the state we need
		theItem = theValue.GetValue(n);



		// Create the value
		if (theItem.IsType(typeid(NDictionary)))
			nodeValue = EncodeMacXML_1_0_Dictionary(theValue.GetValueDictionary(n));

		else if (theItem.IsType(typeid(NArray)))
			nodeValue = EncodeMacXML_1_0_Array(theValue.GetValueArray(n));

		else if (theItem.IsType(typeid(NString)))
			nodeValue = EncodeMacXML_1_0_String(theValue.GetValueString(n));

		else if (theItem.IsNumeric())
			nodeValue = EncodeMacXML_1_0_Number(theValue.GetValue(n));

		else if (theItem.IsType(typeid(NData)))
			nodeValue = EncodeMacXML_1_0_Data(theValue.GetValueData(n));

		else if (theItem.IsType(typeid(NDate)))
			nodeValue = EncodeMacXML_1_0_Date(theValue.GetValueDate(n));
		
		else
			{
			NN_LOG("Unknown property list value!");
			nodeValue = new NXMLNode(kXMLNodeElement, "unknown");
			}



		// Add the value
		theNode->AddChild(nodeValue);
		}
	 
	return(theNode); 
}





//============================================================================
//		NPropertyList::EncodeMacXML_1_0_String : Encode a string.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_String(const NString &theValue)
{	NXMLNode	*theNode;



	// Encode the value
	theNode = new NXMLNode(kXMLNodeElement, "string");
	theNode->SetElementContents(theValue);
	
	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML_1_0_Number : Encode a number.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Number(const NNumber &theValue)
{	NString			textType, textValue;
	SInt64			valueInteger;
	Float64			valueReal;
	NXMLNode		*theNode;



	// Get the state we need
	if (theValue.GetValueSInt64(valueInteger))
		{
		textType = "integer";
		textValue.Format("%lld", valueInteger);
		}
		
	else if (theValue.GetValueFloat64(valueReal))
		{
		textType = "real";
		
		if (isnan(valueReal))
			textValue = "nan";

		else if (isinf(valueReal))
			textValue = (valueReal < 0.0) ? "-infinity" : "+infinity";

		else if (NMathUtilities::IsZero(valueReal))
			textValue = "0.0";

		else
			textValue.Format("%.17g", valueReal);
		}
	
	else
		{
		NN_LOG("Unknown type: %d", theValue.GetType());
		textType = "unknown";
		}



	// Encode the value
	theNode = new NXMLNode(kXMLNodeElement, textType);
	theNode->SetElementContents(textValue);

	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML_1_0_Data : Encode binary data.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Data(const NData &theValue)
{	NB64Encoder		theEncoder;
	NString			valueText;
	NXMLNode		*theNode;



	// Get the state we need
	valueText = theEncoder.Encode(theValue);



	// Encode the value
	theNode = new NXMLNode(kXMLNodeElement, "data");
	theNode->SetElementContents(valueText);

	return(theNode);
}





//============================================================================
//		NPropertyList::EncodeMacXML_1_0_Date : Encode a date.
//----------------------------------------------------------------------------
NXMLNode *NPropertyList::EncodeMacXML_1_0_Date(const NDate &theValue)
{	NGregorianDate		gregDate;
	NString				valueText;
	NXMLNode			*theNode;



	// Get the state we ned
	gregDate = theValue.GetGregorianDate();

	valueText.Format("%04d-%02d-%02dT%02d:%02d:%02dZ",
						gregDate.year, gregDate.month,  gregDate.day,
						gregDate.hour, gregDate.minute, (UInt32) gregDate.second);



	// Encode the value
	theNode = new NXMLNode(kXMLNodeElement, "date");
	theNode->SetElementContents(valueText);
	
	return(theNode);
}

