/*	NAME:
		NLaunchServicesUtilities.h

	DESCRIPTION:
		LaunchServices utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NLAUNCHSERVICESUTILITIES_HDR
#define NLAUNCHSERVICESUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFURL.h"
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NLaunchServicesUtilities {
public:
	// Open a file
	//
	// Opens a file, optionally with a specified application (identified by bundle identifier).
	static OSStatus						OpenFile(const NFile &theFile, const NString &appID="");


	// Open a URL
	static OSStatus						OpenURL(const NCFURL &theURL);
};

	
	


#endif // NLAUNCHSERVICESUTILITIES_HDR


