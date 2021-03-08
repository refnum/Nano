/*	NAME:
		NDate.h

	DESCRIPTION:
		Date object.

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
#ifndef NDATE_H
#define NDATE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.h"
#include "NanoMacros.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
using NYear                                                 = int16_t;
using NMonth                                                = uint8_t;
using NDay                                                  = uint8_t;
using NWeekday                                              = uint8_t;





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Months
inline constexpr NMonth kNJanuary                           = 1;
inline constexpr NMonth kNFebruary                          = 2;
inline constexpr NMonth kNMarch                             = 3;
inline constexpr NMonth kNApril                             = 4;
inline constexpr NMonth kNMay                               = 5;
inline constexpr NMonth kNJune                              = 6;
inline constexpr NMonth kNJuly                              = 7;
inline constexpr NMonth kNAugust                            = 8;
inline constexpr NMonth kNSeptember                         = 9;
inline constexpr NMonth kNOctober                           = 10;
inline constexpr NMonth kNNovember                          = 11;
inline constexpr NMonth kNDecember                          = 12;

// Weekdays
inline constexpr NWeekday kNMonday                          = 1;
inline constexpr NWeekday kNTuesday                         = 2;
inline constexpr NWeekday kNWednesday                       = 3;
inline constexpr NWeekday kNThursday                        = 4;
inline constexpr NWeekday kNFriday                          = 5;
inline constexpr NWeekday kNSaturday                        = 6;
inline constexpr NWeekday kNSunday                          = 7;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NDate final : public NMixinComparable<NDate>
{
public:
	constexpr                           NDate(NYear theYear, NMonth theMonth, NDay theDay);
	constexpr                           NDate();


	// Is the date valid?
	constexpr bool                      IsValid() const;


	// Is this a leap yaer?
	constexpr bool                      IsLeapYear() const;


	// Get / set the year
	constexpr NYear                     GetYear() const;
	constexpr void                      SetYear(NYear theYear);


	// Get / set the month
	constexpr NMonth                    GetMonth() const;
	constexpr void                      SetMonth(NMonth theMonth);


	// Get / set the day
	constexpr NDay                      GetDay() const;
	constexpr void                      SetDay(NDay theDay);


	// Get the day of the week
	NWeekday                            GetDayOfWeek() const;


	// Get the day of the year
	size_t                              GetDayOfYear() const;


	// Get the week of the year
	size_t                              GetWeekOfYear() const;


	// Get the days between two dates
	size_t                              GetDaysBetween(const NDate& theDate) const;


	// Add to the date
	//
	// Adding to a date adds years, then months, then days.
	//
	// If the resulting date is invalid (e.g., adding 1 year to February
	// 29th within a leap year) then the day is reduced to the last valid
	// day of the month.
	void                                Add(      size_t numYears, size_t numMonths, size_t numDays);
	void                                AddYears( size_t numYears);
	void                                AddMonths(size_t numMonths);
	void                                AddDays(  size_t numDays);


	// Subtract from the date
	//
	// Subtracting from a date subtracts years, then months, then days.
	//
	// The resulting date is rounded as per Add.
	void                                Subtract(      size_t numYears, size_t numMonths, size_t numDays);
	void                                SubtractYears( size_t numYears);
	void                                SubtractMonths(size_t numMonths);
	void                                SubtractDays(  size_t numDays);


private:
	constexpr NDay                      GetDaysInMonth()   const;
	size_t                              GetDaysSinceZero() const;



public:
	// NMixinComparable
	bool                                CompareEqual(const NDate& theDate) const;
	NComparison                         CompareOrder(const NDate& theDate) const;



private:
	NYear                               mYear;
	NMonth                              mMonth;
	NDay                                mDay;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDate.inl"


#endif // NDATE_H
