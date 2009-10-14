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
{	const EventTypeSpec	theEvents[] = { { kEventClassTextField, kEventTextAccepted  },
										{ kEventClassTextField, kEventTextDidChange },
										{ kEventClassNone,      kEventKindNone      } };



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NHISearchField::DoEventTextAccepted : Handle kEventTextAccepted.
//----------------------------------------------------------------------------
OSStatus NHISearchField::DoEventTextAccepted(NCarbonEvent &/*theEvent*/)
{


	// Inform our listeners
	//
	// The search field only sends its notification event to itself, and so
	// we re-broadcast the message to allow arbitrary objects to listen in.
	BroadcastMessage(kMsgSearchFieldAccepted, this);
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHISearchField::DoEventTextDidChange : Handle kEventTextDidChange.
//----------------------------------------------------------------------------
OSStatus NHISearchField::DoEventTextDidChange(NCarbonEvent &/*theEvent*/)
{


	// Inform our listeners
	//
	// The search field only sends its notification event to itself, and so
	// we re-broadcast the message to allow arbitrary objects to listen in.
	BroadcastMessage(kMsgSearchFieldChanged, this);
	
	return(eventNotHandledErr);
}



