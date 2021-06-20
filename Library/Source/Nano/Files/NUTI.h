/*	NAME:
		NUTI.h

	DESCRIPTION:
		Uniform Type Identifier.

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
#ifndef NUTI_H
#define NUTI_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.h"
#include "NString.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Tag classes
enum class NUTITag
{
	None,
	MIMEType,
	Extension
};


// UTIs
//
// Abstract base types
inline constexpr const utf8_t* kNUTTypeNone                 = "";
inline constexpr const utf8_t* kNUTTypeItem                 = "public.item";
inline constexpr const utf8_t* kNUTTypeContent              = "public.content";
inline constexpr const utf8_t* kNUTTypeDiskImage            = "public.disk-image";


// Concrete base types
inline constexpr const utf8_t* kNUTTypeData                 = "public.data";
inline constexpr const utf8_t* kNUTTypeDirectory            = "public.directory";


// Text types
inline constexpr const utf8_t* kNUTTypeText                 = "public.text";
inline constexpr const utf8_t* kNUTTypeDelimitedText        = "public.delimited-values-text";
inline constexpr const utf8_t* kNUTTypeCSV                  = "public.comma-separated-values-text";


// XML types
inline constexpr const utf8_t* kNUTTypeXML                  = "public.xml";
inline constexpr const utf8_t* kNUTTypeGPX                  = "public.gpx";
inline constexpr const utf8_t* kNUTTypeKML                  = "public.kml";


// Image types
inline constexpr const utf8_t* kNUTTypeImage                = "public.image";
inline constexpr const utf8_t* kNUTTypeGIF                  = "com.compuserve.gif";
inline constexpr const utf8_t* kNUTTypeJPEG                 = "public.jpeg";
inline constexpr const utf8_t* kNUTTypePNG                  = "public.png";
inline constexpr const utf8_t* kNUTTypeSVG                  = "public.svg-image";
inline constexpr const utf8_t* kNUTTypeTIFF                 = "public.tiff";


// Disk images
inline constexpr const utf8_t* kNUTTypeDMG                  = "com.apple.disk-image-udif";
inline constexpr const utf8_t* kNUTTypeISO                  = "public.iso-image";


// Application types
inline constexpr const utf8_t* kNUTTypeKMZ                  = "public.kmz";





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NUTI final : public NMixinComparable<NUTI>
{
public:
										NUTI(const utf8_t*  theType);
										NUTI(const NString& theType);
										NUTI(NUTITag theTag, const NString& theValue);

										NUTI() = default;


	// Is the UTI valid?
	bool                                IsValid() const;


	// Does the UTI conform to another?
	bool                                ConformsTo(const NUTI& theUTI) const;


	// Clear the value
	void                                Clear();


	// Get the description
	NString                             GetDescription() const;


	// Get a tag
	NString                             GetTag(NUTITag theTag) const;

	NString                             GetMIMEType()  const;
	NString                             GetExtension() const;


	// Get/set the type
	NString                             GetType() const;
	void                                SetType(  const NString& theType);


public:
	// NMixinComparable
	bool                                CompareEqual(const NUTI& theUTI) const;
	NComparison                         CompareOrder(const NUTI& theUTI) const;


private:
	NString                             mType;
};



#endif // NUTI_H
