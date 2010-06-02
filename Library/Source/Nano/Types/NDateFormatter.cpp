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
#include "NTargetTime.h"
#include "NDateFormatter.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Translations
//
// TODO - move to an external strings table to support localisation.
static const char *kTextQuarterNarrow[]									= { "Q1", "Q2", "Q3", "Q4" };
static const char *kTextQuarterAbbrev[]									= { "Q1", "Q2", "Q3", "Q4" };
static const char *kTextQuarterFull[]									= { "1st Quarter", "2nd Quarter", "3rd Quarter", "4th Quarter" };

static const char *kTextMonthNarrow[]									= { "J", "F", "M", "A", "M", "J", "Y", "U", "S", "O", "N", "D" };
static const char *kTextMonthAbbrev[]									= { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static const char *kTextMonthFull[]										= { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

static const char *kTextDayNarrow[]										= { "M", "T", "W", "U", "F", "S", "N" };
static const char *kTextDayAbbrev[]										= { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
static const char *kTextDayFull[]										= { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };


// Literals
//
// Must be #defines to ensure formatting can be perfored during static initialization.
#define kFormatTypes													"GyYuQqMLwWdDFgEecahHKkmsSAzZv'"
#define kFormatUnimplemented											"???"

#define kSingleQuote													'\''





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
NString NDateFormatter::Format(const NGregorianDate &theDate, const NString &theFormat) const
{	const char			*textUTF8, *tokenStart, *tokenEnd;
	NString				theResult, theToken;
	bool				areDone;



	// Get the state we need
	textUTF8 = theFormat.GetUTF8();
	areDone  = false;



	// Format the date
	while (!areDone)
		{
		// Find the next token
		tokenStart = strpbrk(textUTF8, kFormatTypes);
		if (tokenStart == NULL)
			break;

		tokenEnd = GetTokenEnd(tokenStart);
		theToken = NString(tokenStart, tokenEnd - tokenStart + 1);



		// Append the text to this point
		theResult += NString(textUTF8, tokenStart - textUTF8);
		textUTF8   = tokenEnd + 1;



		// Append the token
		switch (*tokenStart) {
			case kSingleQuote:
				theResult += GetLiteral(	theDate, *tokenStart, theToken);
				break;

			case 'G':
				theResult += GetEra(		theDate, *tokenStart, theToken);
				break;
			
			case 'y':
			case 'Y':
			case 'u':
				theResult += GetYear(		theDate, *tokenStart, theToken);
				break;
			
			case 'Q':
			case 'q':
				theResult += GetQuarter(	theDate, *tokenStart, theToken);
				break;
			
			case 'M':
			case 'L':
				theResult += GetMonth(		theDate, *tokenStart, theToken);
				break;
			
			case 'w':
			case 'W':
				theResult += GetWeek(		theDate, *tokenStart, theToken);
				break;

			case 'd':
			case 'D':
			case 'F':
			case 'g':
				theResult += GetDay(		theDate, *tokenStart, theToken);
				break;
			
			case 'E':
			case 'e':
			case 'c':
				theResult += GetWeekDay(	theDate, *tokenStart, theToken);
				break;
			
			case 'a':
				theResult += GetPeriod(		theDate, *tokenStart, theToken);
				break;
			
			case 'h':
			case 'H':
			case 'K':
			case 'k':
				theResult += GetHour(		theDate, *tokenStart, theToken);
				break;

			case 'm':
				theResult += GetMinute(		theDate, *tokenStart, theToken);
				break;

			case 's':
			case 'S':
			case 'A':
				theResult += GetSecond(		theDate, *tokenStart, theToken);
				break;
			
			case 'z':
			case 'Z':
			case 'v':
				theResult += GetZone(		theDate, *tokenStart, theToken);
				break;

			default:
				NN_LOG("Unknown token: %c", *tokenStart);
				areDone = true;
				break;
			}
		}



	// Append any remaining text
	if (*textUTF8 != 0x00)
		theResult += NString(textUTF8);

	return(theResult);
}





//============================================================================
//		NDateFormatter::GetTokenEnd : Get the end of a token.
//----------------------------------------------------------------------------
#pragma mark -
const char *NDateFormatter::GetTokenEnd(const char *tokenStart) const
{	bool			isQuote, areDone;
	const char		*tokenEnd;



	// Validate our parameters
	NN_ASSERT(*tokenStart != 0x00);



	// Get the state we need
	tokenEnd = tokenStart;
	isQuote  = (*tokenStart == kSingleQuote);
	areDone  = (*tokenStart == 0x00);



	// Get the end token
	//
	// Date tokens are a continuous run of the same character, and quoted text
	// is zero or more arbitrary characters bracketed by a single quote.
	//
	// We return a pointer to the last character of the token, which may be the
	// last character in the string.
	while (!areDone)
		{
		tokenEnd++;

		if (isQuote)
			{
			areDone = (*tokenEnd == kSingleQuote || *tokenEnd == 0x00);
			if (*tokenEnd == 0x00)
				tokenEnd--;
			}

		else
			{
			areDone = (*tokenEnd != *tokenStart);
			if (areDone)
				tokenEnd--;
			}
		}

	return(tokenEnd);
}





//============================================================================
//		NDateFormatter::GetDayOfWeek : Get the day of the week.
//----------------------------------------------------------------------------
NIndex NDateFormatter::GetDayOfWeek(const NGregorianDate &theDate) const
{


	// Get the day of the week
	return(NTargetTime::GetDayOfWeek(theDate));
}





//============================================================================
//		NDateFormatter::GetDayOfYear : Get the day of the year.
//----------------------------------------------------------------------------
NIndex NDateFormatter::GetDayOfYear(const NGregorianDate &theDate) const
{	NGregorianDate		yearStart;
	NTime				yearDelta;
	NIndex				theIndex;



	// Get the state we need
	yearStart = GetYearStart(theDate);
	yearDelta = NDate(theDate) - NDate(yearStart);
	theIndex  = ((SInt32) floor(yearDelta / kNTimeDay)) + 1;

	return(theIndex);
}





//============================================================================
//		NDateFormatter::GetWeekOfYear : Get the week of the year.
//----------------------------------------------------------------------------
NIndex NDateFormatter::GetWeekOfYear(const NGregorianDate &theDate) const
{	NGregorianDate		yearStart;
	NTime				yearDelta;
	NIndex				theIndex;



	// Get the state we need
	yearStart = GetYearStart(theDate);
	yearDelta = NDate(theDate) - NDate(yearStart);
	theIndex  = ((SInt32) floor(yearDelta / kNTimeWeek)) + 1;

	return(theIndex);
}





//============================================================================
//		NDateFormatter::GetYearStart : Get the start of a year.
//----------------------------------------------------------------------------
NGregorianDate NDateFormatter::GetYearStart(const NGregorianDate &theDate) const
{	NGregorianDate		yearStart;



	// Get the year start
	yearStart.year     = theDate.year;
	yearStart.month    = 1;
	yearStart.day      = 1;
	yearStart.hour     = 0;
	yearStart.month    = 0;
	yearStart.minute   = 0;
	yearStart.second   = 0;
	yearStart.timeZone = theDate.timeZone;
	
	return(yearStart);
}





//============================================================================
//		NDateFormatter::GetTextQuarter : Get the text for a quarter.
//----------------------------------------------------------------------------
NString NDateFormatter::GetTextQuarter(NIndex theIndex, DateWidth theWidth) const
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextQuarterNarrow));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextQuarterAbbrev));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextQuarterFull));



	// Get the text
	switch (theWidth) {
		case kDateWidthNarrow:		theText = kTextQuarterNarrow[theIndex];		break;
		case kDateWidthAbbrev:		theText = kTextQuarterAbbrev[theIndex];		break;
		case kDateWidthFull:		theText = kTextQuarterFull  [theIndex];		break;
		default:
			NN_LOG("Unknown width: %d", theWidth);
			break;
		}
	
	return(theText);
}





//============================================================================
//		NDateFormatter::GetTextMonth : Get the text for a month.
//----------------------------------------------------------------------------
NString NDateFormatter::GetTextMonth(NIndex theIndex, DateWidth theWidth) const
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextMonthNarrow));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextMonthAbbrev));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextMonthFull));



	// Get the text
	switch (theWidth) {
		case kDateWidthNarrow:		theText = kTextMonthNarrow[theIndex];		break;
		case kDateWidthAbbrev:		theText = kTextMonthAbbrev[theIndex];		break;
		case kDateWidthFull:		theText = kTextMonthFull  [theIndex];		break;
		default:
			NN_LOG("Unknown width: %d", theWidth);
			break;
		}
	
	return(theText);
}





//============================================================================
//		NDateFormatter::GetTextDay : Get the text for a day.
//----------------------------------------------------------------------------
NString NDateFormatter::GetTextDay(NIndex theIndex, DateWidth theWidth) const
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextDayNarrow));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextDayAbbrev));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextDayFull));



	// Get the text
	switch (theWidth) {
		case kDateWidthNarrow:		theText = kTextDayNarrow[theIndex];		break;
		case kDateWidthAbbrev:		theText = kTextDayAbbrev[theIndex];		break;
		case kDateWidthFull:		theText = kTextDayFull  [theIndex];		break;
		default:
			NN_LOG("Unknown width: %d", theWidth);
			break;
		}
	
	return(theText);
}





//============================================================================
//		NDateFormatter::GetLiteral : Get literal text.
//----------------------------------------------------------------------------
NString NDateFormatter::GetLiteral(const NGregorianDate &/*theDate*/, char /*tokenChar*/, const NString &theToken) const
{	NString		theValue;



	// Validate our parameters
	NN_ASSERT(theToken.GetSize() >= 2);



	// Get the text
	theValue = theToken;
	theValue.Trim(kSingleQuote, kNStringNone);

	if (theValue.IsEmpty())
		theValue = NString(kSingleQuote);
	
	return(theValue);
}





//============================================================================
//		NDateFormatter::GetEra : Get an era.
//----------------------------------------------------------------------------
NString NDateFormatter::GetEra(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NString		theValue;
	NIndex		theSize;



	// Validate our parameters
	NN_ASSERT(tokenChar == 'G');
	NN_ASSERT(!theToken.IsEmpty());
	
	NN_UNUSED(tokenChar);



	// Get the state we need
	theSize = theToken.GetSize();
	NN_ASSERT(theSize >= 1 && theSize <= 5);



	// Get the text
	//
	// It's unclear if the standard allows these values to be localised; for now we assume not.
	switch (theSize) {
		case 1:
		case 2:
		case 3:
			theValue = (theDate.year >= 0) ? "AD" : "BC";
			break;

		case 4:
			theValue = (theDate.year >= 0) ? "Anno Domini" : "Before Christ";
			break;
		
		case 5:
			theValue = (theDate.year >= 0) ? "A" : "B";
			break;

		default:
			NN_LOG("Unknown token: %@", theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetYear : Get a year.
//----------------------------------------------------------------------------
NString NDateFormatter::GetYear(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NString		theValue;
	NIndex		theSize;



	// Validate our parameters
	NN_ASSERT(!theToken.IsEmpty());



	// Get the state we need
	theSize = theToken.GetSize();



	// Get the text
	switch (tokenChar) {
		case 'y':
			theValue.Format("%0*ld", theSize, theDate.year);
			break;
		
		case 'Y':
			// Assume same as calendar year
			theValue.Format("%0*ld", theSize, theDate.year);
			break;
		
		case 'u':
			// Assume same as calendar year
			theValue.Format("%0*ld", theSize, theDate.year);
			break;
		
		default:
			NN_LOG("Unknown token: %@", theToken);
			break;
		}



	// Trim the year
	//
	// Normally the length specifies the padding, but a value of 2 also truncates.
	if (theSize == 2 && theValue.GetSize() > 2)
		theValue = theValue.GetRight(2);

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetQuarter : Get a quarter.
//----------------------------------------------------------------------------
NString NDateFormatter::GetQuarter(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NIndex		theSize, theIndex;
	NString		theValue;
	


	// Validate our parameters
	NN_ASSERT(tokenChar == 'Q' || tokenChar == 'q');
	NN_ASSERT(!theToken.IsEmpty());



	// Get the state we need
	theSize  = theToken.GetSize();
	theIndex = theDate.month / 4;



	// Get the text
	switch (theSize) {
		case 1:
		case 2:
			theValue.Format("%0*d", theSize, theIndex + 1);
			break;
		
		case 3:
			theValue = GetTextQuarter(theIndex, kDateWidthAbbrev);
			break;

		case 4:
			theValue = GetTextQuarter(theIndex, kDateWidthFull);
			break;

		case 5:
			theValue = GetTextQuarter(theIndex, kDateWidthAbbrev);
			break;
		
		default:
			NN_LOG("Unknown token: %@", theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetMonth : Get a month.
//----------------------------------------------------------------------------
NString NDateFormatter::GetMonth(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NIndex		theSize, theIndex;
	NString		theValue;
	


	// Validate our parameters
	NN_ASSERT(tokenChar == 'M' || tokenChar == 'L');
	NN_ASSERT(!theToken.IsEmpty());



	// Get the state we need
	theSize  = theToken.GetSize();
	theIndex = theDate.month - 1;



	// Get the text
	switch (theSize) {
		case 1:
		case 2:
			theValue.Format("%0*d", theSize, theIndex + 1);
			break;
		
		case 3:
			theValue = GetTextMonth(theIndex, kDateWidthAbbrev);
			break;

		case 4:
			theValue = GetTextMonth(theIndex, kDateWidthFull);
			break;

		case 5:
			theValue = GetTextMonth(theIndex, kDateWidthNarrow);
			break;

		default:
			NN_LOG("Unknown token: %@", theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetWeek : Get a week.
//----------------------------------------------------------------------------
NString NDateFormatter::GetWeek(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NString		theValue;
	NIndex		theSize;



	// Validate our parameters
	NN_ASSERT(!theToken.IsEmpty());



	// Get the state we need
	theSize = theToken.GetSize();
	


	// Get the text
	switch (tokenChar) {
		case 'w':
			NN_ASSERT(theSize >= 1 && theSize <= 2);
			theValue.Format("%0*d", theSize, GetWeekOfYear(theDate));
			break;

		case 'W':
			NN_ASSERT(theSize == 1);
			theValue.Format("%d", (theDate.day / 7) + 1);
			break;

		default:
			NN_LOG("Unknown token: %@", theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetDay : Get a day.
//----------------------------------------------------------------------------
NString NDateFormatter::GetDay(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NString		theValue;
	NIndex		theSize;



	// Validate our parameters
	NN_ASSERT(!theToken.IsEmpty());



	// Get the state we need
	theSize = theToken.GetSize();
	


	// Get the text
	switch (tokenChar) {
		case 'd':
			NN_ASSERT(theSize >= 1 && theSize <= 2);
			theValue.Format("%0*d", theSize, theDate.day);
			break;

		case 'D':
			NN_ASSERT(theSize >= 1 && theSize <= 3);
			theValue.Format("%0*d", theSize, GetDayOfYear(theDate));
			break;

		case 'F':
		case 'g':
			NN_LOG("Not implemented: %@", theToken);
			theValue = kFormatUnimplemented;
			break;

		default:
			NN_LOG("Unknown token: %@", theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetWeekDay : Get a week day.
//----------------------------------------------------------------------------
NString NDateFormatter::GetWeekDay(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NIndex		theSize, theIndex;
	NString		theValue;
	


	// Validate our parameters
	NN_ASSERT(tokenChar == 'E' || tokenChar == 'e' || tokenChar == 'c');
	NN_ASSERT(!theToken.IsEmpty());



	// Get the state we need
	theSize  = theToken.GetSize();
	theIndex = GetDayOfWeek(theDate) - 1;



	// Get the text
	switch (theSize) {
		case 1:
		case 2:
			if (tokenChar == 'E')
				theValue = GetTextDay(theIndex, kDateWidthAbbrev);
			else
				theValue.Format("%0*d", theSize, theIndex + 1);
			break;
		
		case 3:
			theValue = GetTextDay(theIndex, kDateWidthAbbrev);
			break;

		case 4:
			theValue = GetTextDay(theIndex, kDateWidthFull);
			break;

		case 5:
			theValue = GetTextDay(theIndex, kDateWidthNarrow);
			break;

		default:
			NN_LOG("Unknown token: %@", theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetPeriod : Get a period.
//----------------------------------------------------------------------------
NString NDateFormatter::GetPeriod(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NString		theValue;



	// Validate our parameters
	NN_ASSERT(tokenChar          == 'a');
	NN_ASSERT(theToken.GetSize() == 1);
	
	NN_UNUSED(tokenChar);



	// Get the text
	//
	// It's unclear if the standard allows these values to be localised; for now we assume not.
	if (theDate.hour >= 12)
		theValue = "PM";
	else
		theValue = "AM";
	
	return(theValue);
}





//============================================================================
//		NDateFormatter::GetHour : Get an hour.
//----------------------------------------------------------------------------
NString NDateFormatter::GetHour(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NIndex		theSize, theIndex;
	NString		theValue;



	// Validate our parameters
	NN_ASSERT(!theToken.IsEmpty());



	// Get the state we need
	theSize = theToken.GetSize();
	


	// Get the text
	switch (tokenChar) {
		case 'h':
			theIndex = (theDate.hour % 12) == 0 ? 12 : (theDate.hour % 12);
			NN_ASSERT(theSize >= 1 && theSize <= 2);
			theValue.Format("%0*d", theSize, theIndex);
			break;

		case 'H':
			NN_ASSERT(theSize >= 1 && theSize <= 2);
			theValue.Format("%0*d", theSize, theDate.hour);
			break;
		
		case 'K':
			NN_ASSERT(theSize >= 1 && theSize <= 2);
			theValue.Format("%0*d", theSize, theDate.hour % 12);
			break;

		case 'k':
			theIndex = (theDate.hour == 0) ? 24 : theDate.hour;
			NN_ASSERT(theSize >= 1 && theSize <= 2);
			theValue.Format("%0*d", theSize, theIndex);
			break;

		default:
			NN_LOG("Unknown token: %@", theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetMinute : Get a minute.
//----------------------------------------------------------------------------
NString NDateFormatter::GetMinute(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NString		theValue;
	NIndex		theSize;



	// Validate our parameters
	NN_ASSERT(tokenChar == 'm');
	NN_ASSERT(!theToken.IsEmpty());
	
	NN_UNUSED(tokenChar);



	// Get the state we need
	theSize = theToken.GetSize();



	// Get the text
	NN_ASSERT(theSize >= 1 && theSize <= 2);
	theValue.Format("%0*d", theSize, theDate.minute);

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetSecond : Get a second.
//----------------------------------------------------------------------------
NString NDateFormatter::GetSecond(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NString		theValue;
	NIndex		theSize;



	// Validate our parameters
	NN_ASSERT(!theToken.IsEmpty());



	// Get the state we need
	theSize = theToken.GetSize();
	


	// Get the text
	switch (tokenChar) {
		case 's':
			NN_ASSERT(theSize >= 1 && theSize <= 2);
			theValue.Format("%0*d", theSize, (SInt32) theDate.second);
			break;

		case 'S':
			theValue.Format("%.*f", theSize, theDate.second);
			break;

		case 'A':
			NN_LOG("Not implemented: %@", theToken);
			theValue = kFormatUnimplemented;
			break;

		default:
			NN_LOG("Unknown token: %@", theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetZone : Get a time zone.
//----------------------------------------------------------------------------
NString NDateFormatter::GetZone(const NGregorianDate &theDate, char tokenChar, const NString &theToken) const
{	NString		theValue;
	NIndex		theSize;



	// Validate our parameters
	NN_ASSERT(!theToken.IsEmpty());



	// Get the state we need
	theSize = theToken.GetSize();



	// Get the text
	switch (tokenChar) {
		case 'z':
			theValue = theDate.timeZone;
			break;

		case 'Z':
			NN_LOG("Not implemented: %@", theToken);
			theValue = kFormatUnimplemented;
			break;

		case 'v':
			NN_LOG("Not implemented: %@", theToken);
			theValue = kFormatUnimplemented;
			break;

		default:
			NN_LOG("Unknown token: %@", theToken);
			break;
		}

	return(theValue);
}


