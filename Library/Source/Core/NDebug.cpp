/*	NAME:
		NDebug.cpp

	DESCRIPTION:
		Nano debugging support.
		
		Since the debug class sits underneath every other service, it can not
		use objects such as strings or memory buffers to perform its work.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDebug.h"





//============================================================================
//		NDebug::Get : Get the debug object.
//----------------------------------------------------------------------------
NDebug &NDebug::Get(void)
{	static NDebug	sDebugObject;



	// Get the debug object
	//
	// The debug object is accessed through a reference to a local static, to
	// ensure it can be invoked irrespective of static initialisation order.
	return(sDebugObject);
}





//============================================================================
//		NDebug::LogMessage : Log a message.
//----------------------------------------------------------------------------
void NDebug::LogMessage(const char *fileName, UInt32 lineNum, const char *theFormat, ...)
{	va_list		argList;



	// Log the message
	va_start(argList, theFormat);

	LogMessage(theFormat, argList, fileName, lineNum);

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
//		NDebug::LogMessage : Log a message.
//----------------------------------------------------------------------------
#pragma mark -
void NDebug::LogMessage(const char *theFormat, va_list argList, const char *fileName, UInt32 lineNum)
{	CFMutableStringRef		cfMessage;
	CFStringRef				cfFormat;
	UInt32					theTime;



	// Create our strings
	cfMessage = CFStringCreateMutable(NULL, 0);
	cfFormat  = CFStringCreateWithCString(NULL, theFormat, kCFStringEncodingUTF8);

	if (cfMessage == NULL || cfFormat == NULL)
		return;



	// Build the message
	theTime = (UInt32) (GetCurrentEventTime() / kEventDurationMillisecond);

	CFStringAppendFormat(cfMessage, NULL, CFSTR("[%lu] "), theTime);
	
	if (fileName != NULL)
		CFStringAppendFormat(cfMessage, NULL, CFSTR("%s:%ld: "), fileName, lineNum);
	
	CFStringAppendFormatAndArguments(cfMessage, NULL, cfFormat, argList);
	CFStringAppend(cfMessage, CFSTR("\n"));



	// Print it out
	CFShow(cfMessage);
	
	CFRelease(cfMessage);
	CFRelease(cfFormat);
}
