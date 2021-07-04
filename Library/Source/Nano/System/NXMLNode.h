/*	NAME:
		NXMLNode.h

	DESCRIPTION:
		XML node.

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
#ifndef NXMLNODE_H
#define NXMLNODE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NDictionary.h"
#include "NString.h"
#include "NanoConstants.h"

// System
#include <memory>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Node types
enum class NXMLNodeType
{
	Document,
	DocType,
	Element,
	Comment,
	Text,
	CData
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NXMLNode;

// Node pointer
using NSharedPtrXMLNode       = std::shared_ptr<NXMLNode>;
using NVectorSharedPtrXMLNode = std::vector<NSharedPtrXMLNode>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NXMLNode : public std::enable_shared_from_this<NXMLNode>
{
public:
										NXMLNode(NXMLNodeType theType, const NString& theValue);
	virtual                            ~NXMLNode();

										NXMLNode( const NXMLNode& otherNode) = delete;
	NXMLNode&                           operator=(const NXMLNode& otherNode) = delete;

										NXMLNode( NXMLNode&& otherNode) = delete;
	NXMLNode&                           operator=(NXMLNode&& otherNode) = delete;


	// Query the node state
	bool                                IsType(NXMLNodeType theType)      const;
	bool                                IsElement(const NString& theName) const;


	// Get the parent
	NSharedPtrXMLNode                   GetParent() const;


	// Does the node have any children?
	bool                                HasChildren() const;


	// Add/remove a child
	void                                AddChild(   const NSharedPtrXMLNode& theChild, size_t insertBefore = kNNotFound);
	void                                RemoveChild(const NSharedPtrXMLNode& theChild);


	// Remove the children
	void                                RemoveChildren();


	// Get the children
	NSharedPtrXMLNode                   GetChild(const NString& textValue) const;
	NSharedPtrXMLNode                   GetChild(size_t theIndex)          const;

	NSharedPtrXMLNode                   GetChildAtPath(const NString& thePath) const;
	const NVectorSharedPtrXMLNode&      GetChildren()  const;


	// Get/set the type
	NXMLNodeType                        GetType() const;
	void                                SetType(NXMLNodeType theType);


	// Get/set the value
	//
	// The text value is the text associated with the node (for document type or
	// element nodes, the text value is the name of the node).
	//
	// Nodes always store raw text; encoders must encode entities when fetching
	// the text for encoding, and decoders should decode entities before assigning
	// the text after decoding.
	NString                             GetTextValue() const;
	void                                SetTextValue(  const NString& theValue);


	// Get/set document type properties
	NString                             GetDocTypeSystemID() const;
	NString                             GetDocTypePublicID() const;

	void                                SetDocTypeSystemID(const NString& theID);
	void                                SetDocTypePublicID(const NString& theID);


	// Get/set element properties
	//
	// The contents of an element node are the concatenation of any text or CData nodes
	// under the node. Assigning new contents to an element will replace any existing
	// children with a single text node.
	//
	// As with [GS]etTextValue, nodes always store raw text and encoders/decoders are
	// responsible for performing entity substitution.
	bool                                IsElementUnpaired()    const;
	NString                             GetElementContents()   const;
	NDictionary                         GetElementAttributes() const;
	NString                             GetElementAttribute(   const NString& theName) const;

	void                                SetElementUnpaired(bool                 theValue);
	void                                SetElementContents(  const NString&     theValue);
	void                                SetElementAttributes(const NDictionary& theValue);
	void                                SetElementAttribute( const NString& theName, const NString& theValue);


private:
	std::weak_ptr<NXMLNode>             mParent;
	NVectorSharedPtrXMLNode             mChildren;

	NXMLNodeType                        mType;
	NString                             mValue;
	NDictionary                         mAttributes;

	bool                                mElementUnpaired;
};



#endif // NXMLNODE_H
