/*	NAME:
		NURLResponse.cpp

	DESCRIPTION:
		URL response.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetNetwork.h"
#include "NThread.h"
#include "NURLResponse.h"





//============================================================================
//		NURLResponse::NURLResponse : Constructor.
//----------------------------------------------------------------------------
NURLResponse::NURLResponse(const NURLRequest &theRequest)
{


	// Initialise ourselves
	mRequest  = theRequest;
	mResponse = NTargetNetwork::URLResponseCreate(this);
	
	mDelegateData     = NULL;
	mDelegateFinished = NULL;
}





//============================================================================
//		NURLResponse::~NURLResponse : Destructor.
//----------------------------------------------------------------------------
NURLResponse::~NURLResponse(void)
{


	// Clean up
	NTargetNetwork::URLResponseDestroy(mResponse);
}





//============================================================================
//		NURLResponse::Start : Start the connection.
//----------------------------------------------------------------------------
void NURLResponse::Start(void)
{


	// Start the connection
	NTargetNetwork::URLResponseStart(mResponse);
}





//============================================================================
//		NURLResponse::Cancel : Cancel the connection.
//----------------------------------------------------------------------------
void NURLResponse::Cancel(void)
{


	// Cancel the connection
	NTargetNetwork::URLResponseCancel(mResponse);
}





//============================================================================
//		NURLResponse::GetRequest : Get the request.
//----------------------------------------------------------------------------
NURLRequest NURLResponse::GetRequest(void) const
{


	// Get the request
	return(mRequest);
}





//============================================================================
//		NURLResponse::GetDelegateDataFunctor : Get the data functor.
//----------------------------------------------------------------------------
NURLDelegateDataFunctor NURLResponse::GetDelegateDataFunctor(void) const
{


	// Get the functor
	return(mDelegateData);
}





//============================================================================
//		NURLResponse::GetDelegateFinishedFunctor : Get the finished functor.
//----------------------------------------------------------------------------
NURLDelegateFinishedFunctor NURLResponse::GetDelegateFinishedFunctor(void) const
{


	// Get the functor
	return(mDelegateFinished);
}





//============================================================================
//		NURLResponse::SetDelegateDataFunctor : Set the data functor.
//----------------------------------------------------------------------------
void NURLResponse::SetDelegateDataFunctor(const NURLDelegateDataFunctor &theFunctor)
{


	// Set the functor
	mDelegateData = theFunctor;
}





//============================================================================
//		NURLResponse::SetDelegateFinishedFunctor : Set the finished functor.
//----------------------------------------------------------------------------
void NURLResponse::SetDelegateFinishedFunctor(const NURLDelegateFinishedFunctor &theFunctor)
{


	// Set the functor
	mDelegateFinished = theFunctor;
}





//============================================================================
//		NURLResponse::WaitForReply : Wait for the reply.
//----------------------------------------------------------------------------
NStatus NURLResponse::WaitForReply(NData &theData)
{	bool		areDone;
	NStatus		theErr;



	// Get the state we need
	SetDelegateDataFunctor(    BindSelf(NURLResponse::WaitData,     &theData, _1));
	SetDelegateFinishedFunctor(BindSelf(NURLResponse::WaitFinished, &theErr, &areDone, _1));

	areDone = false;
	theErr  = kNErrInternal;



	// Wait for the reply
	Start();
	
	while (!areDone)
		NThread::Sleep();
	
	return(theErr);
}





//============================================================================
//		NURLResponse::DelegateData : Invoke the delegate.
//----------------------------------------------------------------------------
#pragma mark -
void NURLResponse::DelegateData(const NData &theData)
{


	// Invoke the delegate
	if (mDelegateData != NULL)
		mDelegateData(theData);
}





//============================================================================
//		NURLResponse::DelegateFinished : Invoke the delegate.
//----------------------------------------------------------------------------
void NURLResponse::DelegateFinished(NStatus theErr)
{


	// Invoke the delegate
	if (mDelegateFinished != NULL)
		mDelegateFinished(theErr);
}





//============================================================================
//		NURLResponse::WaitData : Wait data delegate method.
//----------------------------------------------------------------------------
#pragma mark -
void NURLResponse::WaitData(NData *theResult, const NData &theData)
{


	// Update the result
	theResult->AppendData(theData);
}





//============================================================================
//		NURLResponse::WaitFinished : Wait finished delegate method.
//----------------------------------------------------------------------------
void NURLResponse::WaitFinished(NStatus *theResult, bool *areDone, NStatus theErr)
{


	// Update the result
	*theResult = theErr;
	*areDone   = true;
}





