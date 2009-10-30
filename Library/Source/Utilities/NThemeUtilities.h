/*	NAME:
		NThemeUtilities.h

	DESCRIPTION:
		Theme utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHEMEUTILITIES_HDR
#define NTHEMEUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMouseUtilities.h"
#include "NColor.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Theme text info
extern const HIThemeTextInfo kThemeTextSystemFontLeft;
extern const HIThemeTextInfo kThemeTextSystemFontCenter;
extern const HIThemeTextInfo kThemeTextSystemFontRight;

extern const HIThemeTextInfo kThemeTextAppFontLeft;
extern const HIThemeTextInfo kThemeTextAppFontCenter;
extern const HIThemeTextInfo kThemeTextAppFontRight;

extern const HIThemeTextInfo kThemeTextMenuItem;
extern const HIThemeTextInfo kThemeTextMenuItemCmdKey;

extern const HIThemeTextInfo kThemeTextButtonCenter;
extern const HIThemeTextInfo kThemeTextLabelCenter;


// Theme effects
typedef enum {
	kThemeEffectDisappearingItem
} NThemeEffect;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThemeUtilities {
public:
	// Get the current theme
	static NString						GetThemeID(void);


	// Get a theme metric
	static float						GetMetric(ThemeMetric theMetric);


	// Get the text hilight color
	static NColor						GetTextHilight(void);


	// Get menu dimensions
	static float						GetMenuTitleExtra(bool isSquished=false);
	static float						GetMenuItemSeparatorHeight(void);
	static HISize						GetMenuItemExtra(ThemeMenuItemType theType);


	// Get text dimensions
	static float						GetTextDimensions(const NString				&theText,
														  HISize					&theSize,
														  const HIThemeTextInfo		&theInfo,
														  bool						*didTruncate=NULL);

	static HISize						GetTextDimensions(const NString				&theText,
														  const HIThemeTextInfo		&theInfo,
														  bool						*didTruncate=NULL);


	// Show an animation effect
	static void							ShowEffect(NThemeEffect		theEffect,
													const HIPoint	&thePoint = NMouseUtilities::GetPosition(),
													const HISize	&theSize  = CGSizeZero);
};





#endif // NTHEMEUTILITIES_HDR


