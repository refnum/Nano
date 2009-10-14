/*	NAME:
		NW Build.h

	DESCRIPTION:
		NanoWeb build header.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NW_BUILD_HDR
#define NW_BUILD_HDR
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
#define kAppNibFile												STRING(NanoWeb)
#define kAppName												STRING(NanoWeb)
#define kAppCreator												STRING(NWEB)
#define kAppVersion												STRING(1.0)
#define kAppCompany												STRING(refNum Software)
#define kAppBundleIdentifier									STRING(com.refnum.nano.nanoweb)
#define kAppHomePage											"http://www.refnum.com/products/nano/"


// NDocument





//============================================================================
//      Bundle resources
//----------------------------------------------------------------------------
// Strings
#define kStringToolbarDocBack									"ToolbarDocBack"
#define kStringToolbarDocForward								"ToolbarDocForward"
#define kStringToolbarDocAddress								"ToolbarDocAddress"
#define kStringToolbarDocStatus									"ToolbarDocStatus"
#define kStringToolbarPrefsGeneral								"ToolbarPrefsGeneral"


// Images
#define kImageToolbarDocBack									"DocBack.png"
#define kImageToolbarDocForward									"DocForward.png"
#define kImageToolbarPrefsGeneral								"PrefsGeneral.png"





#endif // NW_BUILD_HDR


