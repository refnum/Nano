/*	NAME:
		NColorPanel.cpp

	DESCRIPTION:
		Color panel.

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
#include "NColorPanel.h"





//============================================================================
//		Internal class definition
//----------------------------------------------------------------------------
@interface NColorDelegate : NSObject
{
	BOOL	mIsTracking;
}

+ (NColorDelegate *) sharedDelegate;
- (id) init;
- (BOOL) windowShouldClose:(id) sender;
- (void) colorChanged;
- (void) finishTrack:(id) ignore;
- (void) attachToPanel:(NSColorPanel *) nsPanel;

@end





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NSColorPanel   *NColorPanel::mPanel;
NMsgBroadcaster NColorPanel::mBroadcaster;





//============================================================================
//		NColorDelegate::sharedDelegate : Get the shared delegate.
//----------------------------------------------------------------------------
@implementation NColorDelegate

+ (NColorDelegate *) sharedDelegate;
{	static NColorDelegate	*sSharedDelegate = NULL;



	// Get the delegate
	if (sSharedDelegate == NULL)
		sSharedDelegate = [[NColorDelegate alloc] init];

	return(sSharedDelegate);
}





//============================================================================
//		NColorDelegate::init : Initializer.
//----------------------------------------------------------------------------
- (id) init
{


	// Initialize ourselves
    [super init];
	
	mIsTracking = NO;

    return(self);
}





//============================================================================
//		NColorDelegate::windowShouldClose : Should the window close?
//----------------------------------------------------------------------------
- (BOOL) windowShouldClose:(id) sender
{


	// Ignore unused parameters
	(void) sender;



	// Inform the listeners
	NColorPanel::GetBroadcaster()->BroadcastMessage(kMsgColorPanelClosed);

    return(YES);
}





//============================================================================
//		NColorDelegate::colorChanged : The color has been changed.
//----------------------------------------------------------------------------
- (void) colorChanged
{


	// Begin tracking
	//
	// Although NSColorPanel does not provide any tracking begin/end notifications
	// through its delegate, we can provide these for observers by using the first
	// color change to mark the start of the track.
	//
	// Since the color panel enters a mouse tracking run loop while the mouse is
	// down, by queing up a delayed selector on the main run loop we can use that
	// callback to signal the end of the track.
	if (!mIsTracking && Button())
		{
		mIsTracking = YES;

		[NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(finishTrack:) object:nil];
		[self performSelector:@selector(finishTrack:) withObject:nil afterDelay:0.0];

		NColorPanel::GetBroadcaster()->BroadcastMessage(kMsgColorPanelTrackBegin);
		}



	// Inform the listeners
	NColorPanel::GetBroadcaster()->BroadcastMessage(kMsgColorPanelChanged);
}





//============================================================================
//		NColorDelegate::finishTrack : Finish tracking the mouse.
//----------------------------------------------------------------------------
- (void) finishTrack: (id) ignore
{


	// Suppress compiler warning
	(void) ignore;



	// Finish tracking
	if (mIsTracking)
		{
		mIsTracking = NO;

		NColorPanel::GetBroadcaster()->BroadcastMessage(kMsgColorPanelTrackEnd);
		}
}





//============================================================================
//		NColorDelegate::attachToPanel : Attach to the color panel.
//----------------------------------------------------------------------------
- (void) attachToPanel: (NSColorPanel *) nsPanel
{


	// Attach to the color panel
	[nsPanel   setDelegate:self];
	[nsPanel     setTarget:self];
	[nsPanel setContinuous:TRUE];
	[nsPanel     setAction:@selector(colorChanged)];
}

@end // NColorDelegate





//============================================================================
//		NColorPanel::IsVisible : Is the panel visible?
//----------------------------------------------------------------------------
bool NColorPanel::IsVisible(void)
{	StAutoReleasePool	thePool;



	// Initialize the panel
	InitializePanel();



	// Get the visibility
	return([mPanel isVisible]);
}





//============================================================================
//		NColorPanel::SetVisible : Set the panel visibility.
//----------------------------------------------------------------------------
void NColorPanel::SetVisible(bool isVisible)
{	StAutoReleasePool	thePool;



	// Initialize the panel
	InitializePanel();



	// Set the visibility
	if (isVisible)
		[mPanel orderFront:nil];
	else
		[mPanel orderOut:nil];
}





//============================================================================
//		NColorPanel::IsAlphaVisible : Is the alpha visible?
//----------------------------------------------------------------------------
bool NColorPanel::IsAlphaVisible(void)
{	StAutoReleasePool	thePool;



	// Initialize the panel
	InitializePanel();



	// Get the visibility
	return([mPanel showsAlpha]);
}





//============================================================================
//		NColorPanel::SetAlphaVisible : Set the alpha visibility.
//----------------------------------------------------------------------------
void NColorPanel::SetAlphaVisible(bool isVisible)
{	StAutoReleasePool	thePool;



	// Initialize the panel
	InitializePanel();



	// Set the visibility
	[mPanel setShowsAlpha:isVisible];
}





//============================================================================
//		NColorPanel::GetColor : Get the color.
//----------------------------------------------------------------------------
NColor NColorPanel::GetColor(void)
{	NSColor				*nsColor;
	NColor				theColor;
	StAutoReleasePool	thePool;



	// Initialize the panel
	InitializePanel();



	// Get the color
	nsColor = [[mPanel color] colorUsingColorSpaceName:NSCalibratedRGBColorSpace];

	theColor.SetRed(  [nsColor   redComponent]);
	theColor.SetGreen([nsColor greenComponent]);
	theColor.SetBlue( [nsColor  blueComponent]);
	theColor.SetAlpha([nsColor alphaComponent]);

	return(theColor);
}





//============================================================================
//		NColorPanel::SetColor : Set the color.
//----------------------------------------------------------------------------
void NColorPanel::SetColor(const NColor &theColor)
{	StAutoReleasePool	thePool;



	// Initialize the panel
	InitializePanel();



	// Set the color
	[mPanel setColor:[NSColor colorWithCalibratedRed:theColor.GetRed()
											   green:theColor.GetGreen()
												blue:theColor.GetBlue()
											   alpha:theColor.GetAlpha()]];
}





//============================================================================
//		NColorPanel::GetBroadcaster : Get the broadcaster.
//----------------------------------------------------------------------------
NMsgBroadcaster *NColorPanel::GetBroadcaster(void)
{


	// Initialize the panel
	InitializePanel();



	// Get the broadcaster
	return(&mBroadcaster);
}





//============================================================================
//		NColorPanel::InitializePanel : Initialize the panel.
//----------------------------------------------------------------------------
#pragma mark -
void NColorPanel::InitializePanel(void)
{	StAutoReleasePool	thePool;



	// Initialize the panel
	if (mPanel == NULL)
		{
		// Initialize our state
		mPanel = [NSColorPanel sharedColorPanel];



		// Initialize the delegate
		[[NColorDelegate sharedDelegate] attachToPanel:mPanel];
		}
}


