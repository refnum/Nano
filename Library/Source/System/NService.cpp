/*	NAME:
		NService.cpp

	DESCRIPTION:
		Service support.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAppKitUtilities.h"
#include "NService.h"





//============================================================================
//		NService::NService : Constructor.
//----------------------------------------------------------------------------
NService::NService(const NString &theService, const NString &theValue)
{


	// Initialize ourselves
	mService = theService;
	mValue   = theValue;
}





//============================================================================
//		NService::NService : Constructor.
//----------------------------------------------------------------------------
NService::NService(void)
{
}





//============================================================================
//		NService::~NService : Destructor.
//----------------------------------------------------------------------------
NService::~NService(void)
{
}





//============================================================================
//		NService::GetService : Get the service.
//----------------------------------------------------------------------------
NString NService::GetService(void) const
{


	// Get the service
	return(mService);
}





//============================================================================
//		NService::SetService : Set the service.
//----------------------------------------------------------------------------
void NService::SetService(const NString &theService)
{


	// Set the service
	mService = theService;
}





//============================================================================
//		NService::GetValue : Get the value.
//----------------------------------------------------------------------------
NString NService::GetValue(void) const
{


	// Get the value
	return(mValue);
}





//============================================================================
//		NService::SetValue : Set the value.
//----------------------------------------------------------------------------
void NService::SetValue(const NString &theValue)
{


	// Set the value
	mValue = theValue;
}





//============================================================================
//		NService::Execute : Execute the service.
//----------------------------------------------------------------------------
OSStatus NService::Execute(void) const
{	NSPasteboard		*nsPasteboard;
	StAutoReleasePool	thePool;
	BOOL				wasOK;



	// Execute the service
	nsPasteboard = [NSPasteboard pasteboardWithUniqueName];
	wasOK        = (nsPasteboard != NULL);
	
	if (wasOK)
		{
		[nsPasteboard declareTypes:[NSArray arrayWithObject:NSStringPboardType] owner:NULL];
		[nsPasteboard setString:mValue forType:NSStringPboardType];

		wasOK = NSPerformService(mService, nsPasteboard);
		NN_ASSERT(wasOK);
		
		[nsPasteboard release];
		}

	return(wasOK ? (OSStatus) noErr : (OSStatus) paramErr);
}

