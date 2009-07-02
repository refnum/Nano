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
static const NString kTestEncode =	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
									"<doc>\n"
									"	<child1>This is some text</child1>\n"
									"	<child2>\n"
									"		<!-- This is a comment -->\n"
									"		<child3><![CDATA[This is some &CDATA&]]></child3>\n"
									"	</child2>\n"
									"</doc>";





//============================================================================
//		TXMLEncoder::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TXMLEncoder::Execute(void)
{	NXMLNode		*nodeDoc, *nodeChild1, *nodeChild2, *nodeChild3;
	NXMLNode		*nodeText, *nodeComment, *nodeCData;
	NXMLEncoder		theEncoder;
	NString			theText;



	// Create the XML
	nodeDoc     = new NXMLNode(kXMLNodeElement, "doc");
	nodeChild1  = new NXMLNode(kXMLNodeElement, "child1");
	nodeChild2  = new NXMLNode(kXMLNodeElement, "child2");
	nodeChild3  = new NXMLNode(kXMLNodeElement, "child3");
	nodeText    = new NXMLNode(kXMLNodeText,    "This is some text");
	nodeComment = new NXMLNode(kXMLNodeComment, "This is a comment");
	nodeCData   = new NXMLNode(kXMLNodeCDATA,   "This is some &CDATA&");

	nodeDoc->AddChild(nodeChild1);
	nodeDoc->AddChild(nodeChild2);

	nodeChild1->AddChild(nodeText);

	nodeChild2->AddChild(nodeComment);
	nodeChild2->AddChild(nodeChild3);
	
	nodeChild3->AddChild(nodeCData);



	// Encode the document
	theText = theEncoder.Encode(nodeDoc);
	NN_ASSERT(theText == kTestEncode);



	// Clean up
	delete nodeDoc;
}








