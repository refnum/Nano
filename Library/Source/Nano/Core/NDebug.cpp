/*	NAME:
		NDebug.cpp

	DESCRIPTION:
		Nano debugging support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTimeUtilities.h"
#include "NTargetSystem.h"
#include "NTargetPOSIX.h"
#include "NString.h"
#include "NLock.h"
#include "NDebug.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
static const NIndex kPrefixBufferSize									= 1024;





//============================================================================
//      Public functions
//----------------------------------------------------------------------------
//		NDebug_LogMessage : Log a message from C.
//----------------------------------------------------------------------------
void NDebug_LogMessage(const char *thePath, UInt32 lineNum, const char *theMsg, ...)
{	char		theBuffer[kPrefixBufferSize];
	NString		finalMsg;
	va_list		argList;



	// Construct the message
	//
	// We must use a copy-data string, since our buffer is local.
	va_start(argList, theMsg);
	vsnprintf(theBuffer, sizeof(theBuffer), theMsg, argList);
	va_end(argList);

	finalMsg = NString(theBuffer, kNStringLength);



	// Log the message
	NDebug::Get()->LogMessage(thePath, lineNum, finalMsg);
}





//============================================================================
//      NDebug::NDebug : Constructor.
//----------------------------------------------------------------------------
NDebug::NDebug(void)
{


	// Initialize ourselves
	mShowPath    = false;
	mDebugOutput = NTargetSystem::DebugLog;
}





//============================================================================
//      NDebug::~NDebug : Destructor.
//----------------------------------------------------------------------------
NDebug::~NDebug(void)
{
}





//============================================================================
//      NDebug::ShowPath : Get our show path state.
//----------------------------------------------------------------------------
bool NDebug::ShowPath(void) const
{


	// Get our state
	return(mShowPath);
}





//============================================================================
//      NDebug::SetShowPath : Set our show path state.
//----------------------------------------------------------------------------
void NDebug::SetShowPath(bool showPath)
{


	// Set our state
	mShowPath = showPath;
}





//============================================================================
//      NDebug::GetDebugOutput : Get the debug output callback.
//----------------------------------------------------------------------------
DebugOutputProc NDebug::GetDebugOutput(void) const
{


	// Get the callback
	return(mDebugOutput);
}





//============================================================================
//      NDebug::SetDebugOutput : Set the debug output callback.
//----------------------------------------------------------------------------
void NDebug::SetDebugOutput(DebugOutputProc debugOutput)
{


	// Set the callback
	if (debugOutput == NULL)
		debugOutput = NTargetSystem::DebugLog;
	
	mDebugOutput = debugOutput;
}





//============================================================================
//      NDebug::LogMessage : Log a message.
//----------------------------------------------------------------------------
void NDebug::LogMessage(const char *thePath, UInt32 lineNum, const NString &theMsg)
{	char				thePrefix[kPrefixBufferSize];
	StLock				acquireLock(GetLock());
	UInt32				timeStamp;
	const char			*fileName;
	NString				finalMsg;



	// Get the state we need
	timeStamp = (UInt32) (NTimeUtilities::GetUpTime() / kNTimeMillisecond);
	fileName  = mShowPath ? thePath : GetFileName(thePath);



	// Construct the message
	//
	// We must use a copy-data string, since our buffer is local.
	NTargetPOSIX::snprintf(thePrefix, sizeof(thePrefix), "[%lu] %s:%ld: ", timeStamp, fileName, (long) lineNum);

	finalMsg  = NString(thePrefix, kNStringLength);
	finalMsg += theMsg;
	finalMsg += "\n";



	// Print it out
	mDebugOutput(finalMsg.GetUTF8());
}





//============================================================================
//      NDebug::Get : Get the debug object.
//----------------------------------------------------------------------------
NDebug *NDebug::Get(void)
{   static NDebug   sDebugObject;



    // Get the debug object
    //
    // The debug object is accessed through a reference to a local static, to
    // ensure it can be invoked irrespective of static initialisation order.
    return(&sDebugObject);
}





//============================================================================
//      NDebug::GetFileName : Get the file name.
//----------------------------------------------------------------------------
#pragma mark -
const char *NDebug::GetFileName(const char *thePath)
{	const char		*fileName;



	// Get the file name
	fileName = strrchr(thePath, '/');

	if (fileName == NULL)
		fileName = strrchr(thePath, '\\');

	if (fileName != NULL)
		fileName++;



	// Handle failure
	if (fileName == NULL || *fileName == 0x00)
		fileName = thePath;

	return(fileName);
}





//============================================================================
//      NDebug::GetLock : Get the lock.
//----------------------------------------------------------------------------
NMutexLock &NDebug::GetLock(void)
{	static NMutexLock	sLock;


	// Get the lock
	return(sLock);
}
