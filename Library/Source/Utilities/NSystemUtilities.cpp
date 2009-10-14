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
//		Include Systems
//----------------------------------------------------------------------------
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
	if (theTime == kEventDurationForever)
		theDuration = kDurationForever;
	else
		theDuration = (Duration) (theTime / kEventDurationMillisecond);
	
	return(theDuration);
}


