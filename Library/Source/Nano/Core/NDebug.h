/*	NAME:
		NDebug.h

	DESCRIPTION:
		Nano debugging support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDEBUG_HDR
#define NDEBUG_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"





//============================================================================
//		Build Constants
//----------------------------------------------------------------------------
#ifndef NN_DEBUG
#define NN_DEBUG															1
#endif





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef void (*DebugOutputProc)(const char *theMsg);





//============================================================================
//		Macros
//----------------------------------------------------------------------------
#if NN_DEBUG
	#define NN_ASSERT(_test)																								\
		do																													\
			{																												\
			if (!(_test))																									\
				NDebug::Get()->LogMessage(__FILE__, __LINE__, NStringFormatter().Format("Assertion failed: %s", #_test));	\
			}																												\
		while(0)

	#define NN_ASSERT_MSG(_test, _message, ...)																												\
		do																																					\
			{																																				\
			if (!(_test))																																	\
				NDebug::Get()->LogMessage(__FILE__, __LINE__, NStringFormatter().Format("Assertion failed: %s (" #_message ")", #_test, ##__VA_ARGS__));	\
			}																																				\
		while(0)

	#define NN_ASSERT_NOERR(_error)																							\
		do																													\
			{																												\
			if ((_error) != kNoErr)																							\
				NDebug::Get()->LogMessage(__FILE__, __LINE__, NStringFormatter().Format("Error: %ld", _error));				\
			}																												\
		while(0)

	#define NN_LOG(...)																										\
		do																													\
			{																												\
			NDebug::Get()->LogMessage(__FILE__, __LINE__, NStringFormatter().Format(__VA_ARGS__));							\
			}																												\
		while(0)

#else
	#define NN_ASSERT(_test)
	#define NN_ASSERT_MSG(_test, _message, ...)
	#define NN_ASSERT_NOERR(_error)
	#define NN_LOG(...)
#endif





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NSpinLock;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDebug {
public:
										 NDebug(void);
	virtual								~NDebug(void);


	// Get/set if file paths should be shown
	bool								ShowPath(void) const;
	void								SetShowPath(bool showPath);


	// Get/set the debug output callback
	DebugOutputProc						GetDebugOutput(void) const;
	void								SetDebugOutput(DebugOutputProc debugOutput);


	// Log a message
	void								LogMessage(const char *thePath, UInt32 lineNum, const NStringUTF8 &theMsg);


    // Get the debug instance
    static NDebug						*Get(void);


private:
	const char							*GetFileName(const char *thePath);
	NSpinLock							&GetLock(void);


private:
	bool								mShowPath;
	DebugOutputProc						mDebugOutput;
};





#endif // NDEBUG_HDR


