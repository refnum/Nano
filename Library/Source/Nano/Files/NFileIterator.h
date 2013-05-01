/*	NAME:
		NFileIterator.h

	DESCRIPTION:
		File iterator.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NFILEITERATOR_HDR
#define NFILEITERATOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Iterator start points
typedef struct {
	NFile				theFile;
	NIndex				theDepth;
} IteratorStart;


// Filters
typedef nfunctor<bool (const NFile &theFile, bool *stopNow)>		NFilterFunctor;


// Lists
typedef std::vector<IteratorStart>									IteratorStartList;
typedef IteratorStartList::iterator									IteratorStartListIterator;
typedef IteratorStartList::const_iterator							IteratorStartListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFileIterator {
public:
										NFileIterator(void);
	virtual							   ~NFileIterator(void);


	// Get/set the recurse limit
	//
	// A limit of kNIndexNone, the default, implies no limit to recursion.
	NIndex								GetRecurseLimit(void) const;
	void								SetRecurseLimit(NIndex theLimit);


	// Get/set the recurse filter
	//
	// The recurse filter controls if a directory will be recursed into.
	//
	// The recurse filter is called for all directories within the recurse limit,
	// independently of the item or pattern filters.
	NFilterFunctor						GetRecurseFilter(void) const;
	void								SetRecurseFilter(const NFilterFunctor &theFilter);


	// Get/set the item filter
	//
	// The item filter controls if a file/directory will be added to the results.
	//
	// The item filter is called for all items that pass the pattern filters.
	NFilterFunctor						GetItemFilter(void) const;
	void								SetItemFilter(const NFilterFunctor &theFilter);


	// Get/set the pattern filter
	//
	// Prior to invoking the item filter functor, items are pre-filtered by matching
	// their name or path against a pattern. If a pattern is set, only matching items
	// are passed to the item filter.
	//
	// Patterns are case-insensitive.
	NString								GetPatternName(void) const;
	void								SetPatternName(const NString &thePattern);

	NString								GetPatternPath(void) const;
	void								SetPatternPath(const NString &thePattern);


	// Is the iterator open?
	bool								IsOpen(void) const;


	// Open/close the iterator
	NStatus								Open(const NFile &theRoot);
	void								Close(void);


	// Get the next file
	//
	// Returns true if a valid file was returned.
	bool								GetNext(NFile &theFile);


	// Get the files
	//
	// Returns all matching items from a (closed) iterator.
	NFileList							GetFiles(const NFile &startDir);


private:
	void								ContinueScan(void);


private:
	NIndex								mRecurseLimit;
	NFilterFunctor						mRecurseFilter;
	
	NFilterFunctor						mItemFilter;
	NString								mItemPatternName;
	NString								mItemPatternPath;

	bool								mIsOpen;
	bool								mStopScan;
	NIndex								mScanDepth;
	NFileList							mScanResult;
	IteratorStartList					mScanRemaining;
};





#endif // NFILEITERATOR_HDR
