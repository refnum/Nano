/*	NAME:
		NXMLEncoder.h

	DESCRIPTION:
		XML encoder.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NXMLENCODER_HDR
#define NXMLENCODER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NXMLNode.h"
#include "NXMLParser.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NXMLEncoder {
public:
										 NXMLEncoder(void);
	virtual								~NXMLEncoder(void);


	// Encode/decode an XML document
	NString								Encode( const NXMLNode *theNode);
	NXMLNode							*Decode(const NString  &theXML);


private:
	NString								EncodeNode(    const NXMLNode *theNode, const NString &theIndent);
	NString								EncodeDocument(const NXMLNode *theNode, const NString &theIndent);
	NString								EncodeDocType( const NXMLNode *theNode);
	NString								EncodeElement( const NXMLNode *theNode, const NString &theIndent);
	NString								EncodeComment( const NXMLNode *theNode);
	NString								EncodeText(    const NXMLNode *theNode);
	NString								EncodeCData(   const NXMLNode *theNode);

	bool								DecodeDocType(     const NString &theName, const NXMLDocumentTypeInfo &theInfo);
	bool								DecodeElementStart(const NString &theName, const NDictionary          &theAttributes);
	bool								DecodeElementEnd(  const NString &theName);
	bool								DecodeComment(     const NString &theValue);
	bool								DecodeText(        const NString &theValue, bool isCData);

	void								EncodeElementAttribute(const NDictionary &theAttributes, const NVariant &attributeKey, NString *theResult);
	bool								ContainsElements(const NXMLNodeList *theNodes); 

	NXMLNode							*GetDecodeParent(void);
	

private:
	NXMLNode							*mDecodeRoot;
	NXMLNodeList						mDecodeElements;
};



#endif // NXMLENCODER_HDR
