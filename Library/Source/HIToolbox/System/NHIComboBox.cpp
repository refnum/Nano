/*	NAME:
		NHIComboBox.cpp

	DESCRIPTION:
		ComboBox view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIComboBox.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NHIComboBox, kHIComboBoxClassID, NULL);





//============================================================================
//		NHIComboBox::NHIComboBox : Constructor.
//----------------------------------------------------------------------------
NHIComboBox::NHIComboBox(HIWindowRef					theWindow,
							const HIRect				&theRect,
							const NString				&theText,
							const NArray				*theItems,
							OptionBits					theAttributes,
							const ControlFontStyleRec	*theStyle)
{	CFArrayRef		cfArray;
	HIViewRef		theView;
	OSStatus		theErr;



	// Get the items
	cfArray = NULL;
	
	if (theItems != NULL)
		cfArray = *theItems;



	// Create the view
	theErr = HIComboBoxCreate(NULL, theText, theStyle, cfArray, theAttributes, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NHIComboBox::~NHIComboBox : Destructor.
//----------------------------------------------------------------------------
NHIComboBox::~NHIComboBox(void)
{
}





//============================================================================
//		NHIComboBox::GetAttributes : Get the attributes.
//----------------------------------------------------------------------------
OptionBits NHIComboBox::GetAttributes(void) const
{	OptionBits	theValue;
	OSStatus	theErr;



	// Get the attributes
	theValue = kHIComboBoxNoAttributes;
	theErr   = HIComboBoxGetAttributes(*this, &theValue);
	NN_ASSERT_NOERR(theErr);

	return(theValue);
}





//============================================================================
//		NHIComboBox::SetAttributes : Set the attributes.
//----------------------------------------------------------------------------
void NHIComboBox::SetAttributes(OptionBits setThese, OptionBits clearThese)
{	OSStatus	theErr;



	// Set the attributes
	theErr = HIComboBoxChangeAttributes(*this, setThese, clearThese);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIComboBox::IsListVisible : Is the list visible?
//----------------------------------------------------------------------------
bool NHIComboBox::IsListVisible(void) const
{


	// Get the list visibility
	return(HIComboBoxIsListVisible(*this));
}





//============================================================================
//		NHIComboBox::SetListVisible : Set the list visibility.
//----------------------------------------------------------------------------
void NHIComboBox::SetListVisible(bool isVisible)
{	OSStatus	theErr;



	// Set the list visibility
	theErr = HIComboBoxSetListVisible(*this, isVisible);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIComboBox::GetListWidth : Get the list width.
//----------------------------------------------------------------------------
UInt32 NHIComboBox::GetListWidth(void) const
{


	// Get the width
	return(GetControlUInt32(kControlEntireControl, kHIComboBoxListPixelWidthTag));
}





//============================================================================
//		NHIComboBox::SetListWidth : Set the list width.
//----------------------------------------------------------------------------
void NHIComboBox::SetListWidth(UInt32 theValue)
{


	// Set the width
	SetControlBoolean(kControlEntireControl, kHIComboBoxListPixelWidthTag, theValue);
}





//============================================================================
//		NHIComboBox::GetListHeight : Get the list height.
//----------------------------------------------------------------------------
UInt32 NHIComboBox::GetListHeight(void) const
{


	// Get the height
	return(GetControlUInt32(kControlEntireControl, kHIComboBoxListPixelHeightTag));
}





//============================================================================
//		NHIComboBox::SetListHeight : Set the list height.
//----------------------------------------------------------------------------
void NHIComboBox::SetListHeight(UInt32 theValue)
{


	// Set the height
	SetControlBoolean(kControlEntireControl, kHIComboBoxListPixelHeightTag, theValue);
}





//============================================================================
//		NHIComboBox::GetVisibleCount : Get the number of visible items.
//----------------------------------------------------------------------------
UInt32 NHIComboBox::GetVisibleCount(void) const
{


	// Get the number of visible items
	return(GetControlUInt32(kControlEntireControl, kHIComboBoxNumVisibleItemsTag));
}





//============================================================================
//		NHIComboBox::SetVisibleCount : Set the number of visible items.
//----------------------------------------------------------------------------
void NHIComboBox::SetVisibleCount(UInt32 theValue)
{


	// Set the number of visible items
	SetControlBoolean(kControlEntireControl, kHIComboBoxNumVisibleItemsTag, theValue);
}





//============================================================================
//		NHIComboBox::AddItem : Add an item to the list.
//----------------------------------------------------------------------------
void NHIComboBox::AddItem(const NString &theValue, UInt32 theIndex)
{	OSStatus	theErr;



	// Add the item
	theErr = HIComboBoxInsertTextItemAtIndex(*this, theIndex, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIComboBox::RemoveItem : Remove an item from the list.
//----------------------------------------------------------------------------
void NHIComboBox::RemoveItem(UInt32 theIndex)
{	OSStatus	theErr;



	// Remove the item
	theErr = HIComboBoxRemoveItemAtIndex(*this, theIndex);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIComboBox::GetItemValue : Get an item from the list.
//----------------------------------------------------------------------------
NString NHIComboBox::GetItemValue(UInt32 theIndex)
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Get the value
	cfString = NULL;
	theErr   = HIComboBoxCopyTextItemAtIndex(*this, theIndex, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (cfString != NULL)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NHIComboBox::SetItemValue : Set an item in the list.
//----------------------------------------------------------------------------
void NHIComboBox::SetItemValue(UInt32 theIndex, const NString &theValue)
{	OSStatus	theErr;



	// Set the item
	//
	// There isn't an API to change the value of an existing item, and so we
	// insert at the same index then delete the now-shuffled previous item.
	theErr = HIComboBoxInsertTextItemAtIndex(*this, theIndex, theValue);
	if (theErr == noErr)
		theErr = HIComboBoxRemoveItemAtIndex(*this, theIndex + 1);

	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIComboBox::GetSelection : Get the selection.
//----------------------------------------------------------------------------
ControlEditTextSelectionRec NHIComboBox::GetSelection(void) const
{	ControlEditTextSelectionRec		theSelection;



	// Get the selection
	GetControlData(kControlEntireControl, kControlEditTextSelectionTag, sizeof(theSelection), &theSelection);
	
	return(theSelection);
}





//============================================================================
//		NHIComboBox::SetSelection : Set the selection.
//----------------------------------------------------------------------------
void NHIComboBox::SetSelection(const ControlEditTextSelectionRec &theSelection)
{


	// Set the selection
	SetControlData(kControlEntireControl, kControlEditTextSelectionTag, sizeof(theSelection), &theSelection);
}





//============================================================================
//		NHIComboBox::SetSelection : Set the selection.
//----------------------------------------------------------------------------
void NHIComboBox::SetSelection(SInt16 selStart, SInt16 selEnd)
{	ControlEditTextSelectionRec		theSelection = { selStart, selEnd };



	// Set the selection
	SetSelection(theSelection);
}
