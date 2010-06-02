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
//		Constants
//----------------------------------------------------------------------------
typedef enum {
	kDateWidthNarrow,
	kDateWidthAbbrev,
	kDateWidthFull
} DateWidth;





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

	NIndex								GetDayOfWeek( const NGregorianDate &theDate) const;
	NIndex								GetDayOfYear( const NGregorianDate &theDate) const;
	NIndex								GetWeekOfYear(const NGregorianDate &theDate) const;
	NGregorianDate						GetYearStart( const NGregorianDate &theDate) const;

	NString								GetTextQuarter(NIndex theIndex, DateWidth theWidth) const;
	NString								GetTextMonth(  NIndex theIndex, DateWidth theWidth) const;
	NString								GetTextDay(    NIndex theIndex, DateWidth theWidth) const;

	NString								GetLiteral(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetEra(    const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetYear(   const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetQuarter(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetMonth(  const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetWeek(   const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetDay(    const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetWeekDay(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetPeriod( const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetHour(   const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetMinute( const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetSecond( const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;
	NString								GetZone(   const NGregorianDate &theDate, char tokenChar, const NString &theToken) const;


private:
	

};





#endif // NDATEFORMATTER_HDR


