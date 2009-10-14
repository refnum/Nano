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
#include "NAppKitUtilities.h"
#include "NCursor.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Misc
static const EventTime kDelayAnimateCursor							= 0.25f;


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
	mTimer.RemoveTimer();
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
				mTimer.AddTimer(BindSelf(NCursor::AnimateCursor), 0, kDelayAnimateCursor);

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
//		NCursor::AnimateCursor : Animate the cursor.
//----------------------------------------------------------------------------
#pragma mark -
void NCursor::AnimateCursor(void)
{	OSStatus	theErr;



	// Animate the cursor
	theErr = SetAnimatedThemeCursor(mCursor, mCursorStep++);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NCursor::ShowImageCursor : Show the image cursor.
//----------------------------------------------------------------------------
void NCursor::ShowImageCursor(void)
{	NSImage				*nsImage;
	StAutoReleasePool	thePool;



	// Validate our state
	NN_ASSERT(mCursor == kThemeImageCursor);
	NN_ASSERT(mImage.IsValid());



	// Ensure NSWindow is initialized
	//
	// NSCursor may decide to use an NSWindow to cache the cursor image, but
	// it does not check that NSWindow has been initialized (rdar://4622504).
	//
	// As such, we need to create a temporary window to ensure that class is
	// in a safe state for NSCursor to use.
	[[[NSWindow alloc] init] release];



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
{	StAutoReleasePool	thePool;



	// Hide the cursor
	[mImageCursor release];
	
	mImageCursor = NULL;
}





//============================================================================
//		NCursor::CreateNSImage : Create an NSImage from a CGImageRef.
//----------------------------------------------------------------------------
NSImage *NCursor::CreateNSImage(CGImageRef cgImage)
{	CGContextRef		cgContext;
	NSImage				*nsImage;
	StAutoReleasePool	thePool;
	NSRect				nsRect;



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


