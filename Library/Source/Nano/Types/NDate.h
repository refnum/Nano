/*	NAME:
		NDate.h

	DESCRIPTION:
		Date object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATE_HDR
#define NDATE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NEncodable.h"
#include "NComparable.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Time zones
//
// The default time zone is that of the user's current locale.
//
// Declared with #define to allow use within static initializers.
#define kNTimeZoneDefault											"*default*"
#define kNTimeZoneUTC												"UTC"

#define kNTimeZoneWET												"WET"
#define kNTimeZoneCET												"CET"
#define kNTimeZoneEET												"EET"
#define kNTimeZoneCEST												"CEST"
#define kNTimeZoneBST												"BST"
#define kNTimeZoneIST												"IST"

#define kNTimeZonePST												"PST"
#define kNTimeZoneMST												"MST"
#define kNTimeZoneCST												"CST"
#define kNTimeZoneEST												"EST"
#define kNTimeZonePDT												"PDT"
#define kNTimeZoneMDT												"MDT"
#define kNTimeZoneCDT												"CDT"
#define kNTimeZoneEDT												"EDT"


// Date formats
//
// Date formats are defined by <http://unicode.org/reports/tr35/tr35-6.html#Date_Format_Patterns>.
//
// Declared with #define to allow use within static initializers.
#define kNDateFormatDefault											"HH:mm yy/MM/dd"
#define kNDateFormatShort											"HH:mm yy/MM/dd"
#define kNDateFormatMedium											"hh:mma dd MMM, yyyy"
#define kNDateFormatLong											"hh:mma dd MMMM, yyyy G"


// Epochs
//
// The Nano Epoch starts at 00:00:00 on 2001/01/01 in UTC.
static const NTime kNEpochTimeSince2001								= 0.0;				// Mac OS X
static const NTime kNEpochTimeSince1970								= 978307200.0;		// Unix
static const NTime kNEpochTimeSince1904								= 3061152000.0;		// Mac OS 9
static const NTime kNEpochTimeSince1601								= 12622780800.0;	// Windows





//============================================================================
//      Types
//----------------------------------------------------------------------------
// Gregorian date
//
// Gregorian dates exist within a specific time zone.
typedef struct {
	int32_t			year;			// Absolute value
	int8_t			month;			// 1..12
	int8_t			day;			// 1..31
	int8_t			hour;			// 0..23
	int8_t			minute;			// 0..59
	NTime			second;			// 0..60
	NString			timeZone;		// Time zone
} NGregorianDate;


// Gregorian units
typedef struct {
	int32_t			years;
	int32_t			months;
	int32_t			days;
	int32_t			hours;
	int32_t			minutes;
	NTime			seconds;
} NGregorianUnits;


// Lists
class NDate;

typedef std::vector<NDate>											NDateList;
typedef NDateList::iterator											NDateListIterator;
typedef NDateList::const_iterator									NDateListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDate :	public NEncodable,
				public NComparable<NDate> {
public:
										NENCODABLE_DECLARE(NDate);

										NDate(const NGregorianDate &theDate);
										NDate(const NTime          &theTime);

										NDate(void);
	virtual							   ~NDate(void);


	// Is the date valid?
	bool								IsValid(void) const;


	// Compare the value
	NComparison							Compare(const NDate &theValue) const;


	// Get/set the time
	NTime								GetTime(void) const;
	void								SetTime(const NTime &theTime);


	// Get/set the date
	NGregorianDate						GetDate(const NString &timeZone=kNTimeZoneDefault) const;
	void								SetDate(const NGregorianDate &theDate);


	// Get a string
	NString								GetString(const NString &theFormat=kNDateFormatDefault,
												  const NString  &timeZone=kNTimeZoneDefault) const;


	// Get an interval
	NGregorianUnits						GetInterval(const NDate &theDate) const;


	// Get the day of the week
	//
	// Uses ISO 8601 numbering, where Mon..Sun is numbered 1..7.
	NIndex								GetDayOfWeek(const NString &timeZone=kNTimeZoneDefault) const;


	// Get the day of the year
	//
	// Uses ISO 8601 numbering, where days are numbered from 1.
	NIndex								GetDayOfYear(const NString &timeZone=kNTimeZoneDefault) const;


	// Get the week of the year
	//
	// Uses ISO 8601 numbering, where weeks are numbered from 1.
	//
	// The first week is the week that contains the first Thursday of the year.
	NIndex								GetWeekOfYear(const NString &timeZone=kNTimeZoneDefault) const;


	// Operators
    const NDate&                        operator += (const NTime           &theDelta);
    const NDate&                        operator += (const NGregorianUnits &theDelta);

	NTime								operator +(const NDate &theDate) const;
	NTime								operator -(const NDate &theDate) const;


	// Operators
										operator NFormatArgument(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NGregorianDate						GetYearStart(const NString &timeZone, NIndex theDay=1) const;


private:
	NTime								mTime;
};




#endif // NDATE_HDR


