/*	NAME:
		NHIViewUtilities.cpp

	DESCRIPTION:
		HIView utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIViewUtilities.h"
#include "NSTLUtilities.h"
#include "NChasingArrows.h"
#include "NEditText.h"
#include "NHIWindow.h"





//============================================================================
//		NHIViewUtilities::GetView : Get an NHIView from an HIViewRef.
//----------------------------------------------------------------------------
NHIView *NHIViewUtilities::GetView(HIViewRef hiView)
{	NHIView		*theView;



	// Check our parameters
	if (hiView == NULL)
		return(NULL);



	// Get the NHIView object for the view
	theView = dynamic_cast<NHIView*>(NHIObject::GetNHIObject((HIObjectRef) hiView));
	if (theView != NULL)
		return(theView);



	// Or create a new object
	theView = dynamic_cast<NHIView*>(NHIObjectFactory::CreateObject((HIObjectRef) hiView));
	
	return(theView);
}





//============================================================================
//		NHIViewUtilities::CreateHIView : Create an NHIView.
//----------------------------------------------------------------------------
//		Note :	Although this code could be placed in NHIView, so that apps
//				coudl instantiate a view with 'new NHIView', it has been kept
//				in this method to avoid any confusion within sub-classes of
//				NHIView.
//
//				A subclass of NHIView should use ConstructView to finish its
//				construction when it is created as a wrapper, and will be
//				configured automatically when it is created as a subclass via
//				NHIObjectFactory.
//
//				Sub-classes should never call NHIView's constructor, however
//				if NHIView provided a window/rect/visible-based constructor
//				then apps may think that path is the correct way to initialize
//				an NHIView subclass.
//
//				To ensure this isn't a temptation, we require that creating a
//				plain NHIView object comes through here rather than new.
//----------------------------------------------------------------------------
NHIView *NHIViewUtilities::CreateHIView(HIWindowRef	theWindow, const HIRect &theRect, bool isVisible)
{	HIViewRef	hiView, contentView;
	NHIView		*theView;
	OSStatus	theErr;



	// Create the view
	theErr = HIObjectCreate(kHIViewClassID, NULL, (HIObjectRef *) &hiView);
	if (theErr != noErr)
		return(NULL);



	// Create the wrapper
	theView = NHIViewUtilities::GetView(hiView);
	if (theView == NULL)
		return(NULL);



	// Configure the view
	if (theWindow != NULL)
		{
		contentView = NHIWindow(theWindow).GetHIView(kHIViewWindowContentID);
		NN_ASSERT(contentView != NULL);
		
		theErr = HIViewAddSubview(contentView, hiView);
		NN_ASSERT_NOERR(theErr);
		}
	
	theView->SetFrame(theRect);
	theView->SetVisible(isVisible);
	
	return(theView);
}





//============================================================================
//		NHIViewUtilities::GetLayoutInfo : Get an HILayoutInfo.
//----------------------------------------------------------------------------
HILayoutInfo NHIViewUtilities::GetLayoutInfo(OptionBits bindInfo, OptionBits positionInfo)
{	HILayoutInfo	theInfo;



	// Get the state we need
	theInfo = kHILayoutNone;



	// Configure the binding
	if (bindInfo & kBindTop)
		theInfo.binding.top.kind    = kHILayoutBindTop;
	
	if (bindInfo & kBindBottom)
		theInfo.binding.bottom.kind = kHILayoutBindBottom;

	if (bindInfo & kBindLeft)
		theInfo.binding.left.kind   = kHILayoutBindLeft;
	
	if (bindInfo & kBindRight)
		theInfo.binding.right.kind  = kHILayoutBindRight;



	// Configure the position
	if (positionInfo & kPositionTop)
		theInfo.position.y.kind = kHILayoutPositionTop;
	
	if (positionInfo & kPositionBottom)
		theInfo.position.y.kind = kHILayoutPositionBottom;

	if (positionInfo & kPositionLeft)
		theInfo.position.x.kind = kHILayoutPositionLeft;
	
	if (positionInfo & kPositionRight)
		theInfo.position.x.kind = kHILayoutPositionRight;

	if (positionInfo & kPositionCentreHorizontal)
		theInfo.position.x.kind = kHILayoutPositionCenter;

	if (positionInfo & kPositionCentreVertical)
		theInfo.position.y.kind = kHILayoutPositionCenter;

	return(theInfo);
}





//============================================================================
//      NHIViewUtilities::GetClickablePart : Get the "clickable" part code.
//----------------------------------------------------------------------------
HIViewPartCode NHIViewUtilities::GetClickablePart(NHIView *theView)
{	HIViewPartCode	partCode;



	// Find a suitable part code
	//
	// Although some views have no "clickable part", or have multiple parts,
	// many views only have a single part that responds to mouse clicks.
	if (theView->IsOfClass(kHIPushButtonClassID)  ||
		theView->IsOfClass(kHIBevelButtonClassID) ||
		theView->IsOfClass(kHIRoundButtonClassID) ||
		theView->IsOfClass(kHIDisclosureButtonClassID))
		partCode = kControlButtonPart;

	else if (theView->IsOfClass(kHICheckBoxClassID) ||
			 theView->IsOfClass(kHICheckBoxGroupClassID))
		partCode = kControlCheckBoxPart;

	else if (theView->IsOfClass(kHIRadioButtonClassID))
		partCode = kControlRadioButtonPart;

	else if (theView->IsOfClass(kHIRadioGroupClassID))
		partCode = kControlRadioGroupPart;

	else if (theView->IsOfClass(kHIDisclosureTriangleClassID))
		partCode = kControlTrianglePart;

	else if (theView->IsOfClass(kHIPopupButtonClassID))
		partCode = kControlMenuPart;

	else if (theView->IsOfClass(kHIIconViewClassID))
		partCode = kControlIconPart;

	else if (theView->IsOfClass(kHIImageWellClassID))
		partCode = kControlImageWellPart;
	
	else
		partCode = kHIViewIndicatorPart;
	
	return(partCode);
}





//============================================================================
//		NHIViewUtilities::EmbedInScrollView : Embed a view into a scroll view.
//----------------------------------------------------------------------------
NHIScrollView *NHIViewUtilities::EmbedInScrollView(NHIView *theView, OptionBits theOptions)
{	NHIScrollView		*scrollView;
	NHIView				*superView;



	// Validate our parameters
	NN_ASSERT(theView != NULL);



	// Create the scroll view
	scrollView = new NHIScrollView(theView->GetHIWindow(), theView->GetFrame(), theOptions);
	if (scrollView == NULL)
		return(NULL);



	// Embed the view
	//
	// Unfortunately IB has very limited supported for HIScrollView, and does
	// not support placing a custom view inside an scroll view (rdar://3982065).
	//
	// This function allows an arbitrary view to be placed inside a scroll view.
	//
	// Typically the view would be a subclass of NScrollableView, since this
	// can automatically embed the view based on a property assigned in IB.
	superView = theView->GetSuperView();
	if (superView != NULL)
		{
		superView->RemoveSubView(theView, false);
		superView->AddSubView(scrollView);
		}

	scrollView->AddSubView(   theView);
	scrollView->SetLayoutInfo(theView->GetLayoutInfo());
	scrollView->SetScrollBarAutoHide(true);
	
	return(scrollView);
}





//============================================================================
//		NHIViewUtilities::EmbedInContainerView : Embed a view into a container view.
//----------------------------------------------------------------------------
void NHIViewUtilities::EmbedInContainerView(NHIView *theView, NHIView *theContainer)
{


	// Embed the view
	theContainer->SetFeatures(kHIViewFeatureAllowsSubviews);
	theContainer->RemoveSubViews();
	theContainer->AddSubView(theView);

	theView->SetFrame(theContainer->GetBounds());
	theView->SetLayoutInfo(GetLayoutInfo(kBindAllSides));
}





//============================================================================
//		NHIViewUtilities::WrapSystemViews : Wrap the system views.
//----------------------------------------------------------------------------
void NHIViewUtilities::WrapSystemViews(HIWindowRef theWindow)
{	NHIView							*theWrapper;
	HIViewRefList					theViews;
	HIViewRefListConstIterator		theIter;
	HIViewKind						theKind;
	OSStatus						theErr;



	// Validate our parameters
	NN_ASSERT(theWindow != NULL);



	// Ensure our wrappers are linked in
	//
	// See NHIView.h for more information.
	NEditText::Register();
	NChasingArrows::Register();



	// Wrap the views
	theViews = GetChildren(HIViewGetRoot(theWindow), true);
	
	for (theIter = theViews.begin(); theIter != theViews.end(); theIter++)
		{
		// Get the state we need
		//
		// Support for 10.3 is temporary.
		if (HIViewGetKind != NULL)
			theErr = HIViewGetKind( *theIter,                 &theKind);
		else	
			theErr = GetControlKind(*theIter, (ControlKind *) &theKind);

		NN_ASSERT_NOERR(theErr);
		
		if (theErr == noErr && theKind.signature == kHIViewKindSignatureApple)
			{
			// Create the wrapper
			//
			// The list of views that are auto-wrapped will change over time, but the
			// goal is to add wrappers that fix known bugs or add useful features.
			//
			// More information on the reason for a wrapper (e.g., a rdar number)
			// can be found in the source to that class.
			switch (theKind.kind) {
				case kControlKindEditUnicodeText:			// Provides spell checking
				case kControlKindChasingArrows:				// Fixes invalidation bug
				case kControlKindStaticText:				// Adds shadow support
				case kControlKindHISearchField:				// Adds cancel-edit support
					theWrapper = NHIViewUtilities::GetView(*theIter);
					NN_ASSERT(theWrapper != NULL);
					break;
				
				default:
					break;
				}
			}
		}
}





//============================================================================
//		NHIViewUtilities::GetChildren : Get the children of a view.
//----------------------------------------------------------------------------
HIViewRefList NHIViewUtilities::GetChildren(HIViewRef theView, bool canRecurse)
{	HIViewRefList		theResult;
	HIViewRef			theChild;



	// Validate our parameters
	NN_ASSERT(theView != NULL);
	


	// Collect the children
	theChild = HIViewGetFirstSubview(theView);

	while (theChild != NULL)
		{
		theResult.push_back(theChild);

		if (canRecurse)
			append(theResult, GetChildren(theChild, canRecurse));
			
		theChild = HIViewGetNextView(theChild);
		}
	
	return(theResult);
}

