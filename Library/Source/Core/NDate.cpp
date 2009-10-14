/*	NAME:
		NDate.cpp

	DESCRIPTION:
		Like a CFDate, dates are always stored as a CFAbsoluteTime in the UTC
		time zone.
		
		Dates are translated to/from a specific time zone when converted to
		or from an alternative form, such as a Gregorian date or a string.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NDate.h"





//============================================================================
//		NDate::NDate : Constructor.
//----------------------------------------------------------------------------
NDate::NDate(const CFGregorianDate &theDate, const NString &timeZone)
{


	// Initialize ourselves
	mTime = CFGregorianDateGetAbsoluteTime(theDate, GetTimeZone(timeZone));
}





//============================================================================
//		NDate::NDate : Constructor.
//----------------------------------------------------------------------------
NDate::NDate(const NString &theDate)
{	NCFObject	cfFormatter, cfDate;



	// Create the formatter
	cfFormatter = GetDateFormatter(kCFDateFormatterFullStyle, kCFDateFormatterFullStyle, "");
	if (cfFormatter.IsValid())
		CFDateFormatterSetProperty(cfFormatter, kCFDateFormatterIsLenient, kCFBooleanTrue);



	// Parse the date
	if (cfDate.Set(CFDateFormatterCreateDateFromString(kCFAllocatorNano, cfFormatter, theDate, NULL)))
		mTime = CFDateGetAbsoluteTime(cfDate);
	else
		mTime = CFAbsoluteTimeGetCurrent();
}





//============================================================================
//		NDate::NDate : Constructor.
//----------------------------------------------------------------------------
NDate::NDate(CFDateRef theDate)
{


	// Initialize ourselves
	mTime = CFDateGetAbsoluteTime(theDate);
}





//============================================================================
//		NDate::NDate : Constructor.
//----------------------------------------------------------------------------
NDate::NDate(CFAbsoluteTime theTime)
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
	mTime = CFAbsoluteTimeGetCurrent();
}





//============================================================================
//		NDate::~NDate : Constructor.
//----------------------------------------------------------------------------
NDate::~NDate(void)
{
}





//============================================================================
//		NDate::GetDate : Get the date.
//----------------------------------------------------------------------------
NCFObject NDate::GetDate(void) const
{	NCFObject	cfDate;



	// Get the date
	cfDate.Set(CFDateCreate(kCFAllocatorNano, mTime));
	
	return(cfDate);
}





//============================================================================
//		NDate::SetDate : Set the date.
//----------------------------------------------------------------------------
void NDate::SetDate(const NCFObject &theDate)
{


	// Set the time
	SetTime(CFDateGetAbsoluteTime(theDate));
}





//============================================================================
//		NDate::GetTime : Get the time.
//----------------------------------------------------------------------------
CFAbsoluteTime NDate::GetTime(void) const
{


	// Get the time
	return(mTime);
}





//============================================================================
//		NDate::SetTime : Set the time.
//----------------------------------------------------------------------------
void NDate::SetTime(CFAbsoluteTime theTime)
{


	// Set the time
	mTime = theTime;
}





//============================================================================
//		NDate::GetAsString : Get the date as a string.
//----------------------------------------------------------------------------
NString NDate::GetAsString(CFDateFormatterStyle dateStyle, CFDateFormatterStyle timeStyle, const NString &timeZone) const
{	NCFObject	cfFormatter;
	NString		theString;



	// Get the date
	cfFormatter = GetDateFormatter(dateStyle, timeStyle, timeZone);

	if (cfFormatter.IsValid())
		theString.Set(CFDateFormatterCreateStringWithAbsoluteTime(kCFAllocatorNano, cfFormatter, mTime));
	
	return(theString);
}





//============================================================================
//		NDate::GetAsGregorian : Get the date as a gregorian date.
//----------------------------------------------------------------------------
CFGregorianDate NDate::GetAsGregorian(const NString &timeZone) const
{


	// Get the date
	return(CFAbsoluteTimeGetGregorianDate(mTime, GetTimeZone(timeZone)));
}





//============================================================================
//		NString::+= : Addition operator.
//----------------------------------------------------------------------------
const NDate& NDate::operator += (CFTimeInterval theDelta)
{


	// Add the delta
	mTime += theDelta;
	
	return(*this);
}





//============================================================================
//		NDate::+ : Addition operator.
//----------------------------------------------------------------------------
CFTimeInterval NDate::operator + (const NDate &theDate) const
{


	// Add the dates
	return(mTime + theDate.mTime);
}





//============================================================================
//		NDate::- : Subtraction operator.
//----------------------------------------------------------------------------
CFTimeInterval NDate::operator - (const NDate &theDate) const
{


	// Subtract the dates
	return(mTime - theDate.mTime);
}





//============================================================================
//		NDate::Compare : Compare two objects.
//----------------------------------------------------------------------------
#pragma mark -
CFComparisonResult NDate::Compare(const NComparable &theObject) const
{	const NDate		*theDate = dynamic_cast<const NDate*>(&theObject);



	// Validate our parameters
	NN_ASSERT(theDate != NULL);



	// Compare the dates
	return(GET_CF_COMPARE(mTime, theDate->mTime));
}





//============================================================================
//		NDate::GetTimeZone : Get a time zone.
//----------------------------------------------------------------------------
#pragma mark -
NCFObject NDate::GetTimeZone(const NString &timeZone) const
{	NCFObject	cfTimeZone;



	// Create the time zone
	if (timeZone == kTimeZoneSystem)
		cfTimeZone.Set(CFTimeZoneCopySystem());
	
	else if (timeZone == kTimeZoneDefault)
		cfTimeZone.Set(CFTimeZoneCopyDefault());
	
	else if (timeZone.IsNotEmpty())
		cfTimeZone.Set(CFTimeZoneCreateWithName(kCFAllocatorNano, timeZone, true));

	return(cfTimeZone);
}





//============================================================================
//		NDate::GetDateFormatter : Get a date formatter.
//----------------------------------------------------------------------------
NCFObject NDate::GetDateFormatter(CFDateFormatterStyle dateStyle, CFDateFormatterStyle timeStyle, const NString &timeZone) const
{	NCFObject	cfTimeZone, cfLocale, cfFormatter;



	// Get the state we need
	cfTimeZone = GetTimeZone(timeZone);
	cfLocale.Set(CFLocaleCopyCurrent());



	// Create the date formatter
	if (cfLocale.IsValid())
		cfFormatter.Set(CFDateFormatterCreate(kCFAllocatorNano, cfLocale, dateStyle, timeStyle));
	
	if (cfFormatter.IsValid() && cfTimeZone.IsValid())
		CFDateFormatterSetProperty(cfFormatter, kCFDateFormatterTimeZone, cfTimeZone);

	return(cfFormatter);
}

