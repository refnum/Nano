/*	NAME:
		NStringScanner.h

	DESCRIPTION:
		String scanner.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#ifndef NSTRING_SCANNER_H
#define NSTRING_SCANNER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NRange.h"
#include "NString.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
struct pcre2_real_code_8;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NStringScanner
{
public:
	// Find the first instance of a string
	//
	// kNStringPattern searches return the range of the overall pattern,
	// ignoring any capture groups within that pattern.
	//
	// Supports kNStringNoCase, kNStringPattern, and kNStringMultiLine.
	static NRange                       Find(const NString& theString,
											 const NString& searchFor,
											 const NRange&  searchRange,
											 NStringFlags   theFlags);


	// Find every instance of a string
	//
	// kNStringPattern searches return the ranges of the overall pattern,
	// ignoring any capture groups within that pattern.
	//
	// Supports kNStringNoCase, kNStringPattern, and kNStringMultiLine.
	static NVectorRange                 FindAll(const NString& theString,
												const NString& searchFor,
												const NRange&  searchRange,
												NStringFlags   theFlags);


	// Find the first instance of a capturing pattern
	//
	// kNStringPattern searches return the pattern group, with both the
	// range of the overall pattern and the ranges of any capture groups.
	//
	// Supports kNStringNoCase, kNStringPattern, and kNStringMultiLine.
	static NPatternGroup                FindGroup(const NString& theString,
												  const NString& searchFor,
												  const NRange&  searchRange,
												  NStringFlags   theFlags);


	// Find every instance of a capturing pattern
	//
	// kNStringPattern searches return the pattern group, with both the
	// range of the overall pattern and the ranges of any capture groups.
	//
	// Supports kNStringNoCase, kNStringPattern, and kNStringMultiLine.
	static NVectorPatternGroup          FindAllGroups(const NString& theString,
													  const NString& searchFor,
													  const NRange&  searchRange,
													  NStringFlags   theFlags);



private:
	static NVectorPatternGroup          Find(const NString& theString,
											 const NString& searchFor,
											 const NRange&  searchRange,
											 NStringFlags   theFlags,
											 size_t         maxResult);


	static NVectorPatternGroup          FindPattern(const NString& theString,
													const NString& searchFor,
													const NRange&  theRange,
													NStringFlags   theFlags,
													size_t         maxResult);

	static pcre2_real_code_8*           GetRegexp(const NString& searchFor, NStringFlags theFlags);

	static NVectorPatternGroup          BytesToCodepoints(const NVectorSize&         codePointOffsets,
														  const NVectorPatternGroup& bytePatternGroups);
	static NRange                       BytesToCodepoints(const NVectorSize& codePointOffsets,
														  size_t             codePointOffset,
														  const NRange&      byteRange);
	static NRange                       CodepointsToBytes(const NVectorSize& codePointOffsets,
														  const NRange&      codePointRange);
};



#endif // NSTRING_SCANNER_H
