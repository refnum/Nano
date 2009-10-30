/*	NAME:
		NSourceListItem.cpp

	DESCRIPTION:
		Source list item.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSourceListItem.h"





//============================================================================
//		NSourceListItem::NSourceListItem : Constructor.
//----------------------------------------------------------------------------
NSourceListItem::NSourceListItem(const NString &theID, const NString &theTitle, const NIcon &theIcon, OptionBits theOptions)
{


	// Validate our parameters
	NN_ASSERT(theTitle.IsNotEmpty());
	


	// Initialize ourselves
	mID      = theID;
	mTitle   = theTitle;
	mIcon    = theIcon;
	mOptions = theOptions;
}





//============================================================================
//		NSourceListItem::NSourceListItem : Constructor.
//----------------------------------------------------------------------------
NSourceListItem::NSourceListItem(void)
{


	// Initialize ourselves
	mOptions = kSLItemNone;
}





//============================================================================
//		NSourceListItem::~NSourceListItem : Destructor.
//----------------------------------------------------------------------------
NSourceListItem::~NSourceListItem(void)
{
}





//============================================================================
//		NSourceListItem::GetID : Get the ID.
//----------------------------------------------------------------------------
NString NSourceListItem::GetID(void) const
{


	// Get the ID
	return(mID);
}





//============================================================================
//		NSourceListItem::SetID : Set the ID.
//----------------------------------------------------------------------------
void NSourceListItem::SetID(const NString &theID)
{


	// Set the ID
	mID = theID;
}





//============================================================================
//		NSourceListItem::GetTitle : Get the title.
//----------------------------------------------------------------------------
NString NSourceListItem::GetTitle(void) const
{


	// Get the title
	return(mTitle);
}





//============================================================================
//		NSourceListItem::SetTitle : Set the title.
//----------------------------------------------------------------------------
void NSourceListItem::SetTitle(const NString &theTitle)
{


	// Validate our parameters
	NN_ASSERT(theTitle.IsNotEmpty());
	


	// Set the title
	mTitle = theTitle;
}





//============================================================================
//		NSourceListItem::GetIcon : Get the icon.
//----------------------------------------------------------------------------
NIcon NSourceListItem::GetIcon(void) const
{


	// Get the icon
	return(mIcon);
}





//============================================================================
//		NSourceListItem::SetIcon : Set the icon.
//----------------------------------------------------------------------------
void NSourceListItem::SetIcon(const NIcon &theIcon)
{


	// Set the icon
	mIcon = theIcon;
}





//============================================================================
//		NSourceListItem::GetOptions : Get the options.
//----------------------------------------------------------------------------
OptionBits NSourceListItem::GetOptions(void) const
{


	// Get the options
	return(mOptions);
}





//============================================================================
//		NSourceListItem::SetOptions : Set the options.
//----------------------------------------------------------------------------
void NSourceListItem::SetOptions(OptionBits setThese, OptionBits clearThese)
{


	// Set the options
	mOptions |=    setThese;
	mOptions &= ~clearThese;
}





//============================================================================
//		NSourceListItem::GetChildren : Get the children.
//----------------------------------------------------------------------------
const NSourceListItemList *NSourceListItem::GetChildren(void) const
{


	// Get the children
	return(&mChildren);
}





//============================================================================
//		NSourceListItem::SetChildren : Set the children.
//----------------------------------------------------------------------------
void NSourceListItem::SetChildren(const NSourceListItemList &theChildren)
{


	// Set the children
	mChildren = theChildren;
}





//============================================================================
//		NSourceListItem::AddChild : Add a child.
//----------------------------------------------------------------------------
void NSourceListItem::AddChild(const NSourceListItem &theItem)
{


	// Add the child
	mChildren.push_back(theItem);
}





//============================================================================
//		NSourceListItem::AddChild : Add a child.
//----------------------------------------------------------------------------
void NSourceListItem::AddChild(const NString &theID, const NString &theTitle, const NIcon &theIcon, OptionBits theOptions)
{


	// Add the child
	mChildren.push_back(NSourceListItem(theID, theTitle, theIcon, theOptions));
}



