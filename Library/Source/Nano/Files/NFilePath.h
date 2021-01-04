/*	NAME:
		NFilePath.h

	DESCRIPTION:
		File path.

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
#ifndef NFILE_PATH_H
#define NFILE_PATH_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinAppendable.h"
#include "NMixinComparable.h"
#include "NMixinHashable.h"
#include "NString.h"
#include "NanoTargets.h"

// System
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NFilePath;


// Containers
using NVectorFilePath = std::vector<NFilePath>;





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


	// Is the path an absolute / relative path?
	//
	// An absolute path starts with a root part.
	bool                                IsAbsolute() const;
	bool                                IsRelative() const;


	// Is the path a root path?
	//
	// A root path is the initial component of an absolute path?
	bool                                IsRoot() const;


	// Clear the path
	void                                Clear();


	// Get the root
	//
	// The root is the first part of an absolute path:
	//
	//		/path/to/file.dat		C:\path\to\file.dat
	//      ^                       ^^^
	//
	// The root of a relative path is an empty path.
	NFilePath                           GetRoot() const;


	// Get the parent
	//
	// The parent is all but the last part of the path:
	//
	//		/path/to/file.dat		C:\path\to\file.dat
	//      ^^^^^^^^                ^^^^^^^^^^
	//
	// The final parent of an absolute path is the root path itself.
	//
	// The final parent of a relative path is an empty path.
	NFilePath                           GetParent() const;


	// Get a child of the path
	NFilePath                           GetChild(const NString& theName) const;


	// Get/set the filename
	//
	// The filename is the final part of the path:
	//
	//		/path/to/file.dat		C:\path\to\file.dat
	//               ^^^^^^^^                  ^^^^^^^^
	//
	// The filename includes both stem and extension.
	NString                             GetFilename() const;
	void                                SetFilename(  const NString& theName);


	// Get/set the stem
	//
	// The stem comes before the last period in the final part:
	//
	//		/path/to/file.dat		C:\path\to\file.dat
	//               ^^^^                      ^^^^
	//
	// The stem does not include the period.
	NString                             GetStem() const;
	void                                SetStem(  const NString& theStem);


	// Get/set the extension
	//
	// The extension comes after the last period in the final part:
	//
	//		/path/to/file.dat		C:\path\to\file.dat
	//                    ^^^                       ^^^
	//
	// The extension does not include the period.
	//
	// Assigning an empty extension also removes the last period.
	NString                             GetExtension() const;
	void                                SetExtension(  const NString& theExtension);


	// Get/set the parts
	//
	// An absolute path has the root as its first part.
	NVectorString                       GetParts() const;
	void                                SetParts(  const NVectorString& theParts);


	// Get/set the path
	//
	// Access the path as a single string.
	NString                             GetPath() const;
	void                                SetPath(  const NString& thePath);


	// Get the path as a null-terminated string
	const utf8_t*                       GetUTF8()  const;
	const utf16_t*                      GetUTF16() const;


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
	NString                             GetPart(const NString& thePattern) const;
	bool                                SetPart(const NString& thePattern, const NString& theValue);


private:
	NString                             mPath;
};



#endif // NFILE_PATH_H
