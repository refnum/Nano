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



	// Compare by type
	//
	// Unequal types have no real ordering, but we can give them a
	// consistent order based on their type hash.
	else if (type().hash_code() != theValue.type().hash_code())
	{
		theResult = NCompare(type().hash_code(), theValue.type().hash_code());
	}



	// Compare known types
	//
	// Only known types can be compared.
	else
	{
		if (HasUInt8())
		{
			theResult = NCompare(GetUInt8(), theValue.GetUInt8());
		}
		else if (HasUInt16())
		{
			theResult = NCompare(GetUInt16(), theValue.GetUInt16());
		}
		else if (HasUInt32())
		{
			theResult = NCompare(GetUInt32(), theValue.GetUInt32());
		}
		else if (HasUInt64())
		{
			theResult = NCompare(GetUInt64(), theValue.GetUInt64());
		}

		else if (HasInt8())
		{
			theResult = NCompare(GetInt8(), theValue.GetInt8());
		}
		else if (HasInt16())
		{
			theResult = NCompare(GetInt16(), theValue.GetInt16());
		}
		else if (HasInt32())
		{
			theResult = NCompare(GetInt32(), theValue.GetInt32());
		}
		else if (HasInt64())
		{
			theResult = NCompare(GetInt64(), theValue.GetInt64());
		}

		else if (HasFloat32())
		{
			theResult = NCompare(GetFloat32(), theValue.GetFloat32());
		}
		else if (HasFloat64())
		{
			theResult = NCompare(GetFloat64(), theValue.GetFloat64());
		}

		else
		{
			NN_LOG_ERROR("Unable to compare unknown NAny type!");
		}
	}

	return theResult;
}
