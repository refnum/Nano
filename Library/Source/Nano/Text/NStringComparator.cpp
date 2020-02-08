/*	NAME:
		NStringComparator.cpp

	DESCRIPTION:
		String comparator.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NStringComparator.h"

// Nano
#include "NData.h"
#include "NStdAlgorithm.h"
#include "NTextUtils.h"
#include "NUnicodeView.h"





//=============================================================================
//		NStringComparator::Compare : Compare two strings.
//-----------------------------------------------------------------------------
NComparison NStringComparator::Compare(const NString& stringA,
									   const NString& stringB,
									   NStringFlags   theFlags)
{


	// Compare the strings
	NOptionalComparison theResult = CompareFast(stringA, stringB, theFlags);

	if (!theResult.has_value())
	{
		theResult = CompareGeneral(stringA, stringB, theFlags);
		NN_REQUIRE(theResult.has_value());
	}

	return *theResult;
}





#pragma mark private
//=============================================================================
//		NStringComparator::CompareFast : Perform a fast comparison.
//-----------------------------------------------------------------------------
NOptionalComparison NStringComparator::CompareFast(const NString& stringA,
												   const NString& stringB,
												   NStringFlags   theFlags)
{


	// Fast comparison
	//
	// A case-sensitive lexicographic comparison can be handled with a
	// byte-wise comaprison if the two strings share a common encoding.
	//
	// This ignores normalisation and many other aspects of collation.
	if (theFlags == kNStringNone)
	{
		// Check for a matching encoding
		//
		// If the strings use the same primary encoding we can compare
		// their content directly.
		NStringEncoding encodingA = NStringEncoding::Unknown;
		NStringEncoding encodingB = NStringEncoding::Unknown;

		size_t sizeA = 0;
		size_t sizeB = 0;

		const void* contentA = stringA.GetContent(&encodingA, &sizeA);
		const void* contentB = stringB.GetContent(&encodingB, &sizeB);

		if (encodingA == encodingB)
		{
			return NCompare(sizeA, contentA, sizeB, contentB);
		}



		// Check for a shared encoding
		//
		// Even if the primary encodings are different we may be able to make
		// a fast comparison via an encoding they both have in common.
		NVectorStringEncoding encodingsA = stringA.GetEncodings();
		NVectorStringEncoding encodingsB = stringB.GetEncodings();

		for (auto theEncoding : encodingsA)
		{
			if (nstd::contains(encodingsB, theEncoding))
			{
				NData dataA = stringA.GetData(theEncoding);
				NData dataB = stringB.GetData(theEncoding);

				return dataA.CompareOrder(dataB);
			}
		}
	}

	return std::nullopt;
}





//=============================================================================
//		NStringComparator::CompareGeneral : Perform a general comparison.
//-----------------------------------------------------------------------------
NOptionalComparison NStringComparator::CompareGeneral(const NString& stringA,
													  const NString& stringB,
													  NStringFlags   theFlags)
{


	// Get the state we need
	NUnicodeView viewA(stringA);
	NUnicodeView viewB(stringB);

	NUTF32Iterator iterA = viewA.begin();
	NUTF32Iterator iterB = viewB.begin();

	size_t sizeA = stringA.GetSize();
	size_t sizeB = stringB.GetSize();

	size_t      compareSize = std::min(sizeA, sizeB);
	NComparison theResult   = NCompare(sizeA, sizeB);



	// General comparison
	//
	// This ignores normalisation and many other aspects of collation.
	for (size_t n = 0; n < compareSize; n++)
	{
		// Get the state we need
		utf32_t charA = *iterA;
		utf32_t charB = *iterB;

		if (theFlags & kNStringNoCase)
		{
			charA = NTextUtils::ToLower(charA);
			charB = NTextUtils::ToLower(charB);
		}



		// Compare lexicographically
		if (sizeA != sizeB)
		{
			theResult = NCompare(charA, charB);
			break;
		}



		// Advance to the next character
		++iterA;
		++iterB;
	}

	return theResult;
}
