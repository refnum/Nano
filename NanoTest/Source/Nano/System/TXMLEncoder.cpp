/*	NAME:
		TXMLEncoder.cpp

	DESCRIPTION:
		NXMLEncoder tests.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NXMLEncoder.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const NString kValueDocType                          = "nano";
static const NString kValueDocTypeSystemID                  = "http://www.apple.com/DTDs/PropertyList-1.0.dtd";
static const NString kValueDocTypePublicID                  = "-//Apple Computer//DTD PLIST 1.0//EN";

static const NString kValueContents                         = "contents";
static const NString kValueNodeChild1                       = "child1";
static const NString kValueNodeChild2                       = "child2";
static const NString kValueNodeChild3                       = "child3";

static const NString kValueNodeText                         = "This \" is & a ' test < text > node";
static const NString kValueNodeComment                      = " This is a comment ";
static const NString kValueNodeCData                        = "This is some &CDATA&";

static const NString kAttribute1Name                        = "attribute1";
static const NString kAttribute1Value                       = "some value";

static const NString kAttribute2Name                        = "attribute2";
static const NString kAttribute2Value                       = "other value";

static const NString kTestXML                               = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
								"<!DOCTYPE nano PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" "
								"\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
								"<contents>\n"
								"	<child1 attribute1=\"some value\">This &quot; is &amp; a "
								"&apos; test &lt; text &gt; node</child1>\n"
								"	<child2 attribute2=\"other value\">\n"
								"		<!-- This is a comment -->\n"
								"		<child3><![CDATA[This is some &CDATA&]]></child3>\n"
								"	</child2>\n"
								"</contents>\n";





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TXMLEncoder)
{
	NXMLEncoder theEncoder;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TXMLEncoder, "Encoding")
{


	// Perform the test
	NSharedPtrXMLNode nodeDoc = std::make_shared<NXMLNode>(NXMLNodeType::Document, "");
	NSharedPtrXMLNode nodeDocType =
		std::make_shared<NXMLNode>(NXMLNodeType::DocType, kValueDocType);
	NSharedPtrXMLNode nodeContents =
		std::make_shared<NXMLNode>(NXMLNodeType::Element, kValueContents);
	NSharedPtrXMLNode nodeChild1 =
		std::make_shared<NXMLNode>(NXMLNodeType::Element, kValueNodeChild1);
	NSharedPtrXMLNode nodeChild2 =
		std::make_shared<NXMLNode>(NXMLNodeType::Element, kValueNodeChild2);
	NSharedPtrXMLNode nodeChild3 =
		std::make_shared<NXMLNode>(NXMLNodeType::Element, kValueNodeChild3);
	NSharedPtrXMLNode nodeText = std::make_shared<NXMLNode>(NXMLNodeType::Text, kValueNodeText);
	NSharedPtrXMLNode nodeComment =
		std::make_shared<NXMLNode>(NXMLNodeType::Comment, kValueNodeComment);
	NSharedPtrXMLNode nodeCData = std::make_shared<NXMLNode>(NXMLNodeType::CData, kValueNodeCData);

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

	NString theText = theEncoder.Encode(nodeDoc);
	REQUIRE(theText == kTestXML);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TXMLEncoder, "Decoding")
{


	// Perform the test
	NSharedPtrXMLNode nodeDoc = theEncoder.Decode(kTestXML);
	REQUIRE(nodeDoc->IsType(NXMLNodeType::Document));


	NVectorSharedPtrXMLNode theChildren = nodeDoc->GetChildren();
	REQUIRE(theChildren.size() == 2);

	NSharedPtrXMLNode nodeChild1 = theChildren[0];
	REQUIRE(nodeChild1->IsType(NXMLNodeType::DocType));
	REQUIRE(nodeChild1->GetTextValue() == kValueDocType);
	REQUIRE(nodeChild1->GetDocTypeSystemID() == kValueDocTypeSystemID);
	REQUIRE(nodeChild1->GetDocTypePublicID() == kValueDocTypePublicID);

	NSharedPtrXMLNode nodeChild2 = theChildren[1];
	REQUIRE(nodeChild2->IsType(NXMLNodeType::Element));
	REQUIRE(nodeChild2->GetTextValue() == kValueContents);
	REQUIRE(nodeChild2->GetElementAttributes().IsEmpty());


	theChildren = nodeChild2->GetChildren();
	REQUIRE(theChildren.size() == 2);

	nodeChild1 = theChildren[0];
	REQUIRE(nodeChild1->IsType(NXMLNodeType::Element));
	REQUIRE(nodeChild1->GetTextValue() == kValueNodeChild1);
	REQUIRE(nodeChild1->GetElementAttribute(kAttribute1Name) == kAttribute1Value);
	REQUIRE(nodeChild1->GetElementAttribute(kAttribute2Name).IsEmpty());
	REQUIRE(nodeChild1->GetElementAttributes().GetSize() == 1);

	nodeChild2 = theChildren[1];
	REQUIRE(nodeChild2->IsType(NXMLNodeType::Element));
	REQUIRE(nodeChild2->GetTextValue() == kValueNodeChild2);
	REQUIRE(nodeChild2->GetElementAttribute(kAttribute2Name) == kAttribute2Value);
	REQUIRE(nodeChild2->GetElementAttribute(kAttribute1Name).IsEmpty());
	REQUIRE(nodeChild2->GetElementAttributes().GetSize() == 1);


	theChildren = nodeChild1->GetChildren();
	REQUIRE(theChildren.size() == 1);

	NSharedPtrXMLNode nodeText = theChildren[0];
	REQUIRE(nodeText->IsType(NXMLNodeType::Text));
	REQUIRE(nodeText->GetTextValue() == kValueNodeText);


	theChildren = nodeChild2->GetChildren();
	REQUIRE(theChildren.size() == 2);

	NSharedPtrXMLNode nodeComment = theChildren[0];
	REQUIRE(nodeComment->IsType(NXMLNodeType::Comment));
	REQUIRE(nodeComment->GetTextValue() == kValueNodeComment);

	NSharedPtrXMLNode nodeChild3 = theChildren[1];
	REQUIRE(nodeChild3->IsType(NXMLNodeType::Element));
	REQUIRE(nodeChild3->GetTextValue() == kValueNodeChild3);
	REQUIRE(nodeChild3->GetElementAttribute(kAttribute1Name).IsEmpty());
	REQUIRE(nodeChild3->GetElementAttribute(kAttribute2Name).IsEmpty());
	REQUIRE(nodeChild3->GetElementAttributes().IsEmpty());


	theChildren = nodeChild3->GetChildren();
	REQUIRE(theChildren.size() == 1);

	NSharedPtrXMLNode nodeCData = theChildren[0];
	REQUIRE(nodeCData->IsType(NXMLNodeType::CData));
	REQUIRE(nodeCData->GetTextValue() == kValueNodeCData);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TXMLEncoder, "Recoding")
{


	// Perform the test
	NSharedPtrXMLNode nodeDoc = theEncoder.Decode(kTestXML);
	NString           theText = theEncoder.Encode(nodeDoc);

	REQUIRE(theText == kTestXML);
}
