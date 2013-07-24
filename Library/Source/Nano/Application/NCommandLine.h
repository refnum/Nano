/*	NAME:
		NCommandLine.h

	DESCRIPTION:
		Command line parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOMMANDLINE_HDR
#define NCOMMANDLINE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCommandLine {
public:
										NCommandLine(int argc, const char **argv);
										NCommandLine(const NStringList &theArguments);

										NCommandLine(void);
	virtual							   ~NCommandLine(void);


	// Clear the command line
	void								Clear(void);


	// Does an argument exist?
	bool								HasArgument(const NString &theArgument) const;


	// Get a flag argument
	//
	// Flag arguments take the form "-arg=value" or "--arg=value".
	//
	// If the value is missing, a 0/empty value is returned.
	int64_t								GetFlagInt64(  const NString &theArgument) const;
	float64_t							GetFlagFloat64(const NString &theArgument) const;
	NString								GetFlagString( const NString &theArgument) const;
	NFile								GetFlagFile(   const NString &theArgument) const;


	// Get/set the arguments
	NStringList							GetArguments(void) const;
	void								SetArguments(int argc, const char **argv);
	void								SetArguments(const NStringList &theArguments);


	// Get the comand line
	static NCommandLine				   *Get(void);


private:
	NStringList							mArguments;
};





#endif // NCOMMANDLINE_HDR


