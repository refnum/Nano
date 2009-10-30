/*	NAME:
		NanoDemo Build.h

	DESCRIPTION:
		NanoDemo build header.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANODEMO_BUILD_HDR
#define NANODEMO_BUILD_HDR
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
#define kAppNibFile												STRING(NanoDemo)
#define kAppName												STRING(NanoDemo)
#define kAppCreator												STRING(NDEM)
#define kAppVersion												STRING(1.0)
#define kAppCompany												STRING(refNum Software)
#define kAppBundleIdentifier									STRING(com.refnum.nano.nanodemo)
#define kAppHomePage											"http://www.refnum.com/products/nano/"


// NDocument
#define kDocFileUTI												kAppBundleIdentifier STRING(.document)





//============================================================================
//      Bundle resources
//----------------------------------------------------------------------------
// Strings
#define kStringToolbarDocAbout									"ToolbarDocAbout"
#define kStringToolbarDocShape									"ToolbarDocShape"
#define kStringToolbarDocColor									"ToolbarDocColor"
#define kStringToolbarPrefsGeneral								"ToolbarPrefsGeneral"
#define kStringToolbarPrefsAdvanced								"ToolbarPrefsAdvanced"
#define kStringToolbarPrefsUpdate								"ToolbarPrefsUpdate"
#define kStringUndoColor										"UndoColor"
#define kStringUndoShape										"UndoShape"
#define kStringLibraryMusic										"LibraryMusic"
#define kStringLibraryPhotos									"LibraryPhotos"
#define kStringLibraryMail										"LibraryMail"
#define kStringLibraryInternet									"LibraryInternet"


// Images
#define kImageToolbarDocAbout									"DocAbout.png"
#define kImageToolbarPrefsGeneral								"PrefsGeneral.png"
#define kImageToolbarPrefsAdvanced								"PrefsAdvanced.png"
#define kImageDemoCursor										"Demo Cursor.png"
#define kImageDemoIcon											"Demo Icon.icns"





#endif // NANODEMO_BUILD_HDR


