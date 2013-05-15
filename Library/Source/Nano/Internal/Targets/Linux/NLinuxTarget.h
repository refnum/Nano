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


	// Invoke sysctl
	template <class T> static T			GetSysctl(int nameMajor, int nameMinor);
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
//		NLinuxTarget::GetSysctl : Invoke sysctl.
//----------------------------------------------------------------------------
template <class T> T NLinuxTarget::GetSysctl(int nameMajor, int nameMinor)
{	int			mibNames[2] = { nameMajor, nameMinor }; 
	T			theResult;
	size_t		theSize;
	int			sysErr;



	// Invoke sysctl
	memset(&theResult, 0x00, sizeof(theResult));

	theSize = sizeof(theResult);
	sysErr  = sysctl(mibNames, 2, &theResult, &theSize, NULL, 0);
	NN_ASSERT_NOERR(sysErr);

	return(theResult);
}




#endif // NLINUX_TARGET_HDR
