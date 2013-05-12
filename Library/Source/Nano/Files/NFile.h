/*	NAME:
		NFile.h

	DESCRIPTION:
		File object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFILE_HDR
#define NFILE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NComparable.h"
#include "NEncodable.h"
#include "NString.h"
#include "NDate.h"
#include "NUTI.h"





//============================================================================
//      Constants
//----------------------------------------------------------------------------
// Flags
typedef NBitfield NFileFlags;

static const NFileFlags kNFileNone									= 0;
static const NFileFlags kNFileCacheRequest							= (1 << 0);
static const NFileFlags kNFileCacheSuppress							= (1 << 1);


// Names
//
// The "raw" name is the name on disk, while the "display" name is
// the user's view of that name subject to their preferences (e.g.,
// with extension potentially suppressed).
typedef enum {
	kNNameRaw,
	kNNameDisplay,
	kNNameNoExtension
} NFileName;


// Permissions
typedef enum {
	kNPermissionRead,
	kNPermissionWrite,
	kNPermissionUpdate
} NFilePermission;


// Positions
typedef enum {
	kNPositionFromStart,
	kNPositionFromMark,
	kNPositionFromEnd
} NFilePosition;


// File reference
typedef UIntPtr NFileRef;

static const NFileRef kNFileRefNone									= 0;


// Directory separator
#if NN_TARGET_MAC || NN_TARGET_IOS || NN_TARGET_LINUX
	#define NN_DIR													"/"
#elif NN_TARGET_WINDOWS
	#define NN_DIR													"\\"
#else
	UNKNOWN TARGET
#endif





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NFile;

typedef std::vector<NFile>											NFileList;
typedef NFileList::iterator											NFileListIterator;
typedef NFileList::const_iterator									NFileListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFile :	public NEncodable,
				public NComparable<NFile> {
public:
										NENCODABLE_DECLARE(NFile);

										NFile(const NString &thePath);
										NFile(const NFile   &theFile);

										NFile(void);
	virtual							   ~NFile(void);


	// Is the file valid?
	bool								IsValid(void) const;


	// Is this a file/directory/link?
	//
	// Will always return false if the object does not exist.
	bool								IsFile(     void) const;
	bool								IsDirectory(void) const;
	bool								IsLink(     void) const;


	// Is the file writeable?
	bool								IsWriteable(void) const;


	// Is the file open?
	bool								IsOpen(void) const;


	// Does the file exist?
	bool								Exists(void) const;


	// Clear the value
	void								Clear(void);


	// Compare the value
	NComparison							Compare(const NFile &theValue) const;


	// Get the UTI
	NUTI								GetUTI(void) const;


	// Get/set the path
	NString								GetPath(void) const;
	void								SetPath(const NString &thePath);


	// Get/set the name
	NString								GetName(NFileName theName=kNNameRaw) const;
	NStatus								SetName(const NString &theName, bool renameFile=false);


	// Get/set the extension
	NString								GetExtension(void) const;
	NStatus								SetExtension(const NString &theExtension, bool renameFile=false);


	// Get/set the size
	//
	// Growing the file will zero-fill any new space.
	UInt64								GetSize(void) const;
	NStatus								SetSize(UInt64 theSize);


	// Get the timestamp
	NDate								GetCreationTime(    void) const;
	NDate								GetAccessTime(      void) const;
	NDate								GetModificationTime(void) const;


	// Get the child/parent of a file
	//
	// Only directories have children, however all files/directories except the root
	// directory have a parent.
	//
	// The child file name may include path components.
	NFile								GetChild(const NString &fileName) const;
	NFile								GetParent(void)                   const;


	// Get the target of a file
	NFile								GetTarget(    void) const;
	bool								ResolveTarget(void);


	// Get the children of a directory
	NFileList							GetChildren(void) const;


	// Delete the file
	//
	// Deleting a file, rather than moving it to the trash, is permanent.
	//
	// If the file is open, it will be unlinked. If it is a directory, the
	// contents will be deleted before the directory itself.
	NStatus								Delete(bool moveToTrash=false) const;


	// Delete the contents of a directory
	//
	// Recursively deletes the contents of a directory.
	NStatus								DeleteContents(void) const;


	// Create a file/directory
	//
	// Non-existent parent directories will be created as necessary.
	NStatus								CreateFile(     void);
	NStatus								CreateDirectory(void);


	// Exchange two files
	//
	// Both files must be on the same volume.
	NStatus								ExchangeWith(const NFile &theTarget);


	// Move a file
	//
	// Both file and target directory must be on the same volume.
	NStatus								MoveTo(const NFile &theTarget);


	// Open/close the file
	//
	// Files opened for writing are always appended to.
	//
	// Files opened for updating can be read to/written from the current position.
	NStatus								Open(NFilePermission thePermission=kNPermissionRead, bool canCreate=false);
	void								Close(void);


	// Get/set the read/write position
	//
	// SetPosition is only available for files opened for reading or updating.
	UInt64								GetPosition(void) const;
	NStatus								SetPosition(SInt64 theOffset, NFilePosition thePosition=kNPositionFromStart);


	// Read/write the file
	//
	// The file must be opened with appropriate permissions first.
	NStatus								Read( UInt64 theSize,       void *thePtr, UInt64 &numRead,    SInt64 theOffset=0, NFilePosition thePosition=kNPositionFromMark, NFileFlags theFlags=kNFileNone);
	NStatus								Write(UInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset=0, NFilePosition thePosition=kNPositionFromMark, NFileFlags theFlags=kNFileNone);


	// Operators
	const NFile&						operator = (const NFile &theFile);
										operator NFormatArgument(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	void								InitializeSelf(const NString &thePath="");

	void								CloneFile(const NFile   &theFile);
	NStatus								SetName(  const NString &theName, bool renameFile, bool isPath);


private:
	NString								mPath;
	NFileRef							mFile;
	NFilePermission						mPermission;
};





#endif // NFILE_HDR


