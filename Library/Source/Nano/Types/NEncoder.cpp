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

		// dair, to do

	mState = kNEncoderIdle;
}





//============================================================================
//		NEncoder::HasKey : Does the current object have a key?
//----------------------------------------------------------------------------
bool NEncoder::HasKey(const NString &theKey) const
{	const NXMLNode		*theChild;



	// Validate our state
	NN_ASSERT(mState != kNEncoderIdle);



	// Get the state we need
	theChild = FindChild(theKey);
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
	theChild = FindChild(theKey);



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
#pragma mark -
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
//		NEncoder::GetParentNode : Get the parent node.
//----------------------------------------------------------------------------
#pragma mark -
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
//		NEncoder::GetParentNode : Get the parent node.
//----------------------------------------------------------------------------
NXMLNode *NEncoder::GetParentNode(void)
{	const NXMLNode		*theNode;



	// Get the parent node
	theNode = ((const NEncoder *) this)->GetParentNode();
	
	return((NXMLNode *) theNode);
}





//============================================================================
//		NEncoder::FindChild : Find a child node.
//----------------------------------------------------------------------------
const NXMLNode *NEncoder::FindChild(const NString &theKey) const
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
	theParent = GetParentNode();
	theParent->AddChild(theChild);

	return(theChild);
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



