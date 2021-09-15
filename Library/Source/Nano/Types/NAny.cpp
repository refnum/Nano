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
#include "NDictionary.h"
#include "NNumber.h"
#include "NString.h"
#include "NTime.h"

// System
#include <typeindex>





//=============================================================================
//		NAny::IsArray : Is the value an NArray?
//-----------------------------------------------------------------------------
bool NAny::IsArray() const
{


	// Check the type
	return Is<NArray>();
}





//=============================================================================
//		NAny::IsData : Is the value an NData?
//-----------------------------------------------------------------------------
bool NAny::IsData() const
{


	// Check the type
	return Is<NData>();
}





//=============================================================================
//		NAny::IsDictionary : Is the value an NDictionary?
//-----------------------------------------------------------------------------
bool NAny::IsDictionary() const
{


	// Check the type
	return Is<NDictionary>();
}





//=============================================================================
//		NAny::IsNumber : Is the value an NNumber?
//-----------------------------------------------------------------------------
bool NAny::IsNumber() const
{


	// Check the type
	return Is<NNumber>();
}





//=============================================================================
//		NAny::IsString : Is the value an NString?
//-----------------------------------------------------------------------------
bool NAny::IsString() const
{


	// Check the type
	return Is<NString>();
}





//=============================================================================
//		NAny::IsTime : Is the value an NTime?
//-----------------------------------------------------------------------------
bool NAny::IsTime() const
{


	// Check the type
	return Is<NTime>();
}





//=============================================================================
//		NAny::GetArray : Get an NArray.
//-----------------------------------------------------------------------------
NArray NAny::GetArray() const
{


	// Get the value
	if (IsEmpty())
	{
		return {};
	}

	return Get<NArray>();
}





//=============================================================================
//		NAny::GetData : Get an NData.
//-----------------------------------------------------------------------------
NData NAny::GetData() const
{


	// Get the value
	if (IsEmpty())
	{
		return {};
	}

	return Get<NData>();
}





//=============================================================================
//		NAny::GetDictionary : Get an NDictionary.
//-----------------------------------------------------------------------------
NDictionary NAny::GetDictionary() const
{


	// Get the value
	if (IsEmpty())
	{
		return {};
	}

	return Get<NDictionary>();
}





//=============================================================================
//		NAny::GetNumber : Get an NNumber.
//-----------------------------------------------------------------------------
NNumber NAny::GetNumber() const
{


	// Get the value
	if (IsEmpty())
	{
		return {};
	}

	return Get<NNumber>();
}





//=============================================================================
//		NAny::GetString : Get an NString.
//-----------------------------------------------------------------------------
NString NAny::GetString() const
{


	// Get the value
	if (IsEmpty())
	{
		return {};
	}

	return Get<NString>();
}





//=============================================================================
//		NAny::GetTime : Get an NTime.
//-----------------------------------------------------------------------------
NTime NAny::GetTime() const
{


	// Get the value
	if (IsEmpty())
	{
		return {};
	}

	return Get<NTime>();
}





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
		// Unequal types have no real ordering, so the best we can do is an
		// implementation-defined ordering based on their type.
		//
		// This ordering is stable within a single process but may change
		// between invocations of the same program.
		else if (std::type_index(type()) != std::type_index(theValue.type()))
		{
			theResult = NCompare(std::type_index(type()), std::type_index(theValue.type()));
		}


		// Compare by value
		//
		// Only known value types can be compared.
		else
		{
			if (IsArray())
			{
				theResult = NCompare(GetArray(), theValue.GetArray());
			}
			else if (IsData())
			{
				theResult = NCompare(GetData(), theValue.GetData());
			}
			else if (IsDictionary())
			{
				theResult = NCompare(GetDictionary(), theValue.GetDictionary());
			}
			else if (IsString())
			{
				theResult = NCompare(GetString(), theValue.GetString());
			}
			else if (IsTime())
			{
				theResult = NCompare(GetTime(), theValue.GetTime());
			}
			else if (Is<const utf8_t*>())
			{
				theResult = NCompare(strcmp(Get<const utf8_t*>(), theValue.Get<const utf8_t*>()));
			}
			else
			{
				NN_LOG_ERROR("Unable to compare unknown NAny type '{}'", type().name());
			}
		}
	}

	return theResult;
}
