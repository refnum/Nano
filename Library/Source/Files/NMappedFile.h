/*	NAME:
		NMappedFile.h

	DESCRIPTION:
		Memory-mapped file.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMAPPEDFILE_HDR
#define NMAPPEDFILE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <map>

#include "NFile.h"
#include "NLock.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Map access
typedef enum {
	kAccessReadOnly,
	kAccessReadWrite,
	kAccessCopyOnWrite
} MapAccess;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Map info
typedef struct {
	UInt32		theSize;
	MapAccess	theAccess;
} MapInfo;


// Maps
typedef std::map<const void *, MapInfo>								MapInfoMap;
typedef MapInfoMap::iterator										MapInfoMapIterator;
typedef MapInfoMap::const_iterator									MapInfoMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMappedFile {
public:
										NMappedFile(void);
	virtual								~NMappedFile(void);


	// Is the file open?
	bool								IsOpen(void) const;


	// Open/close the file
	//
	// The file can be opened as read-only, read-write, or copy-on-write.
	//
	// Pages in a read-only file can only be mapped as read-only.
	//
	// Pages in a read-write file can be mapped as either read-only or read-write.
	// If mapped as read-write, and the pages are modified, the file on disk will
	// be updated.
	//
	// Pages in a copy-on-write file can be mapped for either read-only or read-write.
	// If mapped as read-write, and the pages are modified, the file on disk will
	// not be updated (pages are mutable, but transient).
	OSStatus							Open(const NFile &theFile, MapAccess theAccess=kAccessReadOnly);
	void								Close(void);


	// Map/unmap a page
	//
	// A size of 0 will map the entire file.
	void								*Map(UInt64 theOffset=0, UInt32 theSize=0, MapAccess theAccess=kAccessReadOnly, bool noCache=false);
	void								Unmap(const void *thePtr);


private:
	NMutexLock							mLock;

	NFile								mFile;
	int									mFileRef;
	MapAccess							mFileAccess;

	MapInfoMap							mPages;
};





#endif // NMAPPEDFILE_HDR
