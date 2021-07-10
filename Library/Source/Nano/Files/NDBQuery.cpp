/*	NAME:
		NDBQuery.cpp

	DESCRIPTION:
		Database query.

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
#include "NDBQuery.h"

// Nano
#include "NArray.h"
#include "NDictionary.h"





//=============================================================================
//		NDBQuery::NDBQuery : Constructor.
//-----------------------------------------------------------------------------
NDBQuery::NDBQuery(const NString& theSQL)
	: mSQL(theSQL)
	, mParameters()
{


	// Validate our parameters
	NN_REQUIRE(!theSQL.IsEmpty());
}





//=============================================================================
//		NDBQuery::GetSQL : Get the SQL.
//-----------------------------------------------------------------------------
NString NDBQuery::GetSQL() const
{


	// Get the SQL
	return mSQL;
}





//=============================================================================
//		NDBQuery::SetSQL : Set the SQL.
//-----------------------------------------------------------------------------
void NDBQuery::SetSQL(const NString& theSQL)
{


	// Validate our parameters
	NN_REQUIRE(!theSQL.IsEmpty());



	// Set the SQL
	mSQL = theSQL;
}





//=============================================================================
//		NDBQuery::GetParameters : Get the parameters.
//-----------------------------------------------------------------------------
NAny NDBQuery::GetParameters() const
{


	// Get the parameters
	return mParameters;
}





//=============================================================================
//		NDBQuery::SetParameters : Set the parameters.
//-----------------------------------------------------------------------------
void NDBQuery::SetParameters(const NAny& theParameters)
{


	// Validate our parameters
	if (NN_ENABLE_ASSERTIONS && !theParameters.IsEmpty())
	{
		NN_REQUIRE(theParameters.Is<NArray>() || theParameters.Is<NDictionary>());

		if (theParameters.Is<NDictionary>())
		{
			for (const auto& theKey : theParameters.Get<NDictionary>().GetKeys())
			{
				NN_REQUIRE(!theKey.StartsWith(":"));
			}
		}
	}



	// Set the parameters
	mParameters = theParameters;
}
