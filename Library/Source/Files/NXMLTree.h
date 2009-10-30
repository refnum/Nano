/*	NAME:
		NXMLTree.h

	DESCRIPTION:
		XML tree.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NXMLTREE_HDR
#define NXMLTREE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NCFObject.h"
#include "NFile.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const CFXMLNodeTypeCode kCFXMLNodeTypeInvalid			= (CFXMLNodeTypeCode) 0;





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NXMLTree;

typedef std::vector<NXMLTree>									NXMLTreeList;
typedef NXMLTreeList::iterator									NXMLTreeListIterator;
typedef NXMLTreeList::const_iterator							NXMLTreeListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NXMLTree : public NCFObject {
public:
										NXMLTree(CFXMLTreeRef cfTree=NULL);
	virtual								~NXMLTree(void);


	// Load/save the tree
	OSStatus							Load(const NFile   &theFile);
	OSStatus							Load(const NData   &theData);

	OSStatus							Save(const NFile   &theFile) const;
	OSStatus							Save(      NData   &theData) const;


	// Check the node state
	bool								IsType(CFXMLNodeTypeCode theType)    const;
	bool								IsElement(const NString &theName="") const;


	// Get the type
	CFXMLNodeTypeCode					GetType(void) const;


	// Get the data
	//
	// The data for a node is type-specific; for elements it is the name
	// of the node, and for comment or text nodes it is the text value.
	NString								GetData(void) const;


	// Get the number of children
	UInt32								GetChildCount(void) const;


	// Get a child
	//
	// Illegal or missing children can be specified, which will return a NULL tree.
	NXMLTree							GetChild(      UInt32         theIndex) const;
	NXMLTree							GetChild(      const NString &theName)  const;
	NXMLTree							GetChildAtPath(const NString &thePath)  const;


	// Get an attribute
	//
	// Only valid for element nodes.
	NString								GetElementAttribute(const NString &theName) const;


	// Get the value
	//
	// Only valid for element nodes.
	NString								GetElementValue(void) const;


private:
	CFXMLNodeRef						GetNode(void) const;
};





#endif // NXMLTREE_HDR
