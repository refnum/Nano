/*	NAME:
		NLinuxTarget.h

	DESCRIPTION:
		Linux target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NLINUX_TARGET_HDR
#define NLINUX_TARGET_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/sysctl.h>





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NLinuxTarget {
public:
	// Convert a status code
	static NStatus						ConvertSysErr(int sysErr);


	// Convert file state
	static const char				   *ConvertFilePermission(NFilePermission thePermission);
	static int							ConvertFilePosition(  NFilePosition   thePosition);
};







#endif // NLINUX_TARGET_HDR
