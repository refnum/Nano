/*	NAME:
		NFilePath.h

	DESCRIPTION:
		File path.

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
#ifndef NFILE_PATH_H
#define NFILE_PATH_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinAppendable.h"
#include "NMixinComparable.h"
#include "NMixinHashable.h"
#include "NString.h"
#include "NanoTargets.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Directory separator
#if NN_TARGET_WINDOWS
static constexpr const char* kNPathSeparator                = "\\";
#else
static constexpr const char* kNPathSeparator                = "/";
#endif





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NFilePath final
	: public NMixinAppendable<NFilePath>
	, public NMixinComparable<NFilePath>
	, public NMixinHashable<NFilePath>
{
public:
										NFilePath() = default;
										NFilePath(const NString& thePath);


	// Is the path valid?
	bool                                IsValid() const;


	// Clear the path
	void                                Clear();


	// Get/set the path
	NString                             GetPath() const;
	void                                SetPath(  const NString& thePath);


	// Get the parent
	//
	// Get the parent from the path, typically all but the last part.
	NString                             GetParent() const;


	// Get the name
	//
	// Get the name from the path, typically the last part.
	NString                             GetName(bool withExtension = true) const;


	// Get the extension
	//
	// Get the extension of the name, if any.
	NString                             GetExtension() const;


public:
	// NMixinAppendable
	void                                Append(const NFilePath& thePath);


	// NMixinComparable
	bool                                CompareEqual(const NFilePath& thePath) const;
	NComparison                         CompareOrder(const NFilePath& thePath) const;


	// NMixinHashable
	size_t                              HashGet() const;
	size_t                              HashUpdate();
	void                                HashClear();


private:
	NString                             GetPathPart(const NString& thePath, const NString& thePattern) const;


private:
	NString                             mPath;
};



#endif // NFILE_PATH_H
