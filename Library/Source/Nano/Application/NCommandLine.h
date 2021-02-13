/*	NAME:
		NCommandLine.h

	DESCRIPTION:
		Command line support.

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
#ifndef NCOMMAND_LINE_H
#define NCOMMAND_LINE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NString.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Arguments
//
// Arguments are either named or unnamed.
//
// A named argument starts with a '-' or '--' prefix, and may contain a value.
//
// An unnamed argument does not start with a '-' and is simply a raw value.
enum class NArguments
{
	All,
	Named,
	Unnamed
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NFile;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NCommandLine
{
public:
	// Does a flag exist?
	//
	// A flag is a named argument, with or without a value.
	static bool                         HasFlag(const NString& theName);


	// Get a named argument
	//
	// A named argument may be specified as '-name=value' or '--name=value'.
	//
	// If the argument is not present, or no value is supplied, a 0/empty
	// value is returned.
	static bool                         GetBool(   const NString& theName);
	static int64_t                      GetInt64(  const NString& theName);
	static float64_t                    GetFloat64(const NString& theName);
	static NFile                        GetFile(   const NString& theName);
	static NString                      GetString( const NString& theName);


	// Get / set the arguments
	static NVectorString                GetArguments(NArguments           theArguments = NArguments::All);
	static void                         SetArguments(const NVectorString& theArguments);

	static void                         SetArguments(int argc, const char** argv);


private:
	static NVectorString&               GetState();
};



#endif // NCOMMAND_LINE_H
