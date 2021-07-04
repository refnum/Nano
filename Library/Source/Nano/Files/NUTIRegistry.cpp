/*	NAME:
		NUTIRegistry.cpp

	DESCRIPTION:
		UTI Registry.

		For now we implement a simple hard-coded list of UTI types; this should
		be expanded over time, and ideally mapped to the native target equivalent
		(LaunchServices on Mac, Registry on Windows).

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
#include "NUTIRegistry.h"

// Nano
#include "NStdAlgorithm.h"





//=============================================================================
//		NUTIRegistry::NUTIRegistry : Constructor.
//-----------------------------------------------------------------------------
NUTIRegistry::NUTIRegistry()
{


	// Initialise ourselves
	Add(kNUTTypeItem, "", {}, {});
	Add(kNUTTypeContent, "", {}, {});
	Add(kNUTTypeDiskImage, "", {}, {});



	Add(kNUTTypeData, "", {kNUTTypeItem}, {});

	Add(kNUTTypeDirectory, "", {kNUTTypeItem}, {});



	Add(kNUTTypeText, "", {kNUTTypeData, kNUTTypeContent}, {});

	Add(kNUTTypeDelimitedText, "", {kNUTTypeText}, {});

	Add(kNUTTypeCSV,
		"CSV",
		{kNUTTypeDelimitedText},
		{{NUTITag::MIMEType, "text/csv"}, {NUTITag::Extension, "csv"}});



	Add(kNUTTypeXML, "XML", {kNUTTypeText}, {});

	Add(kNUTTypeGPX,
		"GPX",
		{kNUTTypeXML},
		{{NUTITag::MIMEType, "application/gpx+xml"}, {NUTITag::Extension, "gpx"}});

	Add(kNUTTypeKML,
		"Google KML",
		{kNUTTypeXML},
		{{NUTITag::MIMEType, "application/vnd.google-earth.kml+xml"}, {NUTITag::Extension, "kml"}});



	Add(kNUTTypeImage, "Image", {kNUTTypeData, kNUTTypeContent}, {});

	Add(kNUTTypeGIF,
		"GIF Image",
		{kNUTTypeImage},
		{{NUTITag::MIMEType, "image/gif"}, {NUTITag::Extension, "gif"}});

	Add(kNUTTypeJPEG,
		"JPEG Image",
		{kNUTTypeImage},
		{{NUTITag::MIMEType, "image/jpeg"},
		 {NUTITag::Extension, "jpg"},
		 {NUTITag::Extension, "jpeg"}});

	Add(kNUTTypePNG,
		"PNG Image",
		{kNUTTypeImage},
		{{NUTITag::MIMEType, "image/png"}, {NUTITag::Extension, "png"}});

	Add(kNUTTypeSVG,
		"SVG Image",
		{kNUTTypeImage},
		{{NUTITag::MIMEType, "image/svg+xml"}, {NUTITag::Extension, "svg"}});

	Add(kNUTTypeTIFF,
		"TIFF Image",
		{kNUTTypeImage},
		{{NUTITag::MIMEType, "image/tiff"}, {NUTITag::Extension, "tiff"}});



	Add(kNUTTypeDMG,
		"Disk Image",
		{kNUTTypeDiskImage},
		{{NUTITag::MIMEType, "application/x-apple-diskimage"}, {NUTITag::Extension, "dmg"}});

	Add(kNUTTypeISO,
		"ISO Disk Image",
		{kNUTTypeDiskImage},
		{{NUTITag::MIMEType, "application/x-iso-image"}, {NUTITag::Extension, "iso"}});



	Add(kNUTTypeKMZ,
		"Google KMZ",
		{kNUTTypeData},
		{{NUTITag::MIMEType, "application/vnd.google-earth.kmz"}, {NUTITag::Extension, "kmz"}});
}





//=============================================================================
//		NUTIRegistry::HasUTI : Does the registry contain a UTI?
//-----------------------------------------------------------------------------
bool NUTIRegistry::HasUTI(const NString& theUTI) const
{


	// Validate our parameters
	NN_REQUIRE(!theUTI.IsEmpty());



	// Check the registry
	return nstd::contains(mInfo, theUTI);
}





//=============================================================================
//		NUTIRegistry::FindUTI : Find a UTI.
//-----------------------------------------------------------------------------
NString NUTIRegistry::FindUTI(NUTITag theTag, const NString& theValue) const
{


	// Validate our parameters
	NN_REQUIRE(!theValue.IsEmpty());
	NN_REQUIRE(theValue == theValue.GetLower(), "Tags are lower-case!");



	// Find the UTI
	NString theUTI;

	for (const auto& utiInfo : mInfo)
	{
		NString foundValue = nstd::fetch(utiInfo.second.theTags, theTag, "");
		if (foundValue == theValue)
		{
			theUTI = utiInfo.first;
			break;
		}
	}

	return theUTI;
}





//=============================================================================
//		NUTIRegistry::GetDescription : Get a UTI's description.
//-----------------------------------------------------------------------------
NString NUTIRegistry::GetDescription(const NString& theUTI) const
{


	// Validate our parameters
	NN_REQUIRE(HasUTI(theUTI));



	// Get the description
	return mInfo[theUTI].theDescription;
}





//=============================================================================
//		NUTIRegistry::GetConformance : Get a UTI's conformance.
//-----------------------------------------------------------------------------
NVectorString NUTIRegistry::GetConformance(const NString& theUTI) const
{


	// Validate our parameters
	NN_REQUIRE(HasUTI(theUTI));



	// Get the conformance
	return mInfo[theUTI].conformsTo;
}





//=============================================================================
//		NUTIRegistry::GetTags : Get a UTI's tags.
//-----------------------------------------------------------------------------
NMapUTITag NUTIRegistry::GetTags(const NString& theUTI) const
{


	// Validate our parameters
	NN_REQUIRE(HasUTI(theUTI));



	// Get the tags
	return mInfo[theUTI].theTags;
}





//=============================================================================
//		NUTIRegistry::GetTag : Get a UTI's tag.
//-----------------------------------------------------------------------------
NString NUTIRegistry::GetTag(const NString& theUTI, NUTITag theTag) const
{


	// Validate our parameters
	NN_REQUIRE(HasUTI(theUTI));



	// Get the tag
	return nstd::fetch(mInfo[theUTI].theTags, theTag, "");
}





//=============================================================================
//		NUTIRegistry::ConformsTo : Does a UTI conform to a parent UTI?
//-----------------------------------------------------------------------------
bool NUTIRegistry::ConformsTo(const NString& theUTI, const NString& parentUTI) const
{


	// Validate our parameters
	NN_REQUIRE(HasUTI(theUTI));
	NN_REQUIRE(HasUTI(parentUTI));



	// Check for conformance
	//
	// We check for direct equivalence and then our immediate parents.
	//
	// If our parents don't match then we check them recursively, to see
	// if they conform in turn.
	bool conformsTo = (theUTI == parentUTI);

	if (!conformsTo)
	{
		conformsTo = nstd::contains(mInfo[theUTI].conformsTo, parentUTI);

		if (!conformsTo)
		{
			for (const auto& checkUTI : mInfo[theUTI].conformsTo)
			{
				conformsTo = ConformsTo(checkUTI, parentUTI);
				if (conformsTo)
				{
					break;
				}
			}
		}
	}

	return conformsTo;
}





//=============================================================================
//		NUTIRegistry::Add : Add a UTI.
//-----------------------------------------------------------------------------
void NUTIRegistry::Add(const NString&       theUTI,
					   const NString&       theDescription,
					   const NVectorString& conformsTo,
					   const NMapUTITag&    theTags)
{


	// Validate our parameters
	NN_REQUIRE(!HasUTI(theUTI));

	if constexpr (NN_ENABLE_ASSERTIONS)
	{
		for (const auto& theTag : theTags)
		{
			NN_REQUIRE(theTag.second == theTag.second.GetLower(), "Tags are lower-case!");
		}
	}



	// Add the UTI
	auto& theInfo = mInfo[theUTI];

	theInfo.theDescription = theDescription;
	theInfo.conformsTo     = conformsTo;
	theInfo.theTags        = theTags;
}





//=============================================================================
//		NUTIRegistry::Get : Get the registry.
//-----------------------------------------------------------------------------
NUTIRegistry& NUTIRegistry::Get()
{


	// Get the registry
	static NUTIRegistry sRegistry;

	return sRegistry;
}
