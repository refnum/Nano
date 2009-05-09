/*	NAME:
		NMacTarget.h

	DESCRIPTION:
		Mac target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMAC_TARGET_HDR
#define NMAC_TARGET_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMacTarget {
public:
	// Get an FSRef
	static NStatus						GetFSRef(const NString &thePath, FSRef &theFSRef);


	// Get a status code
	static NStatus						GetStatus(OSStatus theErr);


	// Get a file permission
	static SInt8						GetFilePermission(NFilePermission thePermission);


	// Get a file position
	static UInt16						GetFilePosition(NFilePosition thePosition);


	// Get a file reference
	static FSIORefNum					GetFileRefNum(NFileRef theFile);
};





#endif // NMAC_TARGET_HDR
