/*	NAME:
		NCommandLine.cpp

	DESCRIPTION:
		Command line parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"
#include "NTextUtilities.h"
#include "NNumber.h"
#include "NCommandLine.h"





//============================================================================
//		NCommandLine::NCommandLine : Constructor.
//----------------------------------------------------------------------------
NCommandLine::NCommandLine(int argc, const char **argv)
{


	// Initialize ourselves
	SetArguments(argc, argv);
}





//============================================================================
//		NCommandLine::NCommandLine : Constructor.
//----------------------------------------------------------------------------
NCommandLine::NCommandLine(const NStringList &theArguments)
{


	// Initialize ourselves
	SetArguments(theArguments);
}





//============================================================================
//		NCommandLine::NCommandLine : Constructor.
//----------------------------------------------------------------------------
NCommandLine::NCommandLine(void)
{
}





//============================================================================
//		NCommandLine::~NCommandLine : Destructor.
//----------------------------------------------------------------------------
NCommandLine::~NCommandLine(void)
{
}





//============================================================================
//		NCommandLine::Clear : Clear the command line.
//----------------------------------------------------------------------------
void NCommandLine::Clear(void)
{


	// Reset our state
	mArguments.clear();
}





//============================================================================
//		NCommandLine::HasArgument : Does an argument exist?
//----------------------------------------------------------------------------
bool NCommandLine::HasArgument(const NString &theArgument) const
{	NStringList					theTokens;
	NStringListConstIterator	theIter;
	NString						theArg;



	// Check the arguments
	for (theIter = mArguments.begin(); theIter != mArguments.end(); theIter++)
		{
		theTokens = theIter->Split(kNTokenEquals);
		if (theTokens.size() == 2)
			theArg = theTokens[0];
		else
			theArg = *theIter;
		
		theArg.TrimLeft("-*", kNStringPattern);
		if (theArg == theArgument)
			return(true);
		}
	
	return(false);
}





//============================================================================
//		NCommandLine::GetFlagInt64 : Get an sint64_t flag value.
//----------------------------------------------------------------------------
int64_t NCommandLine::GetFlagInt64(const NString &theArgument) const
{	int64_t		theResult;
	NString		theFlag;



	// Get the value
	theFlag   = GetFlagString(theArgument);
	theResult = NNumber(theFlag).GetInt64();

	return(theResult);
}





//============================================================================
//		NCommandLine::GetFlagFloat64 : Get a float64_t flag value.
//----------------------------------------------------------------------------
float64_t NCommandLine::GetFlagFloat64(const NString &theArgument) const
{	float64_t	theResult;
	NString		theFlag;



	// Get the value
	theFlag   = GetFlagString(theArgument);
	theResult = NNumber(theFlag).GetFloat64();

	return(theResult);
}





//============================================================================
//		NCommandLine::GetFlagString : Get a string flag value.
//----------------------------------------------------------------------------
NString NCommandLine::GetFlagString(const NString &theArgument) const
{	NString						theArg, theValue;
	NStringList					theTokens;
	NStringListConstIterator	theIter;



	// Get the value
	for (theIter = mArguments.begin(); theIter != mArguments.end(); theIter++)
		{
		theTokens = theIter->Split(kNTokenEquals);
		if (theTokens.size() == 2)
			{
			theArg   = theTokens[0];
			theValue = theTokens[1];
			
			theArg.TrimLeft("-*", kNStringPattern);
			if (theArg == theArgument)
				return(theValue);
			}
		}
	
	return("");
}





//============================================================================
//		NCommandLine::GetFlagFile : Get a file flag value.
//----------------------------------------------------------------------------
NFile NCommandLine::GetFlagFile(const NString &theArgument) const
{	NFile		theResult;
	NString		theValue;



	// Get the value
	//
	// Relative paths are relative to the current working directory.
	theValue = GetFlagString(theArgument);
	
	if (!theValue.IsEmpty())
		{
		if (!theValue.StartsWith("/"))
			theValue = NFileUtilities::GetCWD().GetPath() + "/" + theValue;

		theResult = NFile(theValue);
		}

	return(theResult);
}





//============================================================================
//		NCommandLine::GetArguments : Get the arguments.
//----------------------------------------------------------------------------
NStringList NCommandLine::GetArguments(void) const
{


	// Get the arguments
	return(mArguments);
}





//============================================================================
//		NCommandLine::SetArguments : Set the arguments.
//----------------------------------------------------------------------------
void NCommandLine::SetArguments(int argc, const char **argv)
{	NIndex		n;



	// Set the arguments
	mArguments.clear();

	for (n = 0; n < argc; n++)
		mArguments.push_back(argv[n]);
}





//============================================================================
//		NCommandLine::SetArguments : Set the arguments.
//----------------------------------------------------------------------------
void NCommandLine::SetArguments(const NStringList &theArgs)
{


	// Set the arguments
	mArguments = theArgs;
}





//============================================================================
//      NCommandLine::Get : Get the command line.
//----------------------------------------------------------------------------
NCommandLine *NCommandLine::Get(void)
{   static NCommandLine   sInstance;



    // Get the instance
	return(&sInstance);
}



