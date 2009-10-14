/*	NAME:
		NIBView.cpp

	DESCRIPTION:
		Nib view loader.
		
		The NIBView and NIBWindow objects work together to provide automatic
		view instantiation through the NIB_VIEW macro.
		
		A window which wishes to contain member variables that are connected
		to the views within its .nib can use this macro in its declaration to
		declare a set of member variables that will be automatically connected
		to the .nib views when the window is constructed.
		
		
		The NIB_VIEW macro relies on the fact that member variables are
		constructed in the order that they are declared.
		
		This means it can define/create an internal "register class" object
		that saves the required state (the view ID) for the next NIBView to
		be created, and so can insert values as well as variables into the
		parent class declaration.
		 
		The macro then creates a templated NIBView subclass, which overrides
		the cast operators to make the member variable type-equivalent with
		a pointer to the desired view type.
		 
		 
		The NIBView class contains an NHIView* that points to the view, and
		is responsible for:
		 
			- Pulling its own view ID from the "current id" state in NIBView
			- Pushing itself onto the "current views" list in NIBWindow
			- Finding its NHIView on demand
			
		The NIBWindow class maintains a static list of NIBViews, which is
		used to initialize the views within each window as they are built.
		 
		Once a NIBWindow sub-class is ready to resolve its views, it can use
		NIBWindow::FindViews to find all of its views (or clear any views that
		could not be found to NULL).
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NIBWindow.h"
#include "NIBView.h"





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
HIViewID NIBView::mNextViewID;





//============================================================================
//		NIBView::NIBView : Constructor.
//----------------------------------------------------------------------------
NIBView::NIBView(void)
{


	// Validate our state
	NN_ASSERT(mNextViewID.signature != 0);



	// Initialize ourselves
	mView   = NULL;
	mViewID = mNextViewID;



	// Update the shared state
	NIBWindow::AddView(this);
	
	mNextViewID = kHIViewIDNone;
}





//============================================================================
//		NIBView::~NIBView : Destructor.
//----------------------------------------------------------------------------
NIBView::~NIBView(void)
{
}





//============================================================================
//		NIBView::IsValid : Is the view valid?
//----------------------------------------------------------------------------
bool NIBView::IsValid(void) const
{


	// Check our state
	return(mView != NULL);
}





//============================================================================
//		NIBView::Clear : Clear the view.
//----------------------------------------------------------------------------
void NIBView::Clear(void)
{


	// Clear the view
	mView = NULL;
}





//============================================================================
//		NIBView::GetView : Get the view.
//----------------------------------------------------------------------------
NHIView *NIBView::GetView(void) const
{


	// Get the view
	return(mView);
}





//============================================================================
//		NIBView::GetViewID : Get the view ID.
//----------------------------------------------------------------------------
HIViewID NIBView::GetViewID(void) const
{


	// Get the view ID
	return(mViewID);
}





//============================================================================
//		NIBView::FindView : Find the view.
//----------------------------------------------------------------------------
OSStatus NIBView::FindView(NHIWindow *theWindow)
{


	// Find the view
	mView = theWindow->GetView(mViewID.signature, mViewID.id);
	if (mView == NULL)
		return(kIBCarbonRuntimeCantFindObject);

	return(noErr);
}





//============================================================================
//		NIBView::SetNextViewID : Set the next view ID.
//----------------------------------------------------------------------------
void NIBView::SetNextViewID(const HIViewID &theID)
{


	// Validate our parameters
	NN_ASSERT(theID.signature != 0);



	// Set the next view ID
	mNextViewID = theID;
}






