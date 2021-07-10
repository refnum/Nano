/*	NAME:
		NDBResult.h

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
#ifndef NDBRESULT_H
#define NDBRESULT_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDBHandle.h"
#include "NData.h"
#include "NMixinContainer.h"
#include "NString.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NDBResult final : public NMixinContainer<NDBResult>

{
public:
										NDBResult(NDBQueryRef theQuery);


	// Get the number of columns
	size_t                              GetSize() const;


	// Get the name of a column
	NString                             GetName(size_t theIndex) const;


	// Get a column value
	int64_t                             GetInt64(  size_t theIndex) const;
	float64_t                           GetFloat64(size_t theIndex) const;
	NString                             GetString( size_t theIndex) const;
	NData                               GetData(   size_t theIndex) const;


	// Get a column from a row
	static void                         GetRowInt64(  const NDBResult& theRow, size_t theIndex, int64_t*   theValue);
	static void                         GetRowFloat64(const NDBResult& theRow, size_t theIndex, float64_t* theValue);
	static void                         GetRowString( const NDBResult& theRow, size_t theIndex, NString*   theValue);
	static void                         GetRowData(   const NDBResult& theRow, size_t theIndex, NData*     theValue);


	// Accumulate a column from a row
	static void                         AccumulateRowInt64(const NDBResult& theRow,
														   size_t           theIndex,
														   NVectorInt64*    theValues);

	static void                         AccumulateRowFloat64(const NDBResult& theRow,
															 size_t           theIndex,
															 NVectorFloat64*  theValues);

	static void                         AccumulateRowString(const NDBResult& theRow,
															size_t           theIndex,
															NVectorString*   theValues);

	static void                         AccumulateRowData(const NDBResult& theRow, size_t theIndex, NVectorData* theValues);


private:
	NDBQueryRef                         mQuery;
};



#endif // NDBRESULT_H
