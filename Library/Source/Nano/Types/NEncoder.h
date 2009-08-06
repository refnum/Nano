/*	NAME:
		NEncoder.h

	DESCRIPTION:
		Object encoder.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NENCODER_HDR
#define NENCODER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NEncodable.h"
#include "NNumber.h"
#include "NString.h"
#include "NData.h"
#include "NLock.h"
#include "NXMLNode.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Keys
//
// These values are considered to be fixed, and will never change.
static const NString kNEncoderValueKey								= "value";


// Encoded types
typedef enum {
	kNEncodedUnknown,
	kNEncodedBoolean,
	kNEncodedNumber,
	kNEncodedString,
	kNEncodedData,
	kNEncodedObject
} NEncodedType;


// Encoder state
typedef enum {
	kNEncoderIdle,
	kNEncoderEncoding,
	kNEncoderDecoding,
} NEncoderState;


// Encoder formats
typedef enum {
	// Specific
	kNEncoderInvalid,
	kNEncoderXML_1_0,
	kNEncoderBinary_1_0,


	// Generic
	kNEncoderXML		= kNEncoderXML_1_0,
	kNEncoderBinary		= kNEncoderBinary_1_0,
} NEncoderFormat;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Functors
typedef nfunctor<const NEncodable *(const NVariant &theValue)>					NEncodableCastFunctor;
typedef nfunctor<NVariant          (const NEncoder &theEncoder)>				NEncodableDecodeFunctor;


// Info
typedef struct {
	NEncodableCastFunctor		castObject;
	NEncodableDecodeFunctor		decodeObject;
} NEncoderClassInfo;


// Lists
typedef std::map<NString, NEncoderClassInfo, NHashableCompare<NString> >		NEncoderClassInfoMap;
typedef NEncoderClassInfoMap::iterator											NEncoderClassInfoMapIterator;
typedef NEncoderClassInfoMap::const_iterator									NEncoderClassInfoMapConstIterator;


// Classes
typedef struct {
	NMutexLock				theLock;
	NEncoderClassInfoMap	theInfo;
} NEncoderClasses;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NEncoder {
public:
										 NEncoder(void);
	virtual								~NEncoder(void);


	// Encode/decode an object
	NData								Encode(const NEncodable &theObject, NEncoderFormat theFormat=kNEncoderBinary);
	NData								Encode(const NVariant   &theObject, NEncoderFormat theFormat=kNEncoderBinary);
	NVariant							Decode(const NData      &theData);


	// Get an encodable object
	//
	// Returns NULL if theObject is not encodable.
	static const NEncodable				*GetEncodable(const NVariant &theObject);


public:
	// Get the keys from the current object.
	//
	// Can be invoked by NEncodable::EncodeSelf/DecodeSelf.
	NStringList							GetKeys(void) const;


	// Get a value type from the current object.
	//
	// Can be invoked by NEncodable::EncodeSelf/DecodeSelf.
	NEncodedType						GetValueType(const NString &theKey) const;


	// Encode/decode a value within the current object.
	//
	// If the value can not be returned as the specified type, 0/empty is returned.
	//
	// Can be invoked by NEncodable::EncodeSelf/DecodeSelf.
	void								EncodeBoolean(const NString &theKey,       bool       theValue);
	void								EncodeNumber( const NString &theKey, const NNumber    &theValue);
	void								EncodeString( const NString &theKey, const NString    &theValue);
	void								EncodeData(   const NString &theKey, const NData      &theValue);
	void								EncodeObject( const NString &theKey, const NEncodable &theValue);

	bool								DecodeBoolean(const NString &theKey) const;
	NNumber								DecodeNumber( const NString &theKey) const;
	NString								DecodeString( const NString &theKey) const;
	NData								DecodeData(   const NString &theKey) const;
	NVariant							DecodeObject( const NString &theKey) const;


	// Register a class
	//
	// Invoked automatically by DEFINE_NENCODABLE.
	static void							RegisterClass(const NString &className, const NEncoderClassInfo &classInfo);


private:
	NEncoderFormat						GetFormat(const NData &theData);

	const NXMLNode						*GetParentNode(void)                 const;
	const NXMLNode						*GetChildNode(const NString &theKey) const;

	NXMLNode							*EncodeChild(const NString &theKey, const NString &theValue, const NString &theType);
	NString								 DecodeChild(const NString &theKey,                          const NString &theType) const;

	NData								 EncodeXML_1_0(      NXMLNode *theRoot);
	NXMLNode							*DecodeXML_1_0(const NData    &theData);

	NData								 EncodeBinary_1_0(      NXMLNode *theRoot);
	NXMLNode							*DecodeBinary_1_0(const NData    &theData);

	static bool							IsKnownClass(const NString &className);
	static NEncoderClasses				*GetClasses(void);
	static NEncoderClassInfoMap			GetClassesInfo(void);


private:
	NEncoderState						mState;
	NXMLNode							*mNodeRoot;
	mutable NXMLNodeList				mNodeStack;
};





#endif // NENCODER_HDR
