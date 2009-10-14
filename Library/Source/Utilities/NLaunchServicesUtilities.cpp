/*	NAME:
		NLaunchServicesUtilities.cpp

	DESCRIPTION:
		LaunchServices utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLaunchServicesUtilities.h"





//============================================================================
//		NLaunchServicesUtilities::OpenFile : Open a file.
//----------------------------------------------------------------------------
OSStatus NLaunchServicesUtilities::OpenFile(const NFile &theFile)
{	FSRef		theFSRef;
	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theFile.Exists());



	// Open the file
	theFSRef = theFile.GetFSRef();
	theErr   = LSOpenFSRef(&theFSRef, NULL);

	return(theErr);
}





//============================================================================
//		NLaunchServicesUtilities::OpenURL : Open a URL.
//----------------------------------------------------------------------------
OSStatus NLaunchServicesUtilities::OpenURL(const NCFURL &theURL)
{	OSStatus	theErr;



	// Open the URL
	//
	// We can't assert on noErr, since opening valid URLs may still fail if
	// the user vetoes any  application launch needed to handle the URL.
	theErr = LSOpenCFURLRef(theURL, NULL);

	return(theErr);
}
