/*	NAME:
		NPropertyList.h

	DESCRIPTION:
		Property list file.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPROPERTYLIST_HDR
#define NPROPERTYLIST_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NXMLNode.h"
#include "NFile.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Encoded formats
typedef enum {
	// Specific
	kNPropertyListInvalid,
	kNPropertyListMacXML1,
	kNPropertyListMacBinary1,


	// Generic
	kNPropertyListXML		= kNPropertyListMacXML1,
	kNPropertyListBinary	= kNPropertyListMacBinary1,
} NPropertyListFormat;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Mac Binary 1.0
typedef struct {
	UInt8			theToken;
	UInt8			objectInfo;
} MacBinary1_Tag;

typedef struct {
	const UInt8		*basePtr;
	UInt64List		theObjects;
	UInt32			objectRefSize;
} MacBinary1_Info;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPropertyList {
public:
										NPropertyList(void);
	virtual							   ~NPropertyList(void);


	// Identify a property list format
	NPropertyListFormat					GetFormat(const NData &theData);


	// Encode/decode a property list
	NData								Encode(const NDictionary &theState, NPropertyListFormat theFormat=kNPropertyListBinary);
	NDictionary							Decode(const NData       &theData);


	// Encode/decode a property list to XML
	//
	// The encoded form uses the kNPropertyListXML format.
	NString								EncodeXML(const NDictionary &theState);
	NDictionary							DecodeXML(const NString     &theXML);


	// Load/save a property list
	NDictionary							Load(const NFile &theFile);
	NStatus								Save(const NFile &theFile, const NDictionary &theState, NPropertyListFormat theFormat=kNPropertyListBinary);


private:
	NData								EncodeMacXML1(const NDictionary &theState);
	NDictionary							DecodeMacXML1(const NData       &theData);

	NData								EncodeMacBinary1(const NDictionary &theState);
	NDictionary							DecodeMacBinary1(const NData       &theData);

	NXMLNode							*EncodeMacXML1_Boolean(         bool         theValue);
	NXMLNode							*EncodeMacXML1_Number(    const NNumber     &theValue);
	NXMLNode							*EncodeMacXML1_String(    const NString     &theValue);
	NXMLNode							*EncodeMacXML1_Data(      const NData       &theValue);
	NXMLNode							*EncodeMacXML1_Date(      const NDate       &theValue);
	NXMLNode							*EncodeMacXML1_Array(     const NArray      &theValue);
	NXMLNode							*EncodeMacXML1_Dictionary(const NDictionary &theValue);

	bool								DecodeMacXML1_Boolean(   const NXMLNode *theNode);
	SInt64								DecodeMacXML1_Integer(   const NXMLNode *theNode);
	Float64								DecodeMacXML1_Real(      const NXMLNode *theNode);
	NString								DecodeMacXML1_String(    const NXMLNode *theNode);
	NData								DecodeMacXML1_Data(      const NXMLNode *theNode);
	NDate								DecodeMacXML1_Date(      const NXMLNode *theNode);
	NArray								DecodeMacXML1_Array(     const NXMLNode *theNode);
	NDictionary							DecodeMacXML1_Dictionary(const NXMLNode *theNode);

	UInt64								DecodeMacBinary1_GetUIntX(UInt32 theSize, const UInt8 *thePtr);
	UInt64List							DecodeMacBinary1_GetObjectRefs(UInt64 numObjects, UInt32 objectRefSize, const UInt8 *thePtr);
	UInt64								DecodeMacBinary1_GetObjectOffset(const MacBinary1_Info &theInfo, UInt64 objectRef);

	MacBinary1_Tag						DecodeMacBinary1_ReadObjectTag( const MacBinary1_Info &theInfo, UInt64 &byteOffset);
	UInt64								DecodeMacBinary1_ReadObjectSize(const MacBinary1_Info &theInfo, UInt64 &byteOffset, const MacBinary1_Tag &theTag);
	UInt64								DecodeMacBinary1_ReadObjectRef( const MacBinary1_Info &theInfo, UInt64 &byteOffset);

	NVariant							DecodeMacBinary1_Value(     const MacBinary1_Info &theInfo, UInt64 &byteOffset);
	bool								DecodeMacBinary1_Boolean(   const MacBinary1_Info &theInfo, UInt64 &byteOffset);
	SInt64								DecodeMacBinary1_Integer(   const MacBinary1_Info &theInfo, UInt64 &byteOffset);
	Float64								DecodeMacBinary1_Real(      const MacBinary1_Info &theInfo, UInt64 &byteOffset);
	NString								DecodeMacBinary1_String(    const MacBinary1_Info &theInfo, UInt64 &byteOffset);
	NData								DecodeMacBinary1_Data(      const MacBinary1_Info &theInfo, UInt64 &byteOffset);
	NDate								DecodeMacBinary1_Date(      const MacBinary1_Info &theInfo, UInt64 &byteOffset);
	NArray								DecodeMacBinary1_Array(     const MacBinary1_Info &theInfo, UInt64 &byteOffset);
	NDictionary							DecodeMacBinary1_Dictionary(const MacBinary1_Info &theInfo, UInt64 &byteOffset);


private:


};




#endif // NPROPERTYLIST_HDR



