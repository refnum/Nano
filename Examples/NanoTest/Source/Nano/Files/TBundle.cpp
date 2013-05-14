/*	NAME:
		TBundle.cpp

	DESCRIPTION:
		NBundle tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NBundle.h"

#include "TBundle.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Paths
#if NN_TARGET_MAC
	static const NString kPathBundle								= "/Applications/Safari.app";
#else
	static const NString kPathBundle								= "";

#endif





//============================================================================
//		TBundle::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TBundle::Execute(void)
{	NBundle			theBundle;
	NString			theString;
	NDictionary		theInfo;
	NFile			theFile;



	// Mac-specific setup
	if (!kPathBundle.IsEmpty())
		theBundle = NBundle(NFile(kPathBundle));



	// Validity
	NN_ASSERT(theBundle.IsValid());
	
	theString = theBundle.GetIdentifier();
	if (!kPathBundle.IsEmpty())
		NN_ASSERT(!theString.IsEmpty());



	// Files
	theFile = theBundle.GetFile();
	NN_ASSERT(theFile.IsDirectory());

	theFile = theBundle.GetResources();
	NN_ASSERT(theFile.IsDirectory());
	
	theFile = theBundle.GetExecutable();
	if (!kPathBundle.IsEmpty())
		NN_ASSERT(theFile.IsFile());



	// Info.plist
	theInfo = theBundle.GetInfoDictionary();
	NN_ASSERT(!theInfo.IsEmpty());
}

