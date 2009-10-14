/*	NAME:
		NHIWindow.cpp

	DESCRIPTION:
		HIWindowRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NHIViewUtilities.h"
#include "NHIObjectFactory.h"
#include "NIBUtilities.h"
#include "NHIToolbar.h"
#include "NCGContext.h"
#include "NHIView.h"
#include "NHIWindow.h"





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NHIWindowList NHIWindow::mWindows;





//============================================================================
//		NHIWindow::NHIWindow : Constructor.
//----------------------------------------------------------------------------
NHIWindow::NHIWindow(const NString &nibName, const NString &resName)
{	HIWindowRef		theWindow;
	IBNibRef		nibRef;



	// Validate our parameters
	NN_ASSERT(nibName.IsNotEmpty());
	NN_ASSERT(resName.IsNotEmpty());



	// Create the window
	theWindow = NULL;
	
	nibRef = NIBUtilities::CreateNib(nibName);
	if (nibRef != NULL)
		{
		theWindow = NIBUtilities::CreateWindow(nibRef, resName);
		DisposeNibReference(nibRef);
		}



	// Initialize ourselves
	InitializeSelf(theWindow, true);
}





//============================================================================
//		NHIWindow::NHIWindow : Constructor.
//----------------------------------------------------------------------------
NHIWindow::NHIWindow(WindowClass theClass, WindowAttributes theAttributes, const HIRect &theRect)
{	HIWindowRef		theWindow;
	QDRect			qdRect;
	OSStatus		theErr;



	// Create the window
	NGeometryUtilities::ConvertRect(theRect, qdRect);

	theWindow = NULL;
	theErr    = CreateNewWindow(theClass, theAttributes, &qdRect, &theWindow);
	NN_ASSERT_NOERR(theErr);



	// Initialize ourselves
	InitializeSelf(theWindow, true);
}





//============================================================================
//		NHIWindow::NHIWindow : Constructor.
//----------------------------------------------------------------------------
NHIWindow::NHIWindow(HIWindowRef theWindow)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theWindow != NULL);



	// Retain the winow
	theErr  = RetainWindow(theWindow);
	NN_ASSERT_NOERR(theErr);



	// Initialize ourselves
	InitializeSelf(theWindow, false);
}





//============================================================================
//		NHIWindow::NHIWindow : Constructor.
//----------------------------------------------------------------------------
NHIWindow::NHIWindow(void)
{


	// Initialize ourselves
	InitializeSelf(NULL, false);
}





//============================================================================
//		NHIWindow::~NHIWindow : Destructor.
//----------------------------------------------------------------------------
NHIWindow::~NHIWindow(void)
{	OSStatus	theErr;



	// Remove our handler
	//
	// Since our window is deleted before us, we need to remove the handler
	// to ensure we don't receive any further events from the window.
	RemoveHandler();



	// Remove the window from the list
	mWindows.remove(this);



	// Clean up
	if (IsValid())
		{
		theErr = ReleaseWindow(mWindow);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHIWindow::IsValid : Is the window valid?
//----------------------------------------------------------------------------
bool NHIWindow::IsValid(void) const
{


	// Check our state
	return(mWindow != NULL);
}





//============================================================================
//		NHIWindow::GetHIWindow : Get the HIWindowRef.
//----------------------------------------------------------------------------
HIWindowRef NHIWindow::GetHIWindow(void) const
{


	// Get the window
	return(mWindow);
}





//============================================================================
//		NHIWindow::GetEventTarget : Get the event target.
//----------------------------------------------------------------------------
EventTargetRef NHIWindow::GetEventTarget(void) const
{	EventTargetRef	theTarget;



	// Get the event target
	theTarget = NULL;
	
	if (IsValid())
		{
		theTarget = GetWindowEventTarget(mWindow);
		NN_ASSERT(theTarget != NULL);
		}
	
	return(theTarget);
}





//============================================================================
//		NHIWindow::GetTitle : Get the title.
//----------------------------------------------------------------------------
NString NHIWindow::GetTitle(void) const
{	CFStringRef		cfString;
	NString			theTitle;
	OSStatus		theErr;



	// Get the title
	theErr = CopyWindowTitleAsCFString(mWindow, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		theTitle.Set(cfString);
	
	return(theTitle);
}





//============================================================================
//		NHIWindow::SetTitle : Set the title.
//----------------------------------------------------------------------------
void NHIWindow::SetTitle(const NString &theTitle)
{	OSStatus	theErr;



	// Set the title
	theErr = SetWindowTitleWithCFString(mWindow, theTitle);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetProxyIcon : Get the proxy icon.
//----------------------------------------------------------------------------
NIcon NHIWindow::GetProxyIcon(void) const
{	NIcon		theIcon;
	IconRef		iconRef;
	OSStatus	theErr;



	// Get the icon
	iconRef = NULL;
	theErr  = GetWindowProxyIcon(mWindow, &iconRef);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr && iconRef != NULL)
		theIcon = NIcon(iconRef);
	
	return(theIcon);
}





//============================================================================
//		NHIWindow::SetProxyIcon : Set the proxy icon.
//----------------------------------------------------------------------------
void NHIWindow::SetProxyIcon(const NIcon &theIcon)
{	OSStatus	theErr;



	// Set/remove the icon
	if (theIcon.IsValid())
		theErr = SetWindowProxyIcon(mWindow, theIcon);
	else
		theErr = RemoveWindowProxy(mWindow);

	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::SetProxyIcon : Set the proxy icon.
//----------------------------------------------------------------------------
void NHIWindow::SetProxyIcon(AliasHandle theAlias)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theAlias != NULL);



	// Set the icon
	theErr = SetWindowProxyAlias(mWindow, theAlias);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetDockTile : Get the Dock tile.
//----------------------------------------------------------------------------
NDockTile NHIWindow::GetDockTile(void) const
{	NDockTile		theTile(this);



	// Get the tile
	return(theTile);
}





//============================================================================
//		NHIWindow::IsVisible : Get the window visibility.
//----------------------------------------------------------------------------
bool NHIWindow::IsVisible(void) const
{


	// Get the visibility
	return(IsWindowVisible(mWindow));
}





//============================================================================
//		NHIWindow::SetVisible : Set the visibility.
//----------------------------------------------------------------------------
void NHIWindow::SetVisible(bool isVisible, bool doFade)
{


	// Set the visibility
	if (doFade)
		SetFrame(GetFrame(), kWindowContentRgn, kWindowFadeTransitionEffect,
				 isVisible ? kWindowShowTransitionAction : kWindowHideTransitionAction);
	else
		{
		if (isVisible)
			ShowWindow(mWindow);
		else
			HideWindow(mWindow);
		}
}





//============================================================================
//		NHIWindow::IsMinimized : Get the window minimized state.
//----------------------------------------------------------------------------
bool NHIWindow::IsMinimized(void) const
{


	// Get the minimized state
	return(IsWindowCollapsed(mWindow));
}





//============================================================================
//		NHIWindow::SetMinimized : Set the window minimized state.
//----------------------------------------------------------------------------
void NHIWindow::SetMinimized(bool isMinimized)
{	OSStatus	theErr;



	// Validate our parameters
	if (isMinimized)
		NN_ASSERT(IsWindowCollapsable(mWindow));



	// Set the minimized state
	theErr = CollapseWindow(mWindow, isMinimized);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::IsModified : Get the window modified state.
//----------------------------------------------------------------------------
bool NHIWindow::IsModified(void) const
{


	// Get the modified state
	return(IsWindowModified(mWindow));
}





//============================================================================
//		NHIWindow::SetModified : Set the window modified state.
//----------------------------------------------------------------------------
void NHIWindow::SetModified(bool isModified)
{	OSStatus	theErr;



	// Set the modified state
	theErr = SetWindowModified(mWindow, isModified);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::IsGrowBoxTransparent : Is the grow box transparent?
//----------------------------------------------------------------------------
bool NHIWindow::IsGrowBoxTransparent(void) const
{


	// Get the state
	if (GetAttributes() & kWindowResizableAttribute)
		return(HIGrowBoxViewIsTransparent(GetHIView(kHIViewWindowGrowBoxID)));
	
	return(false);
}





//============================================================================
//		NHIWindow::SetGrowBoxTransparent : Set the grow box transparency.
//----------------------------------------------------------------------------
void NHIWindow::SetGrowBoxTransparent(bool isTransparent)
{	OSStatus	theErr;



	// Set the state
	if (GetAttributes() & kWindowResizableAttribute)
		{
		theErr = HIGrowBoxViewSetTransparent(GetHIView(kHIViewWindowGrowBoxID), isTransparent);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NHIWindow::IsContentTransparent : Is the content transparent?
//----------------------------------------------------------------------------
bool NHIWindow::IsContentTransparent(void) const
{


	// Get the state
	return(mContentTransparent);
}





//============================================================================
//		NHIWindow::SetContentTransparent : Set the content transparency.
//----------------------------------------------------------------------------
void NHIWindow::SetContentTransparent(bool isTransparent)
{	OSStatus	theErr;



	// Update our state
	mContentTransparent = isTransparent;



	// Update the window
	if (isTransparent)
		{
		AddEvent(                     kEventClassWindow,  kEventWindowGetRegion);
		mDelegateContentView.AddEvent(kEventClassControl, kEventControlDraw);

		SetAttributes(kWindowNoShadowAttribute);
		SetFeatures(0, kWindowIsOpaque);
		}
	else
		{
		RemoveEvent(                     kEventClassWindow,  kEventWindowGetRegion);
		mDelegateContentView.RemoveEvent(kEventClassControl, kEventControlDraw);

		SetAttributes(0, kWindowNoShadowAttribute);
		SetFeatures(kWindowIsOpaque);
		}



	// Force the opaque shape to be recalculated
	theErr = ReshapeCustomWindow(*this);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::IsToolbarVisible : Is the toolbar visible?
//----------------------------------------------------------------------------
bool NHIWindow::IsToolbarVisible(void) const
{


	// Get the state
	return(IsWindowToolbarVisible(*this));
}





//============================================================================
//		NHIWindow::SetToolbarVisible : Set the toolbar visibility.
//----------------------------------------------------------------------------
void NHIWindow::SetToolbarVisible(bool isVisible, bool canAnimate)
{	OSStatus	theErr;



	// Set the state
	theErr = ShowHideWindowToolbar(*this, isVisible, canAnimate);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetContentColor : Get the window content color.
//----------------------------------------------------------------------------
NColor NHIWindow::GetContentColor(void) const
{	NColor		theColor = kColorWhite;
	RGBColor	rgbColor;
	OSStatus	theErr;



	// Get the color
	theErr = GetWindowContentColor(*this, &rgbColor);
	NN_ASSERT_NOERR(theErr);

	if (theErr == noErr)
		theColor.SetColor(rgbColor);
	
	return(theColor);
}





//============================================================================
//		NHIWindow::SetContentColor : Set the window content color.
//----------------------------------------------------------------------------
void NHIWindow::SetContentColor(const NColor &theColor)
{	RGBColor	rgbColor;
	OSStatus	theErr;



	// Set the color
	theColor.GetColor(rgbColor);
	
	theErr = SetWindowContentColor(*this, &rgbColor);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetKeyboardFocus : Get the keyboard focus.
//----------------------------------------------------------------------------
NHIView *NHIWindow::GetKeyboardFocus(void) const
{	ControlRef		theControl;
	NHIView			*theView;
	OSStatus		theErr;



	// Get the keyboard focus
	theView = NULL;
	theErr  = ::GetKeyboardFocus(*this, &theControl);
	
	if (theErr == noErr && theControl != NULL)
		theView = NHIViewUtilities::GetView(theControl);
	
	return(theView);
}





//============================================================================
//		NHIWindow::SetKeyboardFocus : Set the keyboard focus.
//----------------------------------------------------------------------------
void NHIWindow::SetKeyboardFocus(NHIView *theView, HIViewPartCode thePart)
{	OSStatus	theErr;



	// Set the keyboard focus
	//
	// We support the meta-part which sets the focus to the content (or initial
	// part in the view. This ensures that if SetKeyboardFocus is called on the
	// view that has the current focus, we won't "next-part" the focus away.
	if (thePart == kHIViewContentMetaPart)
		{
		theErr  = ::SetKeyboardFocus(*this, *theView, kHIViewFocusNoPart);
		theErr |= ::SetKeyboardFocus(*this, *theView, kHIViewFocusNextPart);
		}
	else
		theErr = ::SetKeyboardFocus(*this, *theView, thePart);

	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetDefaultButton : Get the default button.
//----------------------------------------------------------------------------
NHIView *NHIWindow::GetDefaultButton(void) const
{	ControlRef		theControl;
	NHIView			*theView;
	OSStatus		theErr;



	// Get the button
	theView = NULL;
	theErr  = GetWindowDefaultButton(*this, &theControl);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr && theControl != NULL)
		theView = NHIViewUtilities::GetView(theControl);
	
	return(theView);
}





//============================================================================
//		NHIWindow::SetDefaultButton : Set the default button.
//----------------------------------------------------------------------------
void NHIWindow::SetDefaultButton(NHIView *theView)
{	OSStatus	theErr;



	// Set the button
	theErr = SetWindowDefaultButton(*this, *theView);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetCancelButton : Get the cancel button.
//----------------------------------------------------------------------------
NHIView *NHIWindow::GetCancelButton(void) const
{	ControlRef		theControl;
	NHIView			*theView;
	OSStatus		theErr;



	// Get the button
	theView = NULL;
	theErr  = GetWindowCancelButton(*this, &theControl);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr && theControl != NULL)
		theView = NHIViewUtilities::GetView(theControl);
	
	return(theView);
}





//============================================================================
//		NHIWindow::SetCancelButton : Set the cancel button.
//----------------------------------------------------------------------------
void NHIWindow::SetCancelButton(NHIView *theView)
{	OSStatus	theErr;



	// Set the button
	theErr = SetWindowCancelButton(*this, *theView);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetAlpha : Get the window alpha.
//----------------------------------------------------------------------------
float NHIWindow::GetAlpha(void) const
{	float		theAlpha;
	OSStatus	theErr;



	// Get the window alpha
	theAlpha = 1.0f;
	theErr   = GetWindowAlpha(*this, &theAlpha);
	NN_ASSERT_NOERR(theErr);
	
	return(theAlpha);
}





//============================================================================
//		NHIWindow::SetAlpha : Set the window alpha.
//----------------------------------------------------------------------------
void NHIWindow::SetAlpha(float theAlpha)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theAlpha >= 0.0f && theAlpha <= 1.0f);



	// Set the window alpha
	theErr = SetWindowAlpha(*this, theAlpha);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetAttributes : Get the window attributes.
//----------------------------------------------------------------------------
WindowAttributes NHIWindow::GetAttributes(void) const
{	WindowAttributes	theAttributes;
	OSStatus			theErr;



	// Get the attributes
	theAttributes = kWindowNoAttributes;
	
	theErr = GetWindowAttributes(mWindow, &theAttributes);
	NN_ASSERT_NOERR(theErr);
	
	return(theAttributes);
}





//============================================================================
//		NHIWindow::SetAttributes : Set the window attributes.
//----------------------------------------------------------------------------
void NHIWindow::SetAttributes(WindowAttributes setThese, WindowAttributes clearThese)
{	bool		wasTransparent;
	OSStatus	theErr;



	// Get the state we need
	wasTransparent = IsGrowBoxTransparent();



	// Set the attributes
	theErr = ChangeWindowAttributes(mWindow, setThese, clearThese);
	NN_ASSERT_NOERR(theErr);



	// Restore the window state
	//
	// Work-around rdar://4687777, where enabling the unified toolbar attribute on a
	// resizable window removes the transparent growbox appearance on Mac OS X 10.4.
	if (setThese & kWindowUnifiedTitleAndToolbarAttribute)
		SetGrowBoxTransparent(wasTransparent);
}





//============================================================================
//		NHIWindow::GetFeatures : Get the window features.
//----------------------------------------------------------------------------
UInt64 NHIWindow::GetFeatures(void) const
{	UInt32		theFeatures;
	OSStatus	theErr;



	// Validate our state
	//
	// HIWindowChangeFeatures takes a UInt64, so that's what we expose to
	// callers even though GetWindowFeatures can only return 32 bits.
	NN_ASSERT(sizeof(theFeatures) == sizeof(UInt32));



	// Get the features
	theFeatures = kWindowNoFeatures;
	
	theErr = GetWindowFeatures(mWindow, &theFeatures);
	NN_ASSERT_NOERR(theErr);
	
	return(theFeatures);
}





//============================================================================
//		NHIWindow::SetFeatures : Set the window features.
//----------------------------------------------------------------------------
void NHIWindow::SetFeatures(UInt64 setThese, UInt64 clearThese)
{	OSStatus	theErr;



	// Set the features
	theErr = HIWindowChangeFeatures(mWindow, setThese, clearThese);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetHelpTag : Get the help tag.
//----------------------------------------------------------------------------
NHelpTag NHIWindow::GetHelpTag(void) const
{	NHelpTag	theTag(this);



	// Get the help tag
	return(theTag);
}





//============================================================================
//		NHIWindow::SetHelpTag : Set the help tag.
//----------------------------------------------------------------------------
void NHIWindow::SetHelpTag(const NHelpTag &theTag)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the help tag
	theErr = HMSetWindowHelpContent(*this, theTag);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetFrame : Get the window frame.
//----------------------------------------------------------------------------
HIRect NHIWindow::GetFrame(WindowRegionCode theRegion)
{	HIRect		theRect;
	OSStatus	theErr;
	QDRect		qdRect;



	// Get the window bounds
	theRect = kHIRectZero;
	theErr  = GetWindowBounds(mWindow, theRegion, &qdRect);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		NGeometryUtilities::ConvertRect(qdRect, theRect);
	
	return(theRect);
}





//============================================================================
//		NHIWindow::SetFrame : Set the window frame.
//----------------------------------------------------------------------------
void NHIWindow::SetFrame(const HIRect				&theRect,
							WindowRegionCode		theRegion,
							WindowTransitionEffect	theEffect,
							WindowTransitionAction	theAction,
							EventTime				theDuration,
							bool					doAsync,
							void					*userData)
{	TransitionWindowOptions		theOptions;
	HIRect						theBounds;
	QDRect						qdRect;
	OSStatus					theErr;



	// Set the frame
	switch (theEffect) {
		case kWindowNoTransitionEffect:
			// Set the bounds
			NGeometryUtilities::ConvertRect(theRect, qdRect);
			
			theErr = SetWindowBounds(mWindow, theRegion, &qdRect);
			NN_ASSERT_NOERR(theErr);
			break;


		default:
			// Calculate the structure region
			//
			// TransitionWindowWIthOptions requires the structure region.
			theBounds = theRect;

			if (theRegion == kWindowContentRgn)
				{
				theErr = GetWindowStructureWidths(mWindow, &qdRect);
				NN_ASSERT_NOERR(theErr);
				
				if (theErr == noErr)
					{
					theBounds.origin.x    -= qdRect.left;
					theBounds.origin.y    -= qdRect.top;
					theBounds.size.width  += qdRect.left   + qdRect.right;
					theBounds.size.height += qdRect.bottom + qdRect.top;
					}
				}



			// Prepare the options
			memset(&theOptions, 0x00, sizeof(theOptions));
			
			theOptions.version  = 0;
			theOptions.duration = theDuration;
			theOptions.userData = userData;



			// Transition the window
			theErr = TransitionWindowWithOptions(mWindow, theEffect, theAction, &theBounds, doAsync, &theOptions);
			NN_ASSERT_NOERR(theErr);
			break;
		}
}





//============================================================================
//		NHIWindow::SetPosition : Set the window position.
//----------------------------------------------------------------------------
void NHIWindow::SetPosition(WindowPositionMethod theMethod, NHIWindow *theParent)
{	HIWindowRef		parentWindow;
	OSStatus		theErr;



	// Find the parent
	parentWindow = NULL;
	
	if (theParent != NULL)
		parentWindow = theParent->GetHIWindow();



	// Position the window
	theErr = RepositionWindow(mWindow, parentWindow, theMethod);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetResizeLimits : Get the resize limits.
//----------------------------------------------------------------------------
void NHIWindow::GetResizeLimits(HISize *minSize, HISize *maxSize) const
{	OSStatus	theErr;



	// Get the resize limits
	theErr = GetWindowResizeLimits(mWindow, minSize, maxSize);
	NN_ASSERT_NOERR(theErr);



	// Handle failure
	if (theErr != noErr)
		{
		if (minSize != NULL)
			*minSize = kHISizeZero;

		if (maxSize != NULL)
			*maxSize = kHISizeZero;
		}
}





//============================================================================
//		NHIWindow::SetResizeLimits : Set the resize limits.
//----------------------------------------------------------------------------
void NHIWindow::SetResizeLimits(const HISize *minSize, const HISize *maxSize)
{	OSStatus	theErr;



	// Set the resize limits
	theErr = SetWindowResizeLimits(mWindow, minSize, maxSize);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::GetIdealSize : Get the ideal size.
//----------------------------------------------------------------------------
HISize NHIWindow::GetIdealSize(void) const
{


	// Get the ideal size
	return(mIdealSize);
}





//============================================================================
//		NHIWindow::SetIdealSize : Set the ideal size.
//----------------------------------------------------------------------------
void NHIWindow::SetIdealSize(const HISize &theSize)
{


	// Set the ideal size
	mIdealSize = theSize;
}





//============================================================================
//		NHIWindow::Select : Select the window.
//----------------------------------------------------------------------------
void NHIWindow::Select(bool makeVisible)
{


	// Select the window
	SelectWindow(mWindow);
	
	if (makeVisible)
		SetVisible(true);
}





//============================================================================
//		NHIWindow::Zoom : Zoom the window.
//----------------------------------------------------------------------------
void NHIWindow::Zoom(WindowPartCode thePart)
{	NCarbonEvent	theEvent(kEventClassWindow, kEventWindowZoom);
	OSStatus		theErr;



	// Zoom the window
	//
	// We zoom the window with an event, to allow the standard handler to decide
	// how to zoom (as it does when the zoom widget/Window menu item is used).
	theEvent.SetHIWindow(*this);
	theEvent.SetParameter(kEventParamWindowPartCode, typeWindowPartCode, sizeof(thePart), &thePart);
	
	theErr = theEvent.SendToHIWindow(*this);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::BeginModalEventLoop : Begin a modal event loop.
//----------------------------------------------------------------------------
void NHIWindow::BeginModalEventLoop(void)
{	OSStatus	theErr;



	// Run the event loop
	theErr = RunAppModalLoopForWindow(*this);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::EndModalEventLoop : End a modal event loop.
//----------------------------------------------------------------------------
void NHIWindow::EndModalEventLoop(void)
{	OSStatus	theErr;



	// Quit the event loop
	theErr = QuitAppModalLoopForWindow(*this);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::BeginSheet : Begin a sheet.
//----------------------------------------------------------------------------
void NHIWindow::BeginSheet(NHIWindow *theSheet)
{	OSStatus	theErr;



	// Validate our parameters and state
	NN_ASSERT(theSheet != NULL);
	NN_ASSERT(  mSheet == NULL);



	// Show the sheet
	theErr = ShowSheetWindow(*theSheet, mWindow);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		mSheet = theSheet;
}





//============================================================================
//		NHIWindow::EndSheet : End a sheet.
//----------------------------------------------------------------------------
void NHIWindow::EndSheet(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mSheet != NULL);



	// Hide the sheet
	theErr = HideSheetWindow(*mSheet);
	NN_ASSERT_NOERR(theErr);
	
	mSheet = NULL;
}





//============================================================================
//		NHIWindow::HasSheet : Does the window have a sheet?
//----------------------------------------------------------------------------
bool NHIWindow::HasSheet(void) const
{	bool	hasSheet;



	// Check our state
	hasSheet = HIWindowIsDocumentModalTarget(mWindow, NULL);
	
	return(hasSheet);
}





//============================================================================
//		NHIWindow::GetToolbar : Get the toolbar.
//----------------------------------------------------------------------------
NHIToolbar *NHIWindow::GetToolbar(void) const
{


	// Get the toolbar
	return(mToolbar);
}





//============================================================================
//		NHIWindow::SetToolbar : Set the toolbar.
//----------------------------------------------------------------------------
void NHIWindow::SetToolbar(NHIToolbar *theToolbar, bool makeVisible)
{	OSStatus	theErr;



	// Update our state
	mToolbar = theToolbar;



	// Update the window state
	theErr = SetWindowToolbar(*this, *theToolbar);
	NN_ASSERT_NOERR(theErr);
	
	if (mToolbar != NULL)
		SetAttributes(kWindowToolbarButtonAttribute);
	else
		SetAttributes(kWindowNoAttributes, kWindowToolbarButtonAttribute);



	// Show the toolbar
	if (mToolbar != NULL && makeVisible)
		SetToolbarVisible(true);
}





//============================================================================
//		NHIWindow::GetWindowGroup : Get the window group.
//----------------------------------------------------------------------------
WindowGroupRef NHIWindow::GetWindowGroup(void) const
{


	// Get the window group
	return(::GetWindowGroup(*this));
}





//============================================================================
//		NHIWindow::SetWindowGroup : Set the window group.
//----------------------------------------------------------------------------
void NHIWindow::SetWindowGroup(WindowGroupRef theGroup)
{	OSStatus	theErr;



	// Set the window group
	theErr = ::SetWindowGroup(*this, theGroup);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIWindow::SetWindowGroup : Set the window group.
//----------------------------------------------------------------------------
void NHIWindow::SetWindowGroup(WindowClass theClass)
{


	// Set the window group
	SetWindowGroup(GetWindowGroupOfClass(theClass));
}





//============================================================================
//		NHIWindow::GetHIView : Get an HIViewRef.
//----------------------------------------------------------------------------
HIViewRef NHIWindow::GetHIView(const HIViewID &theID) const
{	HIViewRef	hiView, hiRoot;
	OSStatus	theErr;



	// Get the root
	hiRoot = HIViewGetRoot(mWindow);



	// Get the view
	hiView = NULL;
	
	if (theID.signature == kHIViewWindowRootID.signature && theID.id == kHIViewWindowRootID.id)
		hiView = hiRoot;
	else
		theErr = HIViewFindByID(hiRoot, theID, &hiView);

	return(hiView);
}





//============================================================================
//		NHIWindow::GetView : Get a view.
//----------------------------------------------------------------------------
NHIView *NHIWindow::GetView(const HIViewID &theID) const
{


	// Get the view
	return(NHIViewUtilities::GetView(GetHIView(theID)));
}





//============================================================================
//		NHIWindow::GetView : Get a view.
//----------------------------------------------------------------------------
NHIView *NHIWindow::GetView(OSType theSignature, SInt32 theID) const
{	HIViewID	viewID;



	// Get the view
	viewID.signature = theSignature;
	viewID.id        = theID;

	return(GetView(viewID));
}





//============================================================================
//		NHIWindow::operator HIWindowRef : HIWindowRef-cast operator.
//----------------------------------------------------------------------------
NHIWindow::operator HIWindowRef(void) const
{


	// Get the window
	return(mWindow);
}





//============================================================================
//		NHIWindow::GetWindows : Get the windows.
//----------------------------------------------------------------------------
NHIWindowList NHIWindow::GetWindows(void)
{


	// Get the windows
	return(mWindows);
}





//============================================================================
//		NHIWindow::DoEventWindowGetIdealSize : Handle kEventWindowGetIdealSize.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NHIWindow::DoEventWindowGetIdealSize(NCarbonEvent &theEvent)
{	HIPoint		thePoint;
	OSStatus	theErr;



	// Get the ideal size
	//
	// If no size has been specified, we let the system generate one.
	theErr = eventNotHandledErr;
	
	if (mIdealSize != kHISizeZero)
		{
		thePoint.x = mIdealSize.width;
		thePoint.y = mIdealSize.height;

		theErr = theEvent.SetParameterHIPoint(kEventParamDimensions, thePoint);
		NN_ASSERT_NOERR(theErr);
		}
	
	return(theErr);
}





//============================================================================
//		NHIWindow::DoEventWindowGetRegion : Handle kEventWindowGetRegion.
//----------------------------------------------------------------------------
OSStatus NHIWindow::DoEventWindowGetRegion(NCarbonEvent &theEvent)
{	WindowRegionCode		theCode;
	RgnHandle				qdRgn;
	OSStatus				theErr;



	// Validate our state
	NN_ASSERT(IsContentTransparent());



	// Get the state we need
	theCode = kWindowTitleBarRgn;
	theErr  = theEvent.GetParameter(kEventParamWindowRegionCode, typeWindowRegionCode, sizeof(theCode), &theCode);
	NN_ASSERT_NOERR(theErr);



	// Get the region
	switch (theCode) {
		case kWindowOpaqueRgn:
			// Override the opaque region
			if (theEvent.GetParameter(kEventParamRgnHandle, typeQDRgnHandle, sizeof(qdRgn), &qdRgn) == noErr)
				SetEmptyRgn(qdRgn);
			break;

		default:
			// And leave the rest unchanged
			theErr = eventNotHandledErr;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NHIWindow::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NHIWindow::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext		cgContext(theEvent);
	HIRect			theRect;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsContentTransparent());



	// Get the state we need
	theErr = HIViewGetBounds(theEvent.GetHIView(), &theRect);
	NN_ASSERT_NOERR(theErr);



	// Clear the view
	cgContext.ClearRect(theRect);

	return(noErr);
}





//============================================================================
//		NHIWindow::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NHIWindow::InitializeSelf(HIWindowRef theWindow, bool applyDefaults)
{	const EventTypeSpec	theEvents[] = { { kEventClassWindow, kEventWindowGetIdealSize },
										{ kEventClassNone,   kEventKindNone           } };
	OSStatus	theErr;



	// Initialize ourselves
	mWindow             = theWindow;
	mIdealSize          = kHISizeZero;
	mContentTransparent = false;
	
	mSheet   = NULL;
	mToolbar = NULL;



	// Register for events
	if (IsValid())
		{
		theErr = InstallHandler(mWindow, theEvents);
		NN_ASSERT_NOERR(theErr);

		mDelegateContentView.InstallHandler(GetHIView(kHIViewWindowContentID));
		mDelegateContentView.SetEventDelegate(this);
		}



	// Configure the window
	//
	// Unfortunately IB doesn't provide a way to tag windows with properties,
	// so we assume some standard defaults are always desireable.
	//
	// These can only be applied to windows we create; if we're passed a window
	// which was created externally, we must leave it unchanged.
	if (IsValid() && applyDefaults)
		{
		if (GetAttributes() & kWindowResizableAttribute)
			SetGrowBoxTransparent(true);
		
		theErr = SetAutomaticControlDragTrackingEnabledForWindow(*this, true);
		NN_ASSERT_NOERR(theErr);
		}



	// Add the window to the list
	mWindows.push_back(this);
}







