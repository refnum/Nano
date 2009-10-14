/*	NAME:
		NHIComboBox.h

	DESCRIPTION:
		ComboBox view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHICOMBOBOX_HDR
#define NHICOMBOBOX_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"
#include "NArray.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIComboBox : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NHIComboBox);

										NHIComboBox(HIWindowRef						theWindow,
														const HIRect				&theRect,
														const NString				&theText      = "",
														const NArray				*theItems     = NULL,
														OptionBits					theAttributes = kHIComboBoxNoAttributes,
														const ControlFontStyleRec	*theStyle     = NULL);
	virtual								~NHIComboBox(void);


	// Get/set the attributes
	OptionBits							GetAttributes(void) const;
	void								SetAttributes(OptionBits setThese, OptionBits clearThese=kHIComboBoxNoAttributes);


	// Get/set the list visibility
	bool								IsListVisible(void) const;
	void								SetListVisible(bool theValue);


	// Get/set the list width
	UInt32								GetListWidth(void) const;
	void								SetListWidth(UInt32 theValue);


	// Get/set the list height
	UInt32								GetListHeight(void) const;
	void								SetListHeight(UInt32 theValue);


	// Get/set the number of visible items
	UInt32								GetVisibleCount(void) const;
	void								SetVisibleCount(UInt32 theValue);


	// Add/remove an item
	//
	// If no index is supplied, the item is added to the end of the list.
	void								AddItem(const NString &theValue, UInt32 theIndex=ULONG_MAX);
	void								RemoveItem(UInt32 theIndex);


	// Get/set an item's text
	NString								GetItemValue(UInt32 theIndex);
	void								SetItemValue(UInt32 theIndex, const NString &theValue);


	// Get/set the edit field selection
	ControlEditTextSelectionRec			GetSelection(void) const;
	void								SetSelection(const ControlEditTextSelectionRec &theSelection);
	void								SetSelection(SInt16 selStart=0, SInt16 selEnd=-1);


private:


};





#endif // NHIComboBox_HDR


