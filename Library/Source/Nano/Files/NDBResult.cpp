/*	NAME:
		NDBResult.cpp

	DESCRIPTION:
		Database result.

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
#include "NDBResult.h"

// Nano
#include "Nano_sqlite3.h"





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
#define mStatement                                          reinterpret_cast<sqlite3_stmt*>(mQuery)





//=============================================================================
//		NDBResult::NDBResult : Constructor.
//-----------------------------------------------------------------------------
NDBResult::NDBResult(NDBQueryRef theQuery)
	: mQuery(theQuery)
{
}





//=============================================================================
//		NDBResult::GetSize : Get the number of columns.
//-----------------------------------------------------------------------------
size_t NDBResult::GetSize() const
{


	// Get the number of columns
	int theSize = sqlite3_column_count(mStatement);
	NN_REQUIRE(theSize >= 0);

	return size_t(theSize);
}





//=============================================================================
//		NDBResult::GetName : Get the name of a column.
//-----------------------------------------------------------------------------
NString NDBResult::GetName(size_t theIndex) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < GetSize());



	// Get the number of columns
	return sqlite3_column_name(mStatement, int(theIndex));
}





//=============================================================================
//		NDBResult::GetInt64 : Get an int64_t value.
//-----------------------------------------------------------------------------
int64_t NDBResult::GetInt64(size_t theIndex) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < GetSize());



	// Get the value
	return sqlite3_column_int64(mStatement, int(theIndex));
}





//=============================================================================
//		NDBResult::GetFloat64 : Get a float64_t value.
//-----------------------------------------------------------------------------
float64_t NDBResult::GetFloat64(size_t theIndex) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < GetSize());



	// Get the value
	return sqlite3_column_double(mStatement, int(theIndex));
}





//=============================================================================
//		NDBResult::GetString : Get a string value.
//-----------------------------------------------------------------------------
NString NDBResult::GetString(size_t theIndex) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < GetSize());



	// Get the value
	const void* thePtr  = sqlite3_column_text(mStatement, int(theIndex));
	int         theSize = sqlite3_column_bytes(mStatement, int(theIndex));
	NN_REQUIRE(theSize >= 0);

	if (thePtr != nullptr && theSize != 0)
	{
		return NString(NStringEncoding::UTF8, size_t(theSize), thePtr);
	}

	return {};
}





//=============================================================================
//		NDBResult::GetData : Get a data value.
//-----------------------------------------------------------------------------
NData NDBResult::GetData(size_t theIndex) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < GetSize());



	// Get the value
	const void* thePtr  = sqlite3_column_blob(mStatement, int(theIndex));
	int         theSize = sqlite3_column_bytes(mStatement, int(theIndex));
	NN_REQUIRE(theSize >= 0);

	if (thePtr != nullptr && theSize != 0)
	{
		return NData(size_t(theSize), thePtr);
	}

	return {};
}





//=============================================================================
//		NDBResult::GetRowInt64 : Get a column from a row.
//-----------------------------------------------------------------------------
void NDBResult::GetRowInt64(const NDBResult& theRow, size_t theIndex, int64_t* theValue)
{


	// Get the value
	*theValue = theRow.GetInt64(theIndex);
}





//=============================================================================
//		NDBResult::GetRowFloat64 : Get a column from a row.
//-----------------------------------------------------------------------------
void NDBResult::GetRowFloat64(const NDBResult& theRow, size_t theIndex, float64_t* theValue)
{


	// Get the value
	*theValue = theRow.GetFloat64(theIndex);
}





//=============================================================================
//		NDBResult::GetRowString : Get a column from a row.
//-----------------------------------------------------------------------------
void NDBResult::GetRowString(const NDBResult& theRow, size_t theIndex, NString* theValue)
{


	// Get the value
	*theValue = theRow.GetString(theIndex);
}





//=============================================================================
//		NDBResult::GetRowData : Get a column from a row.
//-----------------------------------------------------------------------------
void NDBResult::GetRowData(const NDBResult& theRow, size_t theIndex, NData* theValue)
{


	// Get the value
	*theValue = theRow.GetData(theIndex);
}





//=============================================================================
//		NDBResult::AccumulateRowInt64 : Get a list of row values.
//-----------------------------------------------------------------------------
void NDBResult::AccumulateRowInt64(const NDBResult& theRow,
								   size_t           theIndex,
								   NVectorInt64*    theValues)
{


	// Get the value
	theValues->emplace_back(theRow.GetInt64(theIndex));
}





//=============================================================================
//		NDBResult::AccumulateRowFloat64 : Get a list of row values.
//-----------------------------------------------------------------------------
void NDBResult::AccumulateRowFloat64(const NDBResult& theRow,
									 size_t           theIndex,
									 NVectorFloat64*  theValues)
{


	// Get the value
	theValues->emplace_back(theRow.GetFloat64(theIndex));
}





//=============================================================================
//		NDBResult::AccumulateRowString : Get a list of row values.
//-----------------------------------------------------------------------------
void NDBResult::AccumulateRowString(const NDBResult& theRow,
									size_t           theIndex,
									NVectorString*   theValues)
{


	// Get the value
	theValues->emplace_back(theRow.GetString(theIndex));
}





//=============================================================================
//		NDBResult::AccumulateRowData : Get a list of row values.
//-----------------------------------------------------------------------------
void NDBResult::AccumulateRowData(const NDBResult& theRow, size_t theIndex, NVectorData* theValues)
{


	// Get the value
	theValues->emplace_back(theRow.GetData(theIndex));
}
