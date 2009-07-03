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
#include "NXMLParser.h"
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


	// Initialise ourselves
	mDecodeRoot  = NULL;
	mDecodeCDATA = NULL;
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
{	NXMLParser		theParser;
	NXMLNode		*theNode;
	NStatus			theErr;



	// Validate our state
	NN_ASSERT(mDecodeText.IsEmpty());
	NN_ASSERT(mDecodeRoot  == NULL);
	NN_ASSERT(mDecodeCDATA == NULL);
	NN_ASSERT(mDecodeElements.empty());
	


	// Prepare the parser
	theParser.SetProcessElementStart(BindSelf(NXMLEncoder::DecodeElementStart, _1, _2));
	theParser.SetProcessElementEnd(  BindSelf(NXMLEncoder::DecodeElementEnd,   _1));
	theParser.SetProcessText(        BindSelf(NXMLEncoder::DecodeText,         _1));
	theParser.SetProcessComment(     BindSelf(NXMLEncoder::DecodeComment,      _1));
	theParser.SetProcessCDATAStart(  BindSelf(NXMLEncoder::DecodeCDATAStart));
	theParser.SetProcessCDATAEnd(    BindSelf(NXMLEncoder::DecodeCDATAEnd));



	// Decode the XML
	theNode = NULL;
	theErr  = theParser.Parse(theXML);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == kNoErr)
		theNode = mDecodeRoot;



	// Clean up
	mDecodeRoot  = NULL;
	mDecodeCDATA = NULL;
	
	mDecodeText.Clear();
	mDecodeElements.clear();
	
	return(theNode);
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
	
	theText.ReplaceAll("&",  "&amp;");
	theText.ReplaceAll("<",  "&lt;");
	theText.ReplaceAll(">",  "&gt;");
	theText.ReplaceAll("'",  "&apos;");
	theText.ReplaceAll("\"", "&quot;");

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
//		NXMLEncoder::DecodeElementStart : Decode an element start.
//----------------------------------------------------------------------------
bool NXMLEncoder::DecodeElementStart(const NString &theName, const NDictionary &theAttributes)
{	NXMLNode		*theParent, *theNode;



	// Decode the node
	theParent = GetDecodeParent();
	theNode   = new NXMLNode(kXMLNodeElement, theName);
	theNode->SetElementAttributes(theAttributes);



	// Update our state
	mDecodeElements.push_back(theNode);

	if (theParent == NULL)
		mDecodeRoot = theNode;
	else
		theParent->AddChild(theNode);
	
	return(true);
}





//============================================================================
//		NXMLEncoder::DecodeElementEnd : Decode an element end.
//----------------------------------------------------------------------------
bool NXMLEncoder::DecodeElementEnd(const NString &theName)
{


	// Validate our parameters and state
	NN_ASSERT(!mDecodeElements.empty());
	NN_ASSERT(mDecodeElements.back()->GetTextValue() == theName);
	
	(void) theName;
	
	
	
	// Update our state
	mDecodeElements.pop_back();

	return(true);
}





//============================================================================
//		NXMLEncoder::DecodeText : Decode text.
//----------------------------------------------------------------------------
bool NXMLEncoder::DecodeText(const NString &theValue)
{	NXMLNode		*theParent, *theNode;



	// Accumulate text within a CDATA
	if (mDecodeCDATA != NULL)
		mDecodeText += theValue;
	
	
	// Decode the node
	else
		{
		theParent = GetDecodeParent();
		theNode   = new NXMLNode(kXMLNodeText, theValue);
	
		theParent->AddChild(theNode);
		}

	return(true);
}





//============================================================================
//		NXMLEncoder::DecodeComment : Decode a comment.
//----------------------------------------------------------------------------
bool NXMLEncoder::DecodeComment(const NString &theValue)
{	NXMLNode		*theParent, *theNode;



	// Decode the node
	theParent = GetDecodeParent();
	theNode   = new NXMLNode(kXMLNodeComment, theValue);
	
	theParent->AddChild(theNode);
	
	return(true);
}





//============================================================================
//		NXMLEncoder::DecodeCDATAStart : Decode a CDATA start.
//----------------------------------------------------------------------------
bool NXMLEncoder::DecodeCDATAStart(void)
{	NXMLNode		*theParent;



	// Validate our state
	NN_ASSERT(mDecodeCDATA == NULL);



	// Decode the node
	theParent    = GetDecodeParent();
	mDecodeCDATA = new NXMLNode(kXMLNodeCDATA, "");
	
	theParent->AddChild(mDecodeCDATA);
	
	return(true);
}





//============================================================================
//		NXMLEncoder::DecodeCDATAEnd : Decode a CDATA end.
//----------------------------------------------------------------------------
bool NXMLEncoder::DecodeCDATAEnd(void)
{


	// Validate our state
	NN_ASSERT(mDecodeCDATA != NULL);



	// Update the node
	mDecodeCDATA->SetTextValue(mDecodeText);
	
	mDecodeText.Clear();
	mDecodeCDATA = NULL;

	return(true);
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
//		NXMLEncoder::GetDecodeParent : Get the parent element.
//----------------------------------------------------------------------------
NXMLNode *NXMLEncoder::GetDecodeParent(void)
{	NXMLNode	*theNode;



	// Get the parent
	theNode = NULL;
	
	if (!mDecodeElements.empty())
		theNode = mDecodeElements.back();
	
	return(theNode);
}




