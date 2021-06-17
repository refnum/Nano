/*	NAME:
		NUTIRegistry.h

	DESCRIPTION:
		UTI Registry.

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
#ifndef NUTIREGISTRY_H
#define NUTIREGISTRY_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NUTI.h"

// System
#include <unordered_map>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// UTI tags
using NMapUTITag                                            = std::unordered_map<NUTITag, NString>;


// UTI info
struct NUTIInfo
{
	NString       theDescription;
	NVectorString conformsTo;
	NMapUTITag    theTags;
};

using NMapUTIInfo = std::unordered_map<NString, NUTIInfo>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NUTIRegistry
{
public:
										NUTIRegistry();


	// Does the registry contain a UTI?
	bool                                HasUTI(const NString& theUTI) const;


	// Find a UTI for a tag
	//
	// Returns an empty string if no UTI can be found.
	NString                             FindUTI(NUTITag theTag, const NString& theValue) const;


	// Get the state of a UTI
	//
	// The UTI must exist in the registry.
	NString                             GetDescription(const NString& theUTI) const;
	NVectorString                       GetConformance(const NString& theUTI) const;
	NMapUTITag                          GetTags(       const NString& theUTI) const;
	NString                             GetTag(        const NString& theUTI, NUTITag theTag) const;


	// Does a UTI conform to a parent UTI?
	//
	// The UTIs must exist in the registry.
	bool                                ConformsTo(const NString& theUTI, const NString& parentUTI) const;


	// Add a UTI
	//
	// The UTIs must not exist in the registry.
	void                                Add(const NString&       theUTI,
											const NString&       theDescription,
											const NVectorString& conformsTo,
											const NMapUTITag&    theTags);


	// Get the registry
	static NUTIRegistry&                Get();


private:
	mutable NMapUTIInfo                 mInfo;
};



#endif // NUTIREGISTRY_H
