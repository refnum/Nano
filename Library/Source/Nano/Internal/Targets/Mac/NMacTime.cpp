/*	NAME:
		NMacTime.cpp

	DESCRIPTION:
		Mac time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/sysctl.h>

#include "NTargetTime.h"





//============================================================================
//		NTargetTime::GetTime : Get the time.
//----------------------------------------------------------------------------
NTime NTargetTime::GetTime(void)
{


	// Get the time
	return(CFAbsoluteTimeGetCurrent());
}





//============================================================================
//		NTargetTime::GetUpTime : Get the time since boot.
//----------------------------------------------------------------------------
NTime NTargetTime::GetUpTime(void)
{	int					mibNames[2] = { CTL_KERN, KERN_BOOTTIME }; 
	struct timeval		timeBoot, timeNow, theDelta;
	size_t				theSize;
	NTime				theTime;
	int					sysErr;



	// Get the state we need
	theSize = sizeof(timeBoot);
	sysErr  = sysctl(mibNames, 2, &timeBoot, &theSize, NULL, 0);
	NN_ASSERT_NOERR(sysErr);

	sysErr |= gettimeofday(&timeNow, NULL);
	NN_ASSERT_NOERR(sysErr);



	// Get the time since boot
	theTime = 0.0;

	if (sysErr == 0)
		{
		timersub(&timeNow, &timeBoot, &theDelta);
		theTime = ((NTime) theDelta.tv_sec) + (((NTime) theDelta.tv_usec) / 1000000.0);
		}
	
	return(theTime);
}


