/*	NAME:
		NDockTile.cpp

	DESCRIPTION:
		Dock tile.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NCarbonEvent.h"
#include "NHIView.h"
#include "NHIWindow.h"
#include "NCFBundle.h"
#include "NDockTile.h"





//============================================================================
//		NDockTile::NDockTile : Constructor.
//----------------------------------------------------------------------------
NDockTile::NDockTile(const NHIWindow *theWindow)
{


	// Initialize ourselves
	InitializeSelf(theWindow->GetHIWindow());
}





//============================================================================
//		NDockTile::NDockTile : Constructor.
//----------------------------------------------------------------------------
NDockTile::NDockTile(const NDockTile &theTile)
{


	// Initialize ourselves
	InitializeSelf(NULL);

	*this = theTile;
}





//============================================================================
//		NDockTile::NDockTile : Constructor.
//----------------------------------------------------------------------------
NDockTile::NDockTile(void)
{


	// Initialize ourselves
	InitializeSelf(NULL);
}





//============================================================================
//		NDockTile::~NDockTile : Destructor.
//----------------------------------------------------------------------------
NDockTile::~NDockTile(void)
{


	// Validate our state
	NN_ASSERT(!mTileContext.IsValid());
	NN_ASSERT(mTilePort == NULL);
}





//============================================================================
//		NDockTile::IsAppTile : Is this the application tile?
//----------------------------------------------------------------------------
bool NDockTile::IsAppTile(void) const
{


	// Check our state
	return(mWindow == NULL);
}





//============================================================================
//		NDockTile::GetMenu : Get the menu.
//----------------------------------------------------------------------------
NMenu NDockTile::GetMenu(void) const
{


	// Get the menu
	if (IsAppTile())
		return(GetApplicationDockTileMenu());
	else
		return(GetWindowDockTileMenu(mWindow));
}





//============================================================================
//		NDockTile::SetMenu : Set the menu.
//----------------------------------------------------------------------------
void NDockTile::SetMenu(const NMenu &theMenu)
{	OSStatus	theErr;



	// Set the menu
	if (IsAppTile())
		theErr = SetApplicationDockTileMenu(    theMenu);
	else
		theErr = SetWindowDockTileMenu(mWindow, theMenu);
	
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDockTile::SetImage : Set the image.
//----------------------------------------------------------------------------
void NDockTile::SetImage(const NCGImage &theImage, bool isSticky)
{	NCGContext		cgContext;
	OSStatus		theErr;



	// Set a sticky image
	if (isSticky)
		{
		NN_ASSERT(IsAppTile());
		
		theErr = SetApplicationDockTileImage(theImage);
		NN_ASSERT_NOERR(theErr);
		}
	
	
	// Set a transient image
	else
		{
		cgContext = CreateContext();
		if (cgContext.IsValid())
			{
			cgContext.DrawImage(theImage, kDockIconRect);
			ReleaseContext();
			}
		}
}





//============================================================================
//		NDockTile::RefreshImage : Refresh the image.
//----------------------------------------------------------------------------
void NDockTile::RefreshImage(void)
{	OSStatus	theErr;



	// Refresh the image
	if (IsAppTile())
		theErr = RestoreApplicationDockTileImage();
	else
		theErr = UpdateCollapsedWindowDockTile(mWindow);
	
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDockTile::DrawBadge : Draw a badge on the tile.
//----------------------------------------------------------------------------
void NDockTile::DrawBadge(const NCGImage &theImage, const CGRect &theRect)
{	NCGContext		cgContext;



	// Validate our state
	NN_ASSERT(IsAppTile());



	// Draw the badge
	cgContext = DrawAppIcon();
	if (cgContext.IsValid())
		{
		cgContext.DrawImage(theImage, theRect);
		ReleaseContext();
		}
}





//============================================================================
//		NDockTile::DrawBadge : Draw a badge on the tile.
//----------------------------------------------------------------------------
void NDockTile::DrawBadge(const NIcon &theIcon, const CGRect &theRect)
{	NCGContext		cgContext;



	// Validate our state
	NN_ASSERT(IsAppTile());



	// Draw the badge
	cgContext = DrawAppIcon();
	if (cgContext.IsValid())
		{
		cgContext.DrawIcon(theIcon, theRect);
		ReleaseContext();
		}
}





//============================================================================
//		NDockTile::DrawBadge : Draw a badge on the tile.
//----------------------------------------------------------------------------
void NDockTile::DrawBadge(NHIView *theView, const CGRect &theRect)
{	NCarbonEvent	theEvent(kEventClassControl, kEventControlDraw);
	HIRect			oldFrame, drawFrame;
	NCGContext		cgContext;
	HIShapeRef		hiShape;
	OSStatus		theErr;
	


	// Draw the badge
	cgContext = DrawAppIcon();
	if (cgContext.IsValid())
		{
		// Prepare the view
		drawFrame.origin = kHIPointZero;
		drawFrame.size   = theRect.size;

		oldFrame = theView->GetFrame();
		theView->SetFrame(drawFrame);



		// Prepare the context
		//
		// kEventControlDraw carries an kHIThemeOrientationNormal context, so we
		// must flip from CG to the HIView coordinate system prior to sending.
		//
		// The context must then be translated to match the desired location in
		// the CG-oriented coordinate system used by theRect.
		cgContext.TranslateCTM(0.0f, kDockIconRect.size.height);
		cgContext.ScaleCTM(1.0f, -1.0f);
		cgContext.TranslateCTM(theRect.origin.x, kDockIconRect.size.height - theRect.origin.y - theRect.size.height);



		// Prepare the event
		//
		// Strictly speaking only the view and context parameters are required,
		// however some system controls (e.g., progress bars) currently require
		// some of the optional parameters.
		hiShape = HIShapeCreateWithRect(&drawFrame);

		theEvent.SetHIView(*theView);
		theEvent.SetViewPartCode(kHIViewEntireView);
		theEvent.SetCGContext(cgContext);
		theEvent.SetParameter(kEventParamShape, typeHIShapeRef, sizeof(hiShape), &hiShape);



		// Draw the view
		theErr = theEvent.SendToHIView(*theView);
		NN_ASSERT_NOERR(theErr);



		// Clean up
		theView->SetFrame(oldFrame);

		CFSafeRelease(hiShape);
		ReleaseContext();
		}
}





//============================================================================
//		NDockTile::CreateContext : Create a drawing context.
//----------------------------------------------------------------------------
NCGContext NDockTile::CreateContext(void)
{	CGContextRef	cgContext;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(!mTileContext.IsValid());
	NN_ASSERT(mTilePort == NULL);
	


	// Create the context
	//
	// When creating a CG context, we do not take ownership of the reference since
	// EndCGContextForApplicationDockTile expects to be able to release it.
	if (IsAppTile())
		{
		cgContext    = BeginCGContextForApplicationDockTile();
		mTileContext = NCGContext(cgContext);
		}
	else
		{
		// Create the context
		//
		// Accessing a window's dock tile currently requires QuickDraw (rdar://5038181).
		theErr = CreateQDContextForCollapsedWindowDockTile(mWindow, &mTilePort);
		if (theErr == noErr)
			{
			theErr = CreateCGContextForPort(mTilePort, &cgContext);
			if (theErr == noErr && cgContext != NULL)
				mTileContext.Set(cgContext);
			}

		NN_ASSERT_NOERR(theErr);



		// Handle failure
		if (theErr != noErr && mTilePort != NULL)
			{
			theErr    = ReleaseQDContextForCollapsedWindowDockTile(mWindow, mTilePort);
			mTilePort = NULL;

			NN_ASSERT_NOERR(theErr);
			}
		}



	// Prepare to draw
	mTileContext.ClearRect(kDockIconRect);
	
	return(mTileContext);
}





//============================================================================
//		NDockTile::ReleaseContext : Release a drawing context.
//----------------------------------------------------------------------------
void NDockTile::ReleaseContext(void)
{	OSStatus	theErr;



	// Validate our xstate
	NN_ASSERT(mTileContext.IsValid());
	
	if (NN_DEBUG && !IsAppTile())
		NN_ASSERT(mTilePort != NULL);



	// Update the Dock
	mTileContext.Flush();



	// Release the context
	if (IsAppTile())
		EndCGContextForApplicationDockTile(mTileContext);
	else
		{
		theErr = ReleaseQDContextForCollapsedWindowDockTile(mWindow, mTilePort);
		NN_ASSERT_NOERR(theErr);
		}
	
	
	
	// Reset our state
	mTilePort = NULL;
	mTileContext.Set(NULL);
}





//============================================================================
//		NDockTile::operator = : Assignment operator.
//----------------------------------------------------------------------------
NDockTile &NDockTile::operator=(const NDockTile &otherTile)
{


	// Assign the tile
	//
	// Since QD ports are not reference counted, if the other tile has a context
	// we need to create a new context for ourself (although we could just bump
	// the ref count on our CGContext, we may also need a releasable CGrafPtr).
	if (this != &otherTile)
		{
		if (mTileContext.IsValid())
			ReleaseContext();
		
		mWindow = otherTile.mWindow;
		
		if (otherTile.mTileContext.IsValid())
			CreateContext();
		}
		
	return(*this);
}





//============================================================================
//		NDockTile::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NDockTile::InitializeSelf(HIWindowRef theWindow)
{


	// Initialize ourselves
	mWindow   = theWindow;
	mTilePort = NULL;
}





//============================================================================
//		NDockTile::DrawAppIcon : Draw the application icon.
//----------------------------------------------------------------------------
NCGContext NDockTile::DrawAppIcon(void)
{	NCGContext		cgContext;
	NCFBundle		appBundle;
	NIcon			appIcon;



	// Validate our state
	NN_ASSERT(IsAppTile());



	// Draw the app icon
	cgContext = CreateContext();
	if (cgContext.IsValid())
		{
		appIcon = appBundle.GetResource(appBundle.GetInfoString(kCFBundleIconFileKey));
		cgContext.DrawIcon(appIcon, kDockIconRect);
		}
	
	return(cgContext);
}



