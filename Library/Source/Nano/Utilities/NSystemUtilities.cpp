/*	NAME:
		NSystemUtilities.cpp

	DESCRIPTION:
		System utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NMathUtilities.h"
#include "NTargetThread.h"
#include "NThread.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NIndex kPartNumber										= 100;
static const NIndex kPartString										= 101;
static const NIndex kPartPeriod										= 102;





//============================================================================
//		NSystemUtilities::GetBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NSystemUtilities::GetBoolean(const NVariant &theValue, const NString &debugID)
{	bool	theResult;



	// Get the value
	theResult = false;

	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to bool", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetSInt32 : Get a SInt32 value.
//----------------------------------------------------------------------------
SInt32 NSystemUtilities::GetSInt32(const NVariant &theValue, const NString &debugID)
{	NNumber		theNumber;
	SInt32		theResult;



	// Get the value
	if (!theNumber.SetValue(theValue))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to SInt32", debugID);
		}
	
	theResult = theNumber.GetSInt32();

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetSInt64 : Get a SInt64 value.
//----------------------------------------------------------------------------
SInt64 NSystemUtilities::GetSInt64(const NVariant &theValue, const NString &debugID)
{	NNumber		theNumber;
	SInt64		theResult;



	// Get the value
	if (!theNumber.SetValue(theValue))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to SInt64", debugID);
		}

	theResult = theNumber.GetSInt64();

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NSystemUtilities::GetFloat32(const NVariant &theValue, const NString &debugID)
{	NNumber		theNumber;
	Float32		theResult;



	// Get the value
	if (!theNumber.SetValue(theValue))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to Float32", debugID);
		}

	theResult = theNumber.GetFloat32();

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NSystemUtilities::GetFloat64(const NVariant &theValue, const NString &debugID)
{	NNumber		theNumber;
	Float64		theResult;



	// Get the value
	if (!theNumber.SetValue(theValue))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to Float64", debugID);
		}

	theResult = theNumber.GetFloat64();

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetString : Get a string value.
//----------------------------------------------------------------------------
NString NSystemUtilities::GetString(const NVariant &theValue, const NString &debugID)
{	NString		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NString", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetData : Get a data value.
//----------------------------------------------------------------------------
NData NSystemUtilities::GetData(const NVariant &theValue, const NString &debugID)
{	NData		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NData", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetDate : Get a date value.
//----------------------------------------------------------------------------
NDate NSystemUtilities::GetDate(const NVariant &theValue, const NString &debugID)
{	NDate		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NDate", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetColor : Get a color value.
//----------------------------------------------------------------------------
NColor NSystemUtilities::GetColor(const NVariant &theValue, const NString &debugID)
{	NColor		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NColor", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetPoint : Get a point value.
//----------------------------------------------------------------------------
NPoint NSystemUtilities::GetPoint(const NVariant &theValue, const NString &debugID)
{	NPoint		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NPoint", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetSize : Get a size value.
//----------------------------------------------------------------------------
NSize NSystemUtilities::GetSize(const NVariant &theValue, const NString &debugID)
{	NSize		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NSize", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetRectangle : Get a rectangle value.
//----------------------------------------------------------------------------
NRectangle NSystemUtilities::GetRectangle(const NVariant &theValue, const NString &debugID)
{	NRectangle	theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NRectangle", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetVector : Get a vector value.
//----------------------------------------------------------------------------
NVector NSystemUtilities::GetVector(const NVariant &theValue, const NString &debugID)
{	NVector		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NVector", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetArray : Get an array value.
//----------------------------------------------------------------------------
NArray NSystemUtilities::GetArray(const NVariant &theValue, const NString &debugID)
{	NArray		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NArray", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::GetDictionary : Get a dictionary value.
//----------------------------------------------------------------------------
NDictionary NSystemUtilities::GetDictionary(const NVariant &theValue, const NString &debugID)
{	NDictionary		theResult;



	// Get the value
	if (!theValue.GetValue(theResult))
		{
		if (NN_DEBUG && theValue.IsValid() && !debugID.IsEmpty())
			NN_LOG("Unable to convert value [%@] to NDictionary", debugID);
		}

	return(theResult);
}





//============================================================================
//		NSystemUtilities::DelayFunctor : Delay a functor.
//----------------------------------------------------------------------------
void NSystemUtilities::DelayFunctor(const NFunctor &theFunctor, NTime theDelay, bool onMainThread)
{	NTimer		*theTimer;



	// Invoke immediately
	//
	// If we're to invoke on a new thread, or are already on the main
	// thread, we can invoke the functor directly without any delay.
	//
	// If we can't (we have a delay or we're not the main thread but
	// the functor must execute on the main thread), we fall through
	// to the timer case.
	if (NMathUtilities::IsZero(theDelay))
		{
		if (!onMainThread)
			{
			NTargetThread::ThreadCreate(theFunctor);
			return;
			}
		
		else if (NThread::IsMain())
			{
			theFunctor();
			return;
			}
		}



	// Invoke with a delay
	theTimer = new NTimer;
	if (theTimer != NULL)
		theTimer->AddTimer(BindFunction(NSystemUtilities::DelayedFunctor, theTimer, theFunctor, onMainThread), theDelay);
}





//============================================================================
//		NSystemUtilities::CompareVersions : Compare two version strings.
//----------------------------------------------------------------------------
//		Note :	Algorithm is based on Ricardo Batista's MacPad.
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
NComparison NSystemUtilities::CompareVersions(const NString &version1, const NString &version2)
{	NComparison				theResult, shorterResult, longerResult;
	NString					part1, part2, nextPart;
	NStringList				parts1, parts2;
	NIndex					type1, type2;
	UInt32					n, numParts;



	// Quick check
	if (version1 == version2)
		return(kNCompareEqualTo);



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
				theResult = part1.Compare(part2, kNStringNumeric);
				if (theResult != kNCompareEqualTo)
					return(theResult);
				}
			}


		// Or by precedence
		else
			{
			// Part 1 wins if it's a digit/period
			if (type1 != kPartString && type2 == kPartString)
				return(kNCompareGreaterThan);
			
			// Part 2 wins if it's a digit/period
			else if (type1 == kPartString && type2 != kPartString)
				return(kNCompareLessThan);

			// Whichever part is numeric wins
			else
				{
				if (type1 == kPartNumber)
					return(kNCompareGreaterThan);
				else
					return(kNCompareLessThan);
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
			shorterResult = kNCompareLessThan;
			longerResult  = kNCompareGreaterThan;
			}
		else
			{
			nextPart      = parts2[n];
			shorterResult = kNCompareGreaterThan;
			longerResult  = kNCompareLessThan;
			}


		// Compare by type
		if (GetPartType(nextPart) == kPartString)
			return(shorterResult);
		else
			return(longerResult);
		}



	// If we're still here, the versions are equal
	return(kNCompareEqualTo);
}





//============================================================================
//		NSystemUtilities::DelayedFunctor : Execute a delayed functor.
//----------------------------------------------------------------------------
#pragma mark -
void NSystemUtilities::DelayedFunctor(NTimer *theTimer, const NFunctor &theFunctor, bool onMainThread)
{


	// Validate our state
	NN_ASSERT(NThread::IsMain());



	// Invoke the functor
	if (onMainThread)
		theFunctor();
	else
		NTargetThread::ThreadCreate(theFunctor);



	// Clean up
	delete theTimer;
}





//============================================================================
//		NSystemUtilities::GetVersionParts : Get a version's parts.
//----------------------------------------------------------------------------
NStringList NSystemUtilities::GetVersionParts(const NString &theVersion)
{	NIndex			lastType, partType;
	NString			thePart, theChar;
	UInt32			n, numChars;
	NStringList		theList;



	// Validate our parameters
	NN_ASSERT(!theVersion.IsEmpty());



	// Get the state we need
	numChars = theVersion.GetSize();
	lastType = GetPartType(theVersion);



	// Split the version into parts
	for (n = 0; n < numChars; n++)
		{
		theChar  = theVersion.GetString(NRange(n, 1));
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
NIndex NSystemUtilities::GetPartType(const NString &thePart)
{	const char	*charPtr;



	// Validate our parameters
	NN_ASSERT(!thePart.IsEmpty());



	// Classify the part
	charPtr = thePart.GetUTF8();
	
	if (charPtr[0] == '.')
		return(kPartPeriod);

	if (isdigit(charPtr[0]))
		return(kPartNumber);
	
	return(kPartString);
}



