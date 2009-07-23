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
#include "NMathUtilities.h"
#include "NTargetPOSIX.h"
#include "NB64Encoder.h"
#include "NXMLEncoder.h"
#include "NEncodable.h"
#include "NEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tokens
static const NString kTokenBool										= "bool";
static const NString kTokenBytes									= "data";
static const NString kTokenEncoder									= "encoder";
static const NString kTokenInteger									= "integer";
static const NString kTokenObject									= "object";
static const NString kTokenReal										= "real";
static const NString kTokenRoot										= "root";
static const NString kTokenUTF8										= "text";

static const NString kTokenClass									= "class";
static const NString kTokenFalse									= "false";
static const NString kTokenInfinityNeg								= "-infinity";
static const NString kTokenInfinityPos								= "+infinity";
static const NString kTokenKey										= "key";
static const NString kTokenNaN										= "nan";
static const NString kTokenTrue										= "true";
static const NString kTokenVersion									= "version";
static const NString kTokenZero										= "0.0";





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
NData NEncoder::Encode(const NEncodable &theObject)
{	NXMLEncoder		xmlEncoder;
	NXMLNode		*nodeDoc;
	NString			theText;
	NData			theData;



	// Validate our state
	NN_ASSERT(mState == kNEncoderIdle);



	// Create the nodes
	mNodeRoot = new NXMLNode(kXMLNodeElement, kTokenEncoder);
	mNodeRoot->SetElementAttribute(kTokenVersion, "1.0");

	nodeDoc = new NXMLNode(kXMLNodeDocument, "");
	nodeDoc->AddChild(mNodeRoot);



	// Encode the objects
	mState = kNEncoderEncoding;
	EncodeObject(kTokenRoot, theObject);
	mState = kNEncoderIdle;



	// Encode the XML
	theText = xmlEncoder.Encode(nodeDoc);
	theData = theText.GetData();



	// Clean up
	delete nodeDoc;
	mNodeRoot = NULL;

	return(theData);
}





//============================================================================
//		NEncoder::Decode : Decode an object.
//----------------------------------------------------------------------------
void NEncoder::Decode(NEncodable &theObject, const NData &theData)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderIdle);



	// Decode the object
	mState = kNEncoderDecoding;

	mState = kNEncoderIdle;
}





//============================================================================
//		NEncoder::EncodeBoolean : Encode a bool.
//----------------------------------------------------------------------------
#pragma mark -
void NEncoder::EncodeBoolean(const NString &theKey, bool theValue)
{	NString		valueText;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the value
	valueText = theValue ? kTokenTrue : kTokenFalse;

	EncodeChild(kTokenBool, theKey, valueText);
}





//============================================================================
//		NEncoder::EncodeSInt32 : Encode an SInt32.
//----------------------------------------------------------------------------
void NEncoder::EncodeSInt32(const NString &theKey, SInt32 theValue)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);
	
	
	
	// Encode the value
	EncodeSInt64(theKey, theValue);
}





//============================================================================
//		NEncoder::EncodeSInt64 : Encode an SInt64.
//----------------------------------------------------------------------------
void NEncoder::EncodeSInt64(const NString &theKey, SInt64 theValue)
{	NString		valueText;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the value
	valueText.Format("%lld", theValue);

	EncodeChild(kTokenInteger, theKey, valueText);
}





//============================================================================
//		NEncoder::EncodeFloat32 : Encode a Float32.
//----------------------------------------------------------------------------
void NEncoder::EncodeFloat32(const NString &theKey, Float32 theValue)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);
	
	
	
	// Encode the value
	EncodeFloat64(theKey, theValue);
}





//============================================================================
//		NEncoder::EncodeFloat64 : Encode a Float64.
//----------------------------------------------------------------------------
void NEncoder::EncodeFloat64(const NString &theKey, Float64 theValue)
{	NString		valueText;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the value
	if (NTargetPOSIX::is_nan(theValue))
		valueText = kTokenNaN;

	else if (NTargetPOSIX::is_inf(theValue))
		valueText = (theValue < 0.0) ? kTokenInfinityNeg : kTokenInfinityPos;

	else if (NMathUtilities::IsZero(theValue))
		valueText = kTokenZero;

	else
		valueText.Format("%.17g", theValue);

	EncodeChild(kTokenReal, theKey, valueText);
}





//============================================================================
//		NEncoder::EncodeBytes : Encode bytes.
//----------------------------------------------------------------------------
void NEncoder::EncodeBytes(const NString &theKey, NIndex theSize, const void *theValue)
{	NB64Encoder		theEncoder;
	NString			valueText;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the value
	valueText = theEncoder.Encode(NData(theSize, theValue, false));

	EncodeChild(kTokenBytes, theKey, valueText);
}





//============================================================================
//		NEncoder::EncodeUTF8 : Encode UTF8 text.
//----------------------------------------------------------------------------
void NEncoder::EncodeUTF8(const NString &theKey, NIndex theSize, const char *theValue)
{	NString		valueText;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the value
	valueText = NString(theValue, theSize);
	
	EncodeChild(kTokenUTF8, theKey, valueText);
}





//============================================================================
//		NEncoder::EncodeObject : Encode an object.
//----------------------------------------------------------------------------
void NEncoder::EncodeObject(const NString &theKey, const NEncodable &theValue)
{	NXMLNode		*theNode;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the value
	theNode = EncodeChild(kTokenObject, theKey, "");
	theNode->SetElementAttribute(kTokenClass, theValue.GetEncoderClass());

	mNodeStack.push_back(theNode);
	theValue.EncodeSelf(*this);
	mNodeStack.pop_back();
}





//============================================================================
//		NEncoder::DecodeBoolean : Decode a bool.
//----------------------------------------------------------------------------
bool NEncoder::DecodeBoolean(const NString &theKey)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeSInt32 : Decode an SInt32.
//----------------------------------------------------------------------------
SInt32 NEncoder::DecodeSInt32(const NString &theKey)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeSInt64 : Decode an SInt64.
//----------------------------------------------------------------------------
SInt64 NEncoder::DecodeSInt64(const NString &theKey)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeFloat32 : Decode a Float32.
//----------------------------------------------------------------------------
Float32 NEncoder::DecodeFloat32(const NString &theKey)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeFloat64 : Decode a Float64.
//----------------------------------------------------------------------------
Float64 NEncoder::DecodeFloat64(const NString &theKey)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeBytes : Decode bytes.
//----------------------------------------------------------------------------
void NEncoder::DecodeBytes(const NString &theKey, NIndex *theSize, void **theValue)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeUTF8 : Decode UTF8 text.
//----------------------------------------------------------------------------
void NEncoder::DecodeUTF8(const NString &theKey, NIndex *theSize, char **theValue)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::DecodeObject : Decode an object.
//----------------------------------------------------------------------------
void NEncoder::DecodeObject(const NString &theKey, NEncodable &theValue)
{


	// Validate our state
	NN_ASSERT(mState == kNEncoderDecoding);
	
	
	
	// Decode the value
}





//============================================================================
//		NEncoder::GetParentNode : Get the parent node.
//----------------------------------------------------------------------------
#pragma mark -
NXMLNode *NEncoder::GetParentNode(void)
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
//		NEncoder::EncodeChild : Encode a child node.
//----------------------------------------------------------------------------
NXMLNode *NEncoder::EncodeChild(const NString &theName, const NString &theKey, const NString &theValue)
{	NXMLNode		*theParent, *theChild;



	// Validate our state
	NN_ASSERT(mState == kNEncoderEncoding);



	// Encode the child
	theChild = new NXMLNode(kXMLNodeElement, theName);
	theChild->SetElementAttribute(kTokenKey, theKey);

	if (theValue.IsNotEmpty())
		theChild->SetElementContents(theValue);



	// Attach to its parent
	theParent = GetParentNode();
	theParent->AddChild(theChild);

	return(theChild);
}




