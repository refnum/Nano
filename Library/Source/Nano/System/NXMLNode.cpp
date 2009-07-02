/*	NAME:
		NXMLNode.cpp

	DESCRIPTION:
		XML node.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NXMLNode.h"





//============================================================================
//		NXMLNode::NXMLNode : Constructor.
//----------------------------------------------------------------------------
NXMLNode::NXMLNode(XMLNodeType theType, const NString &theValue)
{


	// Initialize ourselves
	mParent = NULL;

	mType  = theType;
	mValue = theValue;
}





//============================================================================
//		NXMLNode::~NXMLNode : Destructor.
//----------------------------------------------------------------------------
NXMLNode::~NXMLNode(void)
{


	// Clean up
	if (mParent != NULL)
		mParent->RemoveChild(this, false);

	RemoveChildren();
}





//============================================================================
//		NXMLNode::IsType : Is a node of a type?
//----------------------------------------------------------------------------
bool NXMLNode::IsType(XMLNodeType theType) const
{


	// Check the type
	return(GetType() == theType);
}





//============================================================================
//		NXMLNode::HasChildren : Does a node have any children?
//----------------------------------------------------------------------------
bool NXMLNode::HasChildren(void) const
{


	// Check our state
	return(!mChildren.empty());
}





//============================================================================
//		NXMLNode::GetParent : Get the parent.
//----------------------------------------------------------------------------
NXMLNode *NXMLNode::GetParent(void) const
{


	// Get the parent
	return(mParent);
}





//============================================================================
//		NXMLNode::GetChildren : Get the children.
//----------------------------------------------------------------------------
const NXMLNodeList *NXMLNode::GetChildren(void) const
{


	// Get the children
	return(&mChildren);
}





//============================================================================
//		NXMLNode::AddChild : Add a child.
//----------------------------------------------------------------------------
void NXMLNode::AddChild(NXMLNode *theChild, NIndex insertBefore)
{	NXMLNodeListIterator		theIter;



	// Validate our parameters
	NN_ASSERT(theChild != NULL);
	NN_ASSERT(!contains(mChildren, theChild));



	// Add the child
	if (insertBefore == kNIndexNone)
		mChildren.push_back(theChild);
	else
		{
		theIter  = mChildren.begin();
		theIter += insertBefore;
		
		mChildren.insert(theIter, theChild);
		}
}





//============================================================================
//		NXMLNode::RemoveChild : Remove a child.
//----------------------------------------------------------------------------
void NXMLNode::RemoveChild(NXMLNode *theChild, bool destroyChild)
{


	// Validate our parameters
	NN_ASSERT(theChild != NULL);
	NN_ASSERT(contains(mChildren, theChild));



	// Remove the child
	theChild->mParent = NULL;
	vector_erase(mChildren, theChild);

	if (destroyChild)
		delete theChild;
}





//============================================================================
//		NXMLNode::RemoveChildren : Remove the children.
//----------------------------------------------------------------------------
void NXMLNode::RemoveChildren(void)
{


	// Remove the children
	while (!mChildren.empty())
		RemoveChild(mChildren.back());
}





//============================================================================
//		NXMLNode::GetType : Get the type.
//----------------------------------------------------------------------------
XMLNodeType NXMLNode::GetType(void) const
{


	// Get the type
	return(mType);
}





//============================================================================
//		NXMLNode::SetType : Set the type.
//----------------------------------------------------------------------------
void NXMLNode::SetType(XMLNodeType theType)
{


	// Reset our state
	if (mElementAttributes.IsNotEmpty())
		mElementAttributes.Clear();
	
	mValue.Clear();



	// Set the type
	mType = theType;
}





//============================================================================
//		NXMLNode::GetTextValue : Get the value.
//----------------------------------------------------------------------------
NString NXMLNode::GetTextValue(void) const
{


	// Get the value
	return(mValue);
}





//============================================================================
//		NXMLNode::SetTextValue : Set the value.
//----------------------------------------------------------------------------
void NXMLNode::SetTextValue(const NString &theValue)
{


	// Set the value
	mValue = theValue;
}





//============================================================================
//		NXMLNode::GetElementContents : Get the element contents.
//----------------------------------------------------------------------------
NString NXMLNode::GetElementContents(void) const
{	NXMLNode						*theChild;
	NString							theValue;
	NXMLNodeListConstIterator		theIter;



	// Validate our state
	NN_ASSERT(IsType(kXMLNodeElement));



	// Collect the contents
	for (theIter = mChildren.begin(); theIter != mChildren.end(); theIter++)
		{
		theChild = *theIter;
		
		switch (theChild->GetType()) {
			case kXMLNodeElement:
				theValue += theChild->GetElementContents();
				break;
			
			case kXMLNodeText:
			case kXMLNodeCDATA:
				theValue += theChild->GetTextValue();
				break;
			
			case kXMLNodeComment:
				break;
			
			default:
				NN_LOG("Unknown node type: %d", theChild->GetType());
				break;
			}
		}

	return(theValue);
}





//============================================================================
//		NXMLNode::GetElementAttributes : Get the element attributes.
//----------------------------------------------------------------------------
NDictionary NXMLNode::GetElementAttributes(void) const
{


	// Validate our state
	NN_ASSERT(IsType(kXMLNodeElement));



	// Get the attributes
	return(mElementAttributes);
}





//============================================================================
//		NXMLNode::GetElementAttribute : Get an element attribute.
//----------------------------------------------------------------------------
NString NXMLNode::GetElementAttribute(const NString &theName) const
{	NString		theValue;



	// Validate our state
	NN_ASSERT(IsType(kXMLNodeElement));



	// Get the attribute
	theValue = mElementAttributes.GetValueString(theName);
	
	return(theValue);
}





//============================================================================
//		NXMLNode::SetElementContents : Set the element contents.
//----------------------------------------------------------------------------
void NXMLNode::SetElementContents(const NString &theValue)
{


	// Validate our state
	NN_ASSERT(IsType(kXMLNodeElement));



	// Set the contents
	RemoveChildren();
	AddChild(new NXMLNode(kXMLNodeCDATA, theValue));
}





//============================================================================
//		NXMLNode::SetElementAttributes : Set the element attributes.
//----------------------------------------------------------------------------
void NXMLNode::SetElementAttributes(const NDictionary &theValue)
{


	// Validate our state
	NN_ASSERT(IsType(kXMLNodeElement));



	// Set the attributes
	mElementAttributes = theValue;
}





//============================================================================
//		NXMLNode::SetElementAttribute : Set an element attribute.
//----------------------------------------------------------------------------
void NXMLNode::SetElementAttribute(const NString &theName, const NString &theValue)
{


	// Validate our state
	NN_ASSERT(IsType(kXMLNodeElement));



	// Set the attribute
	mElementAttributes.SetValue(theName, theValue);
}


