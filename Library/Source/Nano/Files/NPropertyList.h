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
	kNPropertyListMacXML_1_0,
	kNPropertyListMacBinary_1_0,


	// Generic
	kNPropertyListXML		= kNPropertyListMacXML_1_0,
	kNPropertyListBinary	= kNPropertyListMacBinary_1_0,
} NPropertyListFormat;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPropertyList {
public:
										NPropertyList(void);
	virtual							   ~NPropertyList(void);


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
	NPropertyListFormat					GetFormat(const NData &theData);

	NData								EncodeMacXML_1_0(const NDictionary &theState);
	NDictionary							DecodeMacXML_1_0(const NData       &theData);

	NData								EncodeMacBinary_1_0(const NDictionary &theState);
	NDictionary							DecodeMacBinary_1_0(const NData       &theData);

	NXMLNode							*EncodeMacXML_1_0_Boolean(         bool         theValue);
	NXMLNode							*EncodeMacXML_1_0_Number(    const NNumber     &theValue);
	NXMLNode							*EncodeMacXML_1_0_String(    const NString     &theValue);
	NXMLNode							*EncodeMacXML_1_0_Data(      const NData       &theValue);
	NXMLNode							*EncodeMacXML_1_0_Date(      const NDate       &theValue);
	NXMLNode							*EncodeMacXML_1_0_Array(     const NArray      &theValue);
	NXMLNode							*EncodeMacXML_1_0_Dictionary(const NDictionary &theValue);

	bool								DecodeMacXML_1_0_Boolean(   const NXMLNode *theNode);
	SInt64								DecodeMacXML_1_0_Integer(   const NXMLNode *theNode);
	Float64								DecodeMacXML_1_0_Real(      const NXMLNode *theNode);
	NString								DecodeMacXML_1_0_String(    const NXMLNode *theNode);
	NData								DecodeMacXML_1_0_Data(      const NXMLNode *theNode);
	NDate								DecodeMacXML_1_0_Date(      const NXMLNode *theNode);
	NArray								DecodeMacXML_1_0_Array(     const NXMLNode *theNode);
	NDictionary							DecodeMacXML_1_0_Dictionary(const NXMLNode *theNode);


private:


};



#endif // NPROPERTYLIST_HDR



