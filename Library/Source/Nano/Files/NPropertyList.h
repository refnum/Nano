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
	kNPropertyListJSON
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
	UInt64List		theOffsets;
	UInt32			objectsSize;
	NData			objectsTable;
} MacBinary1_EncodeInfo;

typedef struct {
	UInt64List		theOffsets;
	UInt32			objectsSize;
	const UInt8		*objectsTable;
} MacBinary1_DecodeInfo;





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
	//
	// The encoded form uses the kNPropertyListBinary format by default.
	NData								Encode(const NDictionary &theState, NPropertyListFormat theFormat=kNPropertyListBinary);
	NDictionary							Decode(const NData       &theData);


	// Encode/decode a property list to XML
	//
	// The encoded form uses the kNPropertyListXML format.
	NString								EncodeXML(const NDictionary &theState);
	NDictionary							DecodeXML(const NString     &theXML);


	// Encode/decode a property list to JSON
	//
	// The encoded form uses the kNPropertyListJSON format.
	NString								EncodeJSON(const NDictionary &theState);
	NDictionary							DecodeJSON(const NString     &theJSON);


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

	void								EncodeMacBinary1_GetObjectCount(const NVariant &theValue, UInt64 *numObjects);
	UInt32								EncodeMacBinary1_GetIntegerSize(UInt64 theSize);
	NData								EncodeMacBinary1_GetIntegerList(const UInt64List &theValues, UInt32 byteSize);

	void								EncodeMacBinary1_WriteObject(    MacBinary1_EncodeInfo &theInfo);
	void								EncodeMacBinary1_WriteObjectTag( MacBinary1_EncodeInfo &theInfo, UInt8 theToken, UInt8 objectInfo, UInt64 objectSize=0);
	void								EncodeMacBinary1_WriteObjectData(MacBinary1_EncodeInfo &theInfo, NIndex theSize, const void *thePtr);

	UInt64								EncodeMacBinary1_Value(     MacBinary1_EncodeInfo &theInfo, const NVariant    &theValue);
	void								EncodeMacBinary1_Boolean(   MacBinary1_EncodeInfo &theInfo,       bool         theValue);
	void								EncodeMacBinary1_Integer(   MacBinary1_EncodeInfo &theInfo, const NNumber     &theValue, bool addObject=true);
	void								EncodeMacBinary1_Real(      MacBinary1_EncodeInfo &theInfo, const NNumber     &theValue);
	void								EncodeMacBinary1_String(    MacBinary1_EncodeInfo &theInfo, const NString     &theValue);
	void								EncodeMacBinary1_Data(      MacBinary1_EncodeInfo &theInfo, const NData       &theValue);
	void								EncodeMacBinary1_Date(      MacBinary1_EncodeInfo &theInfo, const NDate       &theValue);
	void								EncodeMacBinary1_Array(     MacBinary1_EncodeInfo &theInfo, const NArray      &theValue);
	void								EncodeMacBinary1_Dictionary(MacBinary1_EncodeInfo &theInfo, const NDictionary &theValue);

	UInt64								DecodeMacBinary1_GetUIntX(UInt32 theSize, const UInt8 *thePtr);
	UInt64List							DecodeMacBinary1_GetObjectOffsets(UInt64 numObjects, UInt32 offsetsSize, const UInt8 *offsetsTable);
	UInt64								DecodeMacBinary1_GetObjectOffset(const MacBinary1_DecodeInfo &theInfo, UInt64 objectRef);

	MacBinary1_Tag						DecodeMacBinary1_ReadObjectTag( const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);
	UInt64								DecodeMacBinary1_ReadObjectSize(const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset, const MacBinary1_Tag &theTag);
	UInt64								DecodeMacBinary1_ReadObjectRef( const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);

	NVariant							DecodeMacBinary1_Value(     const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);
	bool								DecodeMacBinary1_Boolean(   const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);
	SInt64								DecodeMacBinary1_Integer(   const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);
	Float64								DecodeMacBinary1_Real(      const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);
	NString								DecodeMacBinary1_String(    const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);
	NVariant							DecodeMacBinary1_Data(      const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);
	NDate								DecodeMacBinary1_Date(      const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);
	NArray								DecodeMacBinary1_Array(     const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);
	NDictionary							DecodeMacBinary1_Dictionary(const MacBinary1_DecodeInfo &theInfo, UInt64 &byteOffset);


private:


};




#endif // NPROPERTYLIST_HDR



