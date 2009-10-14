/*	NAME:
		NFile.h

	DESCRIPTION:
		Nano file object.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFILE_HDR
#define NFILE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NString.h"
#include "NCFURL.h"
#include "NUTI.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const FSSpec kFSSpecZero										= { 0 };
static const FSRef  kFSRefZero										= { 0 };

static const SInt16 kInvalidForkRef									= -1;





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NFile;

typedef std::vector<NFile>										NFileList;
typedef NFileList::iterator										NFileListIterator;
typedef NFileList::const_iterator								NFileListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFile {
public:
										NFile(const FSSpec  &theFSSpec);
										NFile(const FSRef   &theFSRef);
										NFile(const NString &thePath, CFURLPathStyle pathStyle=kCFURLPOSIXPathStyle);

										NFile(void);
	virtual								~NFile(void);


	// Is the file valid?
	bool								IsValid(void) const;


	// Does the file exist?
	bool								Exists(  void) const;
	bool								NotExist(void) const;


	// Is this a file/directory?
	//
	// Will always return false if the file/directory does not exist.
	bool								IsFile(     void) const;
	bool								IsDirectory(void) const;
	
	
	// Is this an alias?
	//
	// Will always return false if the alias does not exist.
	bool								IsAlias(void) const;


	// Get/set the file
	//
	// To represent a file which does not exist, an FSRef to the parent directory can
	// be supplied along with the name of the file; if the file does exist, only an
	// FSRef is required.
	FSRef								GetFSRef(void)  const;
	void								SetFSRef(const FSRef &theFSRef, const NString &nameIfParent="");


	// Get/set the file name
	NString								GetFileName(void) const;
	void								SetFileName(const NString &theName, bool renameFile=false);


	// Get/set the file extension
	NString								GetExtension(void) const;
	void								SetExtension(const NString &theExtension, bool renameFile=false);


	// Get/set the path
	NString								GetPath(                        CFURLPathStyle pathStyle=kCFURLPOSIXPathStyle) const;
	void								SetPath(const NString &thePath, CFURLPathStyle pathStyle=kCFURLPOSIXPathStyle);


	// Get/set the file URL
	NCFURL								GetURL(void) const;
	void								SetURL(const NCFURL &theURL);


	// Get the file FSSpec
	FSSpec								GetFSSpec(void) const;


	// Get the file display name
	NString								GetDisplayName(void) const;


	// Get the file UTI
	NUTI								GetUTI(void) const;


	// Get/set the creator/type
	OSType								GetFileCreator(void) const;
	OSType								GetFileType(   void) const;
	void								GetFileCreatorAndType(OSType &fileCreator, OSType &fileType) const;

	void								SetFileCreator(OSType fileCreator);
	void								SetFileType(   OSType fileType);
	void								SetFileCreatorAndType(OSType fileCreator, OSType fileType);


	// Get/set the Finder info
	FileInfo							GetFileInfo(void) const;
	void								SetFileInfo(const FileInfo &theInfo);


	// Get/set the catalog info
	OSStatus							GetCatalogInfo(FSCatalogInfoBitmap whichInfo,       FSCatalogInfo &theInfo) const;
	OSStatus							SetCatalogInfo(FSCatalogInfoBitmap whichInfo, const FSCatalogInfo &theInfo);


	// Get the size of the file
	UInt64								GetFileSize(void) const;


	// Get the child/parent of a file
	//
	// The child is a named file within an NFile that represents a directory. If the
	// child does not exist, an invalid NFile is returned.
	//
	// The parent is the parent directory of any NFile except for the root directory.
	NFile								GetChild(const NString &fileName) const;
	NFile								GetParent(void)                   const;


	// Delete the file
	//
	// If the file is an open file, it will be unlinked.
	//
	// If the file is a diretory, it will only be deleted when empty.
	void								Delete(void);


	// Is the file open?
	bool								IsOpen(void) const;
	

	// Open/close the file
	//
	// Files must be opened with fsRdPerm/fsWrPerm/etc before they can be read/written.
	OSStatus							OpenFile(SInt8 thePermissions, bool canCreate=false);
	void								CloseFile(void);


	// Get the data fork reference
	SInt16								GetForkRef(void) const;
	

	// Read/write the file
	//
	// The file must be opened with appropriate permissions first.
	OSStatus							ReadFile( SInt64 dataSize,       void *dataPtr, UInt64 &numRead,    SInt64 theOffset=0, SInt16 posMode=fsFromMark);
	OSStatus							WriteFile(SInt64 dataSize, const void *dataPtr, UInt64 &numWritten, SInt64 theOffset=0, SInt16 posMode=fsFromStart);
	

	// Operators
	const bool							operator == (const NFile &theFile) const;
	const bool							operator != (const NFile &theFile) const;
	const NFile&						operator =  (const NFile &theFile);


private:
	void								InitializeSelf(void);
	
	bool								ResolveFile(void) const;


private:
	SInt16								mForkRef;
	
	mutable FSRef						mFSRef;
	mutable NString						mNameIfParent;
};





#endif // NFILE_HDR
