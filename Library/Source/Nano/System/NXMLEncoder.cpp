/*	NAME:
		NXMLEncoder.cpp

	DESCRIPTION:
		XML encoder.

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
#include "NXMLEncoder.h"

// Nano
#include "NFormat.h"
#include "NFunction.h"
#include "NStdAlgorithm.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr const char* kXMLHeader                     = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
static constexpr const char* kEncodeIndent                  = "\t";





//=============================================================================
//		NXMLEncoder::NXMLEncoder : Constructor.
//-----------------------------------------------------------------------------
NXMLEncoder::NXMLEncoder()
	: mDecodeRoot()
	, mDecodeElements()
{
}





//=============================================================================
//		NXMLEncoder::Encode : Encode a node to an XML document.
//-----------------------------------------------------------------------------
NString NXMLEncoder::Encode(const NSharedXMLNode& theNode, NStatus* parseErr)
{


	// Validate our parameters
	NN_REQUIRE(theNode->IsType(NXMLNodeType::Document));



	// Encode the XML
	NString theXML = EncodeNode(theNode, "");

	if (parseErr != nullptr)
	{
		if (theNode != nullptr && theXML.IsEmpty())
		{
			*parseErr = NStatus::Internal;
		}
		else
		{
			*parseErr = NStatus::OK;
		}
	}

	return theXML;
}





//=============================================================================
//		NXMLEncoder::Decode : Decode an XML document to a node.
//-----------------------------------------------------------------------------
NSharedXMLNode NXMLEncoder::Decode(const NString&           theXML,
								   NStatus*                 parseErr,
								   const NFunctionProgress& theProgress)
{


	// Validate our state
	NN_REQUIRE(mDecodeRoot == nullptr);
	NN_REQUIRE(mDecodeElements.empty());



	// Prepare the parser
	NXMLParser theParser;

	theParser.SetProgressFunction(theProgress);
	theParser.SetProcessDocumentType(NBindSelf(NXMLEncoder::DecodeDocType, kNArg1, kNArg2));
	theParser.SetProcessElementStart(NBindSelf(NXMLEncoder::DecodeElementStart, kNArg1, kNArg2));
	theParser.SetProcessElementEnd(NBindSelf(NXMLEncoder::DecodeElementEnd, kNArg1));
	theParser.SetProcessComment(NBindSelf(NXMLEncoder::DecodeComment, kNArg1));
	theParser.SetProcessText(NBindSelf(NXMLEncoder::DecodeText, kNArg1, kNArg2));



	// Create the root
	mDecodeRoot            = std::make_shared<NXMLNode>(NXMLNodeType::Document, "");
	NSharedXMLNode theNode = mDecodeRoot;



	// Decode the XML
	NStatus theErr = theParser.Parse(theXML);
	if (theErr != NStatus::OK)
	{
		theNode.reset();
	}



	// Clean up
	mDecodeRoot.reset();
	mDecodeElements.clear();

	if (parseErr != nullptr)
	{
		*parseErr = theErr;
	}

	return theNode;
}





#pragma mark private
//=============================================================================
//		NXMLEncoder::EncodeNode : Encode a node to XML.
//-----------------------------------------------------------------------------
NString NXMLEncoder::EncodeNode(const NSharedXMLNode& theNode, const NString& theIndent)
{


	// Encode the node
	switch (theNode->GetType())
	{
		case NXMLNodeType::Document:
			return EncodeDocument(theNode, theIndent);
			break;

		case NXMLNodeType::DocType:
			return EncodeDocType(theNode);
			break;

		case NXMLNodeType::Element:
			return EncodeElement(theNode, theIndent);
			break;

		case NXMLNodeType::Comment:
			return EncodeComment(theNode);
			break;

		case NXMLNodeType::Text:
			return EncodeText(theNode);
			break;

		case NXMLNodeType::CData:
			return EncodeCData(theNode);
			break;
	}

	NN_UNREACHABLE();
}





//=============================================================================
//		NXMLEncoder::EncodeDocument : Encode a document node.
//-----------------------------------------------------------------------------
NString NXMLEncoder::EncodeDocument(const NSharedXMLNode& theNode, const NString& theIndent)
{


	// Validate our parameters
	NN_REQUIRE(theNode->GetType() == NXMLNodeType::Document);



	// Encode the node
	NString theText = kXMLHeader;

	for (const auto& theChild : theNode->GetChildren())
	{
		theText += EncodeNode(theChild, theIndent);
		theText += "\n";
	}

	return theText;
}





//=============================================================================
//		NXMLEncoder::EncodeDocType : Encode a DOCTYPE node.
//-----------------------------------------------------------------------------
NString NXMLEncoder::EncodeDocType(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->GetType() == NXMLNodeType::DocType);



	// Get the state we need
	NString theName  = theNode->GetTextValue();
	NString systemID = theNode->GetDocTypeSystemID();
	NString publicID = theNode->GetDocTypePublicID();



	// Encode the node
	if (publicID.IsEmpty())
	{
		return NFormat("<!DOCTYPE {} SYSTEM \"{}\">", theName, systemID);
	}
	else
	{
		NN_REQUIRE(!systemID.IsEmpty());
		return NFormat("<!DOCTYPE {} PUBLIC \"{}\" \"{}\">", theName, publicID, systemID);
	}
}





//=============================================================================
//		NXMLEncoder::EncodeElement : Encode an element node.
//-----------------------------------------------------------------------------
NString NXMLEncoder::EncodeElement(const NSharedXMLNode& theNode, const NString& theIndent)
{


	// Validate our parameters
	NN_REQUIRE(theNode->GetType() == NXMLNodeType::Element);



	// Get the state we need
	NString textName       = theNode->GetTextValue();
	NString textAttributes = EncodeAttributes(theNode);



	// Encode the element
	NString theText;

	if (theNode->IsElementUnpaired())
	{
		theText = theIndent + NFormat("<{}{}/>", textName, textAttributes);
	}
	else
	{
		// Encode the open tag
		theText = theIndent + NFormat("<{}{}>", textName, textAttributes);


		// Encode the children
		const NVectorSharedXMLNode& theChildren      = theNode->GetChildren();
		bool                        hasChildElements = ContainsElements(theChildren);
		NString                     childIndent      = theIndent + kEncodeIndent;

		for (const auto& theChild : theChildren)
		{
			if (hasChildElements)
			{
				theText += "\n";

				if (!theChild->IsType(NXMLNodeType::Element))
				{
					theText += childIndent;
				}
			}

			theText += EncodeNode(theChild, childIndent);
		}

		if (hasChildElements)
		{
			theText += "\n" + theIndent;
		}


		// Encode the close tag
		theText += NFormat("</{}>", textName);
	}

	return theText;
}





//=============================================================================
//		NXMLEncoder::EncodeAttributes : Encode an element's attributes.
//-----------------------------------------------------------------------------
NString NXMLEncoder::EncodeAttributes(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->GetType() == NXMLNodeType::Element);



	// Get the state we need
	NDictionary   theAttributes = theNode->GetElementAttributes();
	NVectorString theKeys       = theAttributes.GetKeys();

	nstd::sort(theKeys);



	// Encode the attributes
	NString theText;

	for (const auto& theKey : theKeys)
	{
		theText += NFormat(" {}=\"{}\"", theKey, theAttributes.GetString(theKey));
	}

	return theText;
}





//=============================================================================
//		NXMLEncoder::EncodeComment : Encode a comment node.
//-----------------------------------------------------------------------------
NString NXMLEncoder::EncodeComment(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->GetType() == NXMLNodeType::Comment);



	// Encode the node
	return NFormat("<!--{}-->", theNode->GetTextValue());
}





//=============================================================================
//		NXMLEncoder::EncodeText : Encode a text node.
//-----------------------------------------------------------------------------
NString NXMLEncoder::EncodeText(const NSharedXMLNode& theNode)
{


	// Validate our parameters
	NN_REQUIRE(theNode->GetType() == NXMLNodeType::Text);



	// Encode the node
	//
	// Ampersands are encoded first to avoid double-encoding the others.
	NString theText = theNode->GetTextValue();

	theText.ReplaceAll("&", "&amp;");

	theText.ReplaceAll("\"", "&quot;");
	theText.ReplaceAll("'", "&apos;");
	theText.ReplaceAll("<", "&lt;");
	theText.ReplaceAll(">", "&gt;");

	return theText;
}





//=============================================================================
//		NXMLEncoder::EncodeCData : Encode a CDATA node.
//-----------------------------------------------------------------------------
NString NXMLEncoder::EncodeCData(const NSharedXMLNode& theNode)
{
	NString theText;



	// Validate our parameters
	NN_REQUIRE(theNode->GetType() == NXMLNodeType::CData);



	// Encode the node
	return NFormat("<![CDATA[{}]]>", theNode->GetTextValue());
}





//=============================================================================
//		NXMLEncoder::DecodeDocType : Decode a DOCTYPE.
//-----------------------------------------------------------------------------
bool NXMLEncoder::DecodeDocType(const NString& theName, const NXMLDocumentType& theInfo)
{


	// Decode the node
	NSharedXMLNode theParent = GetDecodeParent();
	NSharedXMLNode theNode   = std::make_shared<NXMLNode>(NXMLNodeType::DocType, theName);

	if (!theInfo.systemID.IsEmpty())
	{
		theNode->SetDocTypeSystemID(theInfo.systemID);
	}

	if (!theInfo.publicID.IsEmpty())
	{
		theNode->SetDocTypePublicID(theInfo.publicID);
	}

	theParent->AddChild(theNode);

	return true;
}





//=============================================================================
//		NXMLEncoder::DecodeElementStart : Decode an element start.
//-----------------------------------------------------------------------------
bool NXMLEncoder::DecodeElementStart(const NString& theName, const NDictionary& theAttributes)
{


	// Decode the node
	NSharedXMLNode theParent = GetDecodeParent();
	NSharedXMLNode theNode   = std::make_shared<NXMLNode>(NXMLNodeType::Element, theName);

	theNode->SetElementAttributes(theAttributes);



	// Update our state
	mDecodeElements.emplace_back(theNode);
	theParent->AddChild(theNode);

	return true;
}





//=============================================================================
//		NXMLEncoder::DecodeElementEnd : Decode an element end.
//-----------------------------------------------------------------------------
bool NXMLEncoder::DecodeElementEnd(const NString& theName)
{


	// Validate our parameters and state
	NN_REQUIRE(!mDecodeElements.empty());
	NN_REQUIRE(mDecodeElements.back()->GetTextValue() == theName);

	NN_UNUSED(theName);



	// Update our state
	mDecodeElements.pop_back();

	return true;
}





//=============================================================================
//		NXMLEncoder::DecodeText : Decode text.
//-----------------------------------------------------------------------------
bool NXMLEncoder::DecodeText(const NString& theValue, bool isCData)
{


	// Decode the node
	NXMLNodeType   theType   = isCData ? NXMLNodeType::CData : NXMLNodeType::Text;
	NSharedXMLNode theParent = GetDecodeParent();
	NSharedXMLNode theNode   = std::make_shared<NXMLNode>(theType, theValue);

	theParent->AddChild(theNode);

	return true;
}





//=============================================================================
//		NXMLEncoder::DecodeComment : Decode a comment.
//-----------------------------------------------------------------------------
bool NXMLEncoder::DecodeComment(const NString& theValue)
{


	// Decode the node
	NSharedXMLNode theParent = GetDecodeParent();
	NSharedXMLNode theNode   = std::make_shared<NXMLNode>(NXMLNodeType::Comment, theValue);

	theParent->AddChild(theNode);

	return true;
}





//=============================================================================
//		NXMLEncoder::ContainsElements : Does a node list contain elements?
//-----------------------------------------------------------------------------
bool NXMLEncoder::ContainsElements(const NVectorSharedXMLNode& theNodes)
{


	// Check the nodes
	for (const auto& theNode : theNodes)
	{
		if (theNode->IsType(NXMLNodeType::Element))
		{
			return true;
		}
	}

	return false;
}





//=============================================================================
//		NXMLEncoder::GetDecodeParent : Get the parent element.
//-----------------------------------------------------------------------------
NSharedXMLNode NXMLEncoder::GetDecodeParent()
{


	// Get the parent
	if (mDecodeElements.empty())
	{
		return mDecodeRoot;
	}
	else
	{
		return mDecodeElements.back();
	}
}
