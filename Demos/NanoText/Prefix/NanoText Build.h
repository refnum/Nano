/*	NAME:
		NanoText Build.h

	DESCRIPTION:
		NanoText build header.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANOTEXT_BUILD_HDR
#define NANOTEXT_BUILD_HDR
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
#define kAppNibFile												STRING(NanoText)
#define kAppName												STRING(NanoText)
#define kAppCreator												STRING(NTXT)
#define kAppVersion												STRING(1.0)
#define kAppCompany												STRING(refNum Software)
#define kAppBundleIdentifier									STRING(com.refnum.nano.nanotext)
#define kAppHomePage											"http://www.refnum.com/products/nano/"


// NDocument
#define kDocFileUTI												STRING(public.rtf)





//============================================================================
//      Bundle resources
//----------------------------------------------------------------------------
// Strings
#define kStringToolbarPanelColors								"ToolbarPanelColors"
#define kStringToolbarPanelFonts								"ToolbarPanelFonts"
#define kStringToolbarPanelSpelling								"ToolbarPanelSpelling"
#define kStringToolbarPrefsGeneral								"ToolbarPrefsGeneral"
#define kStringToolbarPrefsUpdate								"ToolbarPrefsUpdate"


// Images
#define kImageToolbarPanelColors								"PanelColors.png"
#define kImageToolbarPanelFonts									"PanelFonts.png"
#define kImageToolbarPanelSpelling								"com.apple.dictionary"
#define kImageToolbarPrefsGeneral								"PrefsGeneral.png"





#endif // NANOTEXT_BUILD_HDR


