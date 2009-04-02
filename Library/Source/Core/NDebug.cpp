/*	NAME:
		NDebug.cpp

	DESCRIPTION:
		Nano debugging support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <string.h>

#include "NTargetDebug.h"
#include "NTimeUtilities.h"
#include "NLock.h"
#include "NDebug.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
static const UInt32 kDebugBufferSize									= 1024;





//============================================================================
//      NDebug::NDebug : Constructor.
//----------------------------------------------------------------------------
NDebug::NDebug(void)
{


	// Initialize ourselves
	mShowPath = false;
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
//      NDebug::LogMessage : Log a message.
//----------------------------------------------------------------------------
void NDebug::LogMessage(const char *thePath, UInt32 lineNum, const char *theFormat, ...)
{	va_list		argList;



	// Log the message
	va_start(argList, theFormat);
	LogMessage(theFormat, argList, thePath, lineNum);
	va_end(argList);
}





//============================================================================
//		NDebug::LogMessage : Log a message.
//----------------------------------------------------------------------------
void NDebug::LogMessage(const char *theFormat, ...)
{	va_list		argList;



	// Log the message
	va_start(argList, theFormat);
	LogMessage(theFormat, argList, NULL, 0);
	va_end(argList);
}





//============================================================================
//      NDebug::Get : Get the debug object.
//----------------------------------------------------------------------------
NDebug &NDebug::Get(void)
{   static NDebug   sDebugObject;



    // Get the debug object
    //
    // The debug object is accessed through a reference to a local static, to
    // ensure it can be invoked irrespective of static initialisation order.
    return(sDebugObject);
}





//============================================================================
//      NDebug::LogMessage : Log a message.
//----------------------------------------------------------------------------
#pragma mark -
void NDebug::LogMessage(const char *theFormat, const va_list &argList, const char *thePath, UInt32 lineNum)
{	StLock			acquireLock(*GetLock());
	const char		*fileName;
	std::string		theMsg;



	// Construct the message
	theMsg += Format("[%lu] ", NTimeUtilities::GetTimeInMS());
	
	if (thePath != NULL)
		{
		fileName = mShowPath ? thePath : GetFileName(thePath);
		theMsg += Format("%s:%ld: ", fileName, lineNum);
		}

	theMsg += Format(theFormat, argList);
	theMsg += "\n";



	// Print it out
	LogMessage(theMsg.c_str());
}





//============================================================================
//      NDebug::Format : Format a string.
//----------------------------------------------------------------------------
std::string NDebug::Format(const char *theFormat, ...)
{	std::string		theResult;
	va_list			argList;



	// Format the string
	va_start(argList, theFormat);
	theResult = Format(theFormat, argList);
	va_end(argList);

	return(theResult);
}





//============================================================================
//      NDebug::Format : Format a string.
//----------------------------------------------------------------------------
std::string NDebug::Format(const char *theFormat, const va_list &argList)
{	char	theBuffer[kDebugBufferSize];



	// Format the string
	vsnprintf(theBuffer, kDebugBufferSize, theFormat, argList);

	return(std::string(theBuffer));
}





//============================================================================
//      NDebug::GetFileName : Get the file name.
//----------------------------------------------------------------------------
const char *NDebug::GetFileName(const char *thePath)
{	const char	*fileName;



	// Get the file name
	fileName = strrchr(thePath, '/');

	if (fileName == NULL)
		fileName = strrchr(thePath, '\\');

	if (fileName != NULL)
		fileName++;



	// Handle failure
	if (fileName == NULL)
		fileName = thePath;

	return(fileName);
}





//============================================================================
//      NDebug::GetLock : Get the lock.
//----------------------------------------------------------------------------
NSpinLock *NDebug::GetLock(void)
{	static NSpinLock	sLock;


	// Get the lock
	return(&sLock);
}
