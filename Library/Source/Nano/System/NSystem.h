/*	NAME:
		NSystem.h

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
#ifndef NSYSTEM_H
#define NSYSTEM_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.h"
#include "NString.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// OS name
//
// An NOSName represents a formatted name for an OS.
//
// The structure of OS names may vary per platform, and over time.
enum class NOSName
{
	Platform,    // "macOS"
	Version,     // "11.1
	Build,       // "20C69"

	Short,       // "macOS 11.1"
	Detailed,    // "macOS 11.1 (20C69)"
	Maximum,     // "macOS 11.1 (20C69) [Darwin Kernel Version...]"
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NVersion;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NSystem
{
public:
	// Get / set an environment variable
	//
	// Assigning an empty name will remove the variable.
	static NString                      GetEnv(const NString& theName);
	static void                         SetEnv(const NString& theName, const NString& theValue);


	// Get the page size
	static size_t                       GetPageSize();


	// Get the version
	static NVersion                     GetVersion();


	// Get the name
	static NString                      GetName(NOSName theName = NOSName::Short);


private:
	static NString                      SystemName(NOSName theName);
};



#endif // NSYSTEM_H
