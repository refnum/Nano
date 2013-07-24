/*	NAME:
		NURLResponse.h

	DESCRIPTION:
		URL response.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NURLRESPONSE_HDR
#define NURLRESPONSE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NURLRequest.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef uintptr_t NURLResponseRef;

static const NURLResponseRef kNURLResponseRefNone					= 0;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Functors
typedef nfunctor<void (NIndex theSize, const void *thePtr)>			NURLDelegateDataFunctor;
typedef nfunctor<void (NStatus theErr)>								NURLDelegateFinishedFunctor;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NURLResponse {
public:
										NURLResponse(const NURLRequest &theRequest);
	virtual							   ~NURLResponse(void);


	// Start/stop the connection
	void								Start( void);
	void								Cancel(void);


	// Get the request
	NURLRequest							GetRequest(void) const;


	// Get/set the delegate methods
	//
	// The delegate methods may be invoked by an internal worker thread,
	// and may not be called on the same thread they were assigned on.
	NURLDelegateDataFunctor				GetDelegateDataFunctor(    void) const;
	NURLDelegateFinishedFunctor			GetDelegateFinishedFunctor(void) const;
	
	void								SetDelegateDataFunctor(    const NURLDelegateDataFunctor     &theFunctor);
	void								SetDelegateFinishedFunctor(const NURLDelegateFinishedFunctor &theFunctor);


	// Wait for the reply
	//
	// Starts the response, and blocks the current thread until it has completed.
	NStatus								WaitForReply(NData &theData);


public:
	// Invoke the delegate
	void								DelegateData(NIndex theSize, const void *thePtr);
	void								DelegateFinished(NStatus theErr);


private:
	void								WaitData(    NData   *theResult, NIndex theSize, const void *thePtr);
	void								WaitFinished(NStatus *theResult, bool *areDone, NStatus theErr);


protected:
	NURLRequest							mRequest;
	NURLResponseRef						mResponse;
	
	NURLDelegateDataFunctor				mDelegateData;
	NURLDelegateFinishedFunctor			mDelegateFinished;
};




#endif // NURLRESPONSE_HDR


