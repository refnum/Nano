/*	NAME:
		NCommandLine.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCommandLine.h"

// Nano
#include "NFile.h"
#include "NNumber.h"





//=============================================================================
//		NCommandLine::HasFlag : Does a flag exist?
//-----------------------------------------------------------------------------
bool NCommandLine::HasFlag(const NString& theName)
{


	// Validate our parameters
	NN_REQUIRE(!theName.IsEmpty());
	NN_REQUIRE(!theName.StartsWith("-"));



	// Get the state we need
	NString flagNoValue1 = "-" + theName;
	NString flagNoValue2 = "--" + theName;
	NString flagValue1   = "-" + theName + "=";
	NString flagValue2   = "--" + theName + "=";



	// Check the arguments
	for (const auto& theArg : GetState())
	{
		if (theArg == flagNoValue1 || theArg == flagNoValue2)
		{
			return true;
		}

		if (theArg.StartsWith(flagValue1) || theArg.StartsWith(flagValue2))
		{
			return true;
		}
	}

	return false;
}





//=============================================================================
//		NCommandLine::GetBool : Get a bool flag.
//-----------------------------------------------------------------------------
bool NCommandLine::GetBool(const NString& theName)
{


	// Get the value
	NString theValue = GetString(theName).GetLower();

	return theValue == "true" || theValue == "yes" || theValue == "1";
}





//=============================================================================
//		NCommandLine::GetInt64 : Get an int64_t flag.
//-----------------------------------------------------------------------------
int64_t NCommandLine::GetInt64(const NString& theName)
{


	// Get the value
	NString theValue = GetString(theName);
	NNumber theNumber;

	if (theNumber.SetValue(theValue) && theNumber.IsInteger())
	{
		return theNumber.GetInt64();
	}

	return 0;
}





//=============================================================================
//		NCommandLine::GetFloat64 : Get a float64_t flag.
//-----------------------------------------------------------------------------
float64_t NCommandLine::GetFloat64(const NString& theName)
{


	// Get the value
	NString theValue = GetString(theName);
	NNumber theNumber;

	if (theNumber.SetValue(theValue))
	{
		return theNumber.GetFloat64();
	}

	return 0.0;
}





//=============================================================================
//		NCommandLine::GetFile : Get an NFile flag.
//-----------------------------------------------------------------------------
NFile NCommandLine::GetFile(const NString& theName)
{


	// Get the value
	NString theValue = GetString(theName);

	if (!theValue.IsEmpty())
	{
		return NFile(theValue);
	}

	return NFile();
}





//=============================================================================
//		NCommandLine::GetString : Get an NString flag.
//-----------------------------------------------------------------------------
NString NCommandLine::GetString(const NString& theName)
{


	// Get the state we need
	NString flagValue1 = "-" + theName + "=";
	NString flagValue2 = "--" + theName + "=";
	NString theValue;


	// Get the value
	for (const auto& theArg : GetState())
	{
		if (theArg.StartsWith(flagValue1))
		{
			theValue = theArg.GetSubstring(NRange(flagValue1.GetSize(), kNNotFound));
			break;
		}

		else if (theArg.StartsWith(flagValue2))
		{
			theValue = theArg.GetSubstring(NRange(flagValue2.GetSize(), kNNotFound));
			break;
		}
	}



	// Strip quotes
	if (theValue.StartsWith("\""))
	{
		theValue.RemovePrefix(1);
	}

	if (theValue.EndsWith("\""))
	{
		theValue.RemoveSuffix(1);
	}

	return theValue;
}





//=============================================================================
//		NCommandLine::GetArguments : Get the arguments.
//-----------------------------------------------------------------------------
NVectorString NCommandLine::GetArguments(NArguments theArguments)
{


	// Get the arguments
	NVectorString theResult;

	switch (theArguments)
	{
		case NArguments::All:
			theResult = GetState();
			break;

		case NArguments::Named:
			for (const auto& theArg : GetState())
			{
				if (theArg.StartsWith("-"))
				{
					theResult.emplace_back(theArg);
				}
			}
			break;

		case NArguments::Unnamed:
			for (const auto& theArg : GetState())
			{
				if (!theArg.StartsWith("-"))
				{
					theResult.emplace_back(theArg);
				}
			}
			break;
	}

	return theResult;
}





//=============================================================================
//		NCommandLine::SetArguments : Set the arguments.
//-----------------------------------------------------------------------------
void NCommandLine::SetArguments(const NVectorString& theArgs)
{


	// Set the arguments
	GetState() = theArgs;
}





//=============================================================================
//		NCommandLine::SetArguments : Set the arguments.
//-----------------------------------------------------------------------------
void NCommandLine::SetArguments(int argc, const char** argv)
{


	// Set the arguments
	GetState().clear();

	for (int n = 0; n < argc; n++)
	{
		GetState().emplace_back(argv[n]);
	}
}





#pragma mark private
//=============================================================================
//		NCommandLine::GetState : Get the state.
//-----------------------------------------------------------------------------
NVectorString& NCommandLine::GetState()
{


	// Get the state
	static NVectorString sState;

	return sState;
}
