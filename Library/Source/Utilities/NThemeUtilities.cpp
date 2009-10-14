/*	NAME:
		NThemeUtilities.cpp

	DESCRIPTION:
		Theme utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThemeUtilities.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const HIThemeTextInfo kThemeTextSystemFontLeft = {		kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeSystemFont,
														kHIThemeTextHorizontalFlushLeft,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationNone,
														0,
														false };

const HIThemeTextInfo kThemeTextSystemFontCenter = {	kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeSystemFont,
														kHIThemeTextHorizontalFlushCenter,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationNone,
														0,
														false };

const HIThemeTextInfo kThemeTextSystemFontRight = {		kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeSystemFont,
														kHIThemeTextHorizontalFlushRight,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationNone,
														0,
														false };

const HIThemeTextInfo kThemeTextAppFontLeft = {			kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeApplicationFont,
														kHIThemeTextHorizontalFlushLeft,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationNone,
														0,
														false };

const HIThemeTextInfo kThemeTextAppFontCenter = {		kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeApplicationFont,
														kHIThemeTextHorizontalFlushCenter,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationNone,
														0,
														false };

const HIThemeTextInfo kThemeTextAppFontRight = {		kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeApplicationFont,
														kHIThemeTextHorizontalFlushRight,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationNone,
														0,
														false };

const HIThemeTextInfo kThemeTextMenuItem = {			kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeMenuItemFont,
														kHIThemeTextHorizontalFlushLeft,
														kHIThemeTextVerticalFlushTop,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationNone,
														0,
														false };

const HIThemeTextInfo kThemeTextMenuItemCmdKey = {		kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeMenuItemCmdKeyFont,
														kHIThemeTextHorizontalFlushLeft,
														kHIThemeTextVerticalFlushTop,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationNone,
														0,
														false };

const HIThemeTextInfo kThemeTextButtonCenter = {		kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemePushButtonFont,
														kHIThemeTextHorizontalFlushCenter,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationNone,
														0,
														false };

const HIThemeTextInfo kThemeTextLabelCenter = {			kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeLabelFont,
														kHIThemeTextHorizontalFlushCenter,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationNone,
														0,
														false };





//============================================================================
//		NThemeUtilities::GetThemeID : Get the current theme.
//----------------------------------------------------------------------------
NString NThemeUtilities::GetThemeID(void)
{	CFStringRef		cfValue;
	OSStatus		theErr;
	NString			theID;



	// Get the theme ID
	cfValue = NULL;
	theErr  = CopyThemeIdentifier(&cfValue);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		theID.Set(cfValue, true);
	
	return(theID);
}





//============================================================================
//		NThemeUtilities::GetMetric : Get a theme metric.
//----------------------------------------------------------------------------
float NThemeUtilities::GetMetric(ThemeMetric theMetric)
{	SInt32		theValue;
	OSStatus	theErr;



	// Get the metric
	theValue = 0;
	theErr   = GetThemeMetric(theMetric, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return((float) theValue);
}





//============================================================================
//		NThemeUtilities::GetTextHilight : Get the text hilight color.
//----------------------------------------------------------------------------
NColor NThemeUtilities::GetTextHilight(void)
{	Collection		theTheme;
	RGBColor		rgbColor;
	NColor			theColor;
	SInt32			theSize;



	// Get the color
	theSize  = sizeof(RGBColor);
	theTheme = NewCollection();
	
	if (theTheme != NULL)
		{
		GetTheme(theTheme);
		GetTaggedCollectionItem(theTheme, kThemeHighlightColorTag, 1, &theSize, &rgbColor);
		DisposeCollection(theTheme);
		
		theColor.SetColor(rgbColor);
    	}
	
	return(theColor);
}





//============================================================================
//		NThemeUtilities::GetMenuTitleExtra : Get the menu title extra width.
//----------------------------------------------------------------------------
float NThemeUtilities::GetMenuTitleExtra(bool isSquished)
{	SInt16		theValue;
	OSStatus	theErr;



	// Get the value
	theValue = 0;
	theErr   = GetThemeMenuTitleExtra(&theValue, isSquished);
	NN_ASSERT_NOERR(theErr);
	
	return((float) theValue);
}





//============================================================================
//		NThemeUtilities::GetMenuItemSeparatorHeight : Get a menu item separator height.
//----------------------------------------------------------------------------
float NThemeUtilities::GetMenuItemSeparatorHeight(void)
{	SInt16		theValue;
	OSStatus	theErr;



	// Get the value
	theValue = 0;
	theErr   = GetThemeMenuSeparatorHeight(&theValue);
	NN_ASSERT_NOERR(theErr);
	
	return((float) theValue);
}





//============================================================================
//		NThemeUtilities::GetMenuItemExtra : Get a menu item extra size.
//----------------------------------------------------------------------------
HISize NThemeUtilities::GetMenuItemExtra(ThemeMenuItemType theType)
{	SInt16		theWidth, theHeight;
	HISize		theValue;
	OSStatus	theErr;



	// Get the value
	theValue = CGSizeZero;
	theErr   = GetThemeMenuItemExtra(theType, &theHeight, &theWidth);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		{
		theValue.width  = (float) theWidth;
		theValue.height = (float) theHeight;
		}
	
	return(theValue);
}





//============================================================================
//		NThemeUtilities::GetTextDimensions : Get the dimensions of a string.
//----------------------------------------------------------------------------
float NThemeUtilities::GetTextDimensions(const NString				&theText,
										 HISize						&theSize,
										 const HIThemeTextInfo		&theInfo,
										 bool						*didTruncate)
{	float				baseLine;
	HIThemeTextInfo		tmpInfo;
	OSStatus			theErr;



	// Measure the text
	baseLine = 0.0f;
	tmpInfo  = theInfo;
	
	theErr = HIThemeGetTextDimensions(theText,
										theSize.width,
										&tmpInfo,
										&theSize.width,
										&theSize.height,
										&baseLine);
	NN_ASSERT_NOERR(theErr);



	// Return the truncate state
	if (theErr == noErr && didTruncate != NULL)
		*didTruncate = tmpInfo.truncationHappened;

	return(baseLine);
}





//============================================================================
//		NThemeUtilities::GetTextDimensions : Get the dimensions of a string.
//----------------------------------------------------------------------------
HISize NThemeUtilities::GetTextDimensions(const NString				&theText,
										  const HIThemeTextInfo		&theInfo,
										  bool						*didTruncate)
{	float		baseLine;
	HISize		theSize;



	// Measure the text
	theSize  = CGSizeZero;
	baseLine = GetTextDimensions(theText, theSize, theInfo, didTruncate);
	
	return(theSize);
}





//============================================================================
//		NThemeUtilities::ShowEffect : Show a theme effect.
//----------------------------------------------------------------------------
void NThemeUtilities::ShowEffect(NThemeEffect		theEffect,
									const HIPoint	&thePoint,
									const HISize	&theSize)
{	NSAnimationEffect	nsEffect;
	NSPoint				nsPoint;
	NSSize				nsSize;



	// Get the state we need
	nsPoint = NSMakePoint(thePoint.x,    thePoint.y);
	nsSize  = NSMakeSize( theSize.width, theSize.height);
	
	switch (theEffect) {
		case kThemeEffectDisappearingItem:
			nsEffect = NSAnimationEffectDisappearingItemDefault;
			break;
		
		default:
			NN_LOG("Unknown effect: %d", theEffect);
			nsEffect = NSAnimationEffectDisappearingItemDefault;
			break;
		}



	// Show the effect
	//
	// Note that although NSShowAnimationEffect is documented to take "screen coordinates",
	// these are in fact inverted - with the origin at the bottom left of the display that
	// contains the menu bar, rather than the traditional screen coordinate system.
	nsPoint.y = CGDisplayPixelsHigh(kCGDirectMainDisplay) - nsPoint.y;

	NSShowAnimationEffect(nsEffect, nsPoint, nsSize, NULL, NULL, NULL);
}

