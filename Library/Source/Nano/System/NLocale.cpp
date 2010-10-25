/*	NAME:
		NLocale.cpp

	DESCRIPTION:
		System locale.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NTargetSystem.h"
#include "NLocale.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NString kNLocaleChangedAllKey									= "*";
const NString kNLocaleIsMetricKey									= "IsMetric";





//============================================================================
//		NLocale::NLocale : Constructor.
//----------------------------------------------------------------------------
NLocale::NLocale(const NString &theID)
{


	// Initialise ourselves
	mID = theID;
	
	
	
	// Register for events
	NTargetSystem::GetLocaleBroadcaster(theID)->AddListener(this);
}





//============================================================================
//		NLocale::~NLocale : Destructor.
//----------------------------------------------------------------------------
NLocale::~NLocale(void)
{
}





//============================================================================
//		NLocale::IsMetric : Is the measurement system metric?
//----------------------------------------------------------------------------
bool NLocale::IsMetric(void) const
{


	// Get the state
	return(NSystemUtilities::GetBoolean(GetValue(kNLocaleIsMetricKey)));
}





//============================================================================
//		NLocale::DoMessage : Handle messages.
//----------------------------------------------------------------------------
#pragma mark -
void NLocale::DoMessage(BroadcastMsg theMsg, const void *msgData)
{


	// Handle the message
	switch (theMsg) {
		case kMsgNLocaleModified:
			BroadcastMessage(kMsgNLocaleModified, msgData);
			break;
		
		default:
			break;
		}
}





//============================================================================
//		NLocale::GetValue : Get a local value.
//----------------------------------------------------------------------------
#pragma mark -
NVariant NLocale::GetValue(const NString &theKey) const
{


	// Get the value
	return(NTargetSystem::GetLocaleValue(mID, theKey));
}




