/*	NAME:
		NMacTarget.h

	DESCRIPTION:
		Mac target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMAC_TARGET_HDR
#define NMAC_TARGET_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/sysctl.h>

#include "NCFObject.h"
#include "NVariant.h"
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMacTarget {
public:
	// Convert a status code
	static NStatus						ConvertCFStreamError(const CFStreamError &streamErr);
	static NStatus						ConvertSysErr(  int      sysErr);
	static NStatus						ConvertOSStatus(OSStatus  osErr);


	// Convert file state
	static const char				   *ConvertFilePermission(NFilePermission thePermission);
	static int							ConvertFilePosition(  NFilePosition   thePosition);


	// Convert between Nano/CF objects
	static NCFObject					ConvertObjectToCF(const NVariant  &theValue);
	static NVariant						ConvertCFToObject(const NCFObject &theValue);


	// Invoke sysctl
	template <class T> static T			GetSysctl(int nameMajor, int nameMinor);
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
//		NMacTarget::GetSysctl : Invoke sysctl.
//----------------------------------------------------------------------------
template <class T> T NMacTarget::GetSysctl(int nameMajor, int nameMinor)
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









#endif // NMAC_TARGET_HDR
