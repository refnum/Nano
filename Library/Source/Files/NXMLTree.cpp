/*	NAME:
		NXMLTree.cpp

	DESCRIPTION:
		XML tree.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NTextUtilities.h"
#include "NFileUtilities.h"
#include "NXMLTree.h"





//============================================================================
//		NXMLTree::NXMLTree : Constructor.
//----------------------------------------------------------------------------
NXMLTree::NXMLTree(CFXMLTreeRef cfTree)
{


	// Initialize ourselves
	Set(cfTree, false);

	if (cfTree != NULL)
		NN_ASSERT(IsValid());
}





//============================================================================
//		NXMLTree::~NXMLTree : Destructor.
//----------------------------------------------------------------------------
NXMLTree::~NXMLTree(void)
{
}





//============================================================================
//		NXMLTree::Load : Load the tree.
//----------------------------------------------------------------------------
OSStatus NXMLTree::Load(const NFile &theFile)
{


	// Load the tree
	return(Load(NFileUtilities::GetFileData(theFile)));
}





//============================================================================
//		NXMLTree::Load : Load the tree.
//----------------------------------------------------------------------------
OSStatus NXMLTree::Load(const NData &theData)
{


	// Load the tree
	if (!Set(CFXMLTreeCreateFromData(NULL, theData, NULL, kCFXMLParserSkipWhitespace, kCFXMLNodeCurrentVersion)))
		return(badFormat);

	return(noErr);
}





//============================================================================
//		NXMLTree::Save : Save the tree.
//----------------------------------------------------------------------------
OSStatus NXMLTree::Save(const NFile &theFile) const
{	NData		theData;
	OSStatus	theErr;



	// Save the tree
	theErr = Save(theData);

	if (theErr == noErr)
		theErr = NFileUtilities::SetFileData(theFile, theData);
	
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NXMLTree::Save : Save the tree.
//----------------------------------------------------------------------------
OSStatus NXMLTree::Save(NData &theData) const
{


	// Validate our state
	NN_ASSERT(IsValid());
	


	// Save the tree
	if (!theData.Set(CFXMLTreeCreateXMLData(NULL, *this)))
		return(memFullErr);
	
	return(noErr);
}





//============================================================================
//		NXMLTree::IsType : Is the node of a given type?
//----------------------------------------------------------------------------
bool NXMLTree::IsType(CFXMLNodeTypeCode theType) const
{


	// Check our state
	if (!IsValid())
		return(false);



	// Check the type
	return(GetType() == theType);
}





//============================================================================
//		NXMLTree::IsElement : Is the node a (named) element?
//----------------------------------------------------------------------------
bool NXMLTree::IsElement(const NString &theName) const
{	bool	isElement;



	// Check our state
	if (!IsValid())
		return(false);



	// Check the value
	isElement = IsType(kCFXMLNodeTypeElement);
	
	if (isElement && theName.IsNotEmpty())
		isElement = (GetData() == theName);
	
	return(isElement);
}





//============================================================================
//		NXMLTree::GetType : Get the type.
//----------------------------------------------------------------------------
CFXMLNodeTypeCode NXMLTree::GetType(void) const
{


	// Check our state
	if (!IsValid())
		return(kCFXMLNodeTypeInvalid);



	// Get the type
	return(CFXMLNodeGetTypeCode(GetNode()));
}





//============================================================================
//		NXMLTree::GetData : Get the data.
//----------------------------------------------------------------------------
NString NXMLTree::GetData(void) const
{	NString		theData;



	// Check our state
	if (!IsValid())
		return(theData);



	// Get the data
	if (!theData.Set(CFXMLNodeGetString(GetNode()), false))
		NN_ASSERT(theData.IsNotEmpty());

	return(theData);
}





//============================================================================
//		NXMLTree::GetChildCount : Get the number of children.
//----------------------------------------------------------------------------
UInt32 NXMLTree::GetChildCount(void) const
{


	// Check our state
	if (!IsValid())
		return(0);



	// Get the child count
	return(CFTreeGetChildCount(*this));
}





//============================================================================
//		NXMLTree::GetChild : Get a child.
//----------------------------------------------------------------------------
NXMLTree NXMLTree::GetChild(UInt32 theIndex) const
{	NXMLTree	theChild;



	// Check our state
	if (!IsValid() || theIndex >= GetChildCount())
		return(theChild);



	// Get the child
	if (!theChild.Set(CFTreeGetChildAtIndex(*this, theIndex), false))
		NN_ASSERT(theChild.IsValid());
	
	return(theChild);
}





//============================================================================
//		NXMLTree::GetChild : Get a child.
//----------------------------------------------------------------------------
NXMLTree NXMLTree::GetChild(const NString &theName) const
{	UInt32		n, numChildren;
	NXMLTree	theChild;



	// Check our state
	if (!IsValid() || theName.IsEmpty())
		return(theChild);



	// Find the child
	numChildren = GetChildCount();
	
	for (n = 0; n < numChildren; n++)
		{
		theChild = GetChild(n);

		if (theChild.GetData() == theName)
			return(theChild);
		}



	// Reset the child
	theChild.Set(NULL);

	return(theChild);
}





//============================================================================
//		NXMLTree::GetChildAtPath : Get a child with a path.
//----------------------------------------------------------------------------
NXMLTree NXMLTree::GetChildAtPath(const NString &thePath) const
{	NStringList		theNames;
	NXMLTree		theChild;
	NString			theName;



	// Check our state
	if (!IsValid() || thePath.IsEmpty())
		return(theChild);



	// Get the child
	theNames = thePath.Split("/", true);
	theChild.Set(*this, false);
	
	while (!theNames.empty() && theChild.IsValid())
		{
		theName  = extract_front(theNames);
		theChild = theChild.GetChild(theName);
		}
	
	return(theChild);
}





//============================================================================
//		NXMLTree::GetElementAttribute : Get an attribute.
//----------------------------------------------------------------------------
NString NXMLTree::GetElementAttribute(const NString &theName) const
{	NString						theValue;
	const CFXMLElementInfo		*theInfo;
	CFStringRef					cfValue;



	// Validate our state
	NN_ASSERT(!IsValid() || GetType() == kCFXMLNodeTypeElement);



	// Check our state
	if (!IsValid())
		return(theValue);




	// Get the element info
	theInfo = (const CFXMLElementInfo *) CFXMLNodeGetInfoPtr(GetNode());
	if (theInfo == NULL)
		return(theValue);



	// Get the attribute
	//
	// Attribute values may contain encoded characters, which we decode.
	if (theInfo->attributes != NULL && CFDictionaryGetValueIfPresent(theInfo->attributes, theName, (const void **) &cfValue))
		theValue = NTextUtilities::DecodeEntities(cfValue);
	
	return(theValue);
}





//============================================================================
//		NXMLTree::GetElementValue : Get the value.
//----------------------------------------------------------------------------
NString NXMLTree::GetElementValue(void) const
{	NString				theValue, theEntity;
	UInt32				n, numChildren;
	CFXMLNodeTypeCode	childType;
	NXMLTree			theChild;



	// Validate our state
	NN_ASSERT(!IsValid() || GetType() == kCFXMLNodeTypeElement);



	// Check our state
	if (!IsValid())
		return(theValue);
		


	// Process the children
	numChildren = GetChildCount();
	
	for (n = 0; n < numChildren; n++)
		{
		theChild = GetChild(n);
		childType = theChild.GetType();
		
		switch (childType) {
			case kCFXMLNodeTypeText:
			case kCFXMLNodeTypeCDATASection:
			theValue += theChild.GetData();
				break;
			
			case kCFXMLNodeTypeEntity:
			case kCFXMLNodeTypeEntityReference:
				theEntity.Format("&%@;", theChild.GetData());
				theValue += NTextUtilities::DecodeEntities(theEntity);
				break;
			
			default:
				break;
			}
		}

	return(theValue);
}





//============================================================================
//		NXMLTree::GetNode : Get the node.
//----------------------------------------------------------------------------
#pragma mark -
CFXMLNodeRef NXMLTree::GetNode(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the node
	return(CFXMLTreeGetNode(*this));
}




