/*	NAME:
		NSocketRequest.h

	DESCRIPTION:
		Socket request.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSOCKETREQUEST_HDR
#define NSOCKETREQUEST_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRefCounted.h"
#include "NSemaphore.h"
#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NSocketRequest;


// Lists
typedef std::vector<NSocketRequest*>								NSocketRequestList;
typedef NSocketRequestList::iterator								NSocketRequestListIterator;
typedef NSocketRequestList::const_iterator							NSocketRequestListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSocketRequest : public NRefCounted {
public:
										NSocketRequest(const NData &theData);
	virtual							   ~NSocketRequest(void);


	// Is the request finished?
	//
	// The request is finished when all of the data has been processed.
	bool								IsFinished(void) const;


	// Get the total size
	NIndex								GetSize(void) const;


	// Get the finished data
	//
	// Returns the final data for the request.
	NData								GetFinishedData(void) const;


	// Mark data as having been processed
	//
	// Data is processed sequentially, from start to finish.
	void								ProcessedData(NIndex theSize);


	// Get the remaining size
	//
	// The 'remaining' data is the unprocessed section of the request.
	NIndex								GetRemainingSize(void) const;


	// Get the remaining data
	//
	// The 'remaining' data is the unprocessed section of the request.
	const UInt8						   *GetRemainingData(void) const;
	UInt8							   *GetRemainingData(void);


	// Get/set the status
	NStatus								GetStatus(void) const;
	void								SetStatus(NStatus theErr);


	// Get/set the semaphore
	NSemaphore						   *GetSemaphore(void) const;
	void								SetSemaphore(NSemaphore *theSemaphore);


private:
	NData								mData;
	NIndex								mProcessed;

	NStatus								mStatus;
	NSemaphore						   *mSemaphore;
};





#endif // NSOCKETREQUEST_HDR


