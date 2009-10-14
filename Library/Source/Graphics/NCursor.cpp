/*	NAME:
		NCursor.cpp

	DESCRIPTION:
		Cursor object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCursor.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Misc
static const EventTime kDelayAnimateCursor							= 0.25f;


// Timers
static const NTimerID kTimerAnimateCursor							= 1000;


// Cursors
static const Cursor kCursorEmpty = {	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
										  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
										{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
										  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
										{ 0, 0 } };

											



//============================================================================
//		NCursor::NCursor : Constructor.
//----------------------------------------------------------------------------
NCursor::NCursor(void)
{


	// Initialize ourselves
	mCursor     = kThemeCurrentCursor;
	mCursorStep = 0;

	mImageCursor = NULL;
}





//============================================================================
//		NCursor::~NCursor : Destructor.
//----------------------------------------------------------------------------
NCursor::~NCursor(void)
{


	// Clean up
	HideImageCursor();
}





//============================================================================
//		NCursor::GetCursor : Get the cursor.
//----------------------------------------------------------------------------
ThemeCursor NCursor::GetCursor(void) const
{


	// Get the cursor
	return(mCursor);
}





//============================================================================
//		NCursor::SetCursor : Set the cursor.
//----------------------------------------------------------------------------
void NCursor::SetCursor(ThemeCursor theCursor, bool canAnimate)
{	OSStatus	theErr;



	// Reset our state
	RemoveTimer(kTimerAnimateCursor);
	HideImageCursor();



	// Set the cursor
	mCursor     = theCursor;
	mCursorStep = 0;



	// Apply the cursor
	switch (mCursor) {
		case kThemeCurrentCursor:
			// Do nothing
			break;

		case kThemeEmptyCursor:
			::SetCursor(&kCursorEmpty);
			break;

		case kThemeImageCursor:
			ShowImageCursor();
			break;

		case kThemeWatchCursor:
		case kThemeCountingUpHandCursor:
		case kThemeCountingDownHandCursor:
		case kThemeCountingUpAndDownHandCursor:
		case kThemeSpinningCursor:
			if (canAnimate)
				AddTimer(0, kDelayAnimateCursor, kTimerAnimateCursor);
			
			theErr = SetAnimatedThemeCursor(mCursor, mCursorStep);
			NN_ASSERT_NOERR(theErr);
			break;

		default:
			theErr = SetThemeCursor(mCursor);
			NN_ASSERT_NOERR(theErr);
			break;
		}
}





//============================================================================
//		NCursor::GetImage : Get the cursor image.
//----------------------------------------------------------------------------
NCGImage NCursor::GetImage(void) const
{


	// Get the image
	return(mImage);
}





//============================================================================
//		NCursor::SetImage : Set the cursor image.
//----------------------------------------------------------------------------
void NCursor::SetImage(const NCGImage &theImage)
{


	// Set the image
	mImage = theImage;
}





//============================================================================
//		NCursor::GetHotSpot : Get the cursor hot-spot.
//----------------------------------------------------------------------------
HIPoint NCursor::GetHotSpot(void) const
{


	// Get the hot-spot
	return(mImageHotSpot);
}





//============================================================================
//		NCursor::SetHotSpot : Set the cursor hot-spot.
//----------------------------------------------------------------------------
void NCursor::SetHotSpot(const HIPoint &thePoint)
{


	// Set the hot-spot
	mImageHotSpot = thePoint;
}





//============================================================================
//		NCursor::DoTimer : Handle a timer.
//----------------------------------------------------------------------------
#pragma mark -
void NCursor::DoTimer(NTimerID theTimer, TimerMsg theMsg)
{	OSStatus	theErr;



	// Handle the timer
	switch (theTimer) {
		case kTimerAnimateCursor:
			theErr = SetAnimatedThemeCursor(mCursor, mCursorStep++);
			NN_ASSERT_NOERR(theErr);
			break;

		default:
			NTimerTask::DoTimer(theTimer, theMsg);
			break;
		}
}





//============================================================================
//		NCursor::ShowImageCursor : Show the image cursor.
//----------------------------------------------------------------------------
void NCursor::ShowImageCursor(void)
{	NSImage		*nsImage;



	// Validate our state
	NN_ASSERT(mCursor == kThemeImageCursor);
	NN_ASSERT(mImage.IsValid());



	// Create the cursor
	nsImage      = CreateNSImage(mImage);
	mImageCursor = [[NSCursor alloc] initWithImage: nsImage hotSpot: NSMakePoint(mImageHotSpot.x, mImageHotSpot.y)];

	[nsImage release];



	// And set it
	[mImageCursor set];
}





//============================================================================
//		NCursor::HideImageCursor : Hide the image cursor.
//----------------------------------------------------------------------------
void NCursor::HideImageCursor(void)
{


	// Hide the cursor
	[mImageCursor release];
	
	mImageCursor = NULL;
}





//============================================================================
//		NCursor::CreateNSImage : Create an NSImage from a CGImageRef.
//----------------------------------------------------------------------------
NSImage *NCursor::CreateNSImage(CGImageRef cgImage)
{	CGContextRef	cgContext;
	NSImage			*nsImage;
	NSRect			nsRect;



	// Create the image
	nsRect  = NSMakeRect(0.0f, 0.0f, CGImageGetWidth(cgImage), CGImageGetHeight(cgImage));
	nsImage = [[NSImage alloc] initWithSize:nsRect.size];



	// Draw the image
	[nsImage lockFocus];

    cgContext = (CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
    CGContextDrawImage(cgContext, *(CGRect *) &nsRect, cgImage);

    [nsImage unlockFocus];
	
	return(nsImage);
}


