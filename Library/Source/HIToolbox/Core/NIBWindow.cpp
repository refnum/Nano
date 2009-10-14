/*	NAME:
		NIBWindow.cpp

	DESCRIPTION:
		Nib window loader.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NIBView.h"
#include "NIBWindow.h"





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NIBViewList NIBWindow::mCurrentViews;





//============================================================================
//		NIBWindow::NIBWindow : Constructor.
//----------------------------------------------------------------------------
NIBWindow::NIBWindow(void)
{
}





//============================================================================
//		NIBWindow::~NIBWindow : Destructor.
//----------------------------------------------------------------------------
NIBWindow::~NIBWindow(void)
{
}





//============================================================================
//		NIBWindow::GetViews : Get the views.
//----------------------------------------------------------------------------
const NIBViewList *NIBWindow::GetViews(void)
{


	// Initialize the views
	//
	// We need to defer copying the view list until required, since our views
	// only add themselves to the current list as they are constructed.
	//
	// Since we are typically a parent class of the class that contains the
	// views, we can not copy the view list in our constructor (as the views
	// themselves have not yet been constructed).
	if (mViews.empty())
		{
		mViews = mCurrentViews;
		mCurrentViews.clear();
		}

	return(&mViews);
}





//============================================================================
//		NIBWindow::FindViews : Find the views.
//----------------------------------------------------------------------------
void NIBWindow::FindViews(NHIWindow *theWindow)
{	static bool					sLogErrors = false;

	NIBViewListConstIterator	theIter;
	OSStatus					theErr;
	HIViewID					theID;



	// Initialize the view list
	(void) GetViews();



	// Find the views
	//
	// Views are allowed to be missing (to support windows that contain a
	// variable set of views, such as NPanelController), however a flag is
	// provided to debug missing views.
	//
	// If you find that a view was not bound to its member variable, this
	// flag can be set to log out any missing views.
	for (theIter = mViews.begin(); theIter != mViews.end(); theIter++)
		{
		theErr = (*theIter)->FindView(theWindow);
		if (theErr != noErr && sLogErrors)
			{
			theID = (*theIter)->GetViewID();
			NN_LOG("Unable to find view { %4s, %d }", &theID.signature, theID.id);
			}
		}
}





//============================================================================
//		NIBWindow::AddView : Add a view.
//----------------------------------------------------------------------------
void NIBWindow::AddView(NIBView *theView)
{


	// Validate our parameters
	NN_ASSERT(theView != NULL);



	// Add the view
	//
	// Each NIBView adds itself to the list as it is created; when the owning
	// window is constructed, it can clone the list to obtain its own views.
	mCurrentViews.push_back(theView);
}



