/*	NAME:
		NXMLNode.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NXMLNode.h"

// Nano
#include "NStdAlgorithm.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr const char* kNAttributeDocTypeSystemID     = "system";
static constexpr const char* kNAttributeDocTypePublicID     = "public";





//=============================================================================
//		NXMLNode::NXMLNode : Constructor.
//-----------------------------------------------------------------------------
NXMLNode::NXMLNode(NXMLNodeType theType, const NString& theValue)
	: mParent()
	, mChildren()
	, mType(theType)
	, mValue(theValue)
	, mAttributes()
	, mElementUnpaired(false)
{
}





//=============================================================================
//		NXMLNode::~NXMLNode : Destructor.
//-----------------------------------------------------------------------------
NXMLNode::~NXMLNode()
{


	// Validate our state
	NN_REQUIRE(mParent.expired());

	RemoveChildren();
}





//=============================================================================
//		NXMLNode::IsType : Is a node of a type?
//-----------------------------------------------------------------------------
bool NXMLNode::IsType(NXMLNodeType theType) const
{


	// Check the type
	return GetType() == theType;
}





//=============================================================================
//		NXMLNode::IsElement : Is the node a named element?
//-----------------------------------------------------------------------------
bool NXMLNode::IsElement(const NString& theName) const
{


	// Check the node
	return IsType(NXMLNodeType::Element) && GetTextValue() == theName;
}





//=============================================================================
//		NXMLNode::GetParent : Get the parent.
//-----------------------------------------------------------------------------
NSharedPtrXMLNode NXMLNode::GetParent() const
{


	// Get the parent
	return mParent.lock();
}





//=============================================================================
//		NXMLNode::HasChildren : Does a node have any children?
//-----------------------------------------------------------------------------
bool NXMLNode::HasChildren() const
{


	// Check our state
	return !mChildren.empty();
}





//=============================================================================
//		NXMLNode::AddChild : Add a child.
//-----------------------------------------------------------------------------
void NXMLNode::AddChild(const NSharedPtrXMLNode& theChild, size_t insertBefore)
{


	// Validate our parameters
	NN_REQUIRE(theChild->GetParent() == nullptr);
	NN_REQUIRE(!nstd::contains(mChildren, theChild));



	// Add the child
	theChild->mParent = shared_from_this();

	if (insertBefore == kNNotFound)
	{
		mChildren.emplace_back(theChild);
	}
	else
	{
		auto theIter = mChildren.begin();
		theIter += std::ptrdiff_t(insertBefore);

		mChildren.insert(theIter, theChild);
	}
}





//=============================================================================
//		NXMLNode::RemoveChild : Remove a child.
//-----------------------------------------------------------------------------
void NXMLNode::RemoveChild(const NSharedPtrXMLNode& theChild)
{


	// Validate our parameters
	NN_REQUIRE(theChild->GetParent().get() == this || theChild->GetParent().get() == nullptr);
	NN_REQUIRE(nstd::contains(mChildren, theChild));



	// Remove the child
	theChild->mParent.reset();

	nstd::erase(mChildren, theChild);
}





//=============================================================================
//		NXMLNode::RemoveChildren : Remove the children.
//-----------------------------------------------------------------------------
void NXMLNode::RemoveChildren()
{


	// Remove the children
	while (!mChildren.empty())
	{
		RemoveChild(mChildren.back());
	}
}





//=============================================================================
//		NXMLNode::GetChild : Get a child.
//-----------------------------------------------------------------------------
NSharedPtrXMLNode NXMLNode::GetChild(const NString& textValue) const
{


	// Get the child
	for (const auto& theChild : mChildren)
	{
		if (theChild->GetTextValue() == textValue)
		{
			return theChild;
		}
	}

	return {};
}





//=============================================================================
//		NXMLNode::GetChild : Get a child.
//-----------------------------------------------------------------------------
NSharedPtrXMLNode NXMLNode::GetChild(size_t theIndex) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < mChildren.size());



	// Get the child
	return mChildren[theIndex];
}





//=============================================================================
//		NXMLNode::GetChildAtPath : Get a child.
//-----------------------------------------------------------------------------
NSharedPtrXMLNode NXMLNode::GetChildAtPath(const NString& thePath) const
{


	// Get the child
	NXMLNode*         thisPtr   = const_cast<NXMLNode*>(this);
	NSharedPtrXMLNode theParent = thisPtr->shared_from_this();
	NSharedPtrXMLNode theChild{};

	for (const auto& theName : thePath.Split("/"))
	{
		theChild  = theParent->GetChild(theName);
		theParent = theChild;

		if (!theChild)
		{
			break;
		}
	}

	return theChild;
}





//=============================================================================
//		NXMLNode::GetChildren : Get the children.
//-----------------------------------------------------------------------------
const NVectorSharedPtrXMLNode& NXMLNode::GetChildren() const
{


	// Get the children
	return mChildren;
}





//=============================================================================
//		NXMLNode::GetType : Get the type.
//-----------------------------------------------------------------------------
NXMLNodeType NXMLNode::GetType() const
{


	// Get the type
	return mType;
}





//=============================================================================
//		NXMLNode::SetType : Set the type.
//-----------------------------------------------------------------------------
void NXMLNode::SetType(NXMLNodeType theType)
{


	// Reset our state
	if (!mAttributes.IsEmpty())
	{
		mAttributes.Clear();
	}

	mValue.Clear();



	// Set the type
	mType = theType;
}





//=============================================================================
//		NXMLNode::GetTextValue : Get the value.
//-----------------------------------------------------------------------------
NString NXMLNode::GetTextValue() const
{


	// Get the value
	return mValue;
}





//=============================================================================
//		NXMLNode::SetTextValue : Set the value.
//-----------------------------------------------------------------------------
void NXMLNode::SetTextValue(const NString& theValue)
{


	// Set the value
	mValue = theValue;
}





//=============================================================================
//		NXMLNode::GetDocTypeSystemID : Get a DocType system identifier.
//-----------------------------------------------------------------------------
NString NXMLNode::GetDocTypeSystemID() const
{


	// Get the value
	return mAttributes.GetString(kNAttributeDocTypeSystemID);
}





//=============================================================================
//		NXMLNode::GetDocTypePublicID : Get a DocType public identifier.
//-----------------------------------------------------------------------------
NString NXMLNode::GetDocTypePublicID() const
{


	// Get the value
	return mAttributes.GetString(kNAttributeDocTypePublicID);
}





//=============================================================================
//		NXMLNode::SetDocTypeSystemID : Set a DocType system identifier.
//-----------------------------------------------------------------------------
void NXMLNode::SetDocTypeSystemID(const NString& theID)
{


	// Set the value
	mAttributes[kNAttributeDocTypeSystemID] = theID;
}





//=============================================================================
//		NXMLNode::SetDocTypePublicID : Set a DocType public identifier.
//-----------------------------------------------------------------------------
void NXMLNode::SetDocTypePublicID(const NString& theID)
{


	// Set the value
	mAttributes[kNAttributeDocTypePublicID] = theID;
}





//=============================================================================
//		NXMLNode::IsElementUnpaired : Is an element unpaired?
//-----------------------------------------------------------------------------
bool NXMLNode::IsElementUnpaired() const
{


	// Get the state
	return mElementUnpaired;
}





//=============================================================================
//		NXMLNode::GetElementContents : Get an element's contents.
//-----------------------------------------------------------------------------
NString NXMLNode::GetElementContents() const
{


	// Validate our state
	NN_REQUIRE(IsType(NXMLNodeType::Element));



	// Collect the contents
	NString theContents;

	for (const auto& theChild : mChildren)
	{
		switch (theChild->GetType())
		{
			case NXMLNodeType::Document:
			case NXMLNodeType::DocType:
				NN_LOG_ERROR("Unexpected node {} inside an element!", theChild->GetType());
				break;

			case NXMLNodeType::Element:
				theContents += theChild->GetElementContents();
				break;

			case NXMLNodeType::Comment:
				// Ignore
				break;

			case NXMLNodeType::Text:
			case NXMLNodeType::CData:
				theContents += theChild->GetTextValue();
				break;
		}
	}

	return theContents;
}





//=============================================================================
//		NXMLNode::GetElementAttributes : Get an element's attributes.
//-----------------------------------------------------------------------------
NDictionary NXMLNode::GetElementAttributes() const
{


	// Validate our state
	NN_REQUIRE(IsType(NXMLNodeType::Element));



	// Get the attributes
	return mAttributes;
}





//=============================================================================
//		NXMLNode::GetElementAttribute : Get an element attribute.
//-----------------------------------------------------------------------------
NString NXMLNode::GetElementAttribute(const NString& theName) const
{


	// Validate our state
	NN_REQUIRE(IsType(NXMLNodeType::Element));



	// Get the attribute
	return mAttributes.GetString(theName);
}





//=============================================================================
//		NXMLNode::SetElementUnpaired : Set an element's unpaired state.
//-----------------------------------------------------------------------------
void NXMLNode::SetElementUnpaired(bool theValue)
{


	// Validate our state
	if (theValue)
	{
		NN_REQUIRE(mChildren.empty());
	}



	// Set the state
	mElementUnpaired = theValue;
}





//=============================================================================
//		NXMLNode::SetElementContents : Set an element's contents.
//-----------------------------------------------------------------------------
void NXMLNode::SetElementContents(const NString& theValue)
{


	// Validate our state
	NN_REQUIRE(IsType(NXMLNodeType::Element));



	// Get the state we need
	NSharedPtrXMLNode theNode = std::make_shared<NXMLNode>(NXMLNodeType::Text, theValue);



	// Set the contents
	RemoveChildren();
	AddChild(theNode);
}





//=============================================================================
//		NXMLNode::SetElementAttributes : Set an element's attributes.
//-----------------------------------------------------------------------------
void NXMLNode::SetElementAttributes(const NDictionary& theValue)
{


	// Validate our state
	NN_REQUIRE(IsType(NXMLNodeType::Element));



	// Set the attributes
	mAttributes = theValue;
}





//=============================================================================
//		NXMLNode::SetElementAttribute : Set an element attribute.
//-----------------------------------------------------------------------------
void NXMLNode::SetElementAttribute(const NString& theName, const NString& theValue)
{


	// Validate our state
	NN_REQUIRE(IsType(NXMLNodeType::Element));



	// Set the attribute
	mAttributes[theName] = theValue;
}
