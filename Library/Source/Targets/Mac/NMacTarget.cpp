/*	NAME:
		NMacTarget.cpp

	DESCRIPTION:
		Mac target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMacTarget.h"





//============================================================================
//		NMacTarget::GetStatus : Convert an error code.
//----------------------------------------------------------------------------
NStatus NMacTarget::GetStatus(OSStatus theErr)
{	NStatus		theResult;



	// Convert the error
	switch (theErr) {
		case noErr:				theResult = kNoErr;				break;
		case memFullErr:		theResult = kNErrMemFull;		break;
		case paramErr:			theResult = kNErrBadParam;		break;
		case kMPTimeoutErr:		theResult = kNErrTimeout;		break;

		default:
			NN_LOG("Unable to convert %ld", theErr);
			theResult = kNErrBadParam;
			break;
		}
	
	return(theResult);
}


