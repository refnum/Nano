/*	NAME:
		NFileMap.h

	DESCRIPTION:
		Memory-mapped file.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFILEMAP_HDR
#define NFILEMAP_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"
#include "NLock.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Map access
typedef enum {
	kNAccessRead,
	kNAccessReadWrite,
	kNAccessCopyOnWrite
} NMapAccess;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Map info
typedef struct {
	UInt32		theSize;
	NMapAccess	theAccess;
} NMapInfo;


// Maps
typedef std::map<const void *, NMapInfo>						NMapInfoMap;
typedef NMapInfoMap::iterator									NMapInfoMapIterator;
typedef NMapInfoMap::const_iterator								NMapInfoMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFileMap {
public:
										NFileMap(void);
	virtual							   ~NFileMap(void);


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
	// If mapped as read-write, and the pages are modified, the file on disk will not
	// be updated (so pages are mutable, but transient).
	NStatus								Open(const NFile &theFile, NMapAccess theAccess=kNAccessRead);
	void								Close(void);


	// Map/unmap a page
	//
	// A size of 0 will map the entire file. A cache hint can be provided if the
	// mapping is temporary, to avoid polluting the normal disk cache.
	void								*Map(UInt64 theOffset=0, UInt32 theSize=0, NMapAccess theAccess=kNAccessRead, bool noCache=false);
	void								Unmap(const void *thePtr);


private:
	NMutexLock							mLock;

	NFile								mFile;
	NFileRef							mFileRef;
	NMapAccess							mFileAccess;

	NMapInfoMap							mPages;
};





#endif // NFILEMAP_HDR
