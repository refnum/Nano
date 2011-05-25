/*	NAME:
		NXMLNode.cpp

	DESCRIPTION:
		XML node.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
//		Internal constants
//----------------------------------------------------------------------------
static const NString kAttributeDocTypeSystemID						= "system";
static const NString kAttributeDocTypePublicID						= "public";





//============================================================================
//		NXMLNode::NXMLNode : Constructor.
//----------------------------------------------------------------------------
NXMLNode::NXMLNode(NXMLNodeType theType, const NString &theValue)
{


	// Initialize ourselves
	mParent = NULL;

	mType  = theType;
	mValue = theValue;
	
	mElementUnpaired = false;
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
bool NXMLNode::IsType(NXMLNodeType theType) const
{


	// Check the type
	return(GetType() == theType);
}





//============================================================================
//		NXMLNode::IsElement : Is the node a named element?
//----------------------------------------------------------------------------
bool NXMLNode::IsElement(const NString &theName) const
{


	// Check the node
	return(IsType(kNXMLNodeElement) && GetTextValue() == theName);
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
//		NXMLNode::HasChildren : Does a node have any children?
//----------------------------------------------------------------------------
bool NXMLNode::HasChildren(void) const
{


	// Check our state
	return(!mChildren.empty());
}





//============================================================================
//		NXMLNode::GetChild : Get a child.
//----------------------------------------------------------------------------
NXMLNode *NXMLNode::GetChild(const NString &textValue) const
{	NXMLNode						*theChild;
	NXMLNodeListConstIterator		theIter;



	// Get the child
	for (theIter = mChildren.begin(); theIter != mChildren.end(); theIter++)
		{
		theChild = *theIter;
		if (theChild->GetTextValue() == textValue)
			return(theChild);
		}
	
	return(NULL);
}





//============================================================================
//		NXMLNode::GetChild : Get a child.
//----------------------------------------------------------------------------
NXMLNode *NXMLNode::GetChild(NIndex theIndex) const
{	NXMLNode	*theChild;



	// Get the child
	theChild = NULL;
	
	if (theIndex < (NIndex) mChildren.size())
		theChild = mChildren[theIndex];
	
	return(theChild);
}





//============================================================================
//		NXMLNode::GetChildAtPath : Get a child.
//----------------------------------------------------------------------------
NXMLNode *NXMLNode::GetChildAtPath(const NString &thePath) const
{	const NXMLNode					*theParent;
	NXMLNode						*theChild;
	NStringList						theValues;
	NStringListConstIterator		theIter;



	// Get the state we need
	theValues = thePath.Split("/");
	theParent = this;
	theChild  = NULL;



	// Get the child
	for (theIter = theValues.begin(); theIter != theValues.end(); theIter++)
		{
		theChild  = theParent->GetChild(*theIter);
		theParent = theChild;

		if (theChild == NULL)
			break;
		}
	
	return(theChild);
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
	NN_ASSERT(theChild->GetParent() == NULL);
	NN_ASSERT(!contains(mChildren, theChild));



	// Add the child
	theChild->mParent = this;
	
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
	NN_ASSERT(theChild->GetParent() == this);
	NN_ASSERT(contains(mChildren, theChild));



	// Remove the child
	theChild->mParent = NULL;
	erase(mChildren, theChild);

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
NXMLNodeType NXMLNode::GetType(void) const
{


	// Get the type
	return(mType);
}





//============================================================================
//		NXMLNode::SetType : Set the type.
//----------------------------------------------------------------------------
void NXMLNode::SetType(NXMLNodeType theType)
{


	// Reset our state
	if (!mAttributes.IsEmpty())
		mAttributes.Clear();
	
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
//		NXMLNode::GetDocTypeSystemID : Get a DocType system identifier.
//----------------------------------------------------------------------------
NString NXMLNode::GetDocTypeSystemID(void) const
{


	// Get the value
	return(mAttributes.GetValueString(kAttributeDocTypeSystemID));
}





//============================================================================
//		NXMLNode::GetDocTypePublicID : Get a DocType public identifier.
//----------------------------------------------------------------------------
NString NXMLNode::GetDocTypePublicID(void) const
{


	// Get the value
	return(mAttributes.GetValueString(kAttributeDocTypePublicID));
}





//============================================================================
//		NXMLNode::SetDocTypeSystemID : Set a DocType system identifier.
//----------------------------------------------------------------------------
void NXMLNode::SetDocTypeSystemID(const NString &theID)
{


	// Set the value
	mAttributes.SetValue(kAttributeDocTypeSystemID, theID);
}





//============================================================================
//		NXMLNode::SetDocTypePublicID : Set a DocType public identifier.
//----------------------------------------------------------------------------
void NXMLNode::SetDocTypePublicID(const NString &theID)
{


	// Set the value
	mAttributes.SetValue(kAttributeDocTypePublicID, theID);
}





//============================================================================
//		NXMLNode::IsElementUnpaired : Is an element unpaired?
//----------------------------------------------------------------------------
bool NXMLNode::IsElementUnpaired(void) const
{


	// Get the state
	return(mElementUnpaired);
}





//============================================================================
//		NXMLNode::GetElementContents : Get an element's contents.
//----------------------------------------------------------------------------
NString NXMLNode::GetElementContents(void) const
{	NXMLNode						*theChild;
	NString							theValue;
	NXMLNodeListConstIterator		theIter;



	// Validate our state
	NN_ASSERT(IsType(kNXMLNodeElement));



	// Collect the contents
	for (theIter = mChildren.begin(); theIter != mChildren.end(); theIter++)
		{
		theChild = *theIter;
		
		switch (theChild->GetType()) {
			case kNXMLNodeDocument:
			case kNXMLNodeDocType:
				NN_LOG("Unexpected node %d inside an element!", theChild->GetType());
				break;

			case kNXMLNodeElement:
				theValue += theChild->GetElementContents();
				break;
			
			case kNXMLNodeComment:
				// Ignore
				break;

			case kNXMLNodeText:
			case kNXMLNodeCData:
				theValue += theChild->GetTextValue();
				break;
			
			default:
				NN_LOG("Unknown node type: %d", theChild->GetType());
				break;
			}
		}

	return(theValue);
}





//============================================================================
//		NXMLNode::GetElementAttributes : Get an element's attributes.
//----------------------------------------------------------------------------
NDictionary NXMLNode::GetElementAttributes(void) const
{


	// Validate our state
	NN_ASSERT(IsType(kNXMLNodeElement));



	// Get the attributes
	return(mAttributes);
}





//============================================================================
//		NXMLNode::GetElementAttribute : Get an element attribute.
//----------------------------------------------------------------------------
NString NXMLNode::GetElementAttribute(const NString &theName) const
{	NString		theValue;



	// Validate our state
	NN_ASSERT(IsType(kNXMLNodeElement));



	// Get the attribute
	theValue = mAttributes.GetValueString(theName);
	
	return(theValue);
}





//============================================================================
//		NXMLNode::SetElementUnpaired : Set an element's unpaired state.
//----------------------------------------------------------------------------
void NXMLNode::SetElementUnpaired(bool theValue)
{


	// Validate our state
	if (theValue)
		NN_ASSERT(mChildren.empty());



	// Set the state
	mElementUnpaired = theValue;
}





//============================================================================
//		NXMLNode::SetElementContents : Set an element's contents.
//----------------------------------------------------------------------------
void NXMLNode::SetElementContents(const NString &theValue)
{	NXMLNode	*theNode;



	// Validate our state
	NN_ASSERT(IsType(kNXMLNodeElement));



	// Get the state we need
	theNode = new NXMLNode(kNXMLNodeText, theValue);



	// Set the contents
	RemoveChildren();
	AddChild(theNode);
}





//============================================================================
//		NXMLNode::SetElementAttributes : Set an element's attributes.
//----------------------------------------------------------------------------
void NXMLNode::SetElementAttributes(const NDictionary &theValue)
{


	// Validate our state
	NN_ASSERT(IsType(kNXMLNodeElement));



	// Set the attributes
	mAttributes = theValue;
}





//============================================================================
//		NXMLNode::SetElementAttribute : Set an element attribute.
//----------------------------------------------------------------------------
void NXMLNode::SetElementAttribute(const NString &theName, const NString &theValue)
{


	// Validate our state
	NN_ASSERT(IsType(kNXMLNodeElement));



	// Set the attribute
	mAttributes.SetValue(theName, theValue);
}
