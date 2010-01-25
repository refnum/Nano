/*	NAME:
		NDateFormatter.h

	DESCRIPTION:
		Date formatter.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATEFORMATTER_HDR
#define NDATEFORMATTER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NDate.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDateFormatter {
public:
										NDateFormatter(void);
	virtual							   ~NDateFormatter(void);


	// Format a date
	NString								Format(	const NDate		&theDate,
												const NString	&theFormat=kNDateFormatDefault,
												const NString	 &timeZone=kNTimeZoneDefault) const;


private:


};





#endif // NDATEFORMATTER_HDR


