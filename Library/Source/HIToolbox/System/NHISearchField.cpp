/*	NAME:
		NHISearchField.cpp

	DESCRIPTION:
		Search field view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHISearchField.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NHISearchField, kHISearchFieldClassID, NULL);





//============================================================================
//		NHISearchField::NHISearchField : Constructor.
//----------------------------------------------------------------------------
NHISearchField::NHISearchField(HIWindowRef			theWindow,
								const HIRect		&theRect,
								const NString		&theDescription,
								OptionBits			theAttributes,
								MenuRef				theMenu)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = HISearchFieldCreate(NULL, theAttributes, theMenu, theDescription, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NHISearchField::~NHISearchField : Destructor.
//----------------------------------------------------------------------------
NHISearchField::~NHISearchField(void)
{
}





//============================================================================
//		NHISearchField::GetSize : Get the size.
//----------------------------------------------------------------------------
UInt32 NHISearchField::GetSize(void) const
{


	// Get the size
	return(GetTextValue().GetSize());
}





//============================================================================
//		NHISearchField::Clear : Clear the search field.
//----------------------------------------------------------------------------
void NHISearchField::Clear(void)
{


	// Clear the search field
	SetTextValue("");
	UpdateCancel();

	BroadcastMessage(kMsgSearchFieldChanged, this);
}





//============================================================================
//		NHISearchField::GetDescription : Get the description.
//----------------------------------------------------------------------------
NString NHISearchField::GetDescription(void) const
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Get the description
	cfString = NULL;
	theErr   = HISearchFieldCopyDescriptiveText(*this, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (cfString != NULL)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NHISearchField::SetDescription : Set the description.
//----------------------------------------------------------------------------
void NHISearchField::SetDescription(const NString &theValue)
{	OSStatus	theErr;



	// Set the description
	theErr = HISearchFieldSetDescriptiveText(*this, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHISearchField::GetAttributes : Get the attributes.
//----------------------------------------------------------------------------
OptionBits NHISearchField::GetAttributes(void) const
{	OptionBits	theValue;
	OSStatus	theErr;



	// Get the attributes
	theValue = kHISearchFieldNoAttributes;
	theErr   = HISearchFieldGetAttributes(*this, &theValue);
	NN_ASSERT_NOERR(theErr);

	return(theValue);
}





//============================================================================
//		NHISearchField::SetAttributes : Set the attributes.
//----------------------------------------------------------------------------
void NHISearchField::SetAttributes(OptionBits setThese, OptionBits clearThese)
{	OSStatus	theErr;



	// Set the attributes
	theErr = HISearchFieldChangeAttributes(*this, setThese, clearThese);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHISearchField::GetMenu : Get the menu.
//----------------------------------------------------------------------------
MenuRef NHISearchField::GetMenu(void) const
{	MenuRef		theMenu;
	OSStatus	theErr;



	// Get the menu
	theMenu = NULL;
	theErr  = HISearchFieldGetSearchMenu(*this, &theMenu);
	NN_ASSERT_NOERR(theErr);
	
	return(theMenu);
}





//============================================================================
//		NHISearchField::SetMenu : Set the menu.
//----------------------------------------------------------------------------
void NHISearchField::SetMenu(MenuRef theMenu)
{	OSStatus	theErr;



	// Set the opaque value
	theErr = HISearchFieldSetSearchMenu(*this, theMenu);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHISearchField::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NHISearchField::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassSearchField, kEventSearchFieldCancelClicked    },
										{ kEventClassTextField,   kEventTextAccepted                },
										{ kEventClassTextField,   kEventTextDidChange               },
										{ kEventClassTextInput,   kEventTextInputUnicodeForKeyEvent },
										{ kEventClassNone,        kEventKindNone                    } };



	// Initialise ourselves
	SetOptions(kHIViewOptionSearchFieldClearOnCancel);



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NHISearchField::DoEventSearchFieldCancelClicked : Handle kEventSearchFieldCancelClicked.
//----------------------------------------------------------------------------
OSStatus NHISearchField::DoEventSearchFieldCancelClicked(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	if (GetOptions() & kHIViewOptionSearchFieldClearOnCancel)
		Clear();


	return(eventNotHandledErr);
}





//============================================================================
//		NHISearchField::DoEventTextAccepted : Handle kEventTextAccepted.
//----------------------------------------------------------------------------
OSStatus NHISearchField::DoEventTextAccepted(NCarbonEvent &/*theEvent*/)
{


	// Inform our listeners
	//
	// Ths event is only sent to the view and its immediate parent, so we
	// broadcast our own message to allow arbitrary objects to receive it.
	BroadcastMessage(kMsgSearchFieldAccepted, this);
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHISearchField::DoEventTextDidChange : Handle kEventTextDidChange.
//----------------------------------------------------------------------------
OSStatus NHISearchField::DoEventTextDidChange(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	UpdateCancel();



	// Inform our listeners
	//
	// Ths event is only sent to the view and its immediate parent, so we
	// broadcast our own message to allow arbitrary objects to receive it.
	BroadcastMessage(kMsgSearchFieldChanged, this);
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHISearchField::DoEventTextInputUnicodeForKeyEvent : Handle kEventTextInputUnicodeForKeyEvent.
//----------------------------------------------------------------------------
OSStatus NHISearchField::DoEventTextInputUnicodeForKeyEvent(NCarbonEvent &theEvent)
{	NCarbonEvent	keyEvent;
	OSStatus		theErr;



	// Get the state we need
	keyEvent = theEvent.GetParameterEvent(kEventParamTextInputSendKeyboardEvent);
	theErr   = eventNotHandledErr;



	// Cancel the edit
	//
	// Unlike HISearchField, NSSearchField automatically adds the cancel attribute
	// when the search field is non-empty, and clears the input on Escape/Cmd-.
	//
	// Since this is useful behaviour, we add the same system to HISearchField.
	if (GetOptions() & kHIViewOptionSearchFieldClearOnCancel)
		{
		if (IsUserCancelEventRef(keyEvent.GetEvent()))
			{
			Clear();
			theErr = noErr;
			}
		}
	
	return(theErr);
}





//============================================================================
//		NHISearchField::UpdateCancel : Update the cancel state.
//----------------------------------------------------------------------------
#pragma mark -
void NHISearchField::UpdateCancel(void)
{


	// Update our state
	if (GetOptions() & kHIViewOptionSearchFieldClearOnCancel)
		{
		if (IsEmpty())
			SetAttributes(kHISearchFieldNoAttributes, kHISearchFieldAttributesCancel);
		else
			SetAttributes(kHISearchFieldAttributesCancel);
		}
}

