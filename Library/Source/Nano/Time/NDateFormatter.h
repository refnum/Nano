/*	NAME:
		NDateFormatter.h

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
#ifndef NDATE_FORMATTER_H
#define NDATE_FORMATTER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDate.h"
#include "NString.h"
#include "NTime.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Date widths
enum class NDateWidth;



// Date formats
//
// Date formats are defined by <http://unicode.org/reports/tr35/tr35-6.html#Date_Format_Patterns>
inline constexpr const char* kNDateFormatDefault            = "HH:mm yy/MM/dd";
inline constexpr const char* kNDateFormatShort              = "HH:mm yy/MM/dd";
inline constexpr const char* kNDateFormatMedium             = "hh:mma dd MMM, yyyy";
inline constexpr const char* kNDateFormatLong               = "hh:mma dd MMMM, yyyy G";





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NDateFormatter
{
public:
										NDateFormatter();


	// Set the date
	void                                SetDate(const NDate& theDate, NInterval theSeconds = 0.0);
	void                                SetDate(NTime theTime);


	// Format the date
	//
	// Date formats are defined by <http://unicode.org/reports/tr35/tr35-6.html#Date_Format_Patterns>
	NString                             Format(const NString& theFormat = kNDateFormatDefault) const;


private:
	const utf8_t*                       GetTokenEnd(const utf8_t* tokenStart) const;

	NString                             GetTextQuarter(size_t theIndex, NDateWidth theWidth) const;
	NString                             GetTextMonth(  size_t theIndex, NDateWidth theWidth) const;
	NString                             GetTextDay(    size_t theIndex, NDateWidth theWidth) const;

	NString                             GetLiteral() const;
	NString                             GetEra()     const;
	NString                             GetYear()    const;
	NString                             GetQuarter() const;
	NString                             GetMonth()   const;
	NString                             GetWeek()    const;
	NString                             GetDay()     const;
	NString                             GetWeekDay() const;
	NString                             GetPeriod()  const;
	NString                             GetHour()    const;
	NString                             GetMinute()  const;
	NString                             GetSecond()  const;
	NString                             GetZone()    const;


private:
	NDate                               mDate;
	NInterval                           mSeconds;

	mutable NString                     mToken;
	mutable size_t                      mTokenSize;
	mutable utf8_t                      mTokenChar;
};



#endif // NDATE_FORMATTER_H
