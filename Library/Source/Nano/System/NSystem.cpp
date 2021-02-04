/*	NAME:
		NSystem.cpp

	DESCRIPTION:
		System support.

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
#include "NSystem.h"

// Nano
#include "NFormat.h"





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
//		NSystem::GetEnv : Get an environment variable.
//-----------------------------------------------------------------------------
NString NSystem::GetEnv(const NString& theName)
{
	// Get the environment variable
	NString     theValue;
	const char* envVar                                      = getenv(theName.GetUTF8());

	if (envVar != nullptr)
	{
		theValue                                            = NString(envVar);
	}

	return theValue;
}





//=============================================================================
//		NSystem::GetName : Get the OS name.
//-----------------------------------------------------------------------------
NString NSystem::GetName(NOSName theName)
{


	// Get the name
	NString theText;

	switch (theName)
	{
		case NOSName::Platform:
			if (NN_TARGET_ANDROID)
			{
				theText = "Android";
			}
			else if (NN_TARGET_IOS)
			{
				theText = "iOS";
			}
			else if (NN_TARGET_LINUX)
			{
				theText = "Linux";
			}
			else if (NN_TARGET_MACOS)
			{
				theText = "macOS";
			}
			else if (NN_TARGET_TVOS)
			{
				theText = "tvOS";
			}
			else if (NN_TARGET_WINDOWS)
			{
				theText = "Windows";
			}
			break;


		case NOSName::Version:
		{
			NOSVersion theVersion = GetVersion();

			theText = NFormat("{}.{}.{}",
							  uint8_t(theVersion >> 16),
							  uint8_t(theVersion >> 8),
							  uint8_t(theVersion >> 0));

			if (theText.EndsWith(".0"))
			{
				theText.RemoveSuffix(2);
			}
		}
		break;


		case NOSName::Short:
			theText = NFormat("{} {}", GetName(NOSName::Platform), GetName(NOSName::Version));
			break;


		case NOSName::Detailed:
			NN_DIAGNOSTIC_IGNORE_MSVC(4840);
			theText = NFormat("{} {} ({})",
							  GetName(NOSName::Platform),
							  GetName(NOSName::Version),
							  GetName(NOSName::Build));
			break;


		case NOSName::Build:
		case NOSName::Maximum:
			theText = SystemName(theName);
			break;
	}

	return theText;
}





//=============================================================================
//		NSystem::CompareVersions : Compare two version strings.
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
NComparison NSystem::CompareVersions(const NString& versionA, const NString& versionB)
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
	NVectorString partsA = GetVersionParts(versionA);
	NVectorString partsB = GetVersionParts(versionB);

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
				NComparison theResult = partA.Compare(partB, kNStringNumeric);
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





#pragma mark private
//=============================================================================
//		NSystem::GetVersionParts : Get a version's parts.
//-----------------------------------------------------------------------------
NVectorString NSystem::GetVersionParts(const NString& theVersion)
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
//		NSystem::GetPartType : Get the type of a part.
//-----------------------------------------------------------------------------
NVersionPart NSystem::GetPartType(const NString& thePart)
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
