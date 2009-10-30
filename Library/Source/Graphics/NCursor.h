/*	NAME:
		NCursor.h

	DESCRIPTION:
		Cursor object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCURSOR_HDR
#define NCURSOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAppKitUtilities.h"
#include "NCGImage.h"
#include "NTimer.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Custom cursors
//
// ThemeCursor is 32-bits in size, and to date Apple cursors have been
// allocated from 0 upwards.
//
// We use a hopefully clash-free range to store our custom cursors, although
// these values may need to change over time so should never be saved to disk.
static const ThemeCursor kThemeCurrentCursor					= 0xFFFF0000;
static const ThemeCursor kThemeImageCursor                      = 0xFFFF0001;
static const ThemeCursor kThemeEmptyCursor                      = 0xFFFF0002;
static const ThemeCursor kThemeDotCursor                        = 0xFFFF0003;
static const ThemeCursor kThemeZoomInCursor						= 0xFFFF0004;
static const ThemeCursor kThemeZoomOutCursor					= 0xFFFF0005;
static const ThemeCursor kThemeZoomStopCursor					= 0xFFFF0006;





//============================================================================
//		Types
//----------------------------------------------------------------------------
DECLARE_OBJC_CLASS(NSCursor);
DECLARE_OBJC_CLASS(NSImage);





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCursor {
public:
										NCursor(void);
	virtual								~NCursor(void);


	// Get/set the cursor
	ThemeCursor							GetCursor(void) const;
	void								SetCursor(ThemeCursor theCursor, bool canAnimate=true);


	// Get/set the cursor image
	//
	// Only supported by kThemeImageCursor.
	NCGImage							GetImage(void) const;
	void								SetImage(const NCGImage &theImage);


	// Get/set the cursor hot-spot
	//
	// The hot-spot coordinate is relative to the bounds of the cursor image.
	//
	// Only supported by kThemeImageCursor.
	HIPoint								GetHotSpot(void) const;
	void								SetHotSpot(const HIPoint &thePoint);


private:
	void								AnimateCursor(void);
	
	void								ShowImageCursor(void);
	void								HideImageCursor(void);

	NSImage								*CreateNSImage(CGImageRef cgImage);


private:
	NTimer								mTimer;
	ThemeCursor							mCursor;
	UInt32								mCursorStep;
	
	NCGImage							mImage;
	NSCursor							*mImageCursor;
	HIPoint								mImageHotSpot;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class StCursor {
public:
										StCursor(ThemeCursor  preCursor=kThemeWatchCursor,
												 ThemeCursor postCursor=kThemeArrowCursor);
	virtual								~StCursor(void);


private:
	ThemeCursor							mPostCursor;
	NCursor								mCursor;
};







#endif // NCURSOR_HDR


