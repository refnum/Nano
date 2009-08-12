/*	NAME:
		NDate.h

	DESCRIPTION:
		Date object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
#include "NComparable.h"





//============================================================================
//      Types
//----------------------------------------------------------------------------
// Gregorian date
typedef struct {
	SInt32			year;			// Absolute value
	SInt8			month;			// 1..12
	SInt8			day;			// 1..31
	SInt8			hour;			// 0..23
	SInt8			minute;			// 0..59
	NTime			second;			// 0..60
} NGregorianDate;


// Lists
class NDate;

typedef std::vector<NDate>											NDateList;
typedef NDateList::iterator											NDateListIterator;
typedef NDateList::const_iterator									NDateListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDate : public NComparable<NDate> {
public:
										 NDate(const NGregorianDate &theDate);
										 NDate(      NTime           theTime);

										 NDate(void);
	virtual								~NDate(void);


	// Is the date valid?
	bool								IsValid(void) const;


	// Compare the value
	NComparison							Compare(const NDate &theValue) const;


	// Get/set the Gregorian date
	NGregorianDate						GetGregorianDate(void) const;
	void								SetGregorianDate(const NGregorianDate &theDate);


	// Get/set the time
	NTime								GetTime(void) const;
	void								SetTime(NTime theTime);


	// Operators
    const NDate&                        operator += (NTime theDelta);

	NTime								operator +(const NDate &theDate) const;
	NTime								operator -(const NDate &theDate) const;


	// Operators
										operator NFormatArgument(void) const;


private:
	NTime								mTime;
};





#endif // NDATE_HDR


