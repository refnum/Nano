/*	NAME:
		NanoViewer Build.h

	DESCRIPTION:
		NanoViewer build header.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANOVIEWER_BUILD_HDR
#define NANOVIEWER_BUILD_HDR
//============================================================================
//		Macros
//----------------------------------------------------------------------------
#ifdef INFO_PLIST
	#define STRING(_x)											_x
#else
	#define STRING(_x)											# _x
#endif





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// NApplication
#define kAppNibFile												STRING(NanoViewer)
#define kAppName												STRING(NanoViewer)
#define kAppCreator												STRING(NVIE)
#define kAppVersion												STRING(1.0)
#define kAppCompany												STRING(refNum Software)
#define kAppBundleIdentifier									STRING(com.refnum.nano.nanoviewer)
#define kAppHomePage											"http://www.refnum.com/products/nano/"


// NDocument





//============================================================================
//      Bundle resources
//----------------------------------------------------------------------------
// Strings
#define kStringToolbarDocRotateLeft								"ToolbarDocRotateLeft"
#define kStringToolbarDocRotateRight							"ToolbarDocRotateRight"
#define kStringToolbarDocActualSize								"ToolbarDocActualSize"
#define kStringToolbarDocZoomToFit								"ToolbarDocZoomToFit"
#define kStringToolbarDocZoomIn									"ToolbarDocZoomIn"
#define kStringToolbarDocZoomOut								"ToolbarDocZoomOut"
#define kStringToolbarPrefsGeneral								"ToolbarPrefsGeneral"
#define kStringToolbarPrefsUpdate								"ToolbarPrefsUpdate"
#define kStringUndoRotateLeft									"UndoRotateLeft"
#define kStringUndoRotateRight									"UndoRotateRight"
#define kStringUndoFlipHoriz									"UndoFlipHoriz"
#define kStringUndoFlipVert										"UndoFlipVert"


// Images
//
// Note that typically toolbar images would be contained within the application
// bundle, however for simplicity we just grab them out of Preview for this app.
#define kImageExample											"Example.jpg"
#define kImageToolbarPrefsGeneral								"PrefsGeneral.png"

#define kImagePreviewResources									"/Applications/Preview.app/Contents/Resources/"
#define kImageToolbarDocRotateLeft								kImagePreviewResources "RotateLeftToolbarImage.tiff"
#define kImageToolbarDocRotateRight								kImagePreviewResources "RotateRightToolbarImage.tiff"
#define kImageToolbarDocActualSize								kImagePreviewResources "ActualSizeToolbarImage.tiff"
#define kImageToolbarDocZoomToFit								kImagePreviewResources "ZoomToFitToolbarImage.tiff"
#define kImageToolbarDocZoomIn									kImagePreviewResources "ZoomInToolbarImage.tiff"
#define kImageToolbarDocZoomOut									kImagePreviewResources "ZoomOutToolbarImage.tiff"





#endif // NANOVIEWER_BUILD_HDR


