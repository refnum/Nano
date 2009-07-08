/*	NAME:
		TXMLEncoder.cpp

	DESCRIPTION:
		NXMLEncoder tests.

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

#include "TXMLEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kValueDocType									= "nano";
static const NString kValueDocTypeSystemID							= "http://www.apple.com/DTDs/PropertyList-1.0.dtd";
static const NString kValueDocTypePublicID							= "-//Apple Computer//DTD PLIST 1.0//EN";

static const NString kValueContents									= "contents";
static const NString kValueNodeChild1								= "child1";
static const NString kValueNodeChild2								= "child2";
static const NString kValueNodeChild3								= "child3";

static const NString kValueNodeText									= "This \" is & a ' test < text > node";
static const NString kValueNodeComment								= " This is a comment ";
static const NString kValueNodeCData								= "This is some &CDATA&";

static const NString kAttribute1Name								= "attribute1";
static const NString kAttribute1Value								= "some value";

static const NString kAttribute2Name								= "attribute2";
static const NString kAttribute2Value								= "other value";

static const NString kTestXML										=	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
																		"<!DOCTYPE nano PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
																		"<contents>\n"
																		"	<child1 attribute1=\"some value\">This &quot; is &amp; a &apos; test &lt; text &gt; node</child1>\n"
																		"	<child2 attribute2=\"other value\">\n"
																		"		<!-- This is a comment -->\n"
																		"		<child3><![CDATA[This is some &CDATA&]]></child3>\n"
																		"	</child2>\n"
																		"</contents>\n";





//============================================================================
//		TXMLEncoder::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TXMLEncoder::Execute(void)
{	NXMLNode						*nodeDoc, *nodeDocType, *nodeContents;
	NXMLNode						*nodeChild1, *nodeChild2, *nodeChild3;
	NXMLNode						*nodeText, *nodeComment, *nodeCData;
	NXMLNodeList					theChildren;
	NXMLEncoder						theEncoder;
	NString							theText;



	// Encoding
	nodeDoc      = new NXMLNode(kXMLNodeDocument, "");
	nodeDocType  = new NXMLNode(kXMLNodeDocType,  kValueDocType);
	nodeContents = new NXMLNode(kXMLNodeElement,  kValueContents);
	nodeChild1   = new NXMLNode(kXMLNodeElement,  kValueNodeChild1);
	nodeChild2   = new NXMLNode(kXMLNodeElement,  kValueNodeChild2);
	nodeChild3   = new NXMLNode(kXMLNodeElement,  kValueNodeChild3);
	nodeText     = new NXMLNode(kXMLNodeText,     kValueNodeText);
	nodeComment  = new NXMLNode(kXMLNodeComment,  kValueNodeComment);
	nodeCData    = new NXMLNode(kXMLNodeCData,    kValueNodeCData);

	nodeDoc->AddChild(nodeDocType);
	nodeDocType->SetDocTypeSystemID(kValueDocTypeSystemID);
	nodeDocType->SetDocTypePublicID(kValueDocTypePublicID);

	nodeDoc->AddChild(nodeContents);
	nodeContents->AddChild(nodeChild1);
	nodeContents->AddChild(nodeChild2);

	nodeChild1->AddChild(nodeText);
	nodeChild1->SetElementAttribute(kAttribute1Name, kAttribute1Value);
	
	nodeChild2->AddChild(nodeComment);
	nodeChild2->AddChild(nodeChild3);
	nodeChild2->SetElementAttribute(kAttribute2Name, kAttribute2Value);
	
	nodeChild3->AddChild(nodeCData);


	theText = theEncoder.Encode(nodeDoc);
	NN_ASSERT(theText == kTestXML);

	delete nodeDoc;



	// Decoding
	nodeDoc = theEncoder.Decode(theText);
	NN_ASSERT(nodeDoc->IsType(kXMLNodeDocument));


	theChildren = *(nodeDoc->GetChildren());
	NN_ASSERT(theChildren.size() == 2);
	
	nodeChild1 = theChildren[0];
	NN_ASSERT(nodeChild1->IsType(kXMLNodeDocType));
	NN_ASSERT(nodeChild1->GetTextValue() == kValueDocType);
	NN_ASSERT(nodeChild1->GetDocTypeSystemID() == kValueDocTypeSystemID);
	NN_ASSERT(nodeChild1->GetDocTypePublicID() == kValueDocTypePublicID);

	nodeChild2 = theChildren[1];
	NN_ASSERT(nodeChild2->IsType(kXMLNodeElement));
	NN_ASSERT(nodeChild2->GetTextValue() == kValueContents);
	NN_ASSERT(nodeChild2->GetElementAttributes().IsEmpty());


	theChildren = *(nodeChild2->GetChildren());
	NN_ASSERT(theChildren.size() == 2);

	nodeChild1 = theChildren[0];
	NN_ASSERT(nodeChild1->IsType(kXMLNodeElement));
	NN_ASSERT(nodeChild1->GetTextValue() == kValueNodeChild1);
	NN_ASSERT(nodeChild1->GetElementAttribute(kAttribute1Name) == kAttribute1Value);
	NN_ASSERT(nodeChild1->GetElementAttribute(kAttribute2Name).IsEmpty());
	NN_ASSERT(nodeChild1->GetElementAttributes().GetSize() == 1);

	nodeChild2 = theChildren[1];
	NN_ASSERT(nodeChild2->IsType(kXMLNodeElement));
	NN_ASSERT(nodeChild2->GetTextValue() == kValueNodeChild2);
	NN_ASSERT(nodeChild2->GetElementAttribute(kAttribute2Name) == kAttribute2Value);
	NN_ASSERT(nodeChild2->GetElementAttribute(kAttribute1Name).IsEmpty());
	NN_ASSERT(nodeChild2->GetElementAttributes().GetSize() == 1);


	theChildren = *(nodeChild1->GetChildren());
	NN_ASSERT(theChildren.size() == 1);
	
	nodeText = theChildren[0];
	NN_ASSERT(nodeText->IsType(kXMLNodeText));
	NN_ASSERT(nodeText->GetTextValue() == kValueNodeText);


	theChildren = *(nodeChild2->GetChildren());
	NN_ASSERT(theChildren.size() == 2);
	
	nodeComment = theChildren[0];
	NN_ASSERT(nodeComment->IsType(kXMLNodeComment));
	NN_ASSERT(nodeComment->GetTextValue() == kValueNodeComment);

	nodeChild3 = theChildren[1];
	NN_ASSERT(nodeChild3->IsType(kXMLNodeElement));
	NN_ASSERT(nodeChild3->GetTextValue() == kValueNodeChild3);
	NN_ASSERT(nodeChild3->GetElementAttribute(kAttribute1Name).IsEmpty());
	NN_ASSERT(nodeChild3->GetElementAttribute(kAttribute2Name).IsEmpty());
	NN_ASSERT(nodeChild3->GetElementAttributes().IsEmpty());


	theChildren = *(nodeChild3->GetChildren());
	NN_ASSERT(theChildren.size() == 1);
	
	nodeCData = theChildren[0];
	NN_ASSERT(nodeCData->IsType(kXMLNodeCData));
	NN_ASSERT(nodeCData->GetTextValue() == kValueNodeCData);



	// Round-trip
	theText = theEncoder.Encode(nodeDoc);
	NN_ASSERT(theText == kTestXML);

	delete nodeDoc;
}





