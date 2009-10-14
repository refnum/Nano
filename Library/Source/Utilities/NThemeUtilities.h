/*	NAME:
		NThemeUtilities.h

	DESCRIPTION:
		Theme utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHEMEUTILITIES_HDR
#define NTHEMEUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
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





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThemeUtilities {
public:
	// Get a theme metric
	static float						GetMetric(ThemeMetric theMetric);


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

};





#endif // NTHEMEUTILITIES_HDR


