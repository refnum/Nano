/*	NAME:
		NTargetNetwork.h

	DESCRIPTION:
		Network support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_NETWORK_HDR
#define NTARGET_NETWORK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NURL.h"
#include "NURLRequest.h"
#include "NURLResponse.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetNetwork {
public:
	// Is a URL reachable?
	static bool							IsReachable(const NURL &theURL);


	// URL response
	static NURLResponseRef				URLResponseCreate( NURLResponse   *theResponse);
	static void							URLResponseDestroy(NURLResponseRef theResponse);
	static void							URLResponseStart(  NURLResponseRef theResponse);
	static void							URLResponseCancel( NURLResponseRef theResponse);
};





#endif // NTARGET_NETWORK_HDR
