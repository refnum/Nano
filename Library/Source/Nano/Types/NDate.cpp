/*	NAME:
		NDate.cpp

	DESCRIPTION:
		Date object.

		Dates are stored as an offset from the Nano Epoch.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NTimeUtilities.h"
#include "NTargetTime.h"
#include "NDateFormatter.h"
#include "NEncoder.h"
#include "NString.h"
#include "NDate.h"





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NDate);





//============================================================================
//		NDate::NDate : Constructor.
//----------------------------------------------------------------------------
NDate::NDate(const NGregorianDate &theDate)
{


	// Initialize ourselves
	SetDate(theDate);
}





//============================================================================
//		NDate::NDate : Constructor.
//----------------------------------------------------------------------------
NDate::NDate(const NTime &theTime)
{


	// Initialize ourselves
	SetTime(theTime);
}





//============================================================================
//		NDate::NDate : Constructor.
//----------------------------------------------------------------------------
NDate::NDate(void)
{


	// Initialize ourselves
	mTime = NTimeUtilities::GetTime();
}





//============================================================================
//		NDate::~NDate : Destructor.
//----------------------------------------------------------------------------
NDate::~NDate(void)
{
}





//============================================================================
//      NDate::IsValid : Is the date valid?
//----------------------------------------------------------------------------
bool NDate::IsValid(void) const
{


	// Check our state
	return(!NMathUtilities::IsZero(mTime));
}





//============================================================================
//		NDate::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NDate::Compare(const NDate &theValue) const
{


	// Compare the value
	return(GetComparison(mTime, theValue.mTime));
}





//============================================================================
//		NDate::GetTime : Get the time.
//----------------------------------------------------------------------------
NTime NDate::GetTime(void) const
{


	// Get the time
	return(mTime);
}





//============================================================================
//		NDate::SetTime : Set the time.
//----------------------------------------------------------------------------
void NDate::SetTime(const NTime &theTime)
{


	// Set the time
	mTime = theTime;
}





//============================================================================
//		NDate::GetDate : Get the date.
//----------------------------------------------------------------------------
NGregorianDate NDate::GetDate(const NString &timeZone) const
{


	// Get the time
	return(NTargetTime::ConvertTimeToDate(mTime, timeZone));
}





//============================================================================
//		NDate::SetDate : Set the date.
//----------------------------------------------------------------------------
void NDate::SetDate(const NGregorianDate &theDate)
{


	// Validate our parameters
	NN_ASSERT(theDate.month  >= 1   && theDate.month  <= 12);
	NN_ASSERT(theDate.day    >= 1   && theDate.day    <= 31);
	NN_ASSERT(theDate.hour   >= 0   && theDate.hour   <= 23);
	NN_ASSERT(theDate.minute >= 0   && theDate.minute <= 59);
	NN_ASSERT(theDate.second >= 0.0 && theDate.second <= 60.0);



	// Set the time
	mTime = NTargetTime::ConvertDateToTime(theDate);
}





//============================================================================
//		NDate::GetString : Get a string.
//----------------------------------------------------------------------------
NString NDate::GetString(const NString &theFormat, const NString &timeZone) const
{	NDateFormatter		theFormatter;
	NString				theResult;
	NGregorianDate		theDate;



	// Get the time
	theDate   = GetDate(timeZone);
	theResult = theFormatter.Format(theDate, theFormat);

	return(theResult);
}





//============================================================================
//		NDate::GetInterval : Get an interval.
//----------------------------------------------------------------------------
NGregorianUnits NDate::GetInterval(const NDate &theDate) const
{	NGregorianUnits		gregDelta;
	NTime				theDelta;



	// Get the state we need
	memset(&gregDelta, 0x00, sizeof(gregDelta));

	theDelta = theDate.GetTime() - GetTime();
	if (theDelta < 0.0)
		theDelta = -theDelta;



	// Get the interval
	//
	// This is an approximation, and is inaccurate for differences longer
	// than a month (with the larger the difference, the worse the result).
	while (theDelta > 0.0)
		{
		if (theDelta >= kNTimeYearish)
			{
			gregDelta.years += 1;
			theDelta        -= kNTimeYearish;
			}
		
		else if (theDelta >= kNTimeMonthish)
			{
			gregDelta.months += 1;
			theDelta         -= kNTimeMonthish;
			}

		else if (theDelta >= kNTimeDay)
			{
			gregDelta.days += 1;
			theDelta       -= kNTimeDay;
			}
		
		else if (theDelta >= kNTimeHour)
			{
			gregDelta.hours += 1;
			theDelta        -= kNTimeHour;
			}
		
		else if (theDelta >= kNTimeMinute)
			{
			gregDelta.minutes += 1;
			theDelta          -= kNTimeMinute;
			}

		else if (theDelta > 0.0)
			{
			gregDelta.seconds = theDelta;
			break;
			}
		}

	return(gregDelta);
}





//============================================================================
//		NDate::GetDayOfWeek : Get the day of the week.
//----------------------------------------------------------------------------
NIndex NDate::GetDayOfWeek(const NString &timeZone) const
{


	// Get the day of the week
	return(NTargetTime::GetDayOfWeek(GetDate(timeZone)));
}





//============================================================================
//		NDate::GetDayOfYear : Get the day of the year.
//----------------------------------------------------------------------------
NIndex NDate::GetDayOfYear(const NString &timeZone) const
{	NGregorianDate		yearStart;
	NTime				theDelta;
	NIndex				theIndex;



	// Get the day
	yearStart = GetYearStart(timeZone);
	theDelta  = *this - NDate(yearStart);
	theIndex  = ((NIndex) floor(theDelta / kNTimeDay)) + 1;
	
	return(theIndex);
}





//============================================================================
//		NDate::GetWeekOfYear : Get the week of the year.
//----------------------------------------------------------------------------
NIndex NDate::GetWeekOfYear(const NString &timeZone) const
{	NGregorianDate		yearStart, theDate;
	NTime				theDelta;
	NIndex				theIndex;



	// Get the week
	//
	// The first week contains the 4th of January.
	theDate = GetDate(timeZone);

	if (theDate.month == 1 && theDate.day <= 4)
		theIndex = 1;
	else
		{
		yearStart = GetYearStart(timeZone, 4);
		theDelta  = *this - NDate(yearStart);
		theIndex  = ((NIndex) floor(theDelta / kNTimeWeek)) + 1;
		}
	
	return(theIndex);
}





//============================================================================
//		NDate::+= : Addition operator.
//----------------------------------------------------------------------------
const NDate& NDate::operator += (const NTime &theDelta)
{


	// Add the delta
	mTime += theDelta;
	
	return(*this);
}





//============================================================================
//		NDate::+= : Addition operator.
//----------------------------------------------------------------------------
const NDate& NDate::operator += (const NGregorianUnits &theDelta)
{


	// Add the delta
	//
	// This is an approximation, and is inaccurate for differences longer
	// than a month (with the larger the difference, the worse the result).
	mTime += (theDelta.years   * kNTimeYearish);
	mTime += (theDelta.months  * kNTimeMonthish);
	mTime += (theDelta.days    * kNTimeDay);
	mTime += (theDelta.hours   * kNTimeHour);
	mTime += (theDelta.minutes * kNTimeMinute);
	mTime +=  theDelta.seconds;

	return(*this);
}





//============================================================================
//		NDate::+ : Addition operator.
//----------------------------------------------------------------------------
NTime NDate::operator + (const NDate &theDate) const
{


	// Add the dates
	return(mTime + theDate.mTime);
}





//============================================================================
//		NDate::- : Subtraction operator.
//----------------------------------------------------------------------------
NTime NDate::operator - (const NDate &theDate) const
{


	// Subtract the dates
	return(mTime - theDate.mTime);
}





//============================================================================
//		NDate::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NDate::operator NFormatArgument(void) const
{


	// Get the value
	return(GetString());
}





#pragma mark protected
//============================================================================
//      NDate::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NDate::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNEncoderValueKey, mTime);
}





//============================================================================
//      NDate::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NDate::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	mTime = theEncoder.DecodeNumber(kNEncoderValueKey).GetFloat64();
}





#pragma mark private
//============================================================================
//		NDate::GetYearStart : Get the year start.
//----------------------------------------------------------------------------
NGregorianDate NDate::GetYearStart(const NString &timeZone, NIndex theDay) const
{	NGregorianDate		theDate;



	// Get the date
	theDate = GetDate(timeZone);

	theDate.month    = 1;
	theDate.day      = (int8_t) theDay;
	theDate.hour     = 0;
	theDate.minute   = 0;
	theDate.second   = 0;

	return(theDate);
}


