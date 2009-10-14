/*	NAME:
		NFileIterator.h

	DESCRIPTION:
		Filesystem iterator.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFILEITERATOR_HDR
#define NFILEITERATOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NFile.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Iterator start points
typedef struct {
	NFile		theFile;
	UInt32		theDepth;
} IteratorStart;


// Filter info
typedef struct {
	void					*userData;
	NFile					theFile;
	const FSCatalogInfo		*theInfo;
} IteratorFilterInfo;


// Filter callback
typedef bool (*IteratorFilterProc)(bool &stopNow, const IteratorFilterInfo &theInfo);


// Lists
typedef std::vector<IteratorStart>								IteratorStartList;
typedef IteratorStartList::iterator								IteratorStartListIterator;
typedef IteratorStartList::const_iterator						IteratorStartListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFileIterator {
public:
										NFileIterator(void);
	virtual								~NFileIterator(void);


	// Get/set the recurse limit
	//
	// A limit of 0, the default, implies no limit tp recursion.
	UInt32								GetRecurseLimit(void) const;
	void								SetRecurseLimit(UInt32 theLimit);


	// Get/set the recurse filter
	//
	// The recurse filter is called for each directory found while iterating, and
	// controls if the directory should be recursed into.
	//
	// The directory folder is invoked after any recurse limit test, and will be
	// called for directories that were rejected by the item filter (if any).
	IteratorFilterProc					GetRecurseFilter(void **userData=NULL) const;
	void								SetRecurseFilter(IteratorFilterProc theFilter, void *userData=NULL);


	// Get/set the item filter
	//
	// The item filter is called for each item found while iterating, and controls
	// if the item should be considered a match.
	//
	// The item folder is invoked after any pattern match.
	IteratorFilterProc					GetItemFilter(void **userData=NULL) const;
	void								SetItemFilter(IteratorFilterProc theFilter, void *userData=NULL);


	// Get/set the catalog info to retrieve
	//
	// The specified catalog info is available to the filter.
	FSCatalogInfoBitmap					GetCatalogInfo(void) const;
	void								SetCatalogInfo(FSCatalogInfoBitmap theInfo);


	// Get/set the pattern match
	//
	// Prior to invoking the item filter, items are pre-filtered by matching against
	// a pattern applied to the file name or path. If a pattern is set, only matching
	// files are passed to the filter.
	NString								GetPatternName(void) const;
	void								SetPatternName(const NString &thePattern);

	NString								GetPatternPath(void) const;
	void								SetPatternPath(const NString &thePattern);


	// Open/close the iterator
	OSStatus							Open(const NFile &startDir);
	void								Close(void);


	// Get the next file
	//
	// Returns errFSNoMoreItems when the iterator is exhausted.
	OSStatus							GetNext(NFile &theFile, FSCatalogInfo *theInfo=NULL);


	// Get the files
	//
	// Returns all matching items from a (closed) iterator.
	NFileList							GetFiles(const NFile &startDir);


private:
	OSStatus							CollectResults(void);

	OSStatus							CreateIterator( void);
	void								DestroyIterator(void);

	void								ProcessResults(UInt32				numItems,
														const FSRef			*theFSRefs,
														const FSCatalogInfo	*theInfo,
														const HFSUniStr255	*theNames);


private:
	UInt32								mRecurseLimit;
	IteratorFilterProc					mRecurseFilterProc;
	void								*mRecurseFilterData;

	IteratorFilterProc					mItemFilterProc;
	void								*mItemFilterData;

	FSCatalogInfoBitmap					mCatalogInfo;
	NString								mPatternName;
	NString								mPatternPath;

	UInt32								mDepth;
	bool								mStopNow;
	FSIterator							mIterator;
	IteratorStartList					mRemaining;

	NFileList							mResultFiles;
	FSCatalogInfoList					mResultInfo;
};





#endif // NFILEITERATOR_HDR
