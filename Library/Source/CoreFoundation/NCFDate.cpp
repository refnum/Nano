/*	NAME:
		NCFDate.cpp

	DESCRIPTION:
		CoreFoundation date object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFDate.h"





//============================================================================
//		NCFDate::NCFDate : Constructor.
//----------------------------------------------------------------------------
NCFDate::NCFDate(CFAbsoluteTime theTime)
{


	// Initialize ourselves
	InitializeSelf(theTime);
}





//============================================================================
//		NCFDate::NCFDate : Constructor.
//----------------------------------------------------------------------------
NCFDate::NCFDate(const CFGregorianDate &theDate)
{


	// Initialize ourselves
	InitializeSelf(CFGregorianDateGetAbsoluteTime(theDate, NULL));
}





//============================================================================
//		NCFDate::NCFDate : Constructor.
//----------------------------------------------------------------------------
NCFDate::NCFDate(void)
{


	// Initialize ourselves
	InitializeSelf(0.0);
}





//============================================================================
//		NCFDate::~NCFDate : Destructor.
//----------------------------------------------------------------------------
NCFDate::~NCFDate(void)
{
}





//============================================================================
//		NCFDate::GetTime : Get the time.
//----------------------------------------------------------------------------
CFAbsoluteTime NCFDate::GetTime(void) const
{


	// Get the time
	return(CFDateGetAbsoluteTime(*this));
}





//============================================================================
//		NCFDate::GetDate : Get the Date.
//----------------------------------------------------------------------------
CFGregorianDate NCFDate::GetDate(void) const
{


	// Get the date
	return(CFAbsoluteTimeGetGregorianDate(GetTime(), NULL));
}





//============================================================================
//		NCFDate::Compare : Compare two dates.
//----------------------------------------------------------------------------
CFComparisonResult NCFDate::Compare(const NCFDate &theDate) const
{


	// Compare the strings
	return(CFDateCompare(*this, theDate, NULL));
}





//============================================================================
//		NCFDate::== : Are two dates equal?
//----------------------------------------------------------------------------
const bool NCFDate::operator == (const NCFDate &theDate) const
{


	// Compare the dates
	return(Compare(theDate) == kCFCompareEqualTo);
}





//============================================================================
//		NCFDate::!= : Are two dates not equal?
//----------------------------------------------------------------------------
const bool NCFDate::operator != (const NCFDate &theDate) const
{


	// Compare the dates
	return(Compare(theDate) != kCFCompareEqualTo);
}





//============================================================================
//		NCFDate::< : Comparison operator.
//----------------------------------------------------------------------------
const bool NCFDate::operator < (const NCFDate &theDate) const
{


	// Compare the dates
	return(Compare(theDate) == kCFCompareLessThan);
}





//============================================================================
//		NCFDate::GetGregorianDate : Convert a LongDateRec to a CFGregorianDate.
//----------------------------------------------------------------------------
CFGregorianDate NCFDate::GetGregorianDate(const LongDateRec &longDate)
{	CFGregorianDate		gregDate;



	// Convert the date
	gregDate.year   = (SInt32) longDate.ld.year;
	gregDate.month  = (SInt8 ) longDate.ld.month;
	gregDate.day    = (SInt8 ) longDate.ld.day;
	gregDate.hour   = (SInt8 ) longDate.ld.hour;
	gregDate.minute = (SInt8 ) longDate.ld.minute;
	gregDate.second = (double) longDate.ld.second;
	
	return(gregDate);
}





//============================================================================
//		NCFDate::GetLongDate : Convert a CFGregorianDate to a LongDateRec.
//----------------------------------------------------------------------------
LongDateRec NCFDate::GetLongDate(const CFGregorianDate &gregDate)
{	LongDateRec		longDate;



	// Convert the date
	longDate.ld.year   = (SInt16) gregDate.year;
	longDate.ld.month  = (SInt16) gregDate.month;
	longDate.ld.day    = (SInt16) gregDate.day;
	longDate.ld.hour   = (SInt16) gregDate.hour;
	longDate.ld.minute = (SInt16) gregDate.minute;
	longDate.ld.second = (SInt16) gregDate.second;
	
	return(longDate);
}





//============================================================================
//		NCFDate::GetGregorianDate : Convert a LongDateTime to a CFGregorianDate.
//----------------------------------------------------------------------------
CFGregorianDate NCFDate::GetGregorianDate(const LongDateTime &longTime)
{	LongDateRec		longDate;



	// Convert the date
	LongSecondsToDate(&longTime, &longDate);
	
	return(GetGregorianDate(longDate));
}





//============================================================================
//		NCFDate::GetLongTime : Convert a CFGregorianDate to a LongDateTime.
//----------------------------------------------------------------------------
LongDateTime NCFDate::GetLongTime(const CFGregorianDate &gregDate)
{	LongDateTime	longTime;
	LongDateRec		longDate;



	// Convert the date
	longDate = GetLongDate(gregDate);
	
	LongDateToSeconds(&longDate, &longTime);
	
	return(longTime);
}





//============================================================================
//		NCFDate::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NCFDate::GetNullValue(void)
{


	// Get the value
	return(CFDateCreate(NULL, 0.0));
}





//============================================================================
//		NCFDate::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NCFDate::InitializeSelf(CFAbsoluteTime theTime)
{


	// Initialize ourselves
	SetMode(kCFDuplicate);
	
	Set(CFDateCreate(NULL, theTime));



	// Validate our state
	NN_ASSERT(IsValid());
}


