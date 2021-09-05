/*	NAME:
		NVersion.cpp

	DESCRIPTION:
		Version number.

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
#include "NVersion.h"

// Nano
#include "NFormat.h"
#include "NNumber.h"
#include "NStdAlgorithm.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
enum class NVersionPart
{
	Period,
	Number,
	String
};





//=============================================================================
//		NVersion::NVersion : Constructor.
//-----------------------------------------------------------------------------
NVersion::NVersion(const NString& theVersion)
	: mProduct(0)
	, mMajor(0)
	, mMinor(0)
	, mPatch(0)
	, mTag{}
{


	// Set the version
	SetString(theVersion);
}





//=============================================================================
//		NVersion::NVersion : Constructor.
//-----------------------------------------------------------------------------
NVersion::NVersion(uint8_t        theProduct,
				   uint8_t        verMajor,
				   uint8_t        verMinor,
				   uint8_t        verPatch,
				   const NString& theTag)
	: mProduct(theProduct)
	, mMajor(verMajor)
	, mMinor(verMinor)
	, mPatch(verPatch)
	, mTag{}
{


	// Set the tag
	SetTag(theTag);
}





//=============================================================================
//		NVersion::GetString : Get the version as a string.
//-----------------------------------------------------------------------------
NString NVersion::GetString() const
{


	// Get the version
	NString theResult;

	if (mPatch == 0)
	{
		theResult = NFormat("{}.{}{}", mMajor, mMinor, mTag.data());
	}
	else
	{
		theResult = NFormat("{}.{}.{}{}", mMajor, mMinor, mPatch, mTag.data());
	}

	return theResult;
}





//=============================================================================
//		NVersion::SetString : Set the version.
//-----------------------------------------------------------------------------
void NVersion::SetString(const NString& theVersion)
{


	// Get the state we need
	//
	// A decoded version should look like:
	//
	//	{major, period, minor}
	//	{major, period, minor, tag}
	//	{major, period, minor, period, patch}
	//	{major, period, minor, period, patch, tag}
	//
	NVectorString theParts = GetParts(theVersion);
	NN_EXPECT(theParts.size() >= 3);

	std::vector<NVersionPart> partTypes;
	for (const auto& thePart : theParts)
	{
		partTypes.emplace_back(GetPartType(thePart));
	}



	// Merge the tag
	//
	// A tag can contain text and numbers so may have been split into
	// multiple parts.
	//
	// We want to process the tag as a single component so we merge
	// anything following a text part onto the text part.
	for (size_t n = 0; n < theParts.size(); n++)
	{
		if (partTypes[n] == NVersionPart::String)
		{
			for (size_t m = n + 1; m < theParts.size(); m++)
			{
				theParts[n] += theParts[m];
			}

			theParts.resize(n + 1);
			partTypes.resize(n + 1);
			break;
		}
	}

	NN_REQUIRE(theParts.size() == partTypes.size());
	NN_EXPECT(theParts.size() >= 3 && theParts.size() <= 6);



	// Decode the version
	Clear();
	bool canDecode = false;

	switch (theParts.size())
	{
		case 3:
			// major.minor
			canDecode =
				(partTypes[0] == NVersionPart::Number && partTypes[1] == NVersionPart::Period &&
				 partTypes[2] == NVersionPart::Number);

			if (canDecode)
			{
				mMajor = NNumber(theParts[0]).GetUInt8();
				mMinor = NNumber(theParts[2]).GetUInt8();
			}
			break;

		case 4:
			// major.minor<tag>
			canDecode =
				(partTypes[0] == NVersionPart::Number && partTypes[1] == NVersionPart::Period &&
				 partTypes[2] == NVersionPart::Number && partTypes[3] == NVersionPart::String);

			if (canDecode)
			{
				mMajor = NNumber(theParts[0]).GetUInt8();
				mMinor = NNumber(theParts[2]).GetUInt8();
				SetTag(theParts[3]);
			}
			break;

		case 5:
			// major.minor.patch
			canDecode =
				(partTypes[0] == NVersionPart::Number && partTypes[1] == NVersionPart::Period &&
				 partTypes[2] == NVersionPart::Number && partTypes[3] == NVersionPart::Period &&
				 partTypes[4] == NVersionPart::Number);

			if (canDecode)
			{
				mMajor = NNumber(theParts[0]).GetUInt8();
				mMinor = NNumber(theParts[2]).GetUInt8();
				mPatch = NNumber(theParts[4]).GetUInt8();
			}
			break;

		case 6:
			// major.minor.patch<tag>
			canDecode =
				(partTypes[0] == NVersionPart::Number && partTypes[1] == NVersionPart::Period &&
				 partTypes[2] == NVersionPart::Number && partTypes[3] == NVersionPart::Period &&
				 partTypes[4] == NVersionPart::Number && partTypes[5] == NVersionPart::String);

			if (canDecode)
			{
				mMajor = NNumber(theParts[0]).GetUInt8();
				mMinor = NNumber(theParts[2]).GetUInt8();
				mPatch = NNumber(theParts[4]).GetUInt8();
				SetTag(theParts[5]);
			}
			break;

		default:
			// Unknown
			canDecode = false;
			break;
	}


	// Report failure
	if (!canDecode)
	{
		NN_LOG_WARNING("Unable to parse '{}'", theVersion);
	}
}





//=============================================================================
//		NVersion::Compare : Compare two version strings.
//-----------------------------------------------------------------------------
//		Note :	Algorithm is based on Ricardo Batista's MacPad.
//
//				Version numbers are broken down into an array of parts, where
//				each part is an integer/string/period.
//
//				Each part is compared in turn, and their type is used to drive
//				the overall comparison.
//
//				Parts are compared alphabatically or numerically based on the
//				type, and versions with a different number of parts examine
//				the first "extra" part to identify the oldest version.
//----------------------------------------------------------------------------
NComparison NVersion::Compare(const NString& versionA, const NString& versionB)
{


	// Validate our parameters
	NN_REQUIRE(!versionA.IsEmpty());
	NN_REQUIRE(!versionB.IsEmpty());



	// Quick check
	if (versionA == versionB)
	{
		return NComparison::EqualTo;
	}



	// Get the state we need
	NVectorString partsA = GetParts(versionA);
	NVectorString partsB = GetParts(versionB);

	size_t numParts = std::min(partsA.size(), partsB.size());
	size_t n        = 0;



	// Compare the parts
	for (n = 0; n < numParts; n++)
	{
		// Get the state we need
		const NString& partA = partsA[n];
		const NString& partB = partsB[n];

		NVersionPart typeA = GetPartType(partA);
		NVersionPart typeB = GetPartType(partB);



		// Compare by value
		//
		// If we have a string or a number part, we can compare values.
		if (typeA == typeB)
		{
			if (typeA != NVersionPart::Period)
			{
				NComparison theResult = partA.Compare(partB, NStringFind::Numeric);
				if (theResult != NComparison::EqualTo)
				{
					return theResult;
				}
			}
		}


		// Or by precedence
		else
		{
			// Part A wins if it's a digit/period
			if (typeA != NVersionPart::String && typeB == NVersionPart::String)
			{
				return NComparison::GreaterThan;
			}

			// Part B wins if it's a digit/period
			else if (typeA == NVersionPart::String && typeB != NVersionPart::String)
			{
				return NComparison::LessThan;
			}

			// Whichever part is numeric wins
			else
			{
				if (typeA == NVersionPart::Number)
				{
					return NComparison::GreaterThan;
				}
				else
				{
					return NComparison::LessThan;
				}
			}
		}
	}



	// Examine the largest string
	if (partsA.size() != partsB.size())
	{
		// Get its next part
		NComparison shorterResult, longerResult;
		NString     nextPart;

		if (partsA.size() > partsB.size())
		{
			nextPart      = partsA[n];
			shorterResult = NComparison::LessThan;
			longerResult  = NComparison::GreaterThan;
		}
		else
		{
			nextPart      = partsB[n];
			shorterResult = NComparison::GreaterThan;
			longerResult  = NComparison::LessThan;
		}


		// Compare by type
		if (GetPartType(nextPart) == NVersionPart::String)
		{
			return shorterResult;
		}
		else
		{
			return longerResult;
		}
	}



	// If we're still here, the versions are equal
	return NComparison::EqualTo;
}





#pragma mark NMixinComparable
//=============================================================================
//		NVersion::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NVersion::CompareEqual(const NVersion& theVersion) const
{


	// Compare the value
	return (mProduct == theVersion.mProduct) && (mMajor == theVersion.mMajor) &&
		   (mMinor == theVersion.mMinor) && (mPatch == theVersion.mPatch) &&
		   (mTag == theVersion.mTag);
}





//=============================================================================
//		NVersion::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NVersion::CompareOrder(const NVersion& theVersion) const
{


	// Order by component
	NComparison theResult = NCompare(mProduct, theVersion.mProduct);

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mMajor, theVersion.mMajor);
	}

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mMinor, theVersion.mMinor);
	}

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mPatch, theVersion.mPatch);
	}

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mTag, theVersion.mTag);
	}

	return theResult;
}





#pragma mark private
//=============================================================================
//		NVersion::GetParts : Get a version's parts.
//-----------------------------------------------------------------------------
NVectorString NVersion::GetParts(const NString& theVersion)
{


	// Validate our parameters
	NN_REQUIRE(!theVersion.IsEmpty());



	// Get the state we need
	NVectorString theParts;
	NString       thePart;

	size_t       numChars = theVersion.GetSize();
	NVersionPart lastType = GetPartType(theVersion);


	// Split the version into parts
	for (size_t n = 0; n < numChars; n++)
	{
		NString      theChar  = theVersion.GetSubstring(NRange(n, 1));
		NVersionPart partType = GetPartType(theChar);

		if (partType != lastType || partType == NVersionPart::Period)
		{
			NN_REQUIRE(!thePart.IsEmpty());
			theParts.emplace_back(thePart);
			thePart.Clear();
		}

		thePart += theChar;
		lastType = partType;
	}

	NN_REQUIRE(!thePart.IsEmpty());
	theParts.emplace_back(thePart);

	return theParts;
}





//=============================================================================
//		NVersion::GetPartType : Get the type of a part.
//-----------------------------------------------------------------------------
NVersionPart NVersion::GetPartType(const NString& thePart)
{


	// Validate our parameters
	NN_REQUIRE(!thePart.IsEmpty());



	// Classify the part
	const char* charPtr = thePart.GetUTF8();

	if (charPtr[0] == '.')
	{
		return NVersionPart::Period;
	}
	else if (isdigit(charPtr[0]))
	{
		return NVersionPart::Number;
	}

	return NVersionPart::String;
}
