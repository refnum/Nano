/*	NAME:
		NDockTile.h

	DESCRIPTION:
		Dock tile.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDOCKTILE_HDR
#define NDOCKTILE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMenu.h"
#include "NCGImage.h"
#include "NCGContext.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const CGRect kDockIconRect				= CGRectMake( 0,  0, 128, 128);
static const CGRect kDockBadgeRectTL			= CGRectMake( 0, 80,  48,  48);
static const CGRect kDockBadgeRectTR			= CGRectMake(80, 80,  48,  48);
static const CGRect kDockBadgeRectBL			= CGRectMake( 0,  0,  48,  48);
static const CGRect kDockBadgeRectBR			= CGRectMake(80,  0,  48,  48);





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NHIView;
class NHIWindow;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDockTile {
public:
										NDockTile(const NHIWindow *theWindow);
										NDockTile(const NDockTile &theTile);

										NDockTile(void);
	virtual								~NDockTile(void);


	// Is this the application tile?
	bool								IsAppTile(void) const;


	// Get/set the menu
	NMenu								GetMenu(void) const;
	void								SetMenu(const NMenu &theMenu);


	// Set the image
	//
	// When set as 'sticky', the application tile's image will persist after quit.
	void								SetImage(const NCGImage &theImage, bool isSticky=false);


	// Refresh the image
	//
	// Refreshes the tile with its standard image.
	void								RefreshImage(void);


	// Draw a badge
	//
	// Only the application tile can be badged.
	void								DrawBadge(const NCGImage &theImage, const CGRect &theRect=kDockBadgeRectTR);
	void								DrawBadge(const NIcon    &theIcon,  const CGRect &theRect=kDockBadgeRectTR);
	void								DrawBadge(NHIView        *theView,  const CGRect &theRect=kDockBadgeRectTR);


	// Create/release a drawing context
	//
	// The context is cleared on creation, and flushed on destruction.
	//
	// The context must always be released with NDockTile::ReleaseContext, as
	// creating a context will acquire a lock on the Dock's internal storage.
	NCGContext							CreateContext(void);
	void								ReleaseContext(void);


	// Operators
	NDockTile							&operator = (const NDockTile &otherTile);


private:
	void								InitializeSelf(HIWindowRef theWindow);
	NCGContext							DrawAppIcon(void);
	

private:
	HIWindowRef							mWindow;

	NCGContext							mTileContext;
	CGrafPtr							mTilePort;
};





#endif // NDOCKTILE_HDR


