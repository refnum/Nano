/*	NAME:
		NDateFormatter.cpp

	DESCRIPTION:
		Date formatter.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDateFormatter.h"





//============================================================================
//		NDateFormatter::NDateFormatter : Constructor.
//----------------------------------------------------------------------------
NDateFormatter::NDateFormatter(void)
{
}





//============================================================================
//		NDateFormatter::~NDateFormatter : Destructor.
//----------------------------------------------------------------------------
NDateFormatter::~NDateFormatter(void)
{
}





//============================================================================
//		NDateFormatter::Format : Format a date.
//----------------------------------------------------------------------------
NString NDateFormatter::Format(const NDate &theDate, const NString &theFormat, const NString &timeZone) const
{	NString				formatStr, theResult;
	NGregorianDate		gregDate;



	// TO DO - need to support time zones
	NN_UNUSED(timeZone);
	NN_LOG("NDateFormatter only supports UTC");



	// Get the state we need
	gregDate  = theDate.GetGregorianDate();
	formatStr = theFormat;
	
	
	
	// Convert the format
	if (formatStr == kNDateFormatDefault)
		{ }
	
	else if (formatStr == kNDateFormatShort)
		{ }
	
	else if (formatStr == kNDateFormatMedium)
		{ }
	
	else if (formatStr == kNDateFormatLong)
		{ }



	// Format the date
	NN_LOG("NDateFormatter::Format - NOT IMPLEMENTED");
	theResult.Format("%g", theDate.GetTime());
	
	return(theResult);
}



