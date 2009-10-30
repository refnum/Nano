/*	NAME:
		NHIWindow.h

	DESCRIPTION:
		HIWindowRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHIWINDOW_HDR
#define NHIWINDOW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <list>

#include "NCarbonEventHandler.h"
#include "NDockTile.h"
#include "NHelpTag.h"
#include "NString.h"
#include "NColor.h"
#include "NIcon.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Misc
static const UInt64					kWindowNoFeatures						= 0;
static const WindowTransitionEffect	kWindowNoTransitionEffect				= 0;

static const HIViewID kHIViewWindowRootID									= { 'Nano', 'root' };


// Attributes
//
// MacWindows.h does not define kWindowUnifiedTitleAndToolbarAttribute until
// 10.5, however it is also available on 10.4.
//
// See http://developer.apple.com/qa/qa2005/qa1423.html for more information.
#if NN_SDK_VERSION < NN_SDK_VERSION_10_5
static const WindowAttributes kWindowUnifiedTitleAndToolbarAttribute		= (1 << 7);
#endif





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NHIView;
class NHIWindow;
class NHIToolbar;


// Lists
typedef std::list<NHIWindow*>									NHIWindowList;
typedef NHIWindowList::iterator									NHIWindowListIterator;
typedef NHIWindowList::const_iterator							NHIWindowListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIWindow : public NCarbonEventHandler {
public:
										NHIWindow(const NString &nibName, const NString &resName);
										NHIWindow(WindowClass theClass, WindowAttributes theAttributes, const HIRect &theRect);
										NHIWindow(HIWindowRef theWindow);

										NHIWindow(void);
	virtual								~NHIWindow(void);


	// Is the window valid?
	bool								IsValid(void) const;
	

	// Get the HIWindowRef
	HIWindowRef							GetHIWindow(void) const;


	// Get the event target
	EventTargetRef						GetEventTarget(void) const;


	// Get/set the title
	NString								GetTitle(void) const;
	void								SetTitle(const NString &theTitle);


	// Get/set the proxy icon
	NIcon								GetProxyIcon(void) const;
	void								SetProxyIcon(const NIcon &theIcon);
	void								SetProxyIcon(AliasHandle theAlias);


	// Get the Dock tile
	NDockTile							GetDockTile(void) const;


	// Get the window class
	WindowClass							GetWindowClass(void) const;


	// Get/set the visibility
	bool								IsVisible(void) const;
	void								SetVisible( bool isVisible = true,
													bool doFade    = false,
													bool doAsync   = true);


	// Get/set the minimised state
	bool								IsMinimized(void) const;
	void								SetMinimized(bool isMinimized);


	// Get/set the modified state
	bool								IsModified(void) const;
	void								SetModified(bool isModified);


	// Get/set the grow box transparency
	bool								IsGrowBoxTransparent(void) const;
	void								SetGrowBoxTransparent(bool isTransparent);


	// Get/set the content transparency
	bool								IsContentTransparent(void) const;
	void								SetContentTransparent(bool isTransparent);
	

	// Get/set the toolbar visibility
	bool								IsToolbarVisible(void) const;
	void								SetToolbarVisible(bool isVisible, bool canAnimate=true);


	// Get/set the attributes
	WindowAttributes					GetAttributes(void) const;
	void								SetAttributes(WindowAttributes setThese, WindowAttributes clearThese=kWindowNoAttributes);


	// Get/set the features
	UInt64								GetFeatures(void) const;
	void								SetFeatures(UInt64 setThese, UInt64 clearThese=0);


	// Get/set the help tag
	NHelpTag							GetHelpTag(void) const;
	void								SetHelpTag(const NHelpTag &theTag);


	// Get/set the window frame
	HIRect								GetFrame(WindowRegionCode				theRegion   = kWindowContentRgn);
	void								SetFrame(const HIRect					&theRect,
													WindowRegionCode			theRegion   = kWindowContentRgn,
													WindowTransitionEffect		theEffect   = kWindowNoTransitionEffect,
													WindowTransitionAction		theAction   = kWindowMoveTransitionAction,
													EventTime					theDuration = 0.0f,
													bool						doAsync     = true,
													void						*userData   = NULL);


	// Set the position
	void								SetPosition(WindowPositionMethod theMethod, NHIWindow *theParent=NULL);


	// Constrain the position
	void								ConstrainPosition(WindowRegionCode			theRegion   = kWindowStructureRgn,
															WindowConstrainOptions	theOptions  = kWindowConstrainStandardOptions,
															const HIRect			*screenRect = NULL,
															      HIRect			*structRect = NULL);


	// Get/set the resize limits
	void								GetResizeLimits(      HISize *minSize,       HISize *maxSize) const;
	void								SetResizeLimits(const HISize *minSize, const HISize *maxSize);


	// Get/set the ideal size
	//
	// When zoomed, a window will toggle between the user size (the user's last
	// resize) and the ideal size (the optimal size for the window's content).
	HISize								GetIdealSize(void) const;
	void								SetIdealSize(const HISize &theSize);
	

	// Select the window
	void								Select(bool makeVisible=true);


	// Zoom the window
	void								Zoom(WindowPartCode thePart=inZoomOut);


	// Begin/end a modal event loop
	void								BeginModalEventLoop(void);
	void								EndModalEventLoop(  void);
	
	
	// Begin/end a sheet
	void								BeginSheet(NHIWindow *theSheet);
	void								EndSheet(void);
	
	
	// Does the window have a sheet?
	bool								HasSheet(void) const;


	// Get/set the toolbar
	NHIToolbar							*GetToolbar(void) const;
	void								SetToolbar(NHIToolbar *theToolbar, bool makeVisible=true);


	// Get/set the content color
	NColor								GetContentColor(void) const;
	void								SetContentColor(const NColor &theColor);


	// Get/set the keyboard focus
	NHIView								*GetKeyboardFocus(void) const;
	void								 SetKeyboardFocus(NHIView *theView, HIViewPartCode thePart=kHIViewContentMetaPart);


    // Get/set the default button
    NHIView                             *GetDefaultButton(void) const;
    void                                 SetDefaultButton(NHIView *theView);


    // Get/set the cancel button
    NHIView                             *GetCancelButton(void) const;
    void                                 SetCancelButton(NHIView *theView);


	// Get/set the window alpha
	float								GetAlpha(void) const;
	void								SetAlpha(float theAlpha);


	// Get/set the activation scope
	WindowActivationScope				GetActivationScope(void) const;
	void								SetActivationScope(WindowActivationScope theScope);


	// Get/set the window group
	WindowGroupRef						GetWindowGroup(void) const;
	void								SetWindowGroup(WindowGroupRef theGroup);
	void								SetWindowGroup(WindowClass    theClass);


	// Get a view
	HIViewRef							GetHIView(const HIViewID &theID) const;
	NHIView								*GetView( const HIViewID &theID) const;
	NHIView								*GetView(OSType theSignature, SInt32 theID=0) const;


	// Operators
	operator							HIWindowRef(void) const;


	// Get the windows
	static NHIWindowList				GetWindows(void);


protected:
	// Handle events
	OSStatus							DoEventWindowGetIdealSize(NCarbonEvent &theEvent);
	OSStatus							DoEventWindowGetRegion(   NCarbonEvent &theEvent);
	OSStatus							DoEventControlDraw(       NCarbonEvent &theEvent);
	OSStatus							DoEventNAppGetUndoManager(NCarbonEvent &theEvent);


private:
	void								InitializeSelf(HIWindowRef theWindow, bool applyDefaults);


private:
	HIWindowRef							mWindow;
	HISize								mIdealSize;
	bool								mContentTransparent;

	NHIWindow							*mSheet;
	NHIToolbar							*mToolbar;
	NCarbonEventHandler					mDelegateContentView;
	
	static NHIWindowList				mWindows;
};





#endif // NHIWINDOW_HDR


