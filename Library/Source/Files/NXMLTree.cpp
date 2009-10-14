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
#include "NFileUtilities.h"
#include "NXMLTree.h"





//============================================================================
//		NXMLTree::NXMLTree : Constructor.
//----------------------------------------------------------------------------
NXMLTree::NXMLTree(void)
{
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
	return(GetType() == theType);
}





//============================================================================
//		NXMLTree::IsElement : Is the node a (named) element?
//----------------------------------------------------------------------------
bool NXMLTree::IsElement(const NString &theName) const
{	bool	nameMatch;



	// Check our state
	nameMatch = theName.IsEmpty() || (GetData() == theName);
	
	return(IsType(kCFXMLNodeTypeElement) && nameMatch);
}





//============================================================================
//		NXMLTree::GetType : Get the type.
//----------------------------------------------------------------------------
CFXMLNodeTypeCode NXMLTree::GetType(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the type
	return(CFXMLNodeGetTypeCode(GetNode()));
}





//============================================================================
//		NXMLTree::GetData : Get the data.
//----------------------------------------------------------------------------
NString NXMLTree::GetData(void) const
{	NString		theData;



	// Validate our state
	NN_ASSERT(IsValid());



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


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the child count
	return(CFTreeGetChildCount(*this));
}





//============================================================================
//		NXMLTree::GetChild : Get a child.
//----------------------------------------------------------------------------
NXMLTree NXMLTree::GetChild(UInt32 theIndex) const
{	NXMLTree	theChild;



	// Check our parameters
	if (theIndex >= GetChildCount() || !IsValid())
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



	// Check our parameters
	if (theName.IsEmpty() || !IsValid())
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
//		NXMLTree::GetElementAttribute : Get an attribute.
//----------------------------------------------------------------------------
NString NXMLTree::GetElementAttribute(const NString &theName) const
{	NString						theValue;
	const CFXMLElementInfo		*theInfo;
	CFStringRef					cfValue;



	// Validate our parameters and state
	NN_ASSERT(theName.IsNotEmpty());
	NN_ASSERT(IsValid());
	NN_ASSERT(GetType() == kCFXMLNodeTypeElement);



	// Get the element info
	theInfo = (const CFXMLElementInfo *) CFXMLNodeGetInfoPtr(GetNode());
	if (theInfo == NULL)
		return(theValue);



	// Get the attribute
	//
	// Attribute values may contain encoded characters, which we decode.
	if (CFDictionaryGetValueIfPresent(theInfo->attributes, theName, (const void **) &cfValue))
		{
		theValue.Set(cfValue, false);
			
		theValue.ReplaceAll("&amp;",  "&");
		theValue.ReplaceAll("&apos;", "'");
		theValue.ReplaceAll("&gt;",   ">");
		theValue.ReplaceAll("&lt;",   "<");
		theValue.ReplaceAll("&quot;", "\"");
		}
	
	return(theValue);
}





//============================================================================
//		NXMLTree::GetElementContents : Get the contents.
//----------------------------------------------------------------------------
NString NXMLTree::GetElementContents(void) const
{	UInt32		n, numChildren;
	NString		theValue;
	NXMLTree	theChild;



	// Validate our state
	NN_ASSERT(IsValid());
	NN_ASSERT(GetType() == kCFXMLNodeTypeElement);



	// Process the children
	numChildren = GetChildCount();
	
	for (n = 0; n < numChildren; n++)
		{
		theChild = GetChild(n);

		if (theChild.GetType() == kCFXMLNodeTypeText)
			theValue += theChild.GetData();
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




