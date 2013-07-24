/*	NAME:
		NWindows.h

	DESCRIPTION:
		Windows support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NWINDOWS_HDR
#define NWINDOWS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPoint.h"
#include "NSize.h"
#include "NRectangle.h"
#include "NString.h"

#include <objidl.h>





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Nano to Windows
inline POINT ToWN(const NPoint &thePoint)
{	POINT	theResult;

	theResult.x = (LONG) thePoint.x;
	theResult.y = (LONG) thePoint.y;

	return(theResult);
}

inline SIZE ToWN(const NSize &theSize)
{	SIZE	theResult;

	theResult.cx = (LONG) theSize.width;
	theResult.cy = (LONG) theSize.height;

	return(theResult);
}

inline RECT ToWN(const NRectangle &theRect)
{	RECT	theResult;

	SetRect(&theResult, (LONG) theRect.GetMinX(),
						(LONG) theRect.GetMinY(),
						(LONG) theRect.GetMaxX(),
						(LONG) theRect.GetMaxY());
	return(theResult);
}

inline const TCHAR *ToWN(const NString &theString)
{
	return(theString.GetUTF16());
}

inline LARGE_INTEGER ToWN(const int64_t &theValue)
{	LARGE_INTEGER	theResult;

	theResult.QuadPart = theValue;
	return(theResult);
}

inline ULARGE_INTEGER ToWN(const uint64_t &theValue)
{	ULARGE_INTEGER	theResult;

	theResult.QuadPart = theValue;
	return(theResult);
}

inline void ToWN(uint64_t theValue, DWORD &valueHigh, DWORD &valueLow)
{
	valueHigh = (DWORD) ((theValue >> 32) & 0xFFFFFFFF);
	valueLow  = (DWORD) ((theValue >>  0) & 0xFFFFFFFF);
}



// Windows to Nano
inline NPoint ToNN(const POINT &thePoint)
{
	return(NPoint((float32_t) thePoint.x, (float32_t) thePoint.y));
}

inline NPoint ToNN(const POINTS &thePoint)
{
	return(NPoint((float32_t) thePoint.x, (float32_t) thePoint.y));
}

inline NPoint ToNN(const POINTL &thePoint)
{
	return(NPoint((float32_t) thePoint.x, (float32_t) thePoint.y));
}

inline NSize ToNN(const SIZE &thePoint)
{
	return(NSize((float32_t) thePoint.cx, (float32_t) thePoint.cy));
}

inline NRectangle ToNN(const RECT &theRect)
{
	return(NRectangle(	(float32_t)  theRect.left,
						(float32_t)  theRect.top,
						(float32_t) (theRect.right  - theRect.left),
						(float32_t) (theRect.bottom - theRect.top)));
}

inline NRectangle ToNN(const RECTL &theRect)
{
	return(NRectangle(	(float32_t)  theRect.left,
						(float32_t)  theRect.top,
						(float32_t) (theRect.right  - theRect.left),
						(float32_t) (theRect.bottom - theRect.top)));
}

inline NString ToNN(const TCHAR *theString)
{
	return(NString(theString, kNStringLength, kNStringEncodingUTF16));
}

inline int64_t ToNN(const LARGE_INTEGER &theValue)
{
	return(theValue.QuadPart);
}

inline uint64_t ToNN(const ULARGE_INTEGER &theValue)
{
	return(theValue.QuadPart);
}

inline uint64_t ToNN(DWORD valueHigh, DWORD valueLow)
{
	return( (((uint64_t) valueHigh) << 32) | ((uint64_t) valueLow) );
}





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Win32
#define WNSafeCloseHandle(_handle)											\
	do																		\
		{	BOOL	_wasOK;													\
																			\
		if ((_handle) != INVALID_HANDLE_VALUE)								\
			{																\
			_wasOK = CloseHandle((HANDLE) (_handle));						\
			NN_ASSERT(_wasOK);												\
																			\
			(_handle) = INVALID_HANDLE_VALUE;								\
			}																\
		}																	\
	while (false)



// COM
#define WNSafeRelease(_object)												\
	do																		\
		{																	\
		if ((_object) != NULL)												\
			{																\
			(_object)->Release();											\
			(_object) = NULL;												\
			}																\
		}																	\
	while (false)


// Nano
#define REQUIRE_SUCCESS(_winErr)									REQUIRE(SUCCEEDED((_winErr)))

#if NN_DEBUG
	#define NN_ASSERT_SUCCESS(_winErr)								NN_ASSERT(SUCCEEDED((_winErr)))
	
	#define NN_LOG_LASTERR()												\
		do																	\
			{	LPTSTR		theStr;											\
				DWORD		theErr;											\
																			\
			theErr = GetLastError();										\
																			\
			if (!FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER |			\
								FORMAT_MESSAGE_FROM_SYSTEM,					\
								NULL, theErr, 0,							\
								(LPTSTR) &theStr, 0, NULL))					\
				NN_LOG("Windows Error: %d", theErr);						\
			else															\
				{															\
				NN_LOG("Windows Error: %@", ToNN(theStr));					\
				LocalFree(theStr);											\
				}															\
			}																\
		while (0)

#else
	#define NN_ASSERT_SUCCESS(_winErr)								do { } while (0)
	#define NN_LOG_LASTERR()										do { } while (0)
#endif





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class StCOM : public NUncopyable {
public:
										StCOM(DWORD coInit=COINIT_MULTITHREADED);
	virtual							   ~StCOM(void);
};




#endif // NWINDOWS_HDR





