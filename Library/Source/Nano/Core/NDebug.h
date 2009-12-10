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
//		Build Constants
//----------------------------------------------------------------------------
#ifndef NN_DEBUG
#define NN_DEBUG															1
#endif





//============================================================================
//		C Wrapper
//----------------------------------------------------------------------------
//		Note :	Not as functional as the C++ interface, but allows basic
//				assertions and logging from plain C code that includes Nano.h
//				via a precompiled header to use Nano types/assertions.
//----------------------------------------------------------------------------
#if defined(__cplusplus)
extern "C" void NDebug_LogMessage(const char *thePath, UInt32 lineNum, const char *theMsg, ...);
#endif

#if !defined(__cplusplus)
extern     void NDebug_LogMessage(const char *thePath, UInt32 lineNum, const char *theMsg, ...);

#if NN_DEBUG
	#define NN_ASSERT(_test)																								\
		do																													\
			{																												\
			if (!(_test))																									\
				NDebug_LogMessage(__FILE__, __LINE__, "Assertion failed: %s", #_test);										\
			}																												\
		while(0)

	#define NN_ASSERT_MSG(_test, _message, ...)																				\
		do																													\
			{																												\
			if (!(_test))																									\
				NDebug_LogMessage(__FILE__, __LINE__, "Assertion failed: %s (" #_message ")", #_test, ##__VA_ARGS__);		\
			}																												\
		while(0)

	#define NN_ASSERT_NOERR(_error)																							\
		do																													\
			{																												\
			if ((_error) != 0)																								\
				NDebug_LogMessage(__FILE__, __LINE__, "Error: %ld", _error);												\
			}																												\
		while(0)

	#define NN_LOG(...)																										\
		do																													\
			{																												\
			NDebug_LogMessage(__FILE__, __LINE__, __VA_ARGS__);																\
			}																												\
		while(0)

#else
	#define NN_ASSERT(_test)									do { } while (0)
	#define NN_ASSERT_MSG(_test, _message, ...)					do { } while (0)
	#define NN_ASSERT_NOERR(_error)								do { } while (0)
	#define NN_LOG(...)											do { } while (0)
#endif

#else





//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef void (*DebugOutputProc)(const char *theMsg);





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Primitives
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
	#define NN_ASSERT(_test)									do { } while (0)
	#define NN_ASSERT_MSG(_test, _message, ...)					do { } while (0)
	#define NN_ASSERT_NOERR(_error)								do { } while (0)
	#define NN_LOG(...)											do { } while (0)
#endif


// Alignment
#define NN_ASSERT_ALIGNED_2(_value)								NN_ASSERT(NN_ALIGNED_TO(_value, sizeof(UInt16)))
#define NN_ASSERT_ALIGNED_4(_value)								NN_ASSERT(NN_ALIGNED_TO(_value, sizeof(UInt32)))
#define NN_ASSERT_ALIGNED_8(_value)								NN_ASSERT(NN_ALIGNED_TO(_value, sizeof(UInt65)))





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
	virtual							   ~NDebug(void);


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





#endif // !defined(__cplusplus)


#endif // NDEBUG_HDR


