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
	static OSStatus						OpenFile(const NFile &theFile);


	// Open a URL
	static OSStatus						OpenURL(const NCFURL &theURL);
};

	
	


#endif // NLAUNCHSERVICESUTILITIES_HDR


