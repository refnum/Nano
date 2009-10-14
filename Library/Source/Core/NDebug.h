/*	NAME:
		NDebug.h

	DESCRIPTION:
		Nano debugging support.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDEBUG_HDR
#define NDEBUG_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Macros
//----------------------------------------------------------------------------
#if NN_DEBUG
	#define NN_ASSERT(_test)																					\
		do																										\
			{																									\
			if (!(_test))																						\
				NDebug::Get().LogMessage(__FILE__, __LINE__, "Assertion failed: %s", #_test);					\
			}																									\
		while(0)

	#define NN_ASSERT_MSG(_test, _message)																		\
		do																										\
			{																									\
			if (!(_test))																						\
				NDebug::Get().LogMessage(__FILE__, __LINE__, "Assertion failed: %s (%s)", #_test, _message);	\
			}																									\
		while(0)

	#define NN_ASSERT_NOERR(_error)																				\
		do																										\
			{																									\
			if ((_error) != noErr)																				\
				NDebug::Get().LogMessage(__FILE__, __LINE__, "Error: %ld", _error);								\
			}																									\
		while(0)

	#define NN_LOG(...)																							\
		do																										\
			{																									\
			NDebug::Get().LogMessage(__FILE__, __LINE__, __VA_ARGS__);											\
			}																									\
		while(0)

#else
	#define NN_ASSERT(_test)
	#define NN_ASSERT_MSG(_test, _message)
	#define NN_ASSERT_NOERR(_error)
	#define NN_LOG(...)
#endif





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDebug {
public:
	// Get the debug instance
	static NDebug						&Get(void);


	// Log a message
	void								LogMessage(const char *fileName, UInt32 lineNum, const char *theFormat, ...);
	void								LogMessage(                                      const char *theFormat, ...);


private:
	void								LogMessage(const char *theFormat, va_list argList, const char *fileName, UInt32 lineNum);
};





#endif // NDEBUG_HDR


