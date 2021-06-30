/*	NAME:
		NDateFormatter.cpp

	DESCRIPTION:
		Date formatter.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDateFormatter.h"

// Nano
#include "NFormat.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Date widths
enum class NDateWidth
{
	Narrow,
	Abbrev,
	Wide
};


// Date labels
//
// TODO - requires localisation.
static const char* kNLabelQuarterNarrow[]                   = {"Q1", "Q2", "Q3", "Q4"};

static const char* kNLabelQuarterAbbrev[]                   = {"Q1", "Q2", "Q3", "Q4"};

static const char* kNLabelQuarterWide[]                     = {"1st Quarter",
										   "2nd Quarter",
										   "3rd Quarter",
										   "4th Quarter"};

static const char* kNLabelMonthNarrow[] =
	{"J", "F", "M", "A", "M", "J", "Y", "U", "S", "O", "N", "D"};

static const char* kNLabelMonthAbbrev[] =
	{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

static const char* kNLabelMonthWide[]                       = {"January",
										 "February",
										 "March",
										 "April",
										 "May",
										 "June",
										 "July",
										 "August",
										 "September",
										 "October",
										 "November",
										 "December"};

static const char* kNLabelDayNarrow[]                       = {"M", "T", "W", "U", "F", "S", "N"};

static const char* kNLabelDayAbbrev[]                       = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

static const char* kNLabelDayWide[] =
	{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};


// Literals
inline constexpr const utf8_t* kNDateFormatTokens           = "GyYuQqMLwWdDFgEecahHKkmsSAzZv'";
inline constexpr const utf8_t* kNDateFormatUnimplemented    = "???";
inline constexpr utf8_t        kNUTF8Quote                  = '\'';
inline constexpr utf8_t        kNUTF8Null                   = 0;





//=============================================================================
//		NDateFormatter::NDateFormatter : Constructor.
//-----------------------------------------------------------------------------
NDateFormatter::NDateFormatter()
	: mDate()
	, mSeconds(0.0)
	, mToken()
	, mTokenSize(0)
	, mTokenChar(0)
{
}





//=============================================================================
//		NDateFormatter::SetDate : Set the date.
//-----------------------------------------------------------------------------
void NDateFormatter::SetDate(const NDate& theDate, NInterval theSeconds)
{


	// Set the date
	mDate    = theDate;
	mSeconds = theSeconds;
}





//=============================================================================
//		NDateFormatter::SetDate : Set the date.
//-----------------------------------------------------------------------------
void NDateFormatter::SetDate(NTime theTime)
{


	// Get the state we need
	NInterval absTime = std::abs(theTime);
	size_t    numDays = size_t(absTime / kNTimeDay);



	// Set the date
	//
	// The Nano epoch starts at 00:00:00 on 2001/01/01.
	mDate    = NDate(2001, 1, 1);
	mSeconds = absTime - (NInterval(numDays) * kNTimeDay);

	if (theTime >= 0.0)
	{
		mDate.AddDays(numDays);
	}
	else
	{
		mDate.SubtractDays(numDays);
	}
}





//=============================================================================
//		NDateFormatter::Format : Format a date.
//-----------------------------------------------------------------------------
NString NDateFormatter::Format(const NString& theFormat) const
{


	// Get the state we need
	const utf8_t* textUTF8 = theFormat.GetUTF8();
	bool          areDone  = false;

	NString theResult;



	// Format the date
	while (!areDone)
	{
		// Find the next token
		const char* tokenStart = strpbrk(textUTF8, kNDateFormatTokens);
		if (tokenStart == nullptr)
		{
			break;
		}

		const utf8_t* tokenEnd = GetTokenEnd(tokenStart);


		// Accumulate the text
		theResult += NString(NStringEncoding::UTF8, size_t(tokenStart - textUTF8), textUTF8);
		textUTF8 = tokenEnd + 1;



		// Process the token
		mToken     = NString(NStringEncoding::UTF8, size_t(tokenEnd - tokenStart + 1), tokenStart);
		mTokenSize = mToken.GetSize();
		mTokenChar = tokenStart[0];

		switch (mTokenChar)
		{
			case kNUTF8Quote:
				theResult += GetLiteral();
				break;

			case 'G':
				theResult += GetEra();
				break;

			case 'y':
			case 'Y':
			case 'u':
				theResult += GetYear();
				break;

			case 'Q':
			case 'q':
				theResult += GetQuarter();
				break;

			case 'M':
			case 'L':
				theResult += GetMonth();
				break;

			case 'w':
			case 'W':
				theResult += GetWeek();
				break;

			case 'd':
			case 'D':
			case 'F':
			case 'g':
				theResult += GetDay();
				break;

			case 'E':
			case 'e':
			case 'c':
				theResult += GetWeekDay();
				break;

			case 'a':
				theResult += GetPeriod();
				break;

			case 'h':
			case 'H':
			case 'K':
			case 'k':
				theResult += GetHour();
				break;

			case 'm':
				theResult += GetMinute();
				break;

			case 's':
			case 'S':
			case 'A':
				theResult += GetSecond();
				break;

			case 'z':
			case 'Z':
			case 'v':
				theResult += GetZone();
				break;

			default:
				NN_LOG_WARNING("Unknown token '{}'", mToken);
				areDone = true;
				break;
		}
	}



	// Accumulate the text
	if (*textUTF8 != kNUTF8Null)
	{
		theResult += NString(textUTF8);
	}

	return theResult;
}





#pragma mark private
//=============================================================================
//		NDateFormatter::GetTokenEnd : Get the end of a token.
//-----------------------------------------------------------------------------
const utf8_t* NDateFormatter::GetTokenEnd(const utf8_t* tokenStart) const
{


	// Validate our parameters
	NN_REQUIRE(*tokenStart != kNUTF8Null);



	// Get the state we need
	const utf8_t* tokenEnd = tokenStart;
	bool          isQuote  = (*tokenStart == kNUTF8Quote);
	bool          areDone  = (*tokenStart == kNUTF8Null);



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
			areDone = (*tokenEnd == kNUTF8Quote || *tokenEnd == kNUTF8Null);
			if (*tokenEnd == kNUTF8Null)
			{
				tokenEnd--;
			}
		}

		else
		{
			areDone = (*tokenEnd != *tokenStart);
			if (areDone)
			{
				tokenEnd--;
			}
		}
	}

	return tokenEnd;
}





//=============================================================================
//		NDateFormatter::GetTextQuarter : Get the text for a quarter.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetTextQuarter(size_t theIndex, NDateWidth theWidth) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < std::size(kNLabelQuarterNarrow));
	NN_REQUIRE(theIndex < std::size(kNLabelQuarterAbbrev));
	NN_REQUIRE(theIndex < std::size(kNLabelQuarterWide));



	// Get the text
	switch (theWidth)
	{
		case NDateWidth::Narrow:
			return kNLabelQuarterNarrow[theIndex];
			break;
		case NDateWidth::Abbrev:
			return kNLabelQuarterAbbrev[theIndex];
			break;
		case NDateWidth::Wide:
			return kNLabelQuarterWide[theIndex];
			break;
	}

	NN_UNREACHABLE();
}





//=============================================================================
//		NDateFormatter::GetTextMonth : Get the text for a month.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetTextMonth(size_t theIndex, NDateWidth theWidth) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < std::size(kNLabelMonthNarrow));
	NN_REQUIRE(theIndex < std::size(kNLabelMonthAbbrev));
	NN_REQUIRE(theIndex < std::size(kNLabelMonthWide));



	// Get the text
	switch (theWidth)
	{
		case NDateWidth::Narrow:
			return kNLabelMonthNarrow[theIndex];
			break;
		case NDateWidth::Abbrev:
			return kNLabelMonthAbbrev[theIndex];
			break;
		case NDateWidth::Wide:
			return kNLabelMonthWide[theIndex];
			break;
	}

	NN_UNREACHABLE();
}





//=============================================================================
//		NDateFormatter::GetTextDay : Get the text for a day.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetTextDay(size_t theIndex, NDateWidth theWidth) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < std::size(kNLabelDayNarrow));
	NN_REQUIRE(theIndex < std::size(kNLabelDayAbbrev));
	NN_REQUIRE(theIndex < std::size(kNLabelDayWide));



	// Get the text
	switch (theWidth)
	{
		case NDateWidth::Narrow:
			return kNLabelDayNarrow[theIndex];
			break;
		case NDateWidth::Abbrev:
			return kNLabelDayAbbrev[theIndex];
			break;
		case NDateWidth::Wide:
			return kNLabelDayWide[theIndex];
			break;
	}

	NN_UNREACHABLE();
}





//=============================================================================
//		NDateFormatter::GetLiteral : Get literal text.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetLiteral() const
{


	// Validate our parameters
	NN_REQUIRE(mTokenSize >= 2);



	// Get the text
	NString theValue(mToken);
	NString quoteToken("'");

	if (theValue.StartsWith(quoteToken))
	{
		theValue.RemovePrefix(quoteToken.GetSize());
	}

	if (theValue.EndsWith(quoteToken))
	{
		theValue.RemoveSuffix(quoteToken.GetSize());
	}

	if (theValue.IsEmpty())
	{
		theValue = quoteToken;
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetEra : Get an era.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetEra() const
{


	// Validate our parameters
	NN_REQUIRE(mTokenChar == 'G');



	// Get the text
	//
	// It's unclear if the standard allows these values to be localised; for now we assume not.
	NYear   theYear = mDate.GetYear();
	NString theValue;

	switch (mTokenSize)
	{
		case 1:
		case 2:
		case 3:
			theValue = (theYear >= 0) ? "AD" : "BC";
			break;

		case 4:
			theValue = (theYear >= 0) ? "Anno Domini" : "Before Christ";
			break;

		case 5:
			theValue = (theYear >= 0) ? "A" : "B";
			break;

		default:
			NN_LOG_WARNING("Unknown token: {}", mToken);
			break;
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetYear : Get a year.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetYear() const
{


	// Get the text
	NYear   theYear = mDate.GetYear();
	NString theValue;

	switch (mTokenChar)
	{
		case 'y':
			theValue = NFormat("{:0{}}", theYear, mTokenSize);
			break;

		case 'Y':
			// Assume same as calendar year
			theValue = NFormat("{:0{}}", theYear, mTokenSize);
			break;

		case 'u':
			// Assume same as calendar year
			theValue = NFormat("{:0{}}", theYear, mTokenSize);
			break;

		default:
			NN_LOG_WARNING("Unknown token: {}", mToken);
			break;
	}



	// Trim the year
	//
	// Normally the length specifies the padding, but a value of 2 also truncates.
	if (mTokenSize == 2 && theValue.GetSize() > 2)
	{
		theValue = theValue.GetSuffix(2);
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetQuarter : Get a quarter.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetQuarter() const
{


	// Validate our parameters
	NN_REQUIRE(mTokenChar == 'Q' || mTokenChar == 'q');



	// Get the text
	size_t  theQuarter = size_t(mDate.GetMonth()) / 4;
	NString theValue;

	switch (mTokenSize)
	{
		case 1:
		case 2:
			theValue = NFormat("{:0{}}", theQuarter + 1, mTokenSize);
			break;

		case 3:
			theValue = GetTextQuarter(theQuarter, NDateWidth::Abbrev);
			break;

		case 4:
			theValue = GetTextQuarter(theQuarter, NDateWidth::Wide);
			break;

		case 5:
			theValue = GetTextQuarter(theQuarter, NDateWidth::Abbrev);
			break;

		default:
			NN_LOG_WARNING("Unknown token: {}", mToken);
			break;
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetMonth : Get a month.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetMonth() const
{


	// Validate our parameters
	NN_REQUIRE(mTokenChar == 'M' || mTokenChar == 'L');



	// Get the text
	size_t  theMonth = size_t(mDate.GetMonth()) - 1;
	NString theValue;

	switch (mTokenSize)
	{
		case 1:
		case 2:
			theValue = NFormat("{:0{}}", theMonth + 1, mTokenSize);
			break;

		case 3:
			theValue = GetTextMonth(theMonth, NDateWidth::Abbrev);
			break;

		case 4:
			theValue = GetTextMonth(theMonth, NDateWidth::Wide);
			break;

		case 5:
			theValue = GetTextMonth(theMonth, NDateWidth::Narrow);
			break;

		default:
			NN_LOG_WARNING("Unknown token: {}", mToken);
			break;
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetWeek : Get a week.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetWeek() const
{


	// Get the text
	NString theValue;

	switch (mTokenChar)
	{
		case 'w':
			NN_REQUIRE(mTokenSize >= 1 && mTokenSize <= 2);
			theValue = NFormat("{:0{}}", mDate.GetWeekOfYear(), mTokenSize);
			break;

		case 'W':
			NN_REQUIRE(mTokenSize == 1);
			theValue = NFormat("{}", (mDate.GetDay() / 7) + 1);
			break;

		default:
			NN_LOG_WARNING("Unknown token: {}", mToken);
			break;
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetDay : Get a day.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetDay() const
{


	// Get the text
	NString theValue;

	switch (mTokenChar)
	{
		case 'd':
			NN_REQUIRE(mTokenSize >= 1 && mTokenSize <= 2);
			theValue = NFormat("{:0{}}", mDate.GetDay(), mTokenSize);
			break;

		case 'D':
			NN_REQUIRE(mTokenSize >= 1 && mTokenSize <= 3);
			theValue = NFormat("{:0{}}", mDate.GetDayOfYear(), mTokenSize);
			break;

		case 'F':
		case 'g':
			NN_LOG_UNIMPLEMENTED("Unsupported token:{}", mToken);
			theValue = kNDateFormatUnimplemented;
			break;

		default:
			NN_LOG_WARNING("Unknown token: {}", mToken);
			break;
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetWeekDay : Get a week day.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetWeekDay() const
{


	// Validate our parameters
	NN_REQUIRE(mTokenChar == 'E' || mTokenChar == 'e' || mTokenChar == 'c');



	// Get the text
	size_t  dayOfWeek = size_t(mDate.GetDayOfWeek()) - 1;
	NString theValue;

	switch (mTokenSize)
	{
		case 1:
		case 2:
			if (mTokenChar == 'E')
			{
				theValue = GetTextDay(dayOfWeek, NDateWidth::Abbrev);
			}
			else
			{
				theValue = NFormat("{:0{}}", dayOfWeek + 1, mTokenSize);
			}
			break;

		case 3:
			theValue = GetTextDay(dayOfWeek, NDateWidth::Abbrev);
			break;

		case 4:
			theValue = GetTextDay(dayOfWeek, NDateWidth::Wide);
			break;

		case 5:
			theValue = GetTextDay(dayOfWeek, NDateWidth::Narrow);
			break;

		default:
			NN_LOG_WARNING("Unknown token: {}", mToken);
			break;
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetPeriod : Get a period.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetPeriod() const
{


	// Validate our parameters
	NN_REQUIRE(mTokenChar == 'a');
	NN_REQUIRE(mTokenSize == 1);



	// Get the text
	//
	// It's unclear if the standard allows these values to be localised; for now we assume not.
	size_t  theHour = size_t(mSeconds / kNTimeHour);
	NString theValue;

	if (theHour >= 12)
	{
		theValue = "PM";
	}
	else
	{
		theValue = "AM";
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetHour : Get an hour.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetHour() const
{


	// Get the text
	size_t  theHour = size_t(mSeconds / kNTimeHour);
	NString theValue;

	switch (mTokenChar)
	{
		case 'h':
			NN_REQUIRE(mTokenSize >= 1 && mTokenSize <= 2);
			theHour  = (theHour % 12) == 0 ? 12 : (theHour % 12);
			theValue = NFormat("{:0{}}", theHour, mTokenSize);
			break;

		case 'H':
			NN_REQUIRE(mTokenSize >= 1 && mTokenSize <= 2);
			theValue = NFormat("{:0{}}", theHour, mTokenSize);
			break;

		case 'K':
			NN_REQUIRE(mTokenSize >= 1 && mTokenSize <= 2);
			theValue = NFormat("{:0{}}", (theHour % 12), mTokenSize);
			break;

		case 'k':
			NN_REQUIRE(mTokenSize >= 1 && mTokenSize <= 2);
			theHour  = (theHour == 0) ? 24 : theHour;
			theValue = NFormat("{:0{}}", theHour, mTokenSize);
			break;

		default:
			NN_LOG_WARNING("Unknown token: {}", mToken);
			break;
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetMinute : Get a minute.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetMinute() const
{


	// Validate our parameters
	NN_REQUIRE(mTokenChar == 'm');
	NN_REQUIRE(mTokenSize >= 1 && mTokenSize <= 2);



	// Get the text
	size_t    theHours   = size_t(mSeconds / kNTimeHour);
	NInterval theMinutes = NInterval(size_t((mSeconds - (NInterval(theHours) * kNTimeHour)) / kNTimeMinute));
	NString   theValue;

	theValue = NFormat("{:0{}}", theMinutes, mTokenSize);

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetSecond : Get a second.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetSecond() const
{


	// Get the text
	size_t    theMinutes = size_t(mSeconds / kNTimeMinute);
	NInterval theSeconds = mSeconds - (NInterval(theMinutes) * kNTimeMinute);

	size_t    secondInt  = size_t(theSeconds);
	NInterval secondFrac = theSeconds - NInterval(secondInt);
	NString   theValue;

	switch (mTokenChar)
	{
		case 's':
			NN_REQUIRE(mTokenSize >= 1 && mTokenSize <= 2);
			theValue = NFormat("{:0{}}", secondInt, mTokenSize);
			break;

		case 'S':
			theValue = NFormat("{:.{}}", secondFrac, mTokenSize);

			NN_REQUIRE(theValue.StartsWith("0."));
			theValue.RemovePrefix(2);
			break;

		case 'A':
			theValue = NFormat("{}", uint64_t(mSeconds / kNTimeMillisecond));
			break;

		default:
			NN_LOG_WARNING("Unknown token: {}", mToken);
			break;
	}

	return theValue;
}





//=============================================================================
//		NDateFormatter::GetZone : Get a time zone.
//-----------------------------------------------------------------------------
NString NDateFormatter::GetZone() const
{


	// Get the text
	NString theValue;

	switch (mTokenChar)
	{
		case 'z':
			NN_LOG_UNIMPLEMENTED("Unsupported token:{}", mToken);
			theValue = kNDateFormatUnimplemented;
			break;

		case 'Z':
			NN_LOG_UNIMPLEMENTED("Unsupported token:{}", mToken);
			theValue = kNDateFormatUnimplemented;
			break;

		case 'v':
			NN_LOG_UNIMPLEMENTED("Unsupported token:{}", mToken);
			theValue = kNDateFormatUnimplemented;
			break;

		default:
			NN_LOG_WARNING("Unknown token: {}", mToken);
			break;
	}

	return theValue;
}
