/*	NAME:
		NHIToolbarItem.cpp

	DESCRIPTION:
		Toolbar item.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"
#include "NHIToolbar.h"
#include "NHIToolbarItem.h"





//============================================================================
//		NHIToolbarItem::NHIToolbarItem : Constructor.
//----------------------------------------------------------------------------
NHIToolbarItem::NHIToolbarItem(NHIToolbar *theToolbar, const HIToolbarItemInfo &theInfo)
{	const EventTypeSpec	theEvents[] = { { kEventClassToolbarItem, kEventToolbarItemCreateCustomView },
										{ kEventClassNone,		  kEventKindNone					} };
	HIToolbarItemRef	theItem;
	OSStatus			theErr;



	// Validate our parameters
	NN_ASSERT(theToolbar != NULL);
	NN_ASSERT(theInfo.theIdentifier.IsNotEmpty());



	// Initialize ourselves
	mToolbar = theToolbar;
	mView    = NULL;



	// Create the toolbar item
	theErr = HIToolbarItemCreate(theInfo.theIdentifier, theInfo.theAttributes, &theItem);
	NN_ASSERT_NOERR(theErr);
	
	SetHIObject(theItem);



	// Configure the toolbar item
	if (theInfo.theCommand != kHICommandNone)
		SetCommandID(theInfo.theCommand);
	
	SetLabel(  NBundleString(theInfo.keyLabel));
	SetHelpTag(NBundleString(theInfo.keyHelpTagStd), false);
	SetHelpTag(NBundleString(theInfo.keyHelpTagExt), true);
	
	if (theInfo.resImage.IsNotEmpty())
		SetImage(theInfo.resImage);



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NHIToolbarItem::NHIToolbarItem : Constructor.
//----------------------------------------------------------------------------
NHIToolbarItem::NHIToolbarItem(HIToolbarItemRef hiToolbarItem)
{


	// Validate our parameters
	NN_ASSERT(hiToolbarItem != NULL);



	// Assign the object
	SetHIObject(hiToolbarItem);
}





//============================================================================
//		NHIToolbarItem::~NHIToolbarItem : Destructor.
//----------------------------------------------------------------------------
NHIToolbarItem::~NHIToolbarItem(void)
{
}





//============================================================================
//		NHIToolbarItem::IsIdentifier : Is this our identifier?
//----------------------------------------------------------------------------
bool NHIToolbarItem::IsIdentifier(const NString &theIdentifier) const
{


	// Validate our parameters
	NN_ASSERT(theIdentifier.IsNotEmpty());



	// Check our identifier
	return(theIdentifier == GetIdentifier());
}





//============================================================================
//		NHIToolbarItem::GetIdentifier : Get the identifier.
//----------------------------------------------------------------------------
NString NHIToolbarItem::GetIdentifier(void) const
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Get the identifier
	cfString = NULL;
	theErr   = HIToolbarItemCopyIdentifier(*this, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (cfString != NULL)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NHIToolbarItem::IsEnabled : Is the item enabled?
//----------------------------------------------------------------------------
bool NHIToolbarItem::IsEnabled(void) const
{


	// Get the state
	return(HIToolbarItemIsEnabled(*this));
}





//============================================================================
//		NHIToolbarItem::SetEnabled : Set the enabled state.
//----------------------------------------------------------------------------
void NHIToolbarItem::SetEnabled(bool isEnabled)
{	OSStatus	theErr;



	// Set the state
	theErr = HIToolbarItemSetEnabled(*this, isEnabled);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbarItem::GetAttributes : Get the attributes.
//----------------------------------------------------------------------------
OptionBits NHIToolbarItem::GetAttributes(void) const
{	OptionBits		theValue;
	OSStatus		theErr;



	// Get the attributes
	theValue = kHIToolbarItemNoAttributes;
	theErr   = HIToolbarItemGetAttributes(*this, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NHIToolbarItem::SetAttributes : Set the attributes.
//----------------------------------------------------------------------------
void NHIToolbarItem::SetAttributes(OptionBits setThese, OptionBits clearThese)
{	OSStatus	theErr;



	// Set the attributes
	theErr = HIToolbarItemChangeAttributes(*this, setThese, clearThese);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbarItem::GetLabel : Get the label.
//----------------------------------------------------------------------------
NString NHIToolbarItem::GetLabel(void) const
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Get the label
	cfString = NULL;
	theErr   = HIToolbarItemCopyLabel(*this, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (cfString != NULL)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NHIToolbarItem::SetLabel : Set the label.
//----------------------------------------------------------------------------
void NHIToolbarItem::SetLabel(const NString &theValue)
{	OSStatus	theErr;



	// Set the label
	theErr = HIToolbarItemSetLabel(*this, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbarItem::GetHelpTag : Get the help tag.
//----------------------------------------------------------------------------
NString NHIToolbarItem::GetHelpTag(bool isExtended) const
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Get the help tag
	cfString = NULL;
	
	if (isExtended)
		theErr = HIToolbarItemCopyHelpText(*this, NULL, &cfString);
	else
		theErr = HIToolbarItemCopyHelpText(*this, &cfString, NULL);
	
	NN_ASSERT_NOERR(theErr);
	
	if (cfString != NULL)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NHIToolbarItem::SetHelpTag : Set the help tag.
//----------------------------------------------------------------------------
void NHIToolbarItem::SetHelpTag(const NString &theValue, bool isExtended)
{	CFStringRef		cfString;
	OSStatus		theErr;



	// Set the help tag
	if (isExtended)
		{
		theErr  = HIToolbarItemCopyHelpText(*this, &cfString,     NULL);
		theErr |= HIToolbarItemSetHelpText( *this,  cfString, theValue);
		}
	else
		{
		theErr  = HIToolbarItemCopyHelpText(*this,     NULL, &cfString);
		theErr |= HIToolbarItemSetHelpText( *this, theValue,  cfString);
		}

	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbarItem::GetCommandID : Get the command ID.
//----------------------------------------------------------------------------
UInt32 NHIToolbarItem::GetCommandID(void) const
{	MenuCommand		theCmd;
	OSStatus		theErr;



	// Get the command ID
	theCmd = kHICommandNone;
	theErr = HIToolbarItemGetCommandID(*this, &theCmd);
	NN_ASSERT_NOERR(theErr);
	
	return(theCmd);
}





//============================================================================
//		NHIToolbarItem::SetCommandID : Set the command ID.
//----------------------------------------------------------------------------
void NHIToolbarItem::SetCommandID(UInt32 theCmd)
{	OSStatus	theErr;



	// Set the label
	theErr = HIToolbarItemSetCommandID(*this, theCmd);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbarItem::CopyMenu : Get the menu.
//----------------------------------------------------------------------------
MenuRef NHIToolbarItem::CopyMenu(void) const
{	MenuRef		theMenu;
	OSStatus	theErr;



	// Get the menu
	theMenu = NULL;
	theErr  = HIToolbarItemCopyMenu(*this, &theMenu);
	NN_ASSERT_NOERR(theErr);
	
	return(theMenu);
}





//============================================================================
//		NHIToolbarItem::SetMenu : Set the menu.
//----------------------------------------------------------------------------
void NHIToolbarItem::SetMenu(MenuRef theMenu)
{	OSStatus	theErr;



	// Set the menu
	theErr = HIToolbarItemSetMenu(*this, theMenu);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbarItem::GetImage : Get the image.
//----------------------------------------------------------------------------
NCGImage NHIToolbarItem::GetImage(void) const
{	NCGImage		theImage;
	CGImageRef		cgImage;
	OSStatus		theErr;



	// Get the menu
	cgImage = NULL;
	theErr  = HIToolbarItemCopyImage(*this, &cgImage);
	NN_ASSERT_NOERR(theErr);
	
	if (cgImage != NULL)
		theImage.Set(cgImage);
	
	return(theImage);
}





//============================================================================
//		NHIToolbarItem::SetImage : Set the image.
//----------------------------------------------------------------------------
void NHIToolbarItem::SetImage(const NCGImage &theImage)
{	OSStatus	theErr;



	// Validate our parameters
	if (theImage.IsValid())
		NN_ASSERT(theImage.GetHeight() <= 32);



	// Set the image
	theErr = HIToolbarItemSetImage(*this, theImage);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbarItem::SetIcon : Set the icon.
//----------------------------------------------------------------------------
void NHIToolbarItem::SetIcon(const NIcon &theIcon)
{	OSStatus	theErr;



	// Set the icon
	theErr = HIToolbarItemSetIconRef(*this, theIcon);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbarItem::GetView : Get the custom view.
//----------------------------------------------------------------------------
NHIView *NHIToolbarItem::GetView(void) const
{


	// Get the view
	return(mView);
}





//============================================================================
//		NHIToolbarItem::operator HIToolbarItemRef : HIToolbarItemRef-cast operator.
//----------------------------------------------------------------------------
NHIToolbarItem::operator HIToolbarItemRef(void) const
{


	// Get the toolbar item
	return((HIToolbarItemRef) GetHIObject());
}





//============================================================================
//		NHIToolbarItem::DoEventToolbarItemCreateCustomView : Handle kEventToolbarItemCreateCustomView.
//----------------------------------------------------------------------------
OSStatus NHIToolbarItem::DoEventToolbarItemCreateCustomView(NCarbonEvent &theEvent)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mView == NULL);
	


	// Create the custom view
	//
	// The most common customisation of a toolbar item is to create a custom
	// view for the item - to simplify this task, we treat our toolbar as a
	// delegate and ask it to create the view for us.
	//
	// This reduces the amount of sub-classing that applications need to do,
	// as typically you would just sub-class NHIToolbar and then manage all
	// of "the toolbar" (toolbar and items) from there.
	//
	// If this behaviour is not suitable, applications are free to sub-class
	// this object and/or handle other kEventClassToolbarItem events to adjust
	// the item behaviour.
	theErr = eventNotHandledErr;
	mView  = mToolbar->CreateItemView(this);

	if (mView != NULL)
		theErr = theEvent.SetParameterHIView(kEventParamControlRef, *mView);

	return(theErr);
}



