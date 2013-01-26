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


	// Get the request size
	NIndex								GetSize(void) const;


	// Get the request data
	//
	// The data should only be retrieved when the request is finished.
	NData								GetData(void) const;


	// Mark data as having been processed
	//
	// Data is processed sequentially, from start to finish.
	void								ProcessedData(NIndex theSize);


	// Get the unprocessed size
	NIndex								GetUnprocessedSize(void) const;


	// Get the unprocessed data
	const UInt8						   *GetUnprocessedData(void) const;
	UInt8							   *GetUnprocessedData(void);


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


