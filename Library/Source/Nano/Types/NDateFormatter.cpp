/*	NAME:
		NDateFormatter.cpp

	DESCRIPTION:
		Date formatter.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
	NString				theResult;
	NDateContext		theContext;
	bool				areDone;



	// Get the state we need
	theContext.theDate  = NDate(theDate);
	theContext.gregDate =       theDate;
	
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
		
		theContext.theToken  = NString(tokenStart, (NIndex) (tokenEnd - tokenStart + 1));
		theContext.tokenChar = *tokenStart;
		theContext.tokenSize = theContext.theToken.GetSize();



		// Append the text to this point
		theResult += NString(textUTF8, (NIndex) (tokenStart - textUTF8));
		textUTF8   = tokenEnd + 1;



		// Append the token
		switch (theContext.tokenChar) {
			case kSingleQuote:
				theResult += GetLiteral(theContext);
				break;

			case 'G':
				theResult += GetEra(theContext);
				break;
			
			case 'y':
			case 'Y':
			case 'u':
				theResult += GetYear(theContext);
				break;
			
			case 'Q':
			case 'q':
				theResult += GetQuarter(theContext);
				break;
			
			case 'M':
			case 'L':
				theResult += GetMonth(theContext);
				break;
			
			case 'w':
			case 'W':
				theResult += GetWeek(theContext);
				break;

			case 'd':
			case 'D':
			case 'F':
			case 'g':
				theResult += GetDay(theContext);
				break;
			
			case 'E':
			case 'e':
			case 'c':
				theResult += GetWeekDay(theContext);
				break;
			
			case 'a':
				theResult += GetPeriod(theContext);
				break;
			
			case 'h':
			case 'H':
			case 'K':
			case 'k':
				theResult += GetHour(theContext);
				break;

			case 'm':
				theResult += GetMinute(theContext);
				break;

			case 's':
			case 'S':
			case 'A':
				theResult += GetSecond(theContext);
				break;
			
			case 'z':
			case 'Z':
			case 'v':
				theResult += GetZone(theContext);
				break;

			default:
				NN_LOG("Unknown token: %c", theContext.tokenChar);
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
//		NDateFormatter::GetTextQuarter : Get the text for a quarter.
//----------------------------------------------------------------------------
NString NDateFormatter::GetTextQuarter(NIndex theIndex, NDateWidth theWidth) const
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextQuarterNarrow));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextQuarterAbbrev));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextQuarterFull));



	// Get the text
	switch (theWidth) {
		case kNDateWidthNarrow:		theText = kTextQuarterNarrow[theIndex];		break;
		case kNDateWidthAbbrev:		theText = kTextQuarterAbbrev[theIndex];		break;
		case kNDateWidthFull:		theText = kTextQuarterFull  [theIndex];		break;
		default:
			NN_LOG("Unknown width: %d", theWidth);
			break;
		}
	
	return(theText);
}





//============================================================================
//		NDateFormatter::GetTextMonth : Get the text for a month.
//----------------------------------------------------------------------------
NString NDateFormatter::GetTextMonth(NIndex theIndex, NDateWidth theWidth) const
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextMonthNarrow));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextMonthAbbrev));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextMonthFull));



	// Get the text
	switch (theWidth) {
		case kNDateWidthNarrow:		theText = kTextMonthNarrow[theIndex];		break;
		case kNDateWidthAbbrev:		theText = kTextMonthAbbrev[theIndex];		break;
		case kNDateWidthFull:		theText = kTextMonthFull  [theIndex];		break;
		default:
			NN_LOG("Unknown width: %d", theWidth);
			break;
		}
	
	return(theText);
}





//============================================================================
//		NDateFormatter::GetTextDay : Get the text for a day.
//----------------------------------------------------------------------------
NString NDateFormatter::GetTextDay(NIndex theIndex, NDateWidth theWidth) const
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextDayNarrow));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextDayAbbrev));
	NN_ASSERT(theIndex < NN_ARRAY_SIZE(kTextDayFull));



	// Get the text
	switch (theWidth) {
		case kNDateWidthNarrow:		theText = kTextDayNarrow[theIndex];		break;
		case kNDateWidthAbbrev:		theText = kTextDayAbbrev[theIndex];		break;
		case kNDateWidthFull:		theText = kTextDayFull  [theIndex];		break;
		default:
			NN_LOG("Unknown width: %d", theWidth);
			break;
		}
	
	return(theText);
}





//============================================================================
//		NDateFormatter::GetLiteral : Get literal text.
//----------------------------------------------------------------------------
NString NDateFormatter::GetLiteral(const NDateContext &theContext) const
{	NString		theValue;



	// Validate our parameters
	NN_ASSERT(theContext.tokenSize >= 2);



	// Get the text
	theValue = theContext.theToken;
	theValue.Trim(kSingleQuote, kNStringNone);

	if (theValue.IsEmpty())
		theValue = NString(kSingleQuote);
	
	return(theValue);
}





//============================================================================
//		NDateFormatter::GetEra : Get an era.
//----------------------------------------------------------------------------
NString NDateFormatter::GetEra(const NDateContext &theContext) const
{	NString		theValue;



	// Validate our parameters
	NN_ASSERT(theContext.tokenChar == 'G');



	// Get the text
	//
	// It's unclear if the standard allows these values to be localised; for now we assume not.
	switch (theContext.tokenSize) {
		case 1:
		case 2:
		case 3:
			theValue = (theContext.gregDate.year >= 0) ? "AD" : "BC";
			break;

		case 4:
			theValue = (theContext.gregDate.year >= 0) ? "Anno Domini" : "Before Christ";
			break;
		
		case 5:
			theValue = (theContext.gregDate.year >= 0) ? "A" : "B";
			break;

		default:
			NN_LOG("Unknown token: %@", theContext.theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetYear : Get a year.
//----------------------------------------------------------------------------
NString NDateFormatter::GetYear(const NDateContext &theContext) const
{	NString		theValue;



	// Get the text
	switch (theContext.tokenChar) {
		case 'y':
			theValue.Format("%0*ld", theContext.tokenSize, theContext.gregDate.year);
			break;
		
		case 'Y':
			// Assume same as calendar year
			theValue.Format("%0*ld", theContext.tokenSize, theContext.gregDate.year);
			break;
		
		case 'u':
			// Assume same as calendar year
			theValue.Format("%0*ld", theContext.tokenSize, theContext.gregDate.year);
			break;
		
		default:
			NN_LOG("Unknown token: %@", theContext.theToken);
			break;
		}



	// Trim the year
	//
	// Normally the length specifies the padding, but a value of 2 also truncates.
	if (theContext.tokenSize == 2 && theValue.GetSize() > 2)
		theValue = theValue.GetRight(2);

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetQuarter : Get a quarter.
//----------------------------------------------------------------------------
NString NDateFormatter::GetQuarter(const NDateContext &theContext) const
{	NString		theValue;
	NIndex		theIndex;



	// Validate our parameters
	NN_ASSERT(theContext.tokenChar == 'Q' || theContext.tokenChar == 'q');



	// Get the state we need
	theIndex = theContext.gregDate.month / 4;



	// Get the text
	switch (theContext.tokenSize) {
		case 1:
		case 2:
			theValue.Format("%0*d", theContext.tokenSize, theIndex + 1);
			break;
		
		case 3:
			theValue = GetTextQuarter(theIndex, kNDateWidthAbbrev);
			break;

		case 4:
			theValue = GetTextQuarter(theIndex, kNDateWidthFull);
			break;

		case 5:
			theValue = GetTextQuarter(theIndex, kNDateWidthAbbrev);
			break;
		
		default:
			NN_LOG("Unknown token: %@", theContext.theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetMonth : Get a month.
//----------------------------------------------------------------------------
NString NDateFormatter::GetMonth(const NDateContext &theContext) const
{	NString		theValue;
	NIndex		theIndex;



	// Validate our parameters
	NN_ASSERT(theContext.tokenChar == 'M' || theContext.tokenChar == 'L');



	// Get the state we need
	theIndex = theContext.gregDate.month - 1;



	// Get the text
	switch (theContext.tokenSize) {
		case 1:
		case 2:
			theValue.Format("%0*d", theContext.tokenSize, theIndex + 1);
			break;
		
		case 3:
			theValue = GetTextMonth(theIndex, kNDateWidthAbbrev);
			break;

		case 4:
			theValue = GetTextMonth(theIndex, kNDateWidthFull);
			break;

		case 5:
			theValue = GetTextMonth(theIndex, kNDateWidthNarrow);
			break;

		default:
			NN_LOG("Unknown token: %@", theContext.theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetWeek : Get a week.
//----------------------------------------------------------------------------
NString NDateFormatter::GetWeek(const NDateContext &theContext) const
{	NString		theValue;



	// Get the text
	switch (theContext.tokenChar) {
		case 'w':
			NN_ASSERT(theContext.tokenSize >= 1 && theContext.tokenSize <= 2);
			theValue.Format("%0*d", theContext.tokenSize, theContext.theDate.GetWeekOfYear());
			break;

		case 'W':
			NN_ASSERT(theContext.tokenSize == 1);
			theValue.Format("%d", (theContext.gregDate.day / 7) + 1);
			break;

		default:
			NN_LOG("Unknown token: %@", theContext.theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetDay : Get a day.
//----------------------------------------------------------------------------
NString NDateFormatter::GetDay(const NDateContext &theContext) const
{	NString		theValue;



	// Get the text
	switch (theContext.tokenChar) {
		case 'd':
			NN_ASSERT(theContext.tokenSize >= 1 && theContext.tokenSize <= 2);
			theValue.Format("%0*d", theContext.tokenSize, theContext.gregDate.day);
			break;

		case 'D':
			NN_ASSERT(theContext.tokenSize >= 1 && theContext.tokenSize <= 3);
			theValue.Format("%0*d", theContext.tokenSize, theContext.theDate.GetDayOfYear());
			break;

		case 'F':
		case 'g':
			NN_LOG("Not implemented: %@", theContext.theToken);
			theValue = kFormatUnimplemented;
			break;

		default:
			NN_LOG("Unknown token: %@", theContext.theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetWeekDay : Get a week day.
//----------------------------------------------------------------------------
NString NDateFormatter::GetWeekDay(const NDateContext &theContext) const
{	NString		theValue;
	NIndex		theIndex;



	// Validate our parameters
	NN_ASSERT(theContext.tokenChar == 'E' || theContext.tokenChar == 'e' || theContext.tokenChar == 'c');



	// Get the state we need
	theIndex = theContext.theDate.GetDayOfWeek() - 1;



	// Get the text
	switch (theContext.tokenSize) {
		case 1:
		case 2:
			if (theContext.tokenChar == 'E')
				theValue = GetTextDay(theIndex, kNDateWidthAbbrev);
			else
				theValue.Format("%0*d", theContext.tokenSize, theIndex + 1);
			break;
		
		case 3:
			theValue = GetTextDay(theIndex, kNDateWidthAbbrev);
			break;

		case 4:
			theValue = GetTextDay(theIndex, kNDateWidthFull);
			break;

		case 5:
			theValue = GetTextDay(theIndex, kNDateWidthNarrow);
			break;

		default:
			NN_LOG("Unknown token: %@", theContext.theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetPeriod : Get a period.
//----------------------------------------------------------------------------
NString NDateFormatter::GetPeriod(const NDateContext &theContext) const
{	NString		theValue;



	// Validate our parameters
	NN_ASSERT(theContext.tokenChar == 'a');
	NN_ASSERT(theContext.tokenSize == 1);



	// Get the text
	//
	// It's unclear if the standard allows these values to be localised; for now we assume not.
	if (theContext.gregDate.hour >= 12)
		theValue = "PM";
	else
		theValue = "AM";
	
	return(theValue);
}





//============================================================================
//		NDateFormatter::GetHour : Get an hour.
//----------------------------------------------------------------------------
NString NDateFormatter::GetHour(const NDateContext &theContext) const
{	NString		theValue;
	NIndex		theIndex;



	// Get the text
	switch (theContext.tokenChar) {
		case 'h':
			theIndex = (theContext.gregDate.hour % 12) == 0 ? 12 : (theContext.gregDate.hour % 12);
			NN_ASSERT(theContext.tokenSize >= 1 && theContext.tokenSize <= 2);
			theValue.Format("%0*d", theContext.tokenSize, theIndex);
			break;

		case 'H':
			NN_ASSERT(theContext.tokenSize >= 1 && theContext.tokenSize <= 2);
			theValue.Format("%0*d", theContext.tokenSize, theContext.gregDate.hour);
			break;
		
		case 'K':
			NN_ASSERT(theContext.tokenSize >= 1 && theContext.tokenSize <= 2);
			theValue.Format("%0*d", theContext.tokenSize, theContext.gregDate.hour % 12);
			break;

		case 'k':
			theIndex = (theContext.gregDate.hour == 0) ? 24 : theContext.gregDate.hour;
			NN_ASSERT(theContext.tokenSize >= 1 && theContext.tokenSize <= 2);
			theValue.Format("%0*d", theContext.tokenSize, theIndex);
			break;

		default:
			NN_LOG("Unknown token: %@", theContext.theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetMinute : Get a minute.
//----------------------------------------------------------------------------
NString NDateFormatter::GetMinute(const NDateContext &theContext) const
{	NString		theValue;



	// Validate our parameters
	NN_ASSERT(theContext.tokenChar == 'm');
	NN_ASSERT(theContext.tokenSize >= 1 && theContext.tokenSize <= 2);



	// Get the text
	theValue.Format("%0*d", theContext.tokenSize, theContext.gregDate.minute);

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetSecond : Get a second.
//----------------------------------------------------------------------------
NString NDateFormatter::GetSecond(const NDateContext &theContext) const
{	NTime		secondInt, secondFrac;
	NString		theValue;



	// Get the state we need
	secondInt  = floor(theContext.gregDate.second);
	secondFrac = theContext.gregDate.second - secondInt;



	// Get the text
	switch (theContext.tokenChar) {
		case 's':
			NN_ASSERT(theContext.tokenSize >= 1 && theContext.tokenSize <= 2);
			theValue.Format("%0*d", theContext.tokenSize, (SInt32) secondInt);
			break;

		case 'S':
			theValue.Format("%.*f", theContext.tokenSize, secondFrac);
			NN_ASSERT(theValue.GetLeft(2) == "0.");
			theValue.TrimLeft(2);
			break;

		case 'A':
			NN_LOG("Not implemented: %@", theContext.theToken);
			theValue = kFormatUnimplemented;
			break;

		default:
			NN_LOG("Unknown token: %@", theContext.theToken);
			break;
		}

	return(theValue);
}





//============================================================================
//		NDateFormatter::GetZone : Get a time zone.
//----------------------------------------------------------------------------
NString NDateFormatter::GetZone(const NDateContext &theContext) const
{	NString		theValue;



	// Get the text
	switch (theContext.tokenChar) {
		case 'z':
			theValue = theContext.gregDate.timeZone;
			break;

		case 'Z':
			NN_LOG("Not implemented: %@", theContext.theToken);
			theValue = kFormatUnimplemented;
			break;

		case 'v':
			NN_LOG("Not implemented: %@", theContext.theToken);
			theValue = kFormatUnimplemented;
			break;

		default:
			NN_LOG("Unknown token: %@", theContext.theToken);
			break;
		}

	return(theValue);
}


