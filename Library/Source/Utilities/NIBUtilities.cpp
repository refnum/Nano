/*	NAME:
		NIBUtilities.cpp

	DESCRIPTION:
		Interface Builder .nib utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEventHandler.h"
#include "NHIViewUtilities.h"
#include "NHIView.h"
#include "NHIWindow.h"
#include "NIBUtilities.h"





//============================================================================
//		NIBUtilities::CreateNib : Create a .nib reference.
//----------------------------------------------------------------------------
IBNibRef NIBUtilities::CreateNib(const NString &nibName, CFBundleRef cfBundle)
{	OSStatus	theErr;
	IBNibRef	nibRef;



	// Create the .nib reference
	nibRef = NULL;
	
	if (cfBundle != NULL)
		theErr = CreateNibReferenceWithCFBundle(cfBundle, nibName, &nibRef);
	else
		theErr = CreateNibReference(                      nibName, &nibRef);

	NN_ASSERT_NOERR(theErr);

	return(nibRef);
}





//============================================================================
//		NIBUtilities::CreateMenu : Create a menu resource.
//----------------------------------------------------------------------------
MenuRef NIBUtilities::CreateMenu(IBNibRef nibRef, const NString &resName)
{	MenuRef		theMenu;
	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(nibRef != NULL);
	NN_ASSERT(resName.IsNotEmpty());



	// Create the menu
	theMenu = NULL;

	if (nibRef != NULL)
		{
		theErr = CreateMenuFromNib(nibRef, resName, &theMenu);
		NN_ASSERT_NOERR(theErr);
		}
	
	return(theMenu);
}





//============================================================================
//		NIBUtilities::CreateWindow : Create a window resource.
//----------------------------------------------------------------------------
HIWindowRef NIBUtilities::CreateWindow(IBNibRef nibRef, const NString &resName)
{	NCarbonEvent	theEvent(kEventClassNControl, kEventNControlInitialized);
	HIWindowRef		theWindow;
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(nibRef != NULL);
	NN_ASSERT(resName.IsNotEmpty());



	// Create the window
	theWindow = NULL;

	if (nibRef != NULL)
		{
		theErr = CreateWindowFromNib(nibRef, resName, &theWindow);
		NN_ASSERT_NOERR(theErr);
		}



	// Initialize the views
	//
	// Allows NHIView objects to initialize themselves once their
	// properties have been applied but before the window is visible.
	if (theWindow != NULL)
		SendEventToViews(HIViewGetRoot(theWindow), theEvent);
	
	return(theWindow);
}





//============================================================================
//		NIBUtilities::CreateView : Create a view resource.
//----------------------------------------------------------------------------
HIViewRef NIBUtilities::CreateView(IBNibRef nibRef, const NString &resName)
{	NHIWindow		theWindow(NIBUtilities::CreateWindow(nibRef, resName));
	NHIView			*srcView, *dstView, *childView;



	// Validate our parameters
	NN_ASSERT(nibRef != NULL);
	NN_ASSERT(resName.IsNotEmpty());



	// Get the source view
	//
	// Although .nib files can not contain raw views, we can use them for this
	// purpose by using the content view of a window as the source container.
	srcView = NHIViewUtilities::GetHIView(theWindow.GetHIView(kHIViewWindowContentID));



	// Prepare the destination view
	dstView = NHIViewUtilities::CreateHIView();

	dstView->SetFeatures(kHIViewFeatureAllowsSubviews);
	dstView->SetFrame(srcView->GetFrame());



	// Copy the child views
	while (srcView->GetSubViewCount() != 0)
		{
		childView = srcView->GetSubView(0);
		if (childView != NULL)
			{
			childView->RemoveFromSuperView();
			dstView->AddSubView(childView);
			}
		}
	
	return(*dstView);
}





//============================================================================
//		NIBUtilities::SetMenuBar : Set the menu bar.
//----------------------------------------------------------------------------
void NIBUtilities::SetMenuBar(IBNibRef nibRef, const NString &resName)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(nibRef != NULL);
	NN_ASSERT(resName.IsNotEmpty());



	// Set the menu bar
	if (nibRef != NULL)
		{
		theErr = SetMenuBarFromNib(nibRef, resName);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NIBUtilities::SendEventToViews : Send an event to a tree of views.
//----------------------------------------------------------------------------
#pragma mark -
void NIBUtilities::SendEventToViews(HIViewRef theView, const NCarbonEvent &theEvent)
{	HIViewRef	subView;
	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theView != NULL);
	NN_ASSERT(theEvent.IsValid());



	// Send it to the sub views
	subView = HIViewGetFirstSubview(theView);

	while (subView != NULL)
		{
		SendEventToViews(subView, theEvent);
		subView = HIViewGetNextView(subView);
		}



	// Send the event to the view
	theErr = theEvent.SendToHIView(theView);
	NN_ASSERT(theErr == eventNotHandledErr);
}



