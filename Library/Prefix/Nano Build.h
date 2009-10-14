/*	NAME:
		Nano Build.h

	DESCRIPTION:
		Nano build header.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_BUILD_HDR
#define NANO_BUILD_HDR
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
// Nano
#define kNanoVersion											STRING(1.1)
#define kNanoStrings											STRING(Nano)





//============================================================================
//      Bundle resources
//----------------------------------------------------------------------------
// Strings
#define kStringFileDialogOpen									"FileDialogOpen"
#define kStringFileDialogSave									"FileDialogSave"
#define kStringAboutTitle										"AboutTitle"
#define kStringAboutHomePage									"AboutHomePage"
#define kStringDocumentUntitled									"DocumentUntitled"
#define kStringToolbarShow										"ToolbarShow"
#define kStringToolbarHide										"ToolbarHide"
#define kStringUndoNone											"UndoNone"
#define kStringUndoGeneric										"UndoGeneric"
#define kStringUndoCommand										"UndoCommand"
#define kStringRedoNone											"RedoNone"
#define kStringRedoGeneric										"RedoGeneric"
#define kStringRedoCommand										"RedoCommand"
#define kStringRecentItemsClear									"RecentItemsClear"
#define kStringLabelColor										"LabelColor"




#endif // NANO_BUILD_HDR



