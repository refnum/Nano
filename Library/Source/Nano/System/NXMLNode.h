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
	kXMLNodeDocument,
	kXMLNodeDocType,
	kXMLNodeElement,
	kXMLNodeComment,
	kXMLNodeText,
	kXMLNodeCData
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
	bool								IsType(XMLNodeType       theType) const;
	bool								IsElement(const NString &theName) const;


	// Get the parent
	NXMLNode							*GetParent(void) const;


	// Does the node have any children?
	bool								HasChildren(void) const;


	// Get the children
	NXMLNode							*GetChild(NIndex theIndex) const;
	const NXMLNodeList					*GetChildren(void)         const;


	// Add/remove children
	void								AddChild(   NXMLNode *theChild, NIndex insertBefore=kNIndexNone);
	void								RemoveChild(NXMLNode *theChild, bool   destroyChild=true);

	void								RemoveChildren(void);


	// Get/set the type
	XMLNodeType							GetType(void) const;
	void								SetType(XMLNodeType theType);


	// Get/set the value
	//
	// The text value is the text associated with the node (for document
	// type or element nodes, the text value is the name of the node).
	NString								GetTextValue(void) const;
	void								SetTextValue(const NString &theValue);


	// Get/set document type properties
	NString								GetDocTypeSystemID(void) const;
	NString								GetDocTypePublicID(void) const;

	void								SetDocTypeSystemID(const NString &theID);
	void								SetDocTypePublicID(const NString &theID);


	// Get/set element properties
	//
	// The contents of an element node are the concatenation of any text or CData nodes
	// under the node. Assigning new contents to an element will replace any existing
	// children with a single text node.
	bool								 IsElementUnpaired(  void) const;
	NString								GetElementContents(  void) const;
	NDictionary							GetElementAttributes(void) const;
	NString								GetElementAttribute(const NString &theName) const;
	
	void								SetElementUnpaired(        bool         theValue);
	void								SetElementContents(  const NString     &theValue);
	void								SetElementAttributes(const NDictionary &theValue);
	void								SetElementAttribute( const NString     &theName, const NString &theValue);


private:
	NXMLNode							*mParent;
	NXMLNodeList						mChildren;

	XMLNodeType							mType;
	NString								mValue;
	NDictionary							mAttributes;

	bool								mElementUnpaired;
};



#endif // NXMLNODE_HDR
