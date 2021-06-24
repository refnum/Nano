/*	NAME:
		NDate.cpp

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

// Nano
#include "NDebug.h"





//=============================================================================
//		NDate::GetDayOfWeek : Get the day of the week.
//-----------------------------------------------------------------------------
NWeekday NDate::GetDayOfWeek() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Calculate the day of the week
	//
	// Zeller's algorithm.
	int32_t theYear  = mYear;
	int32_t theMonth = mMonth;

	if (theMonth < 2)
	{
		theMonth = (12 + theMonth);
		theYear--;
	}

	int32_t y = theYear % 100;
	int32_t c = theYear / 100;
	int32_t w = int32_t(mDay + floor((13 * (theMonth + 1)) / 5.0) + y + floor(y / 4.0) +
						floor(c / 4.0) + (5 * c));

	return NWeekday((w % 7) - 1);
}





//=============================================================================
//		NDate::GetDayOfYear : Get the day of the year.
//-----------------------------------------------------------------------------
size_t NDate::GetDayOfYear() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the day
	static constexpr size_t kYearDays[2][12] = {
		{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
		{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}};

	size_t indexYear  = size_t(IsLeapYear() ? 1 : 0);
	size_t indexMonth = size_t(mMonth - 1);

	return kYearDays[indexYear][indexMonth] + mDay;
}





//=============================================================================
//		NDate::GetWeekOfYear : Get the week of the year.
//-----------------------------------------------------------------------------
size_t NDate::GetWeekOfYear() const
{


	// Get the week
	size_t yearDay   = GetDayOfYear();
	size_t theResult = ((yearDay - (7) % 7 + 7) / 7);
	NN_REQUIRE(theResult <= size_t(IsLeapYear() ? 52 : 53));

	return theResult;
}





//=============================================================================
//		NDate::GetDaysBetween : Get the days between two dates.
//-----------------------------------------------------------------------------
size_t NDate::GetDaysBetween(const NDate& theDate) const
{


	// Get the state we need
	NDate dateA(*this);
	NDate dateB(theDate);

	if (dateA > dateB)
	{
		std::swap(dateA, dateB);
	}



	// Test for negative
	bool negativeA = (dateA.mYear < 0);
	bool negativeB = (dateB.mYear < 0);

	if (negativeA)
	{
		dateA.mYear = -dateA.mYear;
	}

	if (negativeB)
	{
		dateB.mYear = -dateB.mYear;
	}



	// Calculate the delta
	size_t daysA   = dateA.GetDaysSinceZero();
	size_t daysB   = dateB.GetDaysSinceZero();
	size_t numDays = 0;

	if (negativeA && negativeB)
	{
		NN_REQUIRE(daysA >= daysB);
		numDays = daysA - daysB;
	}
	else if (negativeA && !negativeB)
	{
		numDays = daysA + daysB;
	}
	else
	{
		NN_REQUIRE(!negativeA && !negativeB);
		NN_REQUIRE(daysB >= daysA);
		numDays = daysB - daysA;
	}

	return numDays;
}





//=============================================================================
//		NDate::Add : Add to the date.
//-----------------------------------------------------------------------------
void NDate::Add(size_t numYears, size_t numMonths, size_t numDays)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Add the years
	mYear += numYears;



	// Add the months
	mMonth += numMonths;

	while (mMonth > 12)
	{
		mYear += 1;
		mMonth -= 12;
	}



	// Add the days
	NDay daysInMonth = GetDaysInMonth();

	while (numDays != 0)
	{
		mDay++;
		numDays--;

		if (mDay > daysInMonth)
		{
			mDay = 1;
			mMonth += 1;
			if (mMonth == 13)
			{
				mMonth = 1;
				mYear++;
			}

			daysInMonth = GetDaysInMonth();
		}
	}


	// Round down
	//
	// If the final day is not valid then round it down.
	if (mDay > daysInMonth)
	{
		mDay = daysInMonth;
	}



	// Validate our state
	NN_REQUIRE(IsValid());
}





//=============================================================================
//		NDate::Subtract : Subtract from the date.
//-----------------------------------------------------------------------------
void NDate::Subtract(size_t numYears, size_t numMonths, size_t numDays)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Subtract the years
	mYear -= numYears;



	// Subtract the months
	while (numMonths != 0)
	{
		if (mMonth == 1)
		{
			mMonth = 13;
			mYear--;
		}

		mMonth--;
		numMonths--;
	}



	// Subtract the days
	while (numDays != 0)
	{
		if (mDay == 1)
		{
			if (mMonth == 1)
			{
				mMonth = 13;
				mYear--;
			}

			mMonth--;
			mDay = NDay(GetDaysInMonth() + 1);
		}

		mDay--;
		numDays--;
	}



	// Round down
	//
	// If the final day is not valid then round it down.
	NDay daysInMonth = GetDaysInMonth();

	if (mDay > daysInMonth)
	{
		mDay = daysInMonth;
	}

	NN_REQUIRE(IsValid());
}





#pragma mark NMixinComparable
//=============================================================================
//		NDate::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NDate::CompareEqual(const NDate& theDate) const
{


	// Validate our parameters and state
	NN_REQUIRE(theDate.IsValid());
	NN_REQUIRE(IsValid());



	// Compare the dates
	return (mYear == theDate.mYear) && (mMonth == theDate.mMonth) && (mDay == theDate.mDay);
}





//=============================================================================
//		NDate::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NDate::CompareOrder(const NDate& theDate) const
{


	// Validate our parameters and state
	NN_REQUIRE(theDate.IsValid());
	NN_REQUIRE(IsValid());



	// Order by date
	NComparison theResult = NCompare(mYear, theDate.mYear);

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mMonth, theDate.mMonth);
	}

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mDay, theDate.mDay);
	}

	return theResult;
}





#pragma mark private
//=============================================================================
//		NDate::GetDaysSinceZero : Get the number of days since the zero date.
//-----------------------------------------------------------------------------
size_t NDate::GetDaysSinceZero() const
{


	// Validate our state
	NN_REQUIRE(mYear >= 0);



	size_t theYear  = size_t(mMonth > 2 ? mYear : (mYear - 1));
	size_t leapDays = (theYear / 4) - (theYear / 100) + (theYear / 400);

	return GetDayOfYear() + (size_t(mYear) * 365) + leapDays;
}
