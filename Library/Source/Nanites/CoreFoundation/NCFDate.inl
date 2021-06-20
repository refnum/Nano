/*	NAME:
		NCFDate.inl

	DESCRIPTION:
		CFDateRef wrapper.

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
#include "NCoreFoundation.h"





//=============================================================================
//		NCFDate::NCFDate : Constructor.
//-----------------------------------------------------------------------------
inline NCFDate::NCFDate(const NDate& theDate)
{


	// Initialise ourselves
	SetDate(theDate);
}





//=============================================================================
//		NCFDate::GetDate : Get the date.
//-----------------------------------------------------------------------------
inline NDate NCFDate::GetDate() const
{


	// Get the date
	CFDateRef cfDate = *this;
	NDate     theDate;

	if (cfDate != nullptr)
	{
		CFAbsoluteTime cfTime = CFDateGetAbsoluteTime(cfDate);
		theDate               = NDate(2001, 1, 1);

		theDate.AddDays(size_t(cfTime / kNTimeDay));
	}

	return theDate;
}





//=============================================================================
//		NCFDate::SetDate : Set the date.
//-----------------------------------------------------------------------------
bool NCFDate::SetDate(const NDate& theDate)
{


	// Set the date
	size_t    numDays = theDate.GetDaysBetween(NDate(2001, 1, 1));
	NInterval theTime = NInterval(numDays) * kNTimeDay;

	return Set(CFDateCreate(kCFAllocatorDefault, theTime));
}
