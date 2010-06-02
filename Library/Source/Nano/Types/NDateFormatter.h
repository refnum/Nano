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
	//
	// Date formats are defined by <http://unicode.org/reports/tr35/tr35-6.html#Date_Format_Patterns>.
	NString								Format(	const NGregorianDate	&theDate,
												const NString			&theFormat=kNDateFormatDefault) const;


private:
	const char						   *GetTokenEnd(const char *tokenStart) const;

	NString								GetLiteral(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetEra(    const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetYear(   const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;


private:
	

};





#endif // NDATEFORMATTER_HDR


