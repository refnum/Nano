/*	NAME:
		Nano Build.h

	DESCRIPTION:
		Nano build header.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
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
#define kNanoVersion											STRING(1.3)
#define kNanoNibFile											STRING(Nano)
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
#define kStringSoftwareUpdateErrorChecksum						"SoftwareUpdateErrorChecksum"
#define kStringSoftwareUpdateErrorMounting						"SoftwareUpdateErrorMounting"
#define kStringSoftwareUpdateErrorNetwork						"SoftwareUpdateErrorNetwork"
#define kStringSoftwareUpdateErrorTypeN							"SoftwareUpdateErrorTypeN"
#define kStringSoftwareUpdateFailed								"SoftwareUpdateFailed"
#define kStringSoftwareUpdateProgressDownload					"SoftwareUpdateProgressDownload"
#define kStringSoftwareUpdateProgressInstall					"SoftwareUpdateProgressInstall"
#define kStringSoftwareUpdateProgressVerify						"SoftwareUpdateProgressVerify"
#define kStringSoftwareUpdateServerLost							"SoftwareUpdateServerLost"
#define kStringSoftwareUpdateServerMissing						"SoftwareUpdateServerMissing"
#define kStringSoftwareUpdateValidMajor							"SoftwareUpdateValidMajor"
#define kStringSoftwareUpdateValidMinor							"SoftwareUpdateValidMinor"
#define kStringTransferProgressKB								"TransferProgressKB"
#define kStringTransferProgressMB								"TransferProgressMB"
#define kStringTransferProgressGB								"TransferProgressGB"
#define kStringTransferProgressXofY								"TransferProgressXofY"
#define kStringTransferRateKBSec								"TransferRateKBSec"
#define kStringTransferRateMBSec								"TransferRateMBSec"
#define kStringTransferTimeStalled								"TransferTimeStalled"
#define kStringTransferTimeMinutes								"TransferTimeMinutes"
#define kStringTransferTimeMinute								"TransferTimeMinute"
#define kStringTransferTimeSeconds								"TransferTimeSeconds"
#define kStringTransferTimeSecond								"TransferTimeSecond"




#endif // NANO_BUILD_HDR



