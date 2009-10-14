/*	NAME:
		NColorPicker.cpp

	DESCRIPTION:
		Color picker view.

		Carbon does not provide an alpha-aware color picker (rdar://4297601),
		and so we need to use the Cocoa color picker panel API.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPasteboard.h"
#include "NMathUtilities.h"
#include "NCGContext.h"
#include "NColorPicker.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagColor										= 'colr';
static const OSType kTagShowAlpha									= 'alph';


// Misc
static const float  kChipInset										= 5.0f;
static const UInt32 kPickCommand									= 'CPik';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NColorPicker, "com.nano.ncolorpicker", kHIBevelButtonClassID);





//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
@class NPickerDelegate;

NPickerDelegate *gPickerDelegate;
NColorPicker    *gActivePicker;





//============================================================================
//		Internal class definition
//----------------------------------------------------------------------------
@interface NPickerDelegate : NSObject
{
	BOOL	mIsTracking;
}

- (id)   init;
- (BOOL) windowShouldClose:(id) sender;
- (void) colorChanged;
- (void) finishTrack: (id) ignore;
- (void) setActivePicker:(NColorPicker *) thePicker;
- (void) setColor:(NColor *) theColor;

@end





//============================================================================
//		NPickerDelegate::init : Initializer.
//----------------------------------------------------------------------------
@implementation NPickerDelegate

- (id) init
{


	// Initialize ourselves
    [super init];
	
	mIsTracking = NO;

    return(self);
}





//============================================================================
//		NPickerDelegate::windowShouldClose : Should the window close?
//----------------------------------------------------------------------------
- (BOOL) windowShouldClose:(id) sender
{


	// Ignore unused parameters
	(void) sender;



	// Update our state
	[self setActivePicker:NULL];

    return YES;
}





//============================================================================
//		NPickerDelegate::colorChanged : The color has been changed.
//----------------------------------------------------------------------------
- (void) colorChanged
{	NSColorPanel	*nsPanel;
	NSColor			*nsColor;
	NColor			theColor;



	// Get the color
	nsPanel = [NSColorPanel sharedColorPanel];
	nsColor = [[nsPanel color] colorUsingColorSpaceName:NSCalibratedRGBColorSpace];

	theColor.SetRed(  [nsColor   redComponent]);
	theColor.SetGreen([nsColor greenComponent]);
	theColor.SetBlue( [nsColor  blueComponent]);
	theColor.SetAlpha([nsColor alphaComponent]);



	// Begin tracking
	//
	// Although NColorPanel does not provide any tracking begin/end notifications
	// to its delegate, we can provide these to our observers by using the first
	// color change to mark the start of the track.
	//
	// Since the color panel enters a mouse tracking run loop while the mouse is
	// down, by queing up a delayed selector on the main run loop we can use that
	// selector to mark the end of the track.
	if (!mIsTracking && Button())
		{
		mIsTracking = YES;

		[NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(finishTrack:) object:nil];
		[self performSelector:@selector(finishTrack:) withObject:nil afterDelay:0.0];

		if (gActivePicker != NULL)
			gActivePicker->BroadcastMessage(kMsgColorPickerTrackBegin, gActivePicker);
		}



	// Update the picker
	if (gActivePicker != NULL)
		gActivePicker->SetColor(theColor, true);
}





//============================================================================
//		NPickerDelegate::finishTrack : Finish tracking the mouse.
//----------------------------------------------------------------------------
- (void) finishTrack: (id) ignore
{


	// Suppress compiler warning
	(void) ignore;



	// Finish tracking
	if (mIsTracking)
		{
		mIsTracking = NO;

		if (gActivePicker != NULL)
			gActivePicker->BroadcastMessage(kMsgColorPickerTrackEnd, gActivePicker);
		}
}





//============================================================================
//		NPickerDelegate::setActivePicker : Set the active color picker.
//----------------------------------------------------------------------------
-(void) setActivePicker:(NColorPicker *) thePicker
{	NSColorPanel	*nsPanel;
	NColor			theColor;



	// Validate our state
	NN_ASSERT(!mIsTracking);
	


	// Deselect the existing picker
	if (gActivePicker != NULL)
		gActivePicker->SetCurrentValue(0);



	// Select the new picker
	gActivePicker = thePicker;

	if (gActivePicker != NULL)
		gActivePicker->SetCurrentValue(1);



	// Show/hide the panel
	nsPanel = [NSColorPanel sharedColorPanel];

	if (gActivePicker != NULL)
		{
		theColor = gActivePicker->GetColor();
		[self setColor:&theColor];
	
		[nsPanel setShowsAlpha: gActivePicker->GetShowAlpha()];
		[nsPanel   setDelegate:self];
		[nsPanel     setTarget:self];
		[nsPanel setContinuous:TRUE];
		[nsPanel     setAction:@selector(colorChanged)];

		[nsPanel makeKeyAndOrderFront:nil];
		}
	
	else
		[nsPanel orderOut:nil];
}





//============================================================================
//		NPickerDelegate::setColor : Set the color.
//----------------------------------------------------------------------------
-(void) setColor:(NColor *) theColor
{


	// Set the color
	[[NSColorPanel sharedColorPanel] setColor:[NSColor colorWithCalibratedRed:theColor->GetRed()
																		green:theColor->GetGreen()
																	     blue:theColor->GetBlue()
																	    alpha:theColor->GetAlpha()]];
}

@end // NPickerDelegate





//============================================================================
//		NColorPicker::NColorPicker : Constructor.
//----------------------------------------------------------------------------
NColorPicker::NColorPicker(HIWindowRef			theWindow,
							const HIRect		&theRect,
							const NColor		&theColor)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetColor(theColor);
}





//============================================================================
//		NColorPicker::~NColorPicker : Destructor.
//----------------------------------------------------------------------------
NColorPicker::~NColorPicker(void)
{


	// Reset the picker
	if (gActivePicker == this)
		SetActivePicker(NULL);
}





//============================================================================
//		NColorPicker::GetColor : Get the color.
//----------------------------------------------------------------------------
NColor NColorPicker::GetColor(void) const
{


	// Get the color
	return(mColor);
}





//============================================================================
//		NColorPicker::SetColor : Set the color.
//----------------------------------------------------------------------------
void NColorPicker::SetColor(const NColor &theColor, bool sendMsg)
{	bool	didChange;



	// Set the color
	didChange = (theColor != mColor);
	mColor    = theColor;
	
	if (!didChange)
		return;



	// Update the color panel
	if (gActivePicker == this)
		[gPickerDelegate setColor:&mColor];



	// Update our state
	SetNeedsDisplay();
	
	if (sendMsg)
		BroadcastMessage(kMsgColorPickerChanged, this);
}





//============================================================================
//		NColorPicker::GetShowAlpha : Get the alpha state.
//----------------------------------------------------------------------------
bool NColorPicker::GetShowAlpha(void) const
{


	// Get the state
	return(mShowAlpha);
}





//============================================================================
//		NColorPicker::SetShowAlpha : Set the alpha state.
//----------------------------------------------------------------------------
void NColorPicker::SetShowAlpha(bool showAlpha)
{


	// Set the alpha
	mShowAlpha = showAlpha;
}





//============================================================================
//		NColorPicker::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NColorPicker::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassCommand, kEventCommandProcess           },
										{ kEventClassControl, kEventControlDraw              },
										{ kEventClassControl, kEventControlVisibilityChanged },
										{ kEventClassNone,    kEventKindNone                 } };



	// Initialize ourselves
	mColor     = kColorBlack;
	mShowAlpha = true;
	
	SetCommandID(kPickCommand);



	// Register for events
	AddEvents(theEvents);

	SetDraggedTypes(NArray(kUTNTypeColor, NULL));



	// Apply our properties
	if (HasProperty(               kPropertyNano, kTagColor))
		SetColor(GetPropertyString(kPropertyNano, kTagColor));

	if (HasProperty(                    kPropertyNano, kTagShowAlpha))
		SetShowAlpha(GetPropertyBoolean(kPropertyNano, kTagShowAlpha));
}





//============================================================================
//		NColorPicker::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NColorPicker::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();
	
	switch (hiCmd.commandID) {
		case kPickCommand:
			theErr = SetActivePicker(this);
			break;
		
		default:
			theErr = NHIView::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NColorPicker::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NColorPicker::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext		cgContext(theEvent);
	HIRect			theBounds, theRect;
	OSStatus		theErr;



	// Draw the button
	theErr = CallNextHandler(theEvent);
	if (theErr != noErr)
		return(theErr);



	// Prepare to draw
	theBounds = GetBounds();
	theRect   = CGRectInset(theBounds, kChipInset, kChipInset);



	// Draw the chip
	if (IsEnabled())
		{
		if (NMathUtilities::NotEqual(mColor.GetAlpha(), 1.0f))
			{
			cgContext.SetFillColor(kColorBlack);
			cgContext.MoveTo(   theRect.origin.x,                      theRect.origin.y);
			cgContext.AddLineTo(theRect.origin.x + theRect.size.width, theRect.origin.y);
			cgContext.AddLineTo(theRect.origin.x,                      theRect.origin.y + theRect.size.height);
			cgContext.FillPath();

			cgContext.SetFillColor(kColorWhite);
			cgContext.MoveTo(   theRect.origin.x,                      theRect.origin.y + theRect.size.height);
			cgContext.AddLineTo(theRect.origin.x + theRect.size.width, theRect.origin.y + theRect.size.height);
			cgContext.AddLineTo(theRect.origin.x + theRect.size.width, theRect.origin.y);
			cgContext.FillPath();
			}
		}
	else
		cgContext.SetAlpha(kHIViewDisabledAlpha);
	
	cgContext.SetFillColor(mColor);
	cgContext.FillRect(theRect);



	// Draw the drag hilite
	if (IsDragTarget())
		cgContext.DrawDragHilite(theBounds);

	return(noErr);
}





//============================================================================
//		NColorPicker::DoEventControlVisibilityChanged : Handle kEventControlVisibilityChanged.
//----------------------------------------------------------------------------
OSStatus NColorPicker::DoEventControlVisibilityChanged(NCarbonEvent &/*theEvent*/)
{


	// Reset the picker
	if (gActivePicker == this)
		SetActivePicker(NULL);
	
	return(eventNotHandledErr);
}





//============================================================================
//		NColorPicker::DoDragReceive : Receive a drag.
//----------------------------------------------------------------------------
DragActions NColorPicker::DoDragReceive(DragRef theDrag)
{	NPasteboard		pasteBoard(theDrag);



	// Set the color
	SetColor(pasteBoard.GetValueColor(), true);
	
	return(kDragActionGeneric);
}





//============================================================================
//		NColorPicker::SetActivePicker : Set the active picker.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NColorPicker::SetActivePicker(NColorPicker *thePicker)
{


	// Allocate the delegate
	if (gPickerDelegate == NULL)
		{
		gPickerDelegate = [[NPickerDelegate alloc] init];
		if (gPickerDelegate == NULL)
			return(memFullErr);
		}



	// Set the active picker
	[gPickerDelegate setActivePicker:thePicker];

	return(noErr);
}


