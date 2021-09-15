/*	NAME:
		NVersion.inl

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





//=============================================================================
//		NVersion::NVersion : Constructor.
//-----------------------------------------------------------------------------
constexpr NVersion::NVersion(uint8_t theProduct,
							 uint8_t verMajor,
							 uint8_t verMinor,
							 uint8_t verPatch)
	: mProduct(theProduct)
	, mMajor(verMajor)
	, mMinor(verMinor)
	, mPatch(verPatch)
	, mTag{}
{
}





//=============================================================================
//		NVersion::NVersion : Constructor.
//-----------------------------------------------------------------------------
constexpr NVersion::NVersion()
	: mProduct(0)
	, mMajor(0)
	, mMinor(0)
	, mPatch(0)
	, mTag{}
{


	// Validate our state
	static_assert(sizeof(NVersion) == 16, "Ensure multiple of 16 for vector initialisation");
}





//=============================================================================
//		NVersion::IsValid : Is the version valid?
//-----------------------------------------------------------------------------
constexpr bool NVersion::IsValid() const
{


	// Check the state
	return mProduct != 0 || mMajor != 0 || mMinor != 0 || mPatch != 0 || mTag[0] != 0x00;
}





//=============================================================================
//		NVersion::Clear : Clear the version.
//-----------------------------------------------------------------------------
inline void NVersion::Clear()
{


	// Clear the version
	mProduct = 0;
	mMajor   = 0;
	mMinor   = 0;
	mPatch   = 0;
	SetTag("");
}





//=============================================================================
//		NVersion::GetProduct : Get the product part.
//-----------------------------------------------------------------------------
constexpr uint8_t NVersion::GetProduct() const
{


	// Get the value
	return mProduct;
}





//=============================================================================
//		NVersion::SetProduct : Set the product part.
//-----------------------------------------------------------------------------
constexpr void NVersion::SetProduct(uint8_t theValue)
{


	// Set the value
	mProduct = theValue;
}





//=============================================================================
//		NVersion::GetMajor : Get the major components.
//-----------------------------------------------------------------------------
constexpr uint8_t NVersion::GetMajor() const
{


	// Get the value
	return mMajor;
}





//=============================================================================
//		NVersion::GetMinor : Get the minor components.
//-----------------------------------------------------------------------------
constexpr uint8_t NVersion::GetMinor() const
{


	// Get the value
	return mMinor;
}





//=============================================================================
//		NVersion::GetPatch : Get the patch components.
//-----------------------------------------------------------------------------
constexpr uint8_t NVersion::GetPatch() const
{


	// Get the value
	return mPatch;
}





//=============================================================================
//		NVersion::SetMajor : Set the major components.
//-----------------------------------------------------------------------------
constexpr void NVersion::SetMajor(uint8_t theValue)
{


	// Set the value
	mMajor = theValue;
}





//=============================================================================
//		NVersion::SetMinor : Set the minor components.
//-----------------------------------------------------------------------------
constexpr void NVersion::SetMinor(uint8_t theValue)
{


	// Set the value
	mMinor = theValue;
}





//=============================================================================
//		NVersion::SetPatch : Set the patch components.
//-----------------------------------------------------------------------------
constexpr void NVersion::SetPatch(uint8_t theValue)
{


	// Set the value
	mPatch = theValue;
}





//=============================================================================
//		NVersion::GetTag : Get the tag.
//-----------------------------------------------------------------------------
inline NString NVersion::GetTag() const
{


	// Get the value
	return mTag.data();
}





//=============================================================================
//		NVersion::SetTag : Set the tag.
//-----------------------------------------------------------------------------
inline void NVersion::SetTag(const NString& theValue)
{


	// Set the value
	const char* textUTF8 = theValue.GetUTF8();
	size_t      textLen  = std::min(strlen(textUTF8), mTag.size() - 1);

	memcpy(mTag.data(), textUTF8, textLen);

	if (NN_ENABLE_LOGGING && strlen(textUTF8) > textLen)
	{
		NN_LOG_WARNING("NVersion::SetTag truncated '{}' to '{}'", theValue, mTag.data());
	}
}
