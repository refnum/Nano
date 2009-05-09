/*	NAME:
		NFile.h

	DESCRIPTION:
		File object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFILE_HDR
#define NFILE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"
#include "NString.h"





//============================================================================
//      Constants
//----------------------------------------------------------------------------
// File permissions
typedef enum {
	kFilePermissionRead,
	kFilePermissionWrite,
	kFilePermissionReadWrite
} NFilePermission;


// File positions
typedef enum {
	kFilePositionFromStart,
	kFilePositionFromMark,
	kFilePositionFromEnd
} NFilePosition;


// File reference
typedef void *NFileRef;





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
class NFile : public NComparable<NFile> {
public:
										NFile(const NString &thePath);
										NFile(const NFile   &theFile);

										 NFile(void);
	virtual								~NFile(void);


	// Is the file valid?
	bool								IsValid(void) const;


	// Is this a file/directory?
	//
	// Will always return false if the file/directory does not exist.
	bool								IsFile(     void) const;
	bool								IsDirectory(void) const;


	// Is the file writeable?
	bool								IsWriteable(void) const;


	// Is the file open?
	bool								IsOpen(void) const;


	// Does the file exist?
	bool								Exists(   void) const;
	bool								NotExists(void) const;


	// Compare the value
	NComparison							Compare(const NFile &theValue) const;


	// Get/set the path
	NString								GetPath(void) const;
	void								SetPath(const NString &thePath);


	// Get/set the name
	NString								GetName(bool displayName=false) const;
	NStatus								SetName(const NString &theName, bool renameFile=false);


	// Get/set the extension
	NString								GetExtension(void) const;
	NStatus								SetExtension(const NString &theExtension, bool renameFile=false);


	// Get/set the size
	//
	// Growing the file will zero-fill any new space.
	SInt64								GetSize(void) const;
	NStatus								SetSize(SInt64 theSize);


	// Get the child/parent of a file
	//
	// Only directories have children, however all files/directories except the
	// root directory have a parent.
	NFile								GetChild(const NString &fileName) const;
	NFile								GetParent(void)                   const;


	// Delete the file
	//
	// If the file is an open file, it will be unlinked.
	//
	// If the file is a directory, it will only be deleted if it is empty.
	void								Delete(void);


	// Create a directory
	NStatus								CreateDirectory(void);


	// Exchange two files
	NStatus								ExchangeWith(const NFile &theTarget);


	// Open/close the file
	NStatus								Open(NFilePermission thePermission=kFilePermissionRead, bool canCreate=false);
	void								Close(void);


	// Get/set the read/write position
    //
    // The file must be opened with appropriate permissions first.
	SInt64								GetPosition(void) const;
	NStatus								SetPosition(SInt64 theOffset, NFilePosition thePosition=kFilePositionFromStart);


	// Read/write the file
	//
	// The file must be opened with appropriate permissions first.
	NStatus								Read( SInt64 theSize,       void *thePtr, UInt64 &numRead,    SInt64 theOffset=0, NFilePosition thePosition=kFilePositionFromMark);
	NStatus								Write(SInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset=0, NFilePosition thePosition=kFilePositionFromMark);


	// Operators
	const NFile&						operator = (const NFile &theFile);


private:
	void								InitializeSelf(const NString &thePath="");
	void								CloneFile(     const NFile   &theFile);


private:
	NString								mPath;
	NFileRef							mFile;
};





#endif // NFILE_HDR


