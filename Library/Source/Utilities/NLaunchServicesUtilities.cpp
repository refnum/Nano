/*	NAME:
		NLaunchServicesUtilities.cpp

	DESCRIPTION:
		LaunchServices utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"
#include "NLaunchServicesUtilities.h"





//============================================================================
//		NLaunchServicesUtilities::OpenFile : Open a file.
//----------------------------------------------------------------------------
OSStatus NLaunchServicesUtilities::OpenFile(const NFile &theFile, const NString &appID)
{	FSRef					fileFSRef, appFSRef;
	NCFBundle				theBundle(appID);
	LSLaunchFSRefSpec		launchSpec;
	NFile					appFile;
	OSStatus				theErr;



	// Validate our parameters
	NN_ASSERT(theFile.Exists());



	// Get the state we need
	//
	// If a specific application as asked for, but couldn't be found, we fail to
	// allow the caller to re-submit without a target application if appropriate.
	fileFSRef = theFile.GetFSRef();

	if (appID.IsNotEmpty())
		{
		if (!theBundle.IsValid())
			return(handlerNotFoundErr);

		appFile  = theBundle.GetFile();
		appFSRef = appFile.GetFSRef();
		}



	// Open the file
	memset(&launchSpec, 0x00, sizeof(launchSpec));

	launchSpec.appRef      = appFile.Exists() ? &appFSRef : NULL;
	launchSpec.numDocs     = 1;
	launchSpec.itemRefs    = &fileFSRef;
	launchSpec.launchFlags = kLSLaunchDefaults;

	theErr = LSOpenFromRefSpec(&launchSpec, NULL);

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
	// the user vetoes any application launch needed to handle the URL.
	theErr = LSOpenCFURLRef(theURL, NULL);

	return(theErr);
}
