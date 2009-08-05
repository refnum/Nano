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
typedef nfunctor<NVariant (const NEncoder &theEncoder, const NString &className)>	NEncodableCreateFunctor;


// Lists
typedef std::map<NString, NEncodableCreateFunctor, NHashableCompare<NString> >	NEncodableClassMap;
typedef NEncodableClassMap::iterator											NEncodableClassMapIterator;
typedef NEncodableClassMap::const_iterator										NEncodableClassMapConstIterator;


// Classes
typedef struct {
	NMutexLock				theLock;
	NEncodableClassMap		classFactory;
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
	NStatus								Decode(      NEncodable &theObject, const NData &theData);


public:
	// Does the current object contain a key?
	//
	// Can be invoked by NEncodable::EncodeSelf/DecodeSelf.
	bool								HasKey(const NString &theKey) const;


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
	static void							RegisterClass(const NString &className, const NEncodableCreateFunctor &createFunctor);


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

	static NVariant						 CreateClass(const NString &className, const NEncoder &theEncoder);
	static bool							IsKnownClass(const NString &className);
	static NEncoderClasses				*GetClasses(void);


private:
	NEncoderState						mState;
	NXMLNode							*mNodeRoot;
	mutable NXMLNodeList				mNodeStack;
};





#endif // NENCODER_HDR
