/*	NAME:
		NUTI.cpp

	DESCRIPTION:
		Uniform Type Identifier:

			https://en.wikipedia.org/wiki/Uniform_Type_Identifier.

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
#include "NUTI.h"

// Nano
#include "NUTIRegistry.h"





//=============================================================================
//		NUTI::NUTI : Constructor.
//-----------------------------------------------------------------------------
NUTI::NUTI(const char* typeUTF8)
	: mType(typeUTF8)
{


	// Validate our parameters
	NN_REQUIRE(mType == mType.GetLower(), "Types are lower-case!");
}





//=============================================================================
//		NUTI::NUTI : Constructor.
//-----------------------------------------------------------------------------
NUTI::NUTI(const NString& theType)
	: mType(theType)
{


	// Validate our parameters
	NN_REQUIRE(mType == mType.GetLower(), "Types are lower-case!");
}





//=============================================================================
//		NUTI::NUTI : Constructor.
//-----------------------------------------------------------------------------
NUTI::NUTI(NUTITag theTag, const NString& theValue)
{


	// Validate our parameters
	if (theTag == NUTITag::Extension)
	{
		NN_REQUIRE(!theValue.StartsWith("."));
	}



	// Initialize ourselves
	mType = NUTIRegistry::Get().FindUTI(theTag, theValue);
}





//=============================================================================
//		NUTI::IsValid : Is the UTI valid?
//-----------------------------------------------------------------------------
bool NUTI::IsValid() const
{


	// Check our state
	return !mType.IsEmpty();
}





//=============================================================================
//		NUTI::ConformsTo : Does the UTI conform to another?
//-----------------------------------------------------------------------------
bool NUTI::ConformsTo(const NUTI& theUTI) const
{


	// Check for conformance
	return NUTIRegistry::Get().ConformsTo(mType, theUTI.mType);
}





//=============================================================================
//		NUTI::Clear : Clear the UTI.
//-----------------------------------------------------------------------------
void NUTI::Clear()
{


	// Reset our state
	mType = kNUTTypeNone;
}





//=============================================================================
//		NUTI::GetDescription : Get the description.
//-----------------------------------------------------------------------------
NString NUTI::GetDescription() const
{


	// Get the value
	return NUTIRegistry::Get().GetDescription(mType);
}





//=============================================================================
//		NUTI::GetTag : Get a tag.
//-----------------------------------------------------------------------------
NString NUTI::GetTag(NUTITag theTag) const
{


	// Get the value
	return NUTIRegistry::Get().GetTag(mType, theTag);
}





//=============================================================================
//		NUTI::GetMIMEType : Get the MIME type.
//-----------------------------------------------------------------------------
NString NUTI::GetMIMEType() const
{


	// Get the value
	return GetTag(NUTITag::MIMEType);
}





//=============================================================================
//		NUTI::GetExtension : Get the filename extension.
//-----------------------------------------------------------------------------
NString NUTI::GetExtension() const
{


	// Get the value
	return GetTag(NUTITag::Extension);
}





//=============================================================================
//		NUTI::GetType : Get the type.
//-----------------------------------------------------------------------------
NString NUTI::GetType() const
{


	// Get the type
	return mType;
}





//=============================================================================
//		NUTI::SetType : Set the type.
//-----------------------------------------------------------------------------
void NUTI::SetType(const NString& theType)
{


	// Set the type
	mType = theType;
}





#pragma mark NMixinComparable
//=============================================================================
//		NUTI::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NUTI::CompareEqual(const NUTI& theUTI) const
{


	// Compare the type
	return mType.CompareEqual(theUTI.mType);
}





//=============================================================================
//		NUTI::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NUTI::CompareOrder(const NUTI& theUTI) const
{


	// Order by type
	return mType.CompareOrder(theUTI.mType);
}
