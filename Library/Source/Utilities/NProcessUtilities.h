/*	NAME:
		NProcessUtilities.h

	DESCRIPTION:
		Process utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPROCESSUTILITIES_HDR
#define NPROCESSUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NProcess.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NProcessUtilities {
public:
	// Get the processes
	static NProcessList					GetProcesses(void);
	

	// Transform the current process
	static void							Transform(ProcessApplicationTransformState theState=kProcessTransformToForegroundApplication);


	// Launch a replacement for the current process
	//
	// If no path is supplied, the executable of the current process
	// is restarted once the current process has exited.
	static void							LaunchReplacement(const NString &thePath="");
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline bool operator ==(const ProcessSerialNumber &value1, const ProcessSerialNumber &value2)
{
	return(value1.highLongOfPSN == value2.highLongOfPSN &&
		   value1.lowLongOfPSN  == value2.lowLongOfPSN);
}

inline bool operator !=(const ProcessSerialNumber &value1, const ProcessSerialNumber &value2)
{
	return(value1.highLongOfPSN != value2.highLongOfPSN ||
		   value1.lowLongOfPSN  != value2.lowLongOfPSN);
}

inline bool operator <(const ProcessSerialNumber &value1, const ProcessSerialNumber &value2)
{
	return(value1.highLongOfPSN < value2.highLongOfPSN &&
		   value1.lowLongOfPSN  < value2.lowLongOfPSN);
}





#endif // NPROCESSUTILITIES_HDR


