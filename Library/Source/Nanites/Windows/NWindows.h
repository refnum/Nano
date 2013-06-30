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

inline LARGE_INTEGER ToWN(const SInt64 &theValue)
{	LARGE_INTEGER	theResult;

	theResult.QuadPart = theValue;
	return(theResult);
}

inline ULARGE_INTEGER ToWN(const UInt64 &theValue)
{	ULARGE_INTEGER	theResult;

	theResult.QuadPart = theValue;
	return(theResult);
}

inline void ToWN(UInt64 theValue, DWORD &valueHigh, DWORD &valueLow)
{
	valueHigh = (DWORD) ((theValue >> 32) & 0xFFFFFFFF);
	valueLow  = (DWORD) ((theValue >>  0) & 0xFFFFFFFF);
}



// Windows to Nano
inline NPoint ToNN(const POINT &thePoint)
{
	return(NPoint((Float32) thePoint.x, (Float32) thePoint.y));
}

inline NPoint ToNN(const POINTS &thePoint)
{
	return(NPoint((Float32) thePoint.x, (Float32) thePoint.y));
}

inline NPoint ToNN(const POINTL &thePoint)
{
	return(NPoint((Float32) thePoint.x, (Float32) thePoint.y));
}

inline NSize ToNN(const SIZE &thePoint)
{
	return(NSize((Float32) thePoint.cx, (Float32) thePoint.cy));
}

inline NRectangle ToNN(const RECT &theRect)
{
	return(NRectangle(	(Float32)  theRect.left,
						(Float32)  theRect.top,
						(Float32) (theRect.right  - theRect.left),
						(Float32) (theRect.bottom - theRect.top)));
}

inline NRectangle ToNN(const RECTL &theRect)
{
	return(NRectangle(	(Float32)  theRect.left,
						(Float32)  theRect.top,
						(Float32) (theRect.right  - theRect.left),
						(Float32) (theRect.bottom - theRect.top)));
}

inline NString ToNN(const TCHAR *theString)
{
	return(NString(theString, kNStringLength, kNStringEncodingUTF16));
}

inline SInt64 ToNN(const LARGE_INTEGER &theValue)
{
	return(theValue.QuadPart);
}

inline UInt64 ToNN(const ULARGE_INTEGER &theValue)
{
	return(theValue.QuadPart);
}

inline UInt64 ToNN(DWORD valueHigh, DWORD valueLow)
{
	return( (((UInt64) valueHigh) << 32) | ((UInt64) valueLow) );
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





