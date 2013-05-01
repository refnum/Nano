/*	NAME:
		NDateFormatter.h

	DESCRIPTION:
		Date formatter.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
// Date widths
typedef enum {
	kNDateWidthNarrow,
	kNDateWidthAbbrev,
	kNDateWidthFull
} NDateWidth;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Date context
typedef struct {
	NDate				theDate;
	NGregorianDate		gregDate;

	NString				theToken;
	char				tokenChar;
	NIndex				tokenSize;
} NDateContext;





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

	NString								GetTextQuarter(NIndex theIndex, NDateWidth theWidth) const;
	NString								GetTextMonth(  NIndex theIndex, NDateWidth theWidth) const;
	NString								GetTextDay(    NIndex theIndex, NDateWidth theWidth) const;

	NString								GetLiteral(const NDateContext &theContext) const;
	NString								GetEra(    const NDateContext &theContext) const;
	NString								GetYear(   const NDateContext &theContext) const;
	NString								GetQuarter(const NDateContext &theContext) const;
	NString								GetMonth(  const NDateContext &theContext) const;
	NString								GetWeek(   const NDateContext &theContext) const;
	NString								GetDay(    const NDateContext &theContext) const;
	NString								GetWeekDay(const NDateContext &theContext) const;
	NString								GetPeriod( const NDateContext &theContext) const;
	NString								GetHour(   const NDateContext &theContext) const;
	NString								GetMinute( const NDateContext &theContext) const;
	NString								GetSecond( const NDateContext &theContext) const;
	NString								GetZone(   const NDateContext &theContext) const;


private:
	

};





#endif // NDATEFORMATTER_HDR


