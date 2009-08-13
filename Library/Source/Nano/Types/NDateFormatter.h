/*	NAME:
		NDateFormatter.h

	DESCRIPTION:
		Date formatter.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
//		Constants
//----------------------------------------------------------------------------
// Date formats
typedef enum {
	kNDateSeconds
} NDateFormat;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDateFormatter {
public:
										 NDateFormatter(void);
	virtual								~NDateFormatter(void);


	// Format a date
	NString								Format(const NDate &theDate, NDateFormat theFormat=kNDateSeconds);


private:


};





#endif // NDATEFORMATTER_HDR


