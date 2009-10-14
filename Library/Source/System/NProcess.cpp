/*	NAME:
		NProcess.cpp

	DESCRIPTION:
		Process object.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/sysctl.h>

#include "NProcessUtilities.h"
#include "NProcess.h"





//============================================================================
//		NProcess::NProcess : Constructor.
//----------------------------------------------------------------------------
NProcess::NProcess(const ProcessSerialNumber &thePSN)
{


	// Initialize ourselves
	mPSN = thePSN;
}





//============================================================================
//		NProcess::NProcess : Constructor.
//----------------------------------------------------------------------------
NProcess::NProcess(pid_t thePID)
{	OSStatus	theErr;



	// Initialize ourselves
	mPSN = kInvalidPSN;

	if (thePID != kInvalidPID)
		{
		theErr = GetProcessForPID(thePID, &mPSN);
		NN_ASSERT(theErr == noErr || theErr == procNotFound);
		}
}





//============================================================================
//		NProcess::NProcess : Constructor.
//----------------------------------------------------------------------------
NProcess::NProcess(void)
{


	// Initialize ourselves
	mPSN.highLongOfPSN = 0;
	mPSN.lowLongOfPSN  = kCurrentProcess;
}





//============================================================================
//		NProcess::~NProcess : Destructor.
//----------------------------------------------------------------------------
NProcess::~NProcess(void)
{
}





//============================================================================
//		NProcess::IsValid : Is the process valid?
//----------------------------------------------------------------------------
bool NProcess::IsValid(void) const
{


	// Check our state
	return(mPSN != kInvalidPSN);
}





//============================================================================
//		NProcess::GetParent : Get the parent process.
//----------------------------------------------------------------------------
NProcess NProcess::GetParent(void) const
{


	// Get the parent
	return(GetParentPID());
}





//============================================================================
//		NProcess::GetInfo : Get the info dictionary.
//----------------------------------------------------------------------------
NDictionary NProcess::GetInfo(void) const
{	NDictionary		theDictionary;
	pid_t			parentPID;
	CFStringRef		cfString;



	// Get the dictionary
	//
	// Although the dictionary contains a mixture of documented and undocumented
	// keys, we extend it with some useful values requested by rdar://4888007.
	if (theDictionary.Set(ProcessInformationCopyDictionary(&mPSN, kProcessDictionaryIncludeAllInformationMask)))
		{
		// Get the state we need
		cfString  = NULL;
		parentPID = GetParentPID();

		CopyProcessName(&mPSN, &cfString);



		// Update the dictionary
		if (cfString != NULL)
			theDictionary.SetValueString(kProcessNameKey, cfString);
		
		if (parentPID != kInvalidPID)
			theDictionary.SetValueUInt32(kProcessParentPIDKey, parentPID);
		
		CFSafeRelease(cfString);
		}
	
	return(theDictionary);
}





//============================================================================
//		NProcess::IsAtFront : Is the process at the front?
//----------------------------------------------------------------------------
bool NProcess::IsAtFront(void) const
{	ProcessSerialNumber		frontPSN;
	OSStatus				theErr;



	// Get the state we need
	frontPSN = kInvalidPSN;
	theErr   = GetFrontProcess(&frontPSN);
	NN_ASSERT_NOERR(theErr);



	// Check our state
	return(*this == frontPSN);
}





//============================================================================
//		NProcess::SetAtFront : Set if the process is at the front.
//----------------------------------------------------------------------------
void NProcess::SetAtFront(OptionBits theFlags)
{	OSStatus	theErr;



	// Set our state
	theErr = SetFrontProcessWithOptions(&mPSN, theFlags);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NProcess::IsVisible : Is the process visible?
//----------------------------------------------------------------------------
bool NProcess::IsVisible(void) const
{


	// Get our visibility
	return(IsProcessVisible(&mPSN));
}





//============================================================================
//		NProcess::SetVisible : Set the visibility.
//----------------------------------------------------------------------------
void NProcess::SetVisible(bool isVisible)
{	OSStatus	theErr;



	// Set our visibility
	theErr = ShowHideProcess(&mPSN, isVisible);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NProcess::== : Are two processes equal?
//----------------------------------------------------------------------------
const bool NProcess::operator == (const NProcess &theProcess) const
{	Boolean		areSame;
	OSStatus	theErr;



	// Compare the proceses
	theErr = SameProcess(&mPSN, &theProcess.mPSN, &areSame);
	NN_ASSERT_NOERR(theErr);

	if (theErr != noErr)
		areSame = false;
	
	return(areSame);
}





//============================================================================
//		NProcess::!= : Are two processes not equal?
//----------------------------------------------------------------------------
const bool NProcess::operator != (const NProcess &theProcess) const
{


	// Check our state
	return(!(*this == theProcess));
}





//============================================================================
//		NProcess::< : Comparison operator.
//----------------------------------------------------------------------------
const bool NProcess::operator < (const NProcess &theProcess) const
{


	// Check our state
	return(mPSN < theProcess.mPSN);
}





//============================================================================
//		NProcess::ProcessSerialNumber : Cast operator.
//----------------------------------------------------------------------------
NProcess::operator const ProcessSerialNumber*(void) const
{


	// Get our state
	return(&mPSN);
}





//============================================================================
//		NProcess::pid_t : Cast operator.
//----------------------------------------------------------------------------
NProcess::operator pid_t(void) const
{	pid_t		thePID;
	OSStatus	theErr;



	// Get our state
	theErr = GetProcessPID(&mPSN, &thePID);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		thePID = kInvalidPID;
	
	return(thePID);
}





//============================================================================
//		NProcess::GetParentPID : Get the parent process ID.
//----------------------------------------------------------------------------
#pragma mark -
pid_t NProcess::GetParentPID(void) const
{	int						infoTable[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, 0 };
	pid_t					ourPID, parentPID;
	struct kinfo_proc		procInfo;
	size_t					theSize;



	// Get the state we need
	ourPID    = *this;
	parentPID = kInvalidPID;

	infoTable[3] = ourPID;
	theSize      = sizeof(procInfo);



	// Get our parent PID
    if (sysctl(infoTable, 4, &procInfo, &theSize, NULL, 0) == 0 && theSize != 0)
		parentPID = procInfo.kp_eproc.e_ppid;
	
	return(parentPID);
}

