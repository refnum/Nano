/*	NAME:
		ÇPROJECTNAMEÈ Build.h

	DESCRIPTION:
		ÇPROJECTNAMEÈ build header.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef ÇPROJECTNAMEASIDENTIFIERÈ_BUILD_HDR
#define ÇPROJECTNAMEASIDENTIFIERÈ_BUILD_HDR
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
#define kAppNibFile												STRING(ÇPROJECTNAMEÈ)
#define kAppName												STRING(ÇPROJECTNAMEÈ)
#define kAppCreator												STRING(NAPP)
#define kAppVersion												STRING(1.0)
#define kAppCompany												STRING(ÇORGANIZATIONNAMEÈ)
#define kAppBundleIdentifier									STRING(com.mycompany.ÇPROJECTNAMEASIDENTIFIERÈ)
#define kAppHomePage											"http://www.mycompany.com/ÇPROJECTNAMEÈ/"


// NDocument





//============================================================================
//      Bundle resources
//----------------------------------------------------------------------------
// Strings
#define kStringToolbarPrefsGeneral								"ToolbarPrefsGeneral"
#define kStringToolbarPrefsAdvanced								"ToolbarPrefsAdvanced"
#define kStringToolbarPrefsUpdate								"ToolbarPrefsUpdate"


// Images
#define kImageToolbarPrefsGeneral								"PrefsGeneral.png"
#define kImageToolbarPrefsAdvanced								"PrefsAdvanced.png"





#endif // ÇPROJECTNAMEASIDENTIFIERÈ_BUILD_HDR


