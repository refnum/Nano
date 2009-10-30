/*	NAME:
		NIcon.cpp

	DESCRIPTION:
		IconRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NCFBundle.h"
#include "NIcon.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const OSType kNanoCreator                                    = 'Nano';
static const OSType kNanoType                                       = '0000';





//============================================================================
//      NIcon::NIcon : Constructor.
//----------------------------------------------------------------------------
NIcon::NIcon(OSType iconType, OSType iconCreator)
{	OSStatus	theErr;



	// Initialize ourselves
	AcquireIcon(NULL);



	// Get the icon
	theErr = GetIconRef(kOnSystemDisk, iconCreator, iconType, &mIcon);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NIcon::NIcon : Constructor.
//----------------------------------------------------------------------------
NIcon::NIcon(IconRef theIcon)
{


	// Initialize ourselves
	AcquireIcon(theIcon);
}





//============================================================================
//		NIcon::NIcon : Constructor.
//----------------------------------------------------------------------------
NIcon::NIcon(const NFile &theFile, OSType iconType, OSType iconCreator)
{	FSRef		theFSRef;
	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theFile.Exists());



	// Initialize ourselves
	AcquireIcon(NULL);



	// Load the icon
	//
	// If the file is a .icns file, we'll be able to register it and get the icon.
	theFSRef = theFile.GetFSRef();
	
	if (iconCreator == 0)
        iconCreator = kNanoCreator;

	if (iconType == 0)
		iconType = GetNextType();

	theErr = RegisterIconRefFromFSRef(iconCreator, iconType, &theFSRef, &mIcon);
	if (theErr == noErr)
		{
		mIconCreator = iconCreator;
		mIconType    = iconType;
		}



	// Load the file's icon
	//
	// If the file wasn't a .icns file, we fall back to the icon of the file.
	if (mIcon == NULL)
		{
		theErr = GetIconRefFromFileInfo(&theFSRef, 0, NULL, kFSCatInfoNone, NULL, kIconServicesNormalUsageFlag, &mIcon, NULL);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NIcon::NIcon : Constructor.
//----------------------------------------------------------------------------
NIcon::NIcon(const NString &theName, const NString &subDir, CFBundleRef theBundle)
{	NCFBundle	cfBundle;
	NIcon		theIcon;
	NString		theType;
	NFile		theFile;



	// Validate our parameters
	NN_ASSERT(theName.IsNotEmpty());



	// Look for a bundle resource
	if (theBundle != NULL)
		cfBundle.Set(theBundle, false);

	if (!theName.EndsWith(".icns"))
		theType = "icns";

	theFile = cfBundle.GetResource(theName, theType, subDir);



	// Or try for a bundle ID
	if (subDir.IsEmpty() && theBundle == NULL && !theFile.Exists())
		{
		cfBundle = NCFBundle(theName);
		if (cfBundle.IsValid())
			theFile = cfBundle.GetFile();
		}



    // Initialize ourselves
	if (theFile.Exists())
		theIcon = NIcon(theFile);

	AcquireIcon(theIcon.mIcon);
}





//============================================================================
//		NIcon::NIcon : Constructor.
//----------------------------------------------------------------------------
NIcon::NIcon(const NCGImage &theImage)
{	IconRef		iconRef;
	OSStatus	theErr;



	// Initialize ourselves
	iconRef = NCoreGraphicsUtilities::CreateIconFromImage(theImage);
	AcquireIcon(iconRef);



	// Clean up
	if (iconRef != NULL)
		{
		theErr = ReleaseIconRef(iconRef);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NIcon::NIcon : Constructor.
//----------------------------------------------------------------------------
NIcon::NIcon(const NIcon &theIcon)
{


	// Initialize ourselves
	AcquireIcon(theIcon.mIcon);
}





//============================================================================
//		NIcon::NIcon : Constructor.
//----------------------------------------------------------------------------
NIcon::NIcon(void)
{


	// Initialize ourselves
	AcquireIcon(NULL);
}





//============================================================================
//		NIcon::~NIcon : Destructor.
//----------------------------------------------------------------------------
NIcon::~NIcon(void)
{


	// Clean up
	ReleaseIcon();
}





//============================================================================
//		NIcon::IsValid : Is the icon valid?
//----------------------------------------------------------------------------
bool NIcon::IsValid(void) const
{


	// Check our state
	return(mIcon != NULL);
}





//============================================================================
//      NIcon::Clear : Clear the icon.
//----------------------------------------------------------------------------
void NIcon::Clear(void)
{


	// Clear the icon
	ReleaseIcon();
}





//============================================================================
//		NIcon::operator = : Assignment operator.
//----------------------------------------------------------------------------
NIcon &NIcon::operator=(const NIcon &otherIcon)
{


	// Assign the icon
	if (this != &otherIcon)
		{
		ReleaseIcon();
		AcquireIcon(otherIcon.mIcon);
		}
		
	return(*this);
}





//============================================================================
//		NIcon::operator IconRef : IconRef-cast operator.
//----------------------------------------------------------------------------
NIcon::operator IconRef(void) const
{


	// Get the icon
	return(mIcon);
}





//============================================================================
//		NIcon::AcquireIcon : Acquire an icon.
//----------------------------------------------------------------------------
#pragma mark -
void NIcon::AcquireIcon(IconRef theIcon)
{	OSStatus	theErr;



	// Update our state
	mIcon        = theIcon;
	mIconCreator = 0;
	mIconType    = 0;



	// Retain the icon
	if (mIcon != NULL)
		{
		theErr = AcquireIconRef(mIcon);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NIcon::ReleaseIcon : Release the icon.
//----------------------------------------------------------------------------
void NIcon::ReleaseIcon(void)
{	OSStatus	theErr;



	// Unregister the icon
	//
	// If we registered this icon, we are responsible for unregistering it when
	// it should be released. We reset mIcon to NULL, since unregistering the
	// icon will release the final reference.
	if (mIconType != 0 && mIconCreator != 0)
		{
		mIcon = NULL;
		theErr = UnregisterIconRef(mIconCreator, mIconType);
		NN_ASSERT_NOERR(theErr);
		}



	// Release the icon
	if (mIcon != NULL)
		{
		theErr = ReleaseIconRef(mIcon);
		NN_ASSERT_NOERR(theErr);
		}



	// Reset our state
	mIcon        = NULL;
	mIconCreator = 0;
	mIconType    = 0;
}





//============================================================================
//		NIcon::GetNextType : Get the next available type.
//----------------------------------------------------------------------------
OSType NIcon::GetNextType(void)
{   static OSType   sNextType = kNanoType;



	// Get the next type
	//
	// Icons can only be registered against alphanumeric types, and so we
	// increment a suitable starting value rather than starting from 0.
	sNextType++;
	
	return(sNextType);
}

