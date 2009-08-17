/*	NAME:
		NDate.cpp

	DESCRIPTION:
		Dates are stored as an offset from the epoch (00:00:00 on 2001/01/01),
		in the UTC time zone.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NTimeUtilities.h"
#include "NTargetPOSIX.h"
#include "NDateFormatter.h"
#include "NEncoder.h"
#include "NString.h"
#include "NDate.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NTime kTimeSinceEpoch19070							= 978307200.0;





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
DEFINE_NENCODABLE(NDate);





//============================================================================
//		NDate::NDate : Constructor.
//----------------------------------------------------------------------------
NDate::NDate(const NGregorianDate &theDate, const NString &timeZone)
{


	// Initialize ourselves
	SetGregorianDate(theDate, timeZone);
}





//============================================================================
//		NDate::NDate : Constructor.
//----------------------------------------------------------------------------
NDate::NDate(const NTime &theTime)
{


	// Initialize ourselves
	mTime = theTime;
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
//		NDate::GetString : Get as a string.
//----------------------------------------------------------------------------
NString NDate::GetString(const NString &theFormat, const NString &timeZone) const
{	NDateFormatter		theFormatter;
	NString				theResult;



	// Get the value
	theResult = theFormatter.Format(*this, theFormat, timeZone);

	return(theResult);
}





//============================================================================
//		NDate::GetGregorianDate : Get the Gregorian date.
//----------------------------------------------------------------------------
NGregorianDate NDate::GetGregorianDate(const NString &timeZone) const
{	NTime				secsFloor, secsFrac;
	time_t				timeUnix;
	struct tm			timeGreg;
	NGregorianDate		theDate;



	// TO DO - need to support time zones
	NN_UNUSED(timeZone);
	NN_LOG("NDate only supports UTC");



	// Get the state we need
	secsFloor = floor(mTime);
	secsFrac  = mTime - secsFloor;

	timeUnix = (time_t) (secsFloor + kTimeSinceEpoch19070);
	timeGreg = NTargetPOSIX::gmtime(timeUnix);



	// Convert the date
	theDate.year   = timeGreg.tm_year + 1900;
	theDate.month  = timeGreg.tm_mon  + 1;
	theDate.day    = timeGreg.tm_mday;
	theDate.hour   = timeGreg.tm_hour;
	theDate.minute = timeGreg.tm_min;
	theDate.second = ((Float64) timeGreg.tm_sec) + secsFrac;

	return(theDate);
}





//============================================================================
//		NDate::SetGregorianDate : Set the Gregorian date.
//----------------------------------------------------------------------------
void NDate::SetGregorianDate(const NGregorianDate &theDate, const NString &timeZone)
{	NTime			secsFloor, secsFrac;
	time_t			timeUnix;
	struct tm		timeGreg;



	// TO DO - need to support time zones
	if (timeZone != kNTimeZoneUTC)
		NN_LOG("NDate only supports UTC");



	// Get the state we need
	secsFloor = floor(theDate.second);
	secsFrac  = theDate.second - secsFloor;
	
	memset(&timeGreg, 0x00, sizeof(timeGreg));



	// Convert the date
	timeGreg.tm_year = theDate.year  - 1900;
	timeGreg.tm_mon  = theDate.month - 1;
	timeGreg.tm_mday = theDate.day;
	timeGreg.tm_hour = theDate.hour;
	timeGreg.tm_min  = theDate.minute;
	timeGreg.tm_sec  = (int) secsFloor;

	timeUnix = NTargetPOSIX::timegm(&timeGreg);
	mTime    = (timeUnix - kTimeSinceEpoch19070) + secsFrac;
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

	// TO DO
	NN_UNUSED(theDelta);
	NN_LOG("NDate does not support NGregorianUnits addition");



	// Add the delta
	
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





//============================================================================
//      NDate::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
#pragma mark -
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








