/*	NAME:
		NSystemUtilities.cpp

	DESCRIPTION:
		System utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
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
//		Internal constants
//----------------------------------------------------------------------------
enum {
	kPartNumber = 'digt',
	kPartString = 'text',
	kPartPeriod = 'dot '
};





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





//============================================================================
//		NSystemUtilities::GetMainRunLoop : Get the main thread's run loop.
//----------------------------------------------------------------------------
CFRunLoopRef NSystemUtilities::GetMainRunLoop(void)
{


	// Get the run lop
	return((CFRunLoopRef) GetCFRunLoopFromEventLoop(GetMainEventLoop()));
}





//============================================================================
//		NSystemUtilities::CompareVersions : Compare two version strings.
//----------------------------------------------------------------------------
//		Note :	This algorithm is based on Ricardo Batista's MacPad.
//
//				Version numbers are broken down into an array of parts, where
//				each part is an integer/string/period.
//
//				Each part is compared in turn, and their type is used to drive
//				the overall comparison.
//
//				Parts are compared alphabatically or numerically based on the
//				type, and versions with a different number of parts examine
//				the first "extra" part to identify the oldest version.
//----------------------------------------------------------------------------
CFComparisonResult NSystemUtilities::CompareVersions(const NString &version1, const NString &version2)
{	CFComparisonResult		theResult, shorterResult, longerResult;
	NString					part1, part2, nextPart;
	NStringList				parts1, parts2;
	OSType					type1, type2;
	UInt32					n, numParts;



	// Get the state we need
	parts1 = GetVersionParts(version1);
	parts2 = GetVersionParts(version2);



	// Examine the parts
	numParts = std::min(parts1.size(), parts2.size());
	
	for (n = 0; n < numParts; n++)
		{
		// Get the state we need
		part1 = parts1[n];
		part2 = parts2[n];
		
		type1 = GetPartType(part1);
		type2 = GetPartType(part2);



		// Compare by value
		//
		// If we have a string or a number part, we can compare values.
		if (type1 == type2)
			{
			if (type1 != kPartPeriod)
				{
				theResult = part1.Compare(part2, kCFCompareNumerically);
				if (theResult != kCFCompareEqualTo)
					return(theResult);
				}
			}


		// Or by precedence
		else
			{
			// Part 1 wins if it's a digit/period
			if (type1 != kPartString && type2 == kPartString)
				return(kCFCompareGreaterThan);
			
			// Part 2 wins if it's a digit/period
			else if (type1 == kPartString && type2 != kPartString)
				return(kCFCompareLessThan);

			// Whichever part is numeric wins
			else
				{
				if (type1 == kPartNumber)
					return(kCFCompareGreaterThan);
				else
					return(kCFCompareLessThan);
				}
			}
		}



	// Examine the largest string
	if (parts1.size() != parts2.size())
		{
		// Get its next part
		if (parts1.size() > parts2.size())
			{
			nextPart      = parts1[n];
			shorterResult = kCFCompareLessThan;
			longerResult  = kCFCompareGreaterThan;
			}
		else
			{
			nextPart      = parts2[n];
			shorterResult = kCFCompareGreaterThan;
			longerResult  = kCFCompareLessThan;
			}


		// Compare by type
		if (GetPartType(nextPart) == kPartString)
			return(shorterResult);
		else
			return(longerResult);
		}



	// If we're still here, the versions are equal
	return(kCFCompareEqualTo);
}





//============================================================================
//		NSystemUtilities::GetVersionParts : Get a version's parts.
//----------------------------------------------------------------------------
#pragma mark -
NStringList NSystemUtilities::GetVersionParts(const NString &theVersion)
{	OSType			lastType, partType;
	NString			thePart, theChar;
	UInt32			n, numChars;
	NStringList		theList;



	// Validate our parameters
	NN_ASSERT(theVersion.IsNotEmpty());



	// Get the state we need
	numChars = theVersion.GetSize();
	lastType = GetPartType(theVersion);



	// Split the version into parts
	for (n = 0; n < numChars; n++)
		{
		theChar  = theVersion.GetString(n, 1);
		partType = GetPartType(theChar);

		if (partType != lastType || partType == kPartPeriod)
			{
			theList.push_back(thePart);
			thePart.Clear();
			}

		thePart += theChar;
		lastType = partType;
		}
	
	theList.push_back(thePart);
	
	return(theList);
}





//============================================================================
//		NSystemUtilities::GetPartType : Get the type of a part.
//----------------------------------------------------------------------------
OSType NSystemUtilities::GetPartType(const NString &thePart)
{	const char	*charPtr;



	// Validate our parameters
	NN_ASSERT(thePart.IsNotEmpty());



	// Classify the part
	charPtr = thePart.GetUTF8();
	
	if (charPtr[0] == '.')
		return(kPartPeriod);

	if (isdigit(charPtr[0]))
		return(kPartNumber);
	
	return(kPartString);
}








