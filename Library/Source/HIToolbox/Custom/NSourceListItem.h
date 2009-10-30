/*	NAME:
		NSourceListItem.h

	DESCRIPTION:
		Source list item.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NSOURCELISTITEM_HDR
#define NSOURCELISTITEM_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NString.h"
#include "NIcon.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const OptionBits kSLItemNone								= 0;
static const OptionBits kSLItemCanRemove						= (1 << 1);
static const OptionBits kSLItemCanRename						= (1 << 2);
static const OptionBits kSLItemCanSelect						= (1 << 0);
static const OptionBits kSLItemCanExpand						= (1 << 3);
static const OptionBits kSLItemIsExpanded						= (1 << 4);





// Lists
class NSourceListItem;

typedef std::vector<NSourceListItem>								NSourceListItemList;
typedef NSourceListItemList::iterator								NSourceListItemListIterator;
typedef NSourceListItemList::const_iterator							NSourceListItemListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSourceListItem {
public:
										 NSourceListItem(const NString	&theID,
														 const NString	&theTitle,
														 const NIcon	&theIcon   = NIcon(),
														 OptionBits		theOptions = kSLItemNone);

										 NSourceListItem(void);
										~NSourceListItem(void);


	// Get/set tbe identifier
	NString								GetID(void) const;
	void								SetID(const NString &theID);


	// Get/set tbe title
	NString								GetTitle(void) const;
	void								SetTitle(const NString &theTitle);


	// Get/set tbe icon
	NIcon								GetIcon(void) const;
	void								SetIcon(const NIcon &theIcon);


	// Get/set tbe options
	OptionBits							GetOptions(void) const;
	void								SetOptions(OptionBits setThese, OptionBits clearThese=kSLItemNone);


	// Get/set the children
	const NSourceListItemList			*GetChildren(void) const;
	void								SetChildren(const NSourceListItemList &theItems);


	// Add a child
	void								AddChild(const NSourceListItem &theItem);

	void								AddChild(const NString	&theID,
												 const NString	&theTitle,
												 const NIcon	&theIcon   = NIcon(),
												 OptionBits		theOptions = kSLItemNone);


private:
	NString								mID;
	NString								mTitle;
	NIcon								mIcon;
	OptionBits							mOptions;
	NSourceListItemList					mChildren;
};





#endif // NSOURCELISTITEM_HDR


