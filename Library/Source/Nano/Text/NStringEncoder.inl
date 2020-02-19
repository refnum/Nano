/*	NAME:
		NStringEncoder.inl

	DESCRIPTION:
		String encoder.

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





//=============================================================================
//		NStringEncoder::GetNativeEncoding : Get a native encoding.
//-----------------------------------------------------------------------------
constexpr NStringEncoding NStringEncoder::GetNativeEncoding(NStringEncoding theEncoding)
{


	// Get the native encoding
	switch (theEncoding)
	{
		case NStringEncoding::UTF16BE:
		case NStringEncoding::UTF16LE:
			theEncoding = NStringEncoding::UTF16;
			break;

		case NStringEncoding::UTF32BE:
		case NStringEncoding::UTF32LE:
			theEncoding = NStringEncoding::UTF32;
			break;

		default:
			break;
	}

	return theEncoding;
}





//=============================================================================
//		NStringEncoder::GetCodeUnitSize : Get the size of a code unit.
//-----------------------------------------------------------------------------
constexpr size_t NStringEncoder::GetCodeUnitSize(NStringEncoding theEncoding)
{


	// Get the size
	size_t theSize = 0;

	switch (theEncoding)
	{
		case NStringEncoding::Unknown:
			theSize = 0;
			break;

		case NStringEncoding::UTF8:
		case NStringEncoding::ASCII:
		case NStringEncoding::MacRoman:
		case NStringEncoding::ISOLatin1:
		case NStringEncoding::WindowsLatin1:
			theSize = sizeof(utf8_t);
			break;

		case NStringEncoding::UTF16:
		case NStringEncoding::UTF16BE:
		case NStringEncoding::UTF16LE:
			theSize = sizeof(utf16_t);
			break;

		case NStringEncoding::UTF32:
		case NStringEncoding::UTF32BE:
		case NStringEncoding::UTF32LE:
			theSize = sizeof(utf32_t);
			break;
	}

	return theSize;
}
