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
#define kDocFileUTI												kAppBundleIdentifier STRING(.document)





//============================================================================
//      Bundle resources
//----------------------------------------------------------------------------
// Strings
#define kStringToolbarDocColor									"ToolbarDocColor"
#define kStringToolbarDocInfo									"ToolbarDocInfo"
#define kStringToolbarPrefsGeneral								"ToolbarPrefsGeneral"
#define kStringToolbarPrefsAdvanced								"ToolbarPrefsAdvanced"
#define kStringToolbarPrefsUpdate								"ToolbarPrefsUpdate"
#define kStringUndoColor										"UndoColor"
#define kStringUndoInfo											"UndoInfo"


// Images
#define kImageToolbarDocInfo									"PanelInfo.png"
#define kImageToolbarPrefsGeneral								"PrefsGeneral.png"
#define kImageToolbarPrefsAdvanced								"PrefsAdvanced.png"





#endif // ÇPROJECTNAMEASIDENTIFIERÈ_BUILD_HDR


