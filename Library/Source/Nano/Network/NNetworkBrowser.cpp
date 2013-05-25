/*	NAME:
		NNetworkBrowser.cpp

	DESCRIPTION:
		Network service browser.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetNetwork.h"
#include "NNetworkBrowser.h"





//============================================================================
//		NNetworkBrowser::NNetworkBrowser : Constructor.
//----------------------------------------------------------------------------
NNetworkBrowser::NNetworkBrowser(void)
{
}





//============================================================================
//		NNetworkBrowser::~NNetworkBrowser : Destructor.
//----------------------------------------------------------------------------
NNetworkBrowser::~NNetworkBrowser(void)
{


	// Clean up
	StopBrowsing();
}





//============================================================================
//		NNetworkBrowser::IsActive : Is the browser active?
//----------------------------------------------------------------------------
bool NNetworkBrowser::IsActive(void) const
{


	// Check our state
	return(!mBrowsers.empty());
}





//============================================================================
//		NNetworkBrowser::StartBrowsing : Start browsing.
//----------------------------------------------------------------------------
NStatus NNetworkBrowser::StartBrowsing(void)
{	NNetworkBrowserEventFunctor		theFunctor;
	NServiceBrowserRef				theBrowser;
	NStringListConstIterator		theIter;



	// Validate our state
	NN_ASSERT(!IsActive());



	// Check our state
	if (!NTargetNetwork::ServicesAvailable())
		return(kNErrNotSupported);



	// Get the state we need
	theFunctor = BindSelf(NNetworkBrowser::DispatchEvent, _1);



	// Start browsing
	for (theIter = mServices.begin(); theIter != mServices.end(); theIter++)
		{
		theBrowser = NTargetNetwork::ServiceBrowserCreate(*theIter, theFunctor);
		mBrowsers.push_back(theBrowser);
		}
	
	return(kNoErr);
}





//============================================================================
//		NNetworkBrowser::StopBrowsing : Stop browsing.
//----------------------------------------------------------------------------
void NNetworkBrowser::StopBrowsing(void)
{	NServiceBrowserListConstIterator		theIter;



	// Stop browsing
	for (theIter = mBrowsers.begin(); theIter != mBrowsers.end(); theIter++)
		NTargetNetwork::ServiceBrowserDestroy(*theIter);
	
	mBrowsers.clear();
}





//============================================================================
//		NNetworkBrowser::GetEventHandler : Get the event handler.
//----------------------------------------------------------------------------
NNetworkBrowserEventFunctor NNetworkBrowser::GetEventHandler(void)
{


	// Get the handler
	return(mEventHandler);
}





//============================================================================
//		NNetworkBrowser::SetEventHandler : Set the event handler.
//----------------------------------------------------------------------------
void NNetworkBrowser::SetEventHandler(const NNetworkBrowserEventFunctor &theFunctor)
{


	// Set the handler
	mEventHandler = theFunctor;
}





//============================================================================
//		NNetworkBrowser::GetServices : Get the services.
//----------------------------------------------------------------------------
NStringList NNetworkBrowser::GetServices(void) const
{


	// Get the services
	return(mServices);
}





//============================================================================
//		NNetworkBrowser::SetServices : Set the services.
//----------------------------------------------------------------------------
void NNetworkBrowser::SetServices(const NStringList &theServices)
{	bool	wasActive;



	// Stop browsing
	wasActive = IsActive();
	
	if (wasActive)
		StopBrowsing();
	
	
	
	// Update our state
	mServices = theServices;
	
	
	
	// Resume browsing
	if (wasActive)
		StartBrowsing();
}





#pragma mark private
//============================================================================
//		NNetworkBrowser::DispatchEvent : Dispatch an event.
//----------------------------------------------------------------------------
void NNetworkBrowser::DispatchEvent(const NNetworkBrowserEvent &theEvent)
{


	// Dispatch the event
	if (mEventHandler != NULL)
		mEventHandler(theEvent);
}




