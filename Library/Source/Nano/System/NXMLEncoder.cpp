/*	NAME:
		NXMLEncoder.cpp

	DESCRIPTION:
		XML encoder.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NXMLEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kEncodeIndent									= "\t";





//============================================================================
//		NXMLEncoder::NXMLEncoder : Constructor.
//----------------------------------------------------------------------------
NXMLEncoder::NXMLEncoder(void)
{
}





//============================================================================
//		NXMLEncoder::~NXMLEncoder : Destructor.
//----------------------------------------------------------------------------
NXMLEncoder::~NXMLEncoder(void)
{
}





//============================================================================
//		NXMLEncoder::Encode : Encode a node to an XML document.
//----------------------------------------------------------------------------
NString NXMLEncoder::Encode(const NXMLNode *theNode)
{	NString		theXML;



	// Validate our parameters
	NN_ASSERT(theNode->IsType(kXMLNodeElement));



	// Encode the XML
	theXML  = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	theXML += EncodeNode(theNode, "");
	
	return(theXML);
}





//============================================================================
//		NXMLEncoder::Decode : Decode an XML document to a node.
//----------------------------------------------------------------------------
NXMLNode *NXMLEncoder::Decode(const NString &theXML)
{
	// dair, to do
	return(NULL);
}





//============================================================================
//		NXMLEncoder::EncodeNode : Encode a node to XML.
//----------------------------------------------------------------------------
#pragma mark -
NString NXMLEncoder::EncodeNode(const NXMLNode *theNode, const NString &theIndent)
{	NString			theText;



	// Encode the node
	switch (theNode->GetType()) {
		case kXMLNodeElement:
			theText = EncodeElement(theNode, theIndent);
			break;
		
		case kXMLNodeText:
			theText = EncodeText(theNode);
			break;
		
		case kXMLNodeComment:
			theText = EncodeComment(theNode);
			break;
		
		case kXMLNodeCDATA:
			theText = EncodeCDATA(theNode);
			break;
		
		default:
			NN_LOG("Unknown node type: %d", theNode->GetType());
			break;
		}

	return(theText);
}





//============================================================================
//		NXMLEncoder::EncodeElement : Encode an element node.
//----------------------------------------------------------------------------
NString NXMLEncoder::EncodeElement(const NXMLNode *theNode, const NString &theIndent)
{	NString							tagOpen, tagClose, theText, childIndent;
	NString							textName, textAttributes;
	bool							hasChildElements;
	NDictionary						theAttributes;
	const NXMLNodeList				*theChildren;
	NXMLNode						*theChild;
	NArray							theKeys;
	NXMLNodeListConstIterator		theIter;



	// Validate our parameters
	NN_ASSERT(theNode->GetType() == kXMLNodeElement);



	// Get the state we need
	theChildren   = theNode->GetChildren();
	theAttributes = theNode->GetElementAttributes();
	textName      = theNode->GetTextValue();

	hasChildElements = ContainsElements(theChildren);
	childIndent      = theIndent + kEncodeIndent;
	theKeys          = theAttributes.GetKeys();



	// Collect the attributes
	theKeys.Sort();
	theKeys.ForEach(BindSelf(NXMLEncoder::EncodeElementAttribute, theAttributes, _1, &textAttributes));
	textAttributes.TrimRight();



	// Prepare the tags
	tagOpen.Format("<%@%@>", textName, textAttributes);
	tagClose.Format("</%@>", textName);



	// Encode the node
	theText = theIndent + tagOpen;

	for (theIter = theChildren->begin(); theIter != theChildren->end(); theIter++)
		{
		theChild = *theIter;
		
		if (hasChildElements)
			{
			theText += kNStringNewline;

			if (!theChild->IsType(kXMLNodeElement))
				theText += childIndent;
			}

		theText += EncodeNode(theChild, childIndent);
		}

	if (hasChildElements)
		theText += kNStringNewline + theIndent;

	theText += tagClose;

	return(theText);
}





//============================================================================
//		NXMLEncoder::EncodeText : Encode an text node.
//----------------------------------------------------------------------------
NString NXMLEncoder::EncodeText(const NXMLNode *theNode)
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theNode->GetType() == kXMLNodeText);



	// Encode the node
	theText = theNode->GetTextValue();
	
	return(theText);
}





//============================================================================
//		NXMLEncoder::EncodeComment : Encode a comment node.
//----------------------------------------------------------------------------
NString NXMLEncoder::EncodeComment(const NXMLNode *theNode)
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theNode->GetType() == kXMLNodeComment);



	// Encode the node
	theText.Format("<!-- %@ -->", theNode->GetTextValue());
	
	return(theText);
}





//============================================================================
//		NXMLEncoder::EncodeCDATA : Encode a CDATA node.
//----------------------------------------------------------------------------
NString NXMLEncoder::EncodeCDATA(const NXMLNode *theNode)
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theNode->GetType() == kXMLNodeCDATA);



	// Encode the node
	theText.Format("<![CDATA[%@]]>", theNode->GetTextValue());
	
	return(theText);
}





//============================================================================
//		NXMLEncoder::ContainsElements : Does a node list contain elements?
//----------------------------------------------------------------------------
bool NXMLEncoder::ContainsElements(const NXMLNodeList *theNodes)
{	NXMLNodeListConstIterator		theIter;



	// Check the nodes
	for (theIter = theNodes->begin(); theIter != theNodes->end(); theIter++)
		{
		if ((*theIter)->IsType(kXMLNodeElement))
			return(true);
		}
	
	return(false);
}





//============================================================================
//		NXMLEncoder::EncodeElementAttribute : Encode an element attribute.
//----------------------------------------------------------------------------
void NXMLEncoder::EncodeElementAttribute(const NDictionary &theAttributes, const NVariant &attributeKey, NString *theResult)
{	NString		theKey, theValue, theText;
	bool		gotKey;



	// Validate our parameters
	NN_ASSERT(attributeKey.IsType(theKey));



	// Get the state we need
	gotKey   = attributeKey.GetValue(theKey);
	theValue = theAttributes.GetValueString(theKey);
	
	NN_ASSERT(gotKey);



	// Encode the attribute
	theText.Format(" %@=\"%@\" ", theKey, theValue);
	*theResult += theText;
}



