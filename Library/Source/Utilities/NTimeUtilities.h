/*	NAME:
		NTimeUtilities.h

	DESCRIPTION:
		Time utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTIMEUTILITIES_HDR
#define NTIMEUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDate.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTimeUtilities {
public:
	// Get the current time
	static UInt32						GetTimeInMS(void);
	static UInt64						GetTimeInUS(void);


	// Get the interval between two dates
	static CFGregorianUnits				GetDateInterval(const NDate          &timeStart,
														const NDate          &timeEnd,
														CFGregorianUnitFlags theUnits=kCFGregorianAllUnits);


	// Convert between a LongDateRec and a GregorianDate
	static CFGregorianDate				GetGregorianDate(const LongDateRec     &longDate);
	static LongDateRec					GetLongDate(     const CFGregorianDate &gregDate);


	// Convert between a LongDateTime and a GregorianDate
	static CFGregorianDate				GetGregorianDate(const LongDateTime    &longTime);
	static LongDateTime					GetLongTime(     const CFGregorianDate &gregDate);
};





#endif // NTIMEUTILITIES_HDR


