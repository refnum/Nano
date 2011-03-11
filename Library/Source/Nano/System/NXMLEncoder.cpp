/*	NAME:
		NXMLEncoder.cpp

	DESCRIPTION:
		XML encoder.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTextUtilities.h"
#include "NXMLParser.h"
#include "NXMLEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kEncodeIndent									= "\t";

static const NString kXMLHeader										= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
static const NString kXMLDocTypePublic								= "<!DOCTYPE %@ PUBLIC \"%@\" \"%@\">";
static const NString kXMLDocTypeSystem								= "<!DOCTYPE %@ SYSTEM \"%@\">";





//============================================================================
//		NXMLEncoder::NXMLEncoder : Constructor.
//----------------------------------------------------------------------------
NXMLEncoder::NXMLEncoder(void)
{


	// Initialise ourselves
	mDecodeRoot = NULL;
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
	NN_ASSERT(theNode->IsType(kNXMLNodeDocument));



	// Encode the XML
	theXML = EncodeNode(theNode, "");

	return(theXML);
}





//============================================================================
//		NXMLEncoder::Decode : Decode an XML document to a node.
//----------------------------------------------------------------------------
NXMLNode *NXMLEncoder::Decode(const NString &theXML, const NProgressFunctor &theProgress)
{	NXMLParser		theParser;
	NXMLNode		*theNode;
	NStatus			theErr;



	// Validate our state
	NN_ASSERT(mDecodeRoot == NULL);
	NN_ASSERT(mDecodeElements.empty());



	// Prepare the parser
	theParser.SetProgress(theProgress);
	theParser.SetProcessDocumentType(BindSelf(NXMLEncoder::DecodeDocType,      _1, _2));
	theParser.SetProcessElementStart(BindSelf(NXMLEncoder::DecodeElementStart, _1, _2));
	theParser.SetProcessElementEnd(  BindSelf(NXMLEncoder::DecodeElementEnd,   _1));
	theParser.SetProcessComment(     BindSelf(NXMLEncoder::DecodeComment,      _1));
	theParser.SetProcessText(        BindSelf(NXMLEncoder::DecodeText,         _1, _2));



	// Create the root
	mDecodeRoot = new NXMLNode(kNXMLNodeDocument, "");
	theNode     = mDecodeRoot;



	// Decode the XML
	theErr = theParser.Parse(theXML);
	if (theErr != kNoErr)
		{
		delete mDecodeRoot;
		theNode = NULL;
		}



	// Clean up
	mDecodeRoot = NULL;
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
		case kNXMLNodeDocument:
			theText = EncodeDocument(theNode, theIndent);
			break;
		
		case kNXMLNodeDocType:
			theText = EncodeDocType(theNode);
			break;
		
		case kNXMLNodeElement:
			theText = EncodeElement(theNode, theIndent);
			break;
		
		case kNXMLNodeComment:
			theText = EncodeComment(theNode);
			break;
		
		case kNXMLNodeText:
			theText = EncodeText(theNode);
			break;
		
		case kNXMLNodeCData:
			theText = EncodeCData(theNode);
			break;
		
		default:
			NN_LOG("Unknown node type: %d", theNode->GetType());
			break;
		}

	return(theText);
}





//============================================================================
//		NXMLEncoder::EncodeDocument : Encode a document node.
//----------------------------------------------------------------------------
NString NXMLEncoder::EncodeDocument(const NXMLNode *theNode, const NString &theIndent)
{	const NXMLNodeList				*theChildren;
	NXMLNodeListConstIterator		theIter;
	NString							theText;



	// Validate our parameters
	NN_ASSERT(theNode->GetType() == kNXMLNodeDocument);



	// Get the state we need
	theChildren = theNode->GetChildren();



	// Encode the node
	theText = kXMLHeader;
	
	for (theIter = theChildren->begin(); theIter != theChildren->end(); theIter++)
		{
		theText += EncodeNode(*theIter, theIndent);
		theText += "\n";
		}

	return(theText);
}





//============================================================================
//		NXMLEncoder::EncodeDocType : Encode a DOCTYPE node.
//----------------------------------------------------------------------------
NString NXMLEncoder::EncodeDocType(const NXMLNode *theNode)
{	NString		theName, systemID, publicID;
	NString		theText;



	// Validate our parameters
	NN_ASSERT(theNode->GetType() == kNXMLNodeDocType);



	// Get the state we need
	theName  = theNode->GetTextValue();
	systemID = theNode->GetDocTypeSystemID();
	publicID = theNode->GetDocTypePublicID();



	// Encode the node
	if (!publicID.IsEmpty())
		{
		NN_ASSERT(!systemID.IsEmpty());
		theText.Format(kXMLDocTypePublic, theName, publicID, systemID);
		}
	else
		theText.Format(kXMLDocTypeSystem, theName, systemID);
	
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
	NN_ASSERT(theNode->GetType() == kNXMLNodeElement);



	// Get the state we need
	theChildren   = theNode->GetChildren();
	theAttributes = theNode->GetElementAttributes();
	textName      = theNode->GetTextValue();

	hasChildElements = ContainsElements(theChildren);
	childIndent      = theIndent + kEncodeIndent;
	theKeys          = theAttributes.GetKeys(true);



	// Collect the attributes
	theKeys.ForEach(BindSelf(NXMLEncoder::EncodeElementAttribute, theAttributes, _1, &textAttributes));
	textAttributes.TrimRight();



	// Encode an unpaired element
	if (theNode->IsElementUnpaired())
		{
		// Get the state we need
		if (!textAttributes.IsEmpty())
			textAttributes += " ";


		// Encode the tag
		tagOpen.Format("<%@%@/>", textName, textAttributes);
		theText = theIndent + tagOpen;
		}


	// Encode a paired element
	else
		{
		// Encode the open tag
		tagOpen.Format("<%@%@>", textName, textAttributes);
		theText = theIndent + tagOpen;


		// Encode the children
		for (theIter = theChildren->begin(); theIter != theChildren->end(); theIter++)
			{
			theChild = *theIter;
		
			if (hasChildElements)
				{
				theText += "\n";

				if (!theChild->IsType(kNXMLNodeElement))
					theText += childIndent;
				}

			theText += EncodeNode(theChild, childIndent);
			}

		if (hasChildElements)
			theText += NString("\n") + theIndent;


		// Encode the close tag
		tagClose.Format("</%@>", textName);
		theText += tagClose;
		}

	return(theText);
}





//============================================================================
//		NXMLEncoder::EncodeComment : Encode a comment node.
//----------------------------------------------------------------------------
NString NXMLEncoder::EncodeComment(const NXMLNode *theNode)
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theNode->GetType() == kNXMLNodeComment);



	// Encode the node
	theText.Format("<!--%@-->", theNode->GetTextValue());
	
	return(theText);
}





//============================================================================
//		NXMLEncoder::EncodeText : Encode an text node.
//----------------------------------------------------------------------------
NString NXMLEncoder::EncodeText(const NXMLNode *theNode)
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theNode->GetType() == kNXMLNodeText);



	// Encode the node
	theText = theNode->GetTextValue();
	theText = NTextUtilities::EncodeEntities(theText);

	return(theText);
}





//============================================================================
//		NXMLEncoder::EncodeCData : Encode a CDATA node.
//----------------------------------------------------------------------------
NString NXMLEncoder::EncodeCData(const NXMLNode *theNode)
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theNode->GetType() == kNXMLNodeCData);



	// Encode the node
	theText.Format("<![CDATA[%@]]>", theNode->GetTextValue());
	
	return(theText);
}





//============================================================================
//		NXMLEncoder::DecodeDocType : Decode a DOCTYPE.
//----------------------------------------------------------------------------
bool NXMLEncoder::DecodeDocType(const NString &theName, const NXMLDocumentTypeInfo &theInfo)
{	NXMLNode		*theParent, *theNode;



	// Decode the node
	theParent = GetDecodeParent();
	theNode   = new NXMLNode(kNXMLNodeDocType, theName);
	
	if (!theInfo.systemID.IsEmpty())
		theNode->SetDocTypeSystemID(theInfo.systemID);

	if (!theInfo.publicID.IsEmpty())
		theNode->SetDocTypePublicID(theInfo.publicID);
	
	theParent->AddChild(theNode);

	return(true);
}





//============================================================================
//		NXMLEncoder::DecodeElementStart : Decode an element start.
//----------------------------------------------------------------------------
bool NXMLEncoder::DecodeElementStart(const NString &theName, const NDictionary &theAttributes)
{	NXMLNode		*theParent, *theNode;



	// Decode the node
	theParent = GetDecodeParent();
	theNode   = new NXMLNode(kNXMLNodeElement, theName);
	theNode->SetElementAttributes(theAttributes);



	// Update our state
	mDecodeElements.push_back(theNode);
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
	
	NN_UNUSED(theName);
	
	
	
	// Update our state
	mDecodeElements.pop_back();

	return(true);
}





//============================================================================
//		NXMLEncoder::DecodeText : Decode text.
//----------------------------------------------------------------------------
bool NXMLEncoder::DecodeText(const NString &theValue, bool isCData)
{	NXMLNode		*theParent, *theNode;



	// Decode the node
	theParent = GetDecodeParent();
	theNode   = new NXMLNode(isCData ? kNXMLNodeCData : kNXMLNodeText, theValue);
	
	theParent->AddChild(theNode);

	return(true);
}





//============================================================================
//		NXMLEncoder::DecodeComment : Decode a comment.
//----------------------------------------------------------------------------
bool NXMLEncoder::DecodeComment(const NString &theValue)
{	NXMLNode		*theParent, *theNode;



	// Decode the node
	theParent = GetDecodeParent();
	theNode   = new NXMLNode(kNXMLNodeComment, theValue);
	
	theParent->AddChild(theNode);
	
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
	theText.Format(" %@=\"%@\"", theKey, theValue);
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
		if ((*theIter)->IsType(kNXMLNodeElement))
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
	if (mDecodeElements.empty())
		theNode = mDecodeRoot;
	else
		theNode = mDecodeElements.back();
	
	return(theNode);
}




