/*	NAME:
		NDateFormatter.cpp

	DESCRIPTION:
		Date formatter.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
NString NDateFormatter::Format(const NDate &theDate, NDateFormat theFormat)
{	NString		theResult;



	// Format the date
	switch (theFormat) {
		case kNDateSeconds:
			theResult.Format("%g", theDate.GetTime());
			break;

		default:
			NN_LOG("Unknown date formt: %d", theFormat);
			break;
		}
	
	return(theResult);
}



