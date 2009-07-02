/*	NAME:
		NXMLNode.h

	DESCRIPTION:
		XML node.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NXMLNODE_HDR
#define NXMLNODE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NDictionary.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
typedef enum {
	kXMLNodeElement,
	kXMLNodeText,
	kXMLNodeComment,
	kXMLNodeCDATA
} XMLNodeType;





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NXMLNode;

typedef std::vector<NXMLNode*>										NXMLNodeList;
typedef NXMLNodeList::iterator										NXMLNodeListIterator;
typedef NXMLNodeList::const_iterator								NXMLNodeListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NXMLNode {
public:
										 NXMLNode(XMLNodeType theType, const NString &theValue);
	virtual								~NXMLNode(void);


	// Query the node state
	bool								IsType(XMLNodeType theType) const;
	bool								HasChildren(void)           const;


	// Get the parent
	NXMLNode							*GetParent(void) const;


	// Get the children
	const NXMLNodeList					*GetChildren(void) const;


	// Add/remove children
	void								AddChild(   NXMLNode *theChild, NIndex insertBefore=kNIndexNone);
	void								RemoveChild(NXMLNode *theChild, bool   destroyChild=true);

	void								RemoveChildren(void);


	// Get/set the type
	XMLNodeType							GetType(void) const;
	void								SetType(XMLNodeType theType);


	// Get/set the value
	//
	// The text value is the text associated with the node. For element nodes,
	// this is the name of the element.
	NString								GetTextValue(void) const;
	void								SetTextValue(const NString &theValue);


	// Get/set element properties
	//
	// For element nodes, the contents are the concatenation of any text or CDATA nodes
	// under the node. Assigning new contents to an element will replace any existing
	// children with a single CDATA node.
	NString								GetElementContents( void)                   const;
	NDictionary							GetElementAttributes(void)                  const;
	NString								GetElementAttribute(const NString &theName) const;
	
	void								SetElementContents(  const NString     &theValue);
	void								SetElementAttributes(const NDictionary &theValue);
	void								SetElementAttribute( const NString     &theName, const NString &theValue);


private:
	NXMLNode							*mParent;
	NXMLNodeList						mChildren;

	XMLNodeType							mType;
	NString								mValue;
	NDictionary							mElementAttributes;
};



#endif // NXMLNODE_HDR
