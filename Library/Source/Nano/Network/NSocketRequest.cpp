/*	NAME:
		NSocketRequest.cpp

	DESCRIPTION:
		Socket request.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSocketRequest.h"





//============================================================================
//		NSocketRequest::NSocketRequest : Constructor.
//----------------------------------------------------------------------------
NSocketRequest::NSocketRequest(const NData &theData)
{


	// Validate our parameters
	NN_ASSERT(!theData.IsEmpty());



	// Initialise ourselves
	mData      = theData;
	mProcessed = 0;

	mStatus    = kNoErr;
	mSemaphore = NULL;
}





//============================================================================
//		NSocketRequest::~NSocketRequest : Destructor.
//----------------------------------------------------------------------------
NSocketRequest::~NSocketRequest(void)
{
}





//============================================================================
//		NSocketRequest::IsFinished : Is the request finished?
//----------------------------------------------------------------------------
bool NSocketRequest::IsFinished(void) const
{	bool	isDone;



	// Check our state
	isDone = (mProcessed == mData.GetSize());
	
	return(isDone);
}





//============================================================================
//		NSocketRequest::GetSize : Get the size.
//----------------------------------------------------------------------------
NIndex NSocketRequest::GetSize(void) const
{


	// Get the size
	return(mData.GetSize());
}





//============================================================================
//		NSocketRequest::GetFinishedData : Get the finished data.
//----------------------------------------------------------------------------
NData NSocketRequest::GetFinishedData(void) const
{


	// Validate our state
	NN_ASSERT(IsFinished());
	
	
	
	// Get the data
	return(mData);
}





//============================================================================
//		NSocketRequest::ProcessedData : Mark data as having been processed.
//----------------------------------------------------------------------------
void NSocketRequest::ProcessedData(NIndex theSize)
{


	// Validate our state
	NN_ASSERT(!IsFinished());
	
	
	
	// Process the data
	mProcessed += theSize;
	NN_ASSERT(mProcessed <= mData.GetSize());
}





//============================================================================
//		NSocketRequest::GetRemainingSize : Get the remaining size.
//----------------------------------------------------------------------------
NIndex NSocketRequest::GetRemainingSize(void) const
{	NIndex	theSize;



	// Validate our state
	NN_ASSERT(!IsFinished());
	
	
	
	// Get the size
	theSize = mData.GetSize() - mProcessed;
	NN_ASSERT(theSize >= 1);
	
	return(theSize);
}





//============================================================================
//		NSocketRequest::GetRemainingData : Get the remaining data.
//----------------------------------------------------------------------------
const UInt8 *NSocketRequest::GetRemainingData(void) const
{


	// Validate our state
	NN_ASSERT(!IsFinished());



	// Get the data
	return(mData.GetData(mProcessed));
}





//============================================================================
//		NSocketRequest::GetRemainingData : Get the remaining data.
//----------------------------------------------------------------------------
UInt8 *NSocketRequest::GetRemainingData(void)
{


	// Validate our state
	NN_ASSERT(!IsFinished());



	// Get the data
	return(mData.GetData(mProcessed));
}





//============================================================================
//		NSocketRequest::GetStatus : Get the status.
//----------------------------------------------------------------------------
NStatus NSocketRequest::GetStatus(void) const
{


	// Get the status
	return(mStatus);
}





//============================================================================
//		NSocketRequest::SetStatus : Set the status.
//----------------------------------------------------------------------------
void NSocketRequest::SetStatus(NStatus theErr)
{


	// Set the status
	mStatus = theErr;
}





//============================================================================
//		NSocketRequest::GetSemaphore : Get the semaphore.
//----------------------------------------------------------------------------
NSemaphore *NSocketRequest::GetSemaphore(void) const
{


	// Get the semaphore
	return(mSemaphore);
}





//============================================================================
//		NSocketRequest::SetSemaphore : Set the semaphore.
//----------------------------------------------------------------------------
void NSocketRequest::SetSemaphore(NSemaphore *theSemaphore)
{


	// Set the semaphore
	mSemaphore = theSemaphore;
}



