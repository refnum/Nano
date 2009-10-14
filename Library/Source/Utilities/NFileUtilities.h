/*	NAME:
		NFileUtilities.h

	DESCRIPTION:
		File utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFILEUTILITIES_HDR
#define NFILEUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NData.h"
#include "NFile.h"
#include "NFileIterator.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const OSType kFileTypeFinder									= 'FNDR';
static const OSType kFileTypeApplication							= 'APPL';
static const OSType kFileTypeSymlink								= 'slnk';
static const OSType kFileTypeNone									= 0;

static const OSType kFileCreatorFinder								= 'MACS';
static const OSType kFileCreatorNone								= 0;

static const UInt32 kFileHeaderSizePICT								= 512;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFileUtilities {
public:
	// Get a file from an AEDesc(list)
	static NFile						GetAEFile( const AEDesc     *theDesc);
	static NFileList					GetAEFiles(const AEDescList *theList);


	// Get the file from a Nav Services reply
	static NFileList					GetNavReplyFiles(const NavReplyRecord &navReply);


	// Get/set a file's data
	static NString						GetFileText(const NFile &theFile, CFStringEncoding theEncoding=kCFStringEncodingUTF8);
	static NData						GetFileData(const NFile &theFile);

	static OSStatus						SetFileText(const NFile &theFile, const NString &theText);
	static OSStatus						SetFileData(const NFile &theFile, const NData   &theData);


	// Get the file path separator
	static NString						GetPathSeparator(CFURLPathStyle theStyle);


	// Find files
	static NFileList					FindFiles(const NFile &startAt, const NString &matchName="", const NString &matchPath="");


	// Find a bundle
	static NFile						FindBundle(const NFile &startAt, const NString &bundleID, const NString &matchName="");


	// Find a folder
	//
	// Finds a folder from the specified domain, creating it if necessary.
	//
	// If fileName is supplied, will return an NFile to the file/folder of that name within
	// the specified folder rather than an NFile for the folder itself.
	static NFile						FindFolder(OSType			theFolder,
													const NString	&fileName = "",
													SInt16			theDomain = kUserDomain,
													bool			canCreate = true);


	// Get a file with a unique name
	//
	// An optional baseName can be supplied, to provide the template for the name.
	static NFile						GetUniqueFile(const NFile &theDirectory, const NString &baseName="");


	// Get a temporary file
	//
	//
	// An optional baseName can be supplied, to provide the template for the name.
	static NFile						GetTempFile(const NString &baseName="");


	// Redirect stderr
	//
	// To suppress unnecessary debug output from OS calls (IBCarbonRuntime, rdar://4773703),
	// stderr can be temporarily redirected to /dev/null.
	//
	// To restore stderr output, invoke a second time with the result of the first call.
	static SInt32						RedirectStderr(SInt32 errorFD=-1);


	// Mount/unmount a disk image
	//
	// Disk images are mounted with hdiutil, and a NULL-terminated list of UTF8 arguments
	// for that tool can be supplied when mounting.
	//
	// Mounting a disk image will return the mount point, which can be used to unmount.
	static NFile						MountDMG(  const NFile &theDMG, const char *arg1=NULL, ...);
	static void							UnmountDMG(const NFile &mountPoint);


	// Move a file to the trash
	static OSStatus						MoveToTrash(const NFile &theFile);
	

private:
	static bool							FilterBundleID(bool &stopNow, const IteratorFilterInfo &theInfo);
};

	
	


#endif // NFILEUTILITIES_HDR


