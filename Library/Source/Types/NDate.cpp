/*	NAME:
		NDate.cpp

	DESCRIPTION:
		Dates are stored as an offset from the epoch (00:00:00 on 2000/01/01),
		in the UTC time zone.

		Dates are translated to/from a specific time zone when converted to
		or from an alternative form, such as a Gregorian date or a string.

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
#include "NDate.h"





//============================================================================
//		NDate::NDate : Constructor.
//----------------------------------------------------------------------------
NDate::NDate(NTime theTime)
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
	mTime = 0.0f;				// dair, get current time in UTC
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
	return(NMathUtilities::NotZero(mTime));
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
void NDate::SetTime(NTime theTime)
{


	// Set the time
	mTime = theTime;
}





//============================================================================
//		NDate::+= : Addition operator.
//----------------------------------------------------------------------------
const NDate& NDate::operator += (NTime theDelta)
{


	// Add the delta
	mTime += theDelta;
	
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



