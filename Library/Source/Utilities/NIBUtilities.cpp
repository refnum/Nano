/*	NAME:
		NIBUtilities.cpp

	DESCRIPTION:
		Interface Builder .nib utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEventHandler.h"
#include "NHIViewUtilities.h"
#include "NFileUtilities.h"
#include "NHIView.h"
#include "NHIWindow.h"
#include "NIBUtilities.h"





//============================================================================
//		NIBUtilities::CreateNib : Create a .nib reference.
//----------------------------------------------------------------------------
IBNibRef NIBUtilities::CreateNib(const NString &nibName, CFBundleRef cfBundle)
{	OSStatus	theErr;
	IBNibRef	nibRef;



	// Validate our parameters
	NN_ASSERT(nibName.IsNotEmpty());
	


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
MenuRef NIBUtilities::CreateMenu(IBNibRef nibRef, const NString &resName, bool canFail)
{	MenuRef		theMenu;
	SInt32		errorFD;
	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(nibRef != NULL);
	NN_ASSERT(resName.IsNotEmpty());



	// Suppress stderr
	//
	// IBCarbonRuntime emits unnecessary debug output if a resource could not be
	// found (rdar://4773703), so to work around this we suppress stderr if the
	// resource is allowed to fail.
	errorFD = 0;
	if (canFail)
		errorFD = NFileUtilities::RedirectStderr(-1);



	// Create the menu
	theMenu = NULL;

	if (nibRef != NULL)
		{
		theErr = CreateMenuFromNib(nibRef, resName, &theMenu);
		if (canFail && theErr == kIBCarbonRuntimeCantFindObject)
			theErr = noErr;
		
		NN_ASSERT_NOERR(theErr);
		}



	// Restore stderr
	if (canFail)
		(void) NFileUtilities::RedirectStderr(errorFD);
	
	return(theMenu);
}





//============================================================================
//		NIBUtilities::CreateWindow : Create a window resource.
//----------------------------------------------------------------------------
HIWindowRef NIBUtilities::CreateWindow(IBNibRef nibRef, const NString &resName, bool canFail)
{	NCarbonEvent	theEvent(kEventClassNControl, kEventNControlInitialized);
	HIWindowRef		theWindow;
	SInt32			errorFD;
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(nibRef != NULL);
	NN_ASSERT(resName.IsNotEmpty());



	// Suppress stderr
	//
	// IBCarbonRuntime emits unnecessary debug output if a resource could not be
	// found (rdar://4773703), so to work around this we suppress stderr if the
	// resource is allowed to fail.
	errorFD = 0;
	if (canFail)
		errorFD = NFileUtilities::RedirectStderr(-1);



	// Create the window
	theWindow = NULL;

	if (nibRef != NULL)
		{
		theErr = CreateWindowFromNib(nibRef, resName, &theWindow);
		if (canFail && theErr == kIBCarbonRuntimeCantFindObject)
			theErr = noErr;
		
		NN_ASSERT_NOERR(theErr);
		}



	// Restore stderr
	if (canFail)
		(void) NFileUtilities::RedirectStderr(errorFD);



	// Initialize the views
	//
	// This event allows NHIView objects to initialize themselves once their
	// properties have been applied but before the window is visible.
	if (theWindow != NULL)
		SendEventToViews(HIViewGetRoot(theWindow), theEvent);
	
	return(theWindow);
}





//============================================================================
//		NIBUtilities::CreateView : Create a view resource.
//----------------------------------------------------------------------------
HIViewRef NIBUtilities::CreateView(IBNibRef nibRef, const NString &resName, bool canFail)
{	NHIWindow		theWindow(NIBUtilities::CreateWindow(nibRef, resName, canFail));
	NHIView			*srcView, *dstView, *childView;



	// Validate our parameters
	NN_ASSERT(nibRef != NULL);
	NN_ASSERT(resName.IsNotEmpty());



	// Update the window
	//
	// Our NHIWindow retains a reference to the window, so if we created it we
	// need to release the extra reference to ensure the window is destroyed.
	if (!theWindow.IsValid())
		return(NULL);

	NN_ASSERT(GetWindowRetainCount(theWindow) == 2);
	ReleaseWindow(theWindow);



	// Get the source view
	//
	// Although .nib files can not contain raw views, we can use them for this
	// purpose by using the content view of a window as the source container.
	srcView = theWindow.GetView(kHIViewWindowContentID);



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
{	HIViewRefList					theViews;
	HIViewRefListConstIterator		theIter;
	HIViewRef						subView;
	OSStatus						theErr;



	// Validate our parameters
	NN_ASSERT(theView != NULL);
	NN_ASSERT(theEvent.IsValid());



	// Collect the sub views
	//
	// To allow views to create additional views while processing the event
	// (e.g., an NScrollableView may wish to create an enclosing HIScrollView
	// during initialisation), we collect the views before dispatch.
	subView = HIViewGetFirstSubview(theView);

	while (subView != NULL)
		{
		theViews.push_back(subView);
		subView = HIViewGetNextView(subView);
		}



	// Send the event to the sub views
	for (theIter = theViews.begin(); theIter != theViews.end(); theIter++)
		SendEventToViews(*theIter, theEvent);



	// Send the event to the view
	theErr = theEvent.SendToHIView(theView);
	NN_ASSERT(theErr == eventNotHandledErr);
}



