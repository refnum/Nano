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





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Date formats
//
// Date formats are defined by <http://unicode.org/reports/tr35/tr35-6.html#Date_Format_Patterns>.
//
// However we also support some meta-formats, whose exact representation may
// between releases and targets (or due to the user's preferences).
static const NString kNDateFormatDefault								= "";
static const NString kNDateFormatShort									= "*short*";
static const NString kNDateFormatMedium									= "*medium*";
static const NString kNDateFormatLong									= "*long*";





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NDate;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDateFormatter {
public:
										 NDateFormatter(void);
	virtual								~NDateFormatter(void);


	// Format a date
	NString								Format(const NDate &theDate, const NString &theFormat=kNDateFormatDefault) const;


private:


};





#endif // NDATEFORMATTER_HDR


