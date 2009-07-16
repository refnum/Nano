/*	NAME:
		NMacSystem.cpp

	DESCRIPTION:
		Mac system support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"
#include "NCFString.h"

#include "NTargetSystem.h"





//============================================================================
//      NTargetSystem::FindBundle : Find a bundle.
//----------------------------------------------------------------------------
NFile NTargetSystem(const NString &bundleID)
{	NCFObject		cfBundle, cfURL;
	NCFString		cfString;
	NFile			theFile;



	// Get the bundle
	if (bundleID.IsEmpty())
		cfBundle.Set(CFBundleGetMainBundle(), false);
	else
		{
		cfString = bundleID;
		cfBundle.Set(CFBundleGetBundleWithIdentifier(cfString), false);
		}

	if (!cfBundle.IsValid())
		return(theFile);



	// Get the file
	if (cfURL.Set(CFBundleCopyBundleURL(cfBundle)))
		{
		cfString.Set(CFURLCopyFileSystemPath(cfURL, kCFURLPOSIXPathStyle));
		theFile = NFile(cfString);
		}
	
	return(theFile);
}








