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
//		NMacTarget::GetFSRef : Get an FSRef.
//----------------------------------------------------------------------------
NStatus NMacTarget::GetFSRef(const NString &thePath, FSRef &theFSRef)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(thePath.IsNotEmpty());



	// Get the FSRef
	theErr = FSPathMakeRef((const UInt8 *) thePath.GetUTF8(), &theFSRef, NULL);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		memset(&theFSRef, 0x00, sizeof(theFSRef));

	return(GetStatus(theErr));
}





//============================================================================
//		NMacTarget::GetStatus : Get a status code.
//----------------------------------------------------------------------------
NStatus NMacTarget::GetStatus(OSStatus theErr)
{	NStatus		theResult;



	// Convert the value
	switch (theErr) {
		case noErr:				theResult = kNoErr;					break;
		case paramErr:			theResult = kNErrParam;				break;
		case kMPTimeoutErr:		theResult = kNErrTimeout;			break;
		case memFullErr:		theResult = kNErrMemory;			break;
		case badFormat:			theResult = kNErrMalformed;			break;
		case permErr:			theResult = kNErrPermission;		break;
		case eofErr:			theResult = kNErrExhaustedSrc;		break;
		case fnfErr:			theResult = kNErrNotFound;			break;
		case dskFulErr:			theResult = kNErrDiskFull;			break;

		default:
			NN_LOG("Unable to convert %ld", theErr);
			theResult = kNErrParam;
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NMacTarget::GetFilePermission : Get a file permission.
//----------------------------------------------------------------------------
SInt8 NMacTarget::GetFilePermission(NFilePermission thePermission)
{	SInt8		theResult;



	// Convert the value
	switch (thePermission) {
		case kFilePermissionRead:			theResult = fsRdPerm;		break;
		case kFilePermissionWrite:			theResult = fsWrPerm;		break;
		case kFilePermissionReadWrite:		theResult = fsRdWrPerm;		break;

		default:
			NN_LOG("Unable to convert %ld", thePermission);
			theResult = fsRdWrPerm;
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NMacTarget::GetFilePosition : Get a file position.
//----------------------------------------------------------------------------
UInt16 NMacTarget::GetFilePosition(NFilePosition thePosition)
{	UInt16		theResult;



	// Convert the value
	switch (thePosition) {
		case kFilePositionFromStart:	theResult = fsFromStart;	break;
		case kFilePositionFromMark:		theResult = fsFromMark;		break;
		case kFilePositionFromEnd:		theResult = fsFromLEOF;		break;

		default:
			NN_LOG("Unable to convert %ld", thePosition);
			theResult = fsFromStart;
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NMacTarget::GetFileRefNum : Get a file reference.
//----------------------------------------------------------------------------
FSIORefNum NMacTarget::GetFileRefNum(NFileRef theFile)
{


	// Convert the value
	return((FSIORefNum) ((SInt64) theFile));
}



