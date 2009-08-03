/*	NAME:
		NEncoder.cpp

	DESCRIPTION:
		Object encoder.

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
#include "NMathUtilities.h"
#include "NDataCompressor.h"
#include "NTargetPOSIX.h"
#include "NB64Encoder.h"
#include "NXMLEncoder.h"
#include "NEncodable.h"
#include "NEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Magic
//
// The binary format is simply zipped, so the header is kNCompressionZLib. 
static const UInt8 kMagicXML_1_0[]									= { 0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20  };
static const UInt8 kMagicBinary_1_0[]								= { 0x6E, 0x7A, 0x6C, 0x62 };


// Tokens
static const NString kTokenBoolean									= "bool";
static const NString kTokenData										= "data";
static const NString kTokenEncoder									= "encoder";
static const NString kTokenNumber									= "number";
static const NString kTokenObject									= "object";
static const NString kTokenRoot										= "root";
static const NString kTokenString									= "string";

static const NString kTokenClass									= "class";
static const NString kTokenFalse									= "false";
static const NString kTokenKey										= "key";
static const NString kTokenTrue										= "true";
static const NString kTokenVersion									= "version";

static const NString kVersion_1_0									= "1.0";





//============================================================================
//		NEncoder::NEncoder : Constructor.
//----------------------------------------------------------------------------
NEncoder::NEncoder(void)
{


	// Initialise ourselves
	mState    = kNEncoderIdle;
	mNodeRoot = NULL;
}





//============================================================================
//		NEncoder::~NEncoder : Destructor.
//----------------------------------------------------------------------------
NEncoder::~NEncoder(void)
{


	// Validate our state
	NN_ASSERT(mState    == kNEncoderIdle);
	NN_ASSERT(mNodeRoot == NULL);
	NN_ASSERT(mNodeStack.empty());
}





//============================================================================
//		NEncoder::Encode : Encode an object.
//----------------------------------------------------------------------------
NData NEncoder::Encode(const NEncodable &theObject, NEncoderFormat theFormat)
{	NXMLNode		*theRoot;
	NData			theData;



	// Validate our state
	NN_ASSERT(mState == kNEncoderIdle);



	// Get the state we need
	mState    = kNEncoderEncoding;
	mNodeRoot = new NXMLNode(kXMLNodeDocument, "");
	theRoot   = NULL;



	// Encode the object
	EncodeObject(kTokenRoot, theObject);

	if (mNodeRoot->GetChildren()->size() == 1)
		theRoot = mNodeRoot->GetChild(0);

	if (theRoot == NULL)
		{
		NN_LOG("Unable to encode object!");
		return(theData);
		}



	// Encode the data
	//
	// The encoded object is extracted as a root XML node.
	mNodeRoot->RemoveChild(theRoot, false);

	switch (theFormat) {
		case kNEncoderXML_1_0:
			theData = EncodeXML_1_0(theRoot);
			break;

		case kNEncoderBinary_1_0:
			theData = EncodeBinary_1_0(theRoot);
			break;

		default:
			NN_LOG("Unknown encoder format: %d", theFormat);
			break;
		}



	// Clean up
	delete theRoot;
	delete mNodeRoot;

	mNodeRoot = NULL;
	mState    = kNEncoderIdle;

	return(theData);
}





//============================================================================
//		NEncoder::Decode : Decode an object.
//----------------------------------------------------------------------------
NStatus NEncoder::Decode(NEncodable &theObject, const NData &theData)
{	NEncoderFormat		theFormat;



	// Get the state we need
	mState    = kNEncoderDecoding;
	theFormat = GetFormat(theData);
	


	// Decode the data
	switch (theFormat) {
		case kNEncoderXML_1_0:
			mNodeRoot = DecodeXML_1_0(theData);
			break;

		case kNEncoderBinary_1_0:
			mNodeRoot = DecodeBinary_1_0(theData);
			break;
		
		default:
			NN_LOG("Unknown encoder format: %d", theFormat);
			return(kNErrParam);
			break;
		}



	// Decode the object
	theObject.DecodeSelf(*this);



	// Clean up
	delete mNodeRoot;

	mNodeRoot = NULL;
	mState    = kNEncoderIdle;
	
	return(kNoErr);
}





//============================================================================
//		NEncoder::HasKey : Does the current object have a key?
//----------------------------------------------------------------------------
#pragma mark -
bool NEncoder::HasKey(const NString &theKey) const
{	const NXMLNode		*theChild;



	// Validate our state
	NN_ASSERT(mState != kNEncoderIdle);



	// Get the state we need
	theChild = GetChildNode(theKey);
	return(theChild != NULL);
}





//============================================================================
//		NEncoder::GetValueType : Get a value type from the current object.
//----------------------------------------------------------------------------
NEncodedType NEncoder::GetValueType(const NString &theKey) const
{	const NXMLNode		*theChild;
	NEncodedType		theType;



	// Validate our state
	NN_ASSERT(mState != kNEncoderIdle);



	// Get the state we need
	theChild = GetChildNode(theKey);



	// Get the type
	if (theChild->IsElement(kTokenBoolean))
		theType = kNEncodedBoolean;
	
	else if (theChild->IsElement(kTokenNumber))
		theType = kNEncodedNumber;
	
	else if (theChild->IsElement(kTokenString))
		theType = kNEncodedString;
	
	else if (theChild->IsElement(kTokenData))
		theType = kNEncodedData;
	
	else if (theChild->IsElement(kTokenObject))
		theType = kNEncodedObject;
	
	else
		{
		NN_LOG("Unknown encoded type: (%d) %@", theChild->GetType(), theChild->GetTextValue());
		theType = kNEncodedUnknown;
		}
	
	return(theType);
}





//============================================================================
//		NEncoder::EncodeBoolean : Encode a boolean.
//----------------------------------------------------------------------------
void NEncoder::EncodeBoolean(const NString &theKey, bool theValue)
{	NString		valueText;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the value
	valueText = theValue ? kTokenTrue : kTokenFalse;

	EncodeChild(theKey, valueText, kTokenBoolean);
}





//============================================================================
//		NEncoder::EncodeNumber : Encode a number.
//----------------------------------------------------------------------------
void NEncoder::EncodeNumber(const NString &theKey, const NNumber &theValue)
{	NString		valueText;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);
	
	
	
	// Encode the value
	valueText = theValue.GetString();
	
	EncodeChild(theKey, valueText, kTokenNumber);
}





//============================================================================
//		NEncoder::EncodeData : Encode data.
//----------------------------------------------------------------------------
void NEncoder::EncodeData(const NString &theKey, const NData &theValue)
{	NB64Encoder		theEncoder;
	NString			valueText;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the value
	valueText = theEncoder.Encode(theValue);

	EncodeChild(theKey, valueText, kTokenData);
}





//============================================================================
//		NEncoder::EncodeString : Encode a string.
//----------------------------------------------------------------------------
void NEncoder::EncodeString(const NString &theKey, const NString &theValue)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the value
	EncodeChild(theKey, theValue, kTokenString);
}





//============================================================================
//		NEncoder::EncodeObject : Encode an object.
//----------------------------------------------------------------------------
void NEncoder::EncodeObject(const NString &theKey, const NEncodable &theValue)
{	NString			className;
	NXMLNode		*theNode;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Get the state we need
	className = theValue.EncodableGetClass();
	NN_ASSERT(IsKnownClass(className));



	// Encode the value
	theNode = EncodeChild(theKey, "", kTokenObject);
	theNode->SetElementAttribute(kTokenClass, className);

	mNodeStack.push_back(theNode);
	theValue.EncodeSelf(*this);
	mNodeStack.pop_back();
}





//============================================================================
//		NEncoder::DecodeBoolean : Decode a bool.
//----------------------------------------------------------------------------
bool NEncoder::DecodeBoolean(const NString &theKey) const
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeNumber : Decode a number.
//----------------------------------------------------------------------------
NNumber NEncoder::DecodeNumber(const NString &theKey) const
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeData : Decode data.
//----------------------------------------------------------------------------
NData NEncoder::DecodeData(const NString &theKey) const
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeString : Decode a string.
//----------------------------------------------------------------------------
NString NEncoder::DecodeString(const NString &theKey) const
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeObject : Decode an object.
//----------------------------------------------------------------------------
NVariant NEncoder::DecodeObject(const NString &theKey) const
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::RegisterClass : Register a class.
//----------------------------------------------------------------------------
void NEncoder::RegisterClass(const NString &className, const NEncodableCreateFunctor &createFunctor)
{	NEncoderClasses		*theClasses;



	// Validate our parameters
	NN_ASSERT(!IsKnownClass(className));
	NN_ASSERT(createFunctor != NULL);
	
	
	
	// Get the state we need
	theClasses = GetClasses();



	// Register the class
	theClasses->theLock.Lock();
		theClasses->classFactory[className] = createFunctor;
	theClasses->theLock.Unlock();
}





//============================================================================
//		NEncoder::GetFormat : Get the format of encoded data.
//----------------------------------------------------------------------------
#pragma mark -
NEncoderFormat NEncoder::GetFormat(const NData &theData)
{	NEncoderFormat		theFormat;
	const UInt8			*dataPtr;
	NIndex				dataSize;



	// Get the state we need
	theFormat = kNEncoderInvalid;
	dataSize  = theData.GetSize();
	dataPtr   = theData.GetData();



	// Identify the format
	#define MATCH_FORMAT(_magic, _format)													\
		do																					\
			{																				\
			if (theFormat == kNEncoderInvalid && dataSize >= GET_ARRAY_SIZE(_magic))		\
				{																			\
				if (memcmp(dataPtr, _magic, GET_ARRAY_SIZE(_magic)) == 0)					\
					{																		\
					theFormat = _format;													\
					}																		\
				}																			\
			}																				\
		while (0)

	MATCH_FORMAT(kMagicXML_1_0,    kNEncoderXML_1_0);
	MATCH_FORMAT(kMagicBinary_1_0, kNEncoderBinary_1_0);
	
	#undef MATCH_FORMAT
	
	return(theFormat);
}





//============================================================================
//		NEncoder::EncodeXML_1_0 : Encode to the XML 1.0 format.
//----------------------------------------------------------------------------
NData NEncoder::EncodeXML_1_0(NXMLNode *theRoot)
{	NXMLNode		*nodeDoc, *nodeEncoder;
	NXMLEncoder		xmlEncoder;
	NData			theData;
	NString			theXML;



	// Create the nodes
	nodeEncoder = new NXMLNode(kXMLNodeElement, kTokenEncoder);
	nodeEncoder->SetElementAttribute(kTokenVersion, kVersion_1_0);
	nodeEncoder->AddChild(theRoot);

	nodeDoc = new NXMLNode(kXMLNodeDocument, "");
	nodeDoc->AddChild(nodeEncoder);



	// Encode the XML
	theXML  = xmlEncoder.Encode(nodeDoc);
	theData = theXML.GetData();
	


	// Clean up
	nodeEncoder->RemoveChild(theRoot, false);
	delete nodeDoc;

	return(theData);
}





//============================================================================
//		NEncoder::DecodeXML_1_0 : Decode the XML 1.0 format.
//----------------------------------------------------------------------------
NXMLNode *NEncoder::DecodeXML_1_0(const NData &theData)
{	NXMLNode		*nodeDoc, *nodeEncoder, *nodeRoot;
	NXMLEncoder		xmlEncoder;
	NStatus			theErr;



	// Validate our state
	NN_ASSERT(mState == kNEncoderIdle);



	// Get the state we need
	theErr   = kNErrMalformed;
	nodeRoot = NULL;



	// Decode the XML
	nodeDoc = xmlEncoder.Decode(theData);
	if (nodeDoc == NULL || !nodeDoc->IsType(kXMLNodeDocument) || nodeDoc->GetChildren()->size() != 1)
		goto cleanup;



	// Extract the encoder
	nodeEncoder = nodeDoc->GetChildren()->at(0);
	if (!nodeEncoder->IsElement(kTokenEncoder) || nodeEncoder->GetChildren()->size() != 1 || nodeEncoder->GetElementAttribute(kTokenVersion) != kVersion_1_0)
		goto cleanup;



	// Extract the root
	nodeRoot = nodeEncoder->GetChildren()->at(0);
	if (!nodeRoot->IsElement(kTokenObject) || nodeRoot->GetElementAttribute(kTokenKey) != kTokenRoot)
		goto cleanup;

	nodeEncoder->RemoveChild(nodeRoot, false);
	theErr = kNoErr;



	// Clean up
cleanup:
	if (theErr != kNoErr)
		NN_LOG("Unable to decode XML (%d)", theErr);

	delete nodeDoc;
	
	return(nodeRoot);
}





//============================================================================
//		NEncoder::EncodeBinary_1_0 : Encode to the binary 1.0 format.
//----------------------------------------------------------------------------
NData NEncoder::EncodeBinary_1_0(NXMLNode *theRoot)
{	NDataCompressor		theCompressor;
	NData				theData;



	// Encode the state
	theData = EncodeXML_1_0(theRoot);
	theData = theCompressor.Compress(theData);
	
	return(theData);
}





//============================================================================
//		NEncoder::DecodeBinary_1_0 : Decode the binary 1.0 format.
//----------------------------------------------------------------------------
NXMLNode *NEncoder::DecodeBinary_1_0(const NData &theData)
{	NDataCompressor		theCompressor;
	NXMLNode			*theRoot;
	NData				xmlData;



	// Decode the state
	xmlData = theCompressor.Decompress(theData);
	theRoot = DecodeXML_1_0(xmlData);
	
	return(theRoot);
}





//============================================================================
//		NEncoder::EncodeChild : Encode a child node.
//----------------------------------------------------------------------------
NXMLNode *NEncoder::EncodeChild(const NString &theKey, const NString &theValue, const NString &theName)
{	NXMLNode		*theParent, *theChild;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the child
	theChild = new NXMLNode(kXMLNodeElement, theName);
	theChild->SetElementAttribute(kTokenKey, theKey);

	if (theValue.IsNotEmpty())
		theChild->SetElementContents(theValue);



	// Attach to its parent
	theParent = const_cast<NXMLNode*>(GetParentNode());
	theParent->AddChild(theChild);

	return(theChild);
}





//============================================================================
//		NEncoder::GetParentNode : Get the parent node.
//----------------------------------------------------------------------------
const NXMLNode *NEncoder::GetParentNode(void) const
{	NXMLNode	*theNode;



	// Validate our state
	NN_ASSERT(mNodeRoot != NULL);



	// Get the parent node
	if (mNodeStack.empty())
		theNode = mNodeRoot;
	else
		theNode = mNodeStack.back();
	
	return(theNode);
}





//============================================================================
//		NEncoder::GetChildNode : Get a child node.
//----------------------------------------------------------------------------
const NXMLNode *NEncoder::GetChildNode(const NString &theKey) const
{	const NXMLNodeList				*theChildren;
	NString							theAttribute;
	const NXMLNode					*theParent;
	NXMLNodeListConstIterator		theIter;



	// Validate our state
	NN_ASSERT(mState != kNEncoderIdle);



	// Get the state we need
	theParent   = GetParentNode();
	theChildren = theParent->GetChildren();



	// Find the child
	for (theIter = theChildren->begin(); theIter != theChildren->end(); theIter++)
		{
		theAttribute = (*theIter)->GetElementAttribute(kTokenKey);
		if (theAttribute == theKey)
			return(*theIter);
		}
	
	return(NULL);
}





//============================================================================
//		NEncoder::IsKnownClass : Is a class name known?
//----------------------------------------------------------------------------
bool NEncoder::IsKnownClass(const NString &className)
{	NEncoderClasses						*theClasses;
	NEncodableClassMapConstIterator		theIter;
	bool								isKnown;



	// Get the state we need
	theClasses = GetClasses();



	// Find the class
	theClasses->theLock.Lock();
		theIter = theClasses->classFactory.find(className);
		isKnown = (theIter != theClasses->classFactory.end());
	theClasses->theLock.Unlock();
	
	return(isKnown);
}





//============================================================================
//		NEncoder::GetClasses : Get the classes.
//----------------------------------------------------------------------------
NEncoderClasses *NEncoder::GetClasses(void)
{	static NEncoderClasses		sClasses;



	// Get the classes
	//
	// We return a local static to ensure our state is constructed before
	// any other static initialisers.
	return(&sClasses);
}



