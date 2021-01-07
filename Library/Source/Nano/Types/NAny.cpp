/*	NAME:
		NAny.cpp

	DESCRIPTION:
		std::any object.

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
#include "NAny.h"

// Nano
#include "NArray.h"
#include "NData.h"
#include "NNumber.h"
#include "NString.h"
#include "NTime.h"





#pragma mark NMixinComparable
//=============================================================================
//		NAny::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NAny::CompareEqual(const NAny& theValue) const
{


	// Compare the value
	return CompareOrder(theValue) == NComparison::EqualTo;
}





//=============================================================================
//		NAny::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NAny::CompareOrder(const NAny& theValue) const
{


	// Compare by contents
	//
	// Empty values are sorted before actual values.
	NComparison theResult = NComparison::EqualTo;

	if (IsEmpty() && !theValue.IsEmpty())
	{
		theResult = NComparison::LessThan;
	}
	else if (!IsEmpty() && theValue.IsEmpty())
	{
		theResult = NComparison::GreaterThan;
	}



	// Compare by value
	else
	{
		// Compare by number
		//
		// An NNumber can accept multiple numeric types so allows
		// us to compare values even across differing types.
		NNumber numberA, numberB;

		if (numberA.SetValue(*this) && numberB.SetValue(theValue))
		{
			theResult = NCompare(numberA, numberB);
		}


		// Compare by type
		//
		// Unequal types have no real ordering, but we can give them
		// a consistent order based on their type hash.
		else if (type().hash_code() != theValue.type().hash_code())
		{
			theResult = NCompare(type().hash_code(), theValue.type().hash_code());
		}



		// Compare by value
		//
		// Only known value types can be compared.
		else
		{
			if (Has<NArray>())
			{
				theResult = NCompare(Get<NArray>(), theValue.Get<NArray>());
			}
			else if (Has<NData>())
			{
				theResult = NCompare(Get<NData>(), theValue.Get<NData>());
			}
			else if (Has<NString>())
			{
				theResult = NCompare(Get<NString>(), theValue.Get<NString>());
			}
			else if (Has<NTime>())
			{
				theResult = NCompare(Get<NTime>(), theValue.Get<NTime>());
			}
			else
			{
				NN_LOG_ERROR("Unable to compare unknown NAny type '{}'", type().name());
			}
		}
	}

	return theResult;
}
