/*	NAME:
		NDate.h

	DESCRIPTION:
		Date object.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
// The default time zone is that of the current user.
static const NString kNTimeZoneDefault								= "*default*";
static const NString kNTimeZoneUTC									= "UTC";

static const NString kNTimeZoneWET									= "WET";
static const NString kNTimeZoneCET									= "CET";
static const NString kNTimeZoneEET									= "EET";

static const NString kNTimeZonePST									= "PST";
static const NString kNTimeZoneMST									= "MST";
static const NString kNTimeZoneCST									= "CST";
static const NString kNTimeZoneEST									= "EST";


// Date formats
//
// Date formats are defined by <http://unicode.org/reports/tr35/tr35-6.html#Date_Format_Patterns>.
static const NString kNDateFormatDefault							= "HH:mm yy/mm/dd";
static const NString kNDateFormatShort								= "HH:mm yy/mm/dd";
static const NString kNDateFormatMedium								= "hh:mma dd mmm, yyyy";
static const NString kNDateFormatLong								= "hh:mma dd mmmm, yyyy G";


// Epochs
//
// The Nano Epoch starts at 00:00:00 on 2001/01/01 in UTC.
static const NTime kNEpochTimeSince2001								= 0.0;				// CF
static const NTime kNEpochTimeSince1970								= 978307200.0;		// Unix
static const NTime kNEpochTimeSince1904								= 3061152000.0;		// Classic Mac
static const NTime kNEpochTimeSince1601								= 14705625600.0;	// Windows





//============================================================================
//      Types
//----------------------------------------------------------------------------
// Gregorian date
//
// Gregorian dates only exist relative to a given time zone.
typedef struct {
	SInt32			year;			// Absolute value
	SInt8			month;			// 1..12
	SInt8			day;			// 1..31
	SInt8			hour;			// 0..23
	SInt8			minute;			// 0..59
	NTime			second;			// 0..60
} NGregorianDate;


// Gregorian units
typedef struct {
	SInt32			years;
	SInt32			months;
	SInt32			days;
	SInt32			hours;
	SInt32			minutes;
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

										NDate(const NGregorianDate &theDate, const NString &timeZone);
										NDate(const NTime          &theTime);

										NDate(void);
	virtual							   ~NDate(void);


	// Is the date valid?
	bool								IsValid(void) const;


	// Compare the value
	NComparison							Compare(const NDate &theValue) const;


	// Get as a string
	NString								GetString(const NString &theFormat=kNDateFormatDefault,
												  const NString  &timeZone=kNTimeZoneDefault) const;


	// Get/set the Gregorian date
	NGregorianDate						GetGregorianDate(const NString &timeZone=kNTimeZoneDefault) const;
	void								SetGregorianDate(const NGregorianDate &theDate, const NString &timeZone);


	// Get/set the time
	NTime								GetTime(void) const;
	void								SetTime(const NTime &theTime);


	// Get an interval
	NGregorianUnits						GetInterval(const NDate &theDate) const;


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
	NTime								mTime;
};





#endif // NDATE_HDR


