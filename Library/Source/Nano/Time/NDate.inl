/*	NAME:
		NDate.inl

	DESCRIPTION:
		Date object.

		Dates are stored as an offset from the Nano Epoch.

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
#include "NDate.h"





//=============================================================================
//		NDate::NDate : Constructor.
//-----------------------------------------------------------------------------
constexpr NDate::NDate(NYear theYear, NMonth theMonth, NDay theDay)
	: mYear(theYear)
	, mMonth(theMonth)
	, mDay(theDay)
{
}





//=============================================================================
//		NDate::NDate : Constructor.
//-----------------------------------------------------------------------------
constexpr NDate::NDate()
	: mYear(0)
	, mMonth(0)
	, mDay(0)
{
}





//=============================================================================
//		NDate::IsValid : Is the date valid?
//-----------------------------------------------------------------------------
constexpr bool NDate::IsValid() const
{


	// Check the range
	return mDay >= 1 && mDay <= GetDaysInMonth();
}





//=============================================================================
//		NDate::IsLeapYaer : Is this a leap year?
//-----------------------------------------------------------------------------
constexpr bool NDate::IsLeapYear() const
{


	// Check the year
	//
	// A leap year is exactly divisible by 4 except for century years,
	// which are only leap years if they are exactly divisible by 400.
	return ((mYear % 4) == 0) && (((mYear % 100) != 0) || ((mYear % 400) == 0));
}





//=============================================================================
//		NDate::GetYear : Get the year.
//-----------------------------------------------------------------------------
constexpr NYear NDate::GetYear() const
{


	// Get the year
	return mYear;
}





//=============================================================================
//		NDate::SetYear : Set the year.
//-----------------------------------------------------------------------------
constexpr void NDate::SetYear(NYear theYear)
{


	// Set the year
	mYear = theYear;
}





//=============================================================================
//		NDate::GetMonth : Get the month.
//-----------------------------------------------------------------------------
constexpr NMonth NDate::GetMonth() const
{


	// Get the month
	return mMonth;
}





//=============================================================================
//		NDate::SetMonth : Set the month.
//-----------------------------------------------------------------------------
constexpr void NDate::SetMonth(NMonth theMonth)
{


	// Set the month
	mMonth = theMonth;
}





//=============================================================================
//		NDate::GetDay : Get the day.
//-----------------------------------------------------------------------------
constexpr NDay NDate::GetDay() const
{


	// Get the day
	return mDay;
}





//=============================================================================
//		NDate::SetDay : Set the day.
//-----------------------------------------------------------------------------
constexpr void NDate::SetDay(NDay theDay)
{


	// Set the day
	mDay = theDay;
}





//=============================================================================
//		NDate::AddYears : Add years.
//-----------------------------------------------------------------------------
inline void NDate::AddYears(size_t numYears)
{


	// Add to the date
	Add(numYears, 0, 0);
}





//=============================================================================
//		NDate::AddMonths : Add months.
//-----------------------------------------------------------------------------
inline void NDate::AddMonths(size_t numMonths)
{


	// Add to the date
	Add(0, numMonths, 0);
}





//=============================================================================
//		NDate::AddDays : Add days.
//-----------------------------------------------------------------------------
inline void NDate::AddDays(size_t numDays)
{


	// Add to the date
	Add(0, 0, numDays);
}





//=============================================================================
//		NDate::SubtractYears : Subtract years.
//-----------------------------------------------------------------------------
inline void NDate::SubtractYears(size_t numYears)
{


	// Subtract from the date
	Subtract(numYears, 0, 0);
}





//=============================================================================
//		NDate::SubtractMonths : Subtract months.
//-----------------------------------------------------------------------------
inline void NDate::SubtractMonths(size_t numMonths)
{


	// Subtract from the date
	Subtract(0, numMonths, 0);
}





//=============================================================================
//		NDate::SubtractDays : Subtract days.
//-----------------------------------------------------------------------------
inline void NDate::SubtractDays(size_t numDays)
{


	// Subtract from the date
	Subtract(0, 0, numDays);
}





#pragma mark private
//=============================================================================
//		NDate::GetDaysInMonth : Get the days in a month.
//-----------------------------------------------------------------------------
constexpr NDay NDate::GetDaysInMonth() const
{


	// Get the days in the month
	switch (mMonth)
	{
		case kNJanuary:
		case kNMarch:
		case kNMay:
		case kNJuly:
		case kNAugust:
		case kNOctober:
		case kNDecember:
			return 31;
			break;

		case kNApril:
		case kNJune:
		case kNSeptember:
		case kNNovember:
			return 30;
			break;

		case kNFebruary:
			return IsLeapYear() ? 29 : 28;
			break;
	}

	return 0;
}
