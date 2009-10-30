/*	NAME:
		NDate.h

	DESCRIPTION:
		Date object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATE_HDR
#define NDATE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NComparable.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Time zones
static const NString kTimeZoneSystem								= "meta*system";
static const NString kTimeZoneDefault								= "meta*default";
static const NString kTimeZoneUTC									= "UTC";

static const NString kTimeZoneWET									= "WET";
static const NString kTimeZoneCET									= "CET";
static const NString kTimeZoneEET									= "EET";

static const NString kTimeZonePST									= "PST";
static const NString kTimeZoneMST									= "MST";
static const NString kTimeZoneCST									= "CST";
static const NString kTimeZoneEST									= "EST";

// Dates
static const CFGregorianDate  kCFGregorianDateZero					= { 0, 0, 0, 0, 0, 0.0 };
static const CFGregorianUnits kCFGregorianUnitsZero					= { 0, 0, 0, 0, 0, 0.0 };





//============================================================================
//      Types
//----------------------------------------------------------------------------
class NDate;

typedef std::vector<NDate>											NDateList;
typedef NDateList::iterator											NDateListIterator;
typedef NDateList::const_iterator									NDateListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDate : public NComparable {
public:
										NDate(const CFGregorianDate &theDate, const NString &timeZone);
										NDate(const NString         &theDate);

										NDate(CFDateRef              theDate);
										NDate(CFAbsoluteTime         theTime);

										NDate(void);
	virtual								~NDate(void);


	// Is the date valid?
	bool								IsValid(void) const;
	

	// Get/set the date
	NCFObject							GetDate(void) const;
	void								SetDate(const NCFObject &theDate);


	// Get/set the time
	CFAbsoluteTime						GetTime(void) const;
	void								SetTime(CFAbsoluteTime theTime);


	// Get the date as a string
	//
	// String formats are defined by <http://unicode.org/reports/tr35/tr35-6.html#Date_Format_Patterns>.
	NString								GetAsString(const NString &formatString,
													const NString &timeZone=kTimeZoneDefault) const;

	NString								GetAsString(CFDateFormatterStyle   dateStyle=kCFDateFormatterShortStyle,
													CFDateFormatterStyle   timeStyle=kCFDateFormatterMediumStyle,
													const NString          &timeZone=kTimeZoneDefault) const;


	// Get the date as a gregorian date
	CFGregorianDate						GetAsGregorian(const NString &timeZone=kTimeZoneDefault) const;


	// Operators
    const NDate&                        operator += (      CFTimeInterval    theDelta);
    const NDate&                        operator += (const CFGregorianUnits &theDelta);

	CFTimeInterval						operator +	(const NDate &theDate) const;
	CFTimeInterval						operator -  (const NDate &theDate) const;


protected:
	// Compare two objects
	CFComparisonResult					Compare(const NComparable &theObject) const;


private:
	NCFObject							GetTimeZone(const NString &timeZone) const;

	NCFObject							GetDateFormatter(CFDateFormatterStyle	dateStyle,
														 CFDateFormatterStyle	timeStyle,
														 const NString			&timeZone) const;


private:
	CFAbsoluteTime						mTime;
};





#endif // NDATE_HDR


