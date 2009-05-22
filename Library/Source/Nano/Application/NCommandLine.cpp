/*	NAME:
		NCommandLine.cpp

	DESCRIPTION:
		Command line parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNumber.h"
#include "NFileUtilities.h"
#include "NCommandLine.h"





//============================================================================
//		NCommandLine::NCommandLine : Constructor.
//----------------------------------------------------------------------------
NCommandLine::NCommandLine(int argc, const char **argv)
{	NIndex		n;



	// Initialize ourselves
	for (n = 0; n < argc; n++)
		mArguments.push_back(argv[n]);
}





//============================================================================
//		NCommandLine::NCommandLine : Constructor.
//----------------------------------------------------------------------------
NCommandLine::NCommandLine(const NStringList &theArguments)
{


	// Initialize ourselves
	mArguments = theArguments;
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
		theTokens = theIter->Split("=");
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
//		NCommandLine::GetFlagSInt32 : Get a SInt32 flag value.
//----------------------------------------------------------------------------
SInt32 NCommandLine::GetFlagSInt32(const NString &theArgument) const
{	SInt32		theResult;
	NNumber		theNumber;



	// Get the value
	theResult = 0;

	if (theNumber.SetValue(GetFlagString(theArgument)))
		theNumber.GetValueSInt32(theResult);

	return(theResult);
}





//============================================================================
//		NCommandLine::GetFlagFloat32 : Get a Float32 flag value.
//----------------------------------------------------------------------------
Float32 NCommandLine::GetFlagFloat32(const NString &theArgument) const
{	Float32		theResult;
	NNumber		theNumber;



	// Get the value
	theResult = 0.0f;

	if (theNumber.SetValue(GetFlagString(theArgument)))
		theNumber.GetValueFloat32(theResult);

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
		theTokens = theIter->Split("=");
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
	if (!theValue.StartsWith("/"))
		theValue = NFileUtilities::GetCWD().GetPath() + "/" + theValue;

	theResult = NFile(theValue);
	
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



