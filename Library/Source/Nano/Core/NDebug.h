/*	NAME:
		NDebug.h

	DESCRIPTION:
		Nano debugging support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
	#define NN_ASSERT(__test)																					\
		do																										\
			{																									\
			if (!(__test))																						\
				NDebug_LogMessage(	__FILE__, __LINE__,															\
									"Assertion failed: %s", #__test);											\
			}																									\
		while(0)

	#define NN_ASSERT_MSG(__test, __message, ...)																\
		do																										\
			{																									\
			if (!(__test))																						\
				NDebug_LogMessage(	__FILE__, __LINE__,															\
									"Assertion failed: %s (" __message ")", #__test, ##__VA_ARGS__);			\
			}																									\
		while(0)

	#define NN_ASSERT_NOERR(__error)																			\
		do																										\
			{																									\
			if ((__error) != 0)																					\
				NDebug_LogMessage(	__FILE__, __LINE__,															\
									"Error: %ld", __error);														\
			}																									\
		while(0)

	#define NN_LOG(...)																							\
		do																										\
			{																									\
			NDebug_LogMessage(__FILE__, __LINE__, __VA_ARGS__);													\
			}																									\
		while(0)

#else
	#define NN_ASSERT(__test)									do { } while (0)
	#define NN_ASSERT_MSG(__test, __message, ...)				do { } while (0)
	#define NN_ASSERT_NOERR(__error)							do { } while (0)
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
	#define NN_ASSERT(__test)																					\
		do																										\
			{																									\
			if (!(__test))																						\
				NDebug::Get()->LogMessage(	__FILE__, __LINE__,													\
											"Assertion failed: %s", #__test);									\
			}																									\
		while(0)

	#define NN_ASSERT_MSG(__test, __message, ...)																\
		do																										\
			{																									\
			if (!(__test))																						\
				NDebug::Get()->LogMessage(	__FILE__, __LINE__,													\
											"Assertion failed: %s (" __message ")", #__test, ##__VA_ARGS__);	\
			}																									\
		while(0)

	#define NN_ASSERT_NOERR(__error)																			\
		do																										\
			{																									\
			if ((__error) != kNoErr)																			\
				NDebug::Get()->LogMessage(	__FILE__, __LINE__,													\
											"Error: %ld", __error);												\
			}																									\
		while(0)

	#define NN_LOG(...)																							\
		do																										\
			{																									\
			NDebug::Get()->LogMessage(__FILE__, __LINE__, __VA_ARGS__);											\
			}																									\
		while(0)

#else
	#define NN_ASSERT(__test)									do { } while (0)
	#define NN_ASSERT_MSG(__test, __message, ...)				do { } while (0)
	#define NN_ASSERT_NOERR(__error)							do { } while (0)
	#define NN_LOG(...)											do { } while (0)
#endif


// Alignment
#define NN_ASSERT_ALIGNED_2(_value)								NN_ASSERT(NN_ALIGNED_TO(_value, sizeof(UInt16)))
#define NN_ASSERT_ALIGNED_4(_value)								NN_ASSERT(NN_ALIGNED_TO(_value, sizeof(UInt32)))
#define NN_ASSERT_ALIGNED_8(_value)								NN_ASSERT(NN_ALIGNED_TO(_value, sizeof(UInt65)))





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NMutex;





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
	void								LogMessage(const char *thePath, UInt32 lineNum, const NString &msgFormat, NN_FORMAT_ARGS);
	void								LogMessage(const char *thePath, UInt32 lineNum, const char    *msgFormat, NN_FORMAT_ARGS);


    // Get the debug instance
    static NDebug					   *Get(void);


private:
	const char						   *GetFileName(const char *thePath);
	NMutex							   &GetLock(void);


private:
	bool								mShowPath;
	DebugOutputProc						mDebugOutput;
};





#endif // !defined(__cplusplus)


#endif // NDEBUG_HDR


