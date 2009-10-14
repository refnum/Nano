/*	NAME:
		NSystemUtilities.cpp

	DESCRIPTION:
		System utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NTextUtilities.h"
#include "NSystemUtilities.h"





//============================================================================
//		NSystemUtilities::GetGestaltValue : Get a gestalt value.
//----------------------------------------------------------------------------
UInt32 NSystemUtilities::GetGestaltValue(OSType theSelector)
{	long		theValue;
	OSStatus	theErr;



	// Get the value
	theErr = Gestalt(theSelector, &theValue);
	if (theErr != noErr)
		theValue = 0;
	
	return((UInt32) theValue);
}





//============================================================================
//		NSystemUtilities::GetSystemVersion : Get the system version.
//----------------------------------------------------------------------------
SystemVersion NSystemUtilities::GetSystemVersion(void)
{	static SystemVersion	sSystemVersion = kSystemUnknown;

	UInt32	verMajor, verMinor, verBugFix;



	// Construct the version
	//
	// Note that SystemVersion is not in gestaltSystemVersion format,
	// to support more than 9 minor and bug fix releases.
	if (sSystemVersion == kSystemUnknown)
		{
		verMajor  = GetGestaltValue(gestaltSystemVersionMajor);
		verMinor  = GetGestaltValue(gestaltSystemVersionMinor);
		verBugFix = GetGestaltValue(gestaltSystemVersionBugFix);
		
		verMajor <<= 16;
		verMinor <<=  8;
		
		sSystemVersion = (SystemVersion) (verMajor | verMinor | verBugFix);
		}
	
	return(sSystemVersion);
}





//============================================================================
//		NSystemUtilities::SystemIsAtLeast : Test the system version.
//----------------------------------------------------------------------------
bool NSystemUtilities::SystemIsAtLeast(SystemVersion theVersion)
{


	// Check the system version
	return(GetSystemVersion() >= theVersion);
}





//============================================================================
//		NSystemUtilities::EventTimeToDuration : Convert an EventTime to a Duration.
//----------------------------------------------------------------------------
Duration NSystemUtilities::EventTimeToDuration(EventTime theTime)
{	Duration	theDuration;



	// Convert the time
	if (NMathUtilities::AreEqual(theTime, kEventDurationForever))
		theDuration = kDurationForever;
	else
		theDuration = (Duration) (theTime / kEventDurationMillisecond);
	
	return(theDuration);
}





//============================================================================
//		NSystemUtilities::ShowEffect : Show an animation effect.
//----------------------------------------------------------------------------
void NSystemUtilities::ShowEffect(NAnimationEffect theEffect,
									const HIPoint	&thePoint,
									const HISize	&theSize)
{	NSAnimationEffect	nsEffect;
	NSPoint				nsPoint;
	NSSize				nsSize;



	// Get the state we need
	nsPoint = NSMakePoint(thePoint.x,    thePoint.y);
	nsSize  = NSMakeSize( theSize.width, theSize.height);
	
	switch (theEffect) {
		case kAnimationDisappearingItem:
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
	// contains the menu bar.
	nsPoint.y = CGDisplayPixelsHigh(kCGDirectMainDisplay) - nsPoint.y;

	NSShowAnimationEffect(nsEffect, nsPoint, nsSize, NULL, NULL, NULL);
}





//============================================================================
//		NSystemUtilities::GetLabelNames : Get the Finder label names.
//----------------------------------------------------------------------------
NStringList NSystemUtilities::GetLabelNames(void)
{	static const UInt16		kLabelIndices[7] = { 6, 7, 5, 2, 4, 3, 1 };
	NStringList				theNames;
	RGBColor				qdColor;
	Str255					theStr;
	UInt32					n;



	// Get the label names
	//
	// The order of label names changed from Classic to Mac OS X, so we
	// need to push the index through a table to obtain the correct index.
	//
	// In addition, the CopyLabelColorAndName function used by the Finder to
	// obtain a CFString-based name is an SPI (rdar://4801991) so we need to
	// fetch the name via a Str255 using the system encoding.
	for (n = 0; n < 7; n++)
		{
		GetLabel(kLabelIndices[n], &qdColor, theStr);
		theNames.push_back(NTextUtilities::FromStr255(theStr));
		}
	
	return(theNames);
}





