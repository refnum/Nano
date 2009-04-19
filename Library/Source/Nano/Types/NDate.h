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
#include "NComparable.h"





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
class NDate : public NComparable<NDate> {
public:
										 NDate(NTime theTime);
										 NDate(void);
	virtual								~NDate(void);


	// Is the date valid?
	bool								IsValid(void) const;


	// Compare the value
	NComparison							Compare(const NDate &theValue) const;


	// Get/set the time
	NTime								GetTime(void) const;
	void								SetTime(NTime theTime);


	// Operators
    const NDate&                        operator += (NTime theDelta);

	NTime								operator +(const NDate &theDate) const;
	NTime								operator -(const NDate &theDate) const;


private:
	NTime								mTime;
};





#endif // NDATE_HDR


