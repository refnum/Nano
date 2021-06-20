/*	NAME:
		NURL.inl

	DESCRIPTION:
		Uniform Resource Locator.

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
//		NURL::NURL : Constructor.
//-----------------------------------------------------------------------------
inline NURL::NURL(const utf8_t* theURL)
	: mURL(theURL)
{
}





//=============================================================================
//		NURL::NURL : Constructor.
//-----------------------------------------------------------------------------
inline NURL::NURL(const NString& theURL)
	: mURL(theURL)
{
}





//=============================================================================
//		NURL::IsValid : Is the UTI valid?
//-----------------------------------------------------------------------------
inline bool NURL::IsValid() const
{


	// Check our state
	return !mURL.IsEmpty();
}





//=============================================================================
//		NURL::Clear : Clear the URL.
//-----------------------------------------------------------------------------
inline void NURL::Clear()
{


	// Reset our state
	mURL.Clear();
}





//=============================================================================
//		NURL::GetURL : Get the URL.
//-----------------------------------------------------------------------------
inline NString NURL::GetURL() const
{


	// Get the URL
	return mURL;
}





//=============================================================================
//		NURL::SetURL : Set the URL.
//-----------------------------------------------------------------------------
inline void NURL::SetURL(const NString& theURL)
{


	// Set the URL
	mURL = theURL;
}





//=============================================================================
//		NURL::GetScheme : Get the scheme.
//-----------------------------------------------------------------------------
inline NString NURL::GetScheme() const
{


	// Get the scheme
	return GetToken("^(\\w+)://");
}





//=============================================================================
//		NURL::GetHost : Get the host.
//-----------------------------------------------------------------------------
inline NString NURL::GetHost() const
{


	// Get the scheme
	return GetToken("^\\w+://(.*?)[:/]");
}





//=============================================================================
//		NURL::GetPath : Get the path.
//-----------------------------------------------------------------------------
inline NString NURL::GetPath() const
{


	// Get the scheme
	return GetToken("\\w+://.*?(/.*)");
}





#pragma mark NMixinComparable
//=============================================================================
//		NURL::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
inline bool NURL::CompareEqual(const NURL& theURL) const
{


	// Compare the URL
	return mURL.CompareEqual(theURL.mURL);
}





//=============================================================================
//		NURL::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
inline NComparison NURL::CompareOrder(const NURL& theURL) const
{


	// Order by URL
	return mURL.CompareOrder(theURL.mURL);
}
