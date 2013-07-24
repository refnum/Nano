/*	NAME:
		NPropertyList.h

	DESCRIPTION:
		Property list file.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
	uint8_t			theToken;
	uint8_t			objectInfo;
} MacBinary1_Tag;

typedef struct {
	UInt64List		theOffsets;
	uint32_t		objectsSize;
	NData			objectsTable;
} MacBinary1_EncodeInfo;

typedef struct {
	UInt64List		theOffsets;
	uint32_t		objectsSize;
	const uint8_t	*objectsTable;
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
	int64_t								DecodeMacXML1_Integer(   const NXMLNode *theNode);
	float64_t							DecodeMacXML1_Real(      const NXMLNode *theNode);
	NString								DecodeMacXML1_String(    const NXMLNode *theNode);
	NData								DecodeMacXML1_Data(      const NXMLNode *theNode);
	NDate								DecodeMacXML1_Date(      const NXMLNode *theNode);
	NArray								DecodeMacXML1_Array(     const NXMLNode *theNode);
	NDictionary							DecodeMacXML1_Dictionary(const NXMLNode *theNode);

	void								EncodeMacBinary1_GetObjectCount(const NVariant &theValue, uint32_t *numObjects);
	uint32_t							EncodeMacBinary1_GetIntegerSize(uint64_t theSize);
	NData								EncodeMacBinary1_GetIntegerList(const UInt64List &theValues, uint32_t byteSize);

	void								EncodeMacBinary1_WriteObject(    MacBinary1_EncodeInfo &theInfo);
	void								EncodeMacBinary1_WriteObjectTag( MacBinary1_EncodeInfo &theInfo, uint8_t theToken, uint8_t objectInfo, uint64_t objectSize=0);
	void								EncodeMacBinary1_WriteObjectData(MacBinary1_EncodeInfo &theInfo, NIndex theSize, const void *thePtr);

	uint64_t							EncodeMacBinary1_Value(     MacBinary1_EncodeInfo &theInfo, const NVariant    &theValue);
	void								EncodeMacBinary1_Boolean(   MacBinary1_EncodeInfo &theInfo,       bool         theValue);
	void								EncodeMacBinary1_Integer(   MacBinary1_EncodeInfo &theInfo, const NNumber     &theValue, bool addObject=true);
	void								EncodeMacBinary1_Real(      MacBinary1_EncodeInfo &theInfo, const NNumber     &theValue);
	void								EncodeMacBinary1_String(    MacBinary1_EncodeInfo &theInfo, const NString     &theValue);
	void								EncodeMacBinary1_Data(      MacBinary1_EncodeInfo &theInfo, const NData       &theValue);
	void								EncodeMacBinary1_Date(      MacBinary1_EncodeInfo &theInfo, const NDate       &theValue);
	void								EncodeMacBinary1_Array(     MacBinary1_EncodeInfo &theInfo, const NArray      &theValue);
	void								EncodeMacBinary1_Dictionary(MacBinary1_EncodeInfo &theInfo, const NDictionary &theValue);

	uint64_t							DecodeMacBinary1_GetUIntX(uint32_t theSize, const uint8_t *thePtr);
	UInt64List							DecodeMacBinary1_GetObjectOffsets(uint64_t numObjects, uint32_t offsetsSize, const uint8_t *offsetsTable);
	uint64_t							DecodeMacBinary1_GetObjectOffset(const MacBinary1_DecodeInfo &theInfo, uint64_t objectRef);

	MacBinary1_Tag						DecodeMacBinary1_ReadObjectTag( const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);
	uint64_t							DecodeMacBinary1_ReadObjectSize(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset, const MacBinary1_Tag &theTag);
	uint64_t							DecodeMacBinary1_ReadObjectRef( const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);

	NVariant							DecodeMacBinary1_Value(     const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);
	bool								DecodeMacBinary1_Boolean(   const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);
	int64_t								DecodeMacBinary1_Integer(   const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);
	float64_t							DecodeMacBinary1_Real(      const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);
	NString								DecodeMacBinary1_String(    const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);
	NVariant							DecodeMacBinary1_Data(      const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);
	NDate								DecodeMacBinary1_Date(      const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);
	NArray								DecodeMacBinary1_Array(     const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);
	NDictionary							DecodeMacBinary1_Dictionary(const MacBinary1_DecodeInfo &theInfo, uint64_t &byteOffset);


private:


};




#endif // NPROPERTYLIST_HDR



