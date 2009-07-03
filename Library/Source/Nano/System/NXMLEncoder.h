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
	NString								EncodeNode(   const NXMLNode *theNode, const NString &theIndent);
	NString								EncodeElement(const NXMLNode *theNode, const NString &theIndent);
	NString								EncodeText(   const NXMLNode *theNode);
	NString								EncodeComment(const NXMLNode *theNode);
	NString								EncodeCDATA(  const NXMLNode *theNode);

	bool								DecodeElementStart(const NString &theName, const NDictionary &theAttributes);
	bool								DecodeElementEnd(  const NString &theName);
	bool								DecodeText(        const NString &theValue);
	bool								DecodeComment(     const NString &theValue);
	bool								DecodeCDATAStart(  void);
	bool								DecodeCDATAEnd(    void);

	void								EncodeElementAttribute(const NDictionary &theAttributes, const NVariant &attributeKey, NString *theResult);
	bool								ContainsElements(const NXMLNodeList *theNodes); 

	NXMLNode							*GetDecodeParent(void);
	

private:
	NString								mDecodeText;
	NXMLNode							*mDecodeRoot;
	NXMLNode							*mDecodeCDATA;
	NXMLNodeList						mDecodeElements;
};



#endif // NXMLENCODER_HDR
