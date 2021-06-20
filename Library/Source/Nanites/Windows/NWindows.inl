/*	NAME:
		NWindows.inl

	DESCRIPTION:
		Windows support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------



//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
//		ToWN : Convert NPoint to POINT.
//----------------------------------------------------------------------------
inline POINT ToWN(const NPoint &thePoint)
{	POINT	theResult;

	theResult.x = LONG(thePoint.x);
	theResult.y = LONG(thePoint.y);

	return(theResult);
}

//=============================================================================
//		ToWN : Convert NSize to SIZE.
//-----------------------------------------------------------------------------
inline SIZE ToWN(const NSize &theSize)
{	SIZE	theResult;

	theResult.cx = LONG(theSize.width);
	theResult.cy = LONG(theSize.height);

	return(theResult);
}

//=============================================================================
//		ToWN : Convert NRectangle to RECT.
//-----------------------------------------------------------------------------
inline RECT ToWN(const NRectangle &theRect)
{	RECT	theResult;

	SetRect(&theResult, LONG(theRect.GetMinX()),
						LONG(theRect.GetMinY()),
						LONG(theRect.GetMaxX()),
						LONG(theRect.GetMaxY()));
	return(theResult);
}

//=============================================================================
//		ToWN : Convert NString to TCHAR*.
//-----------------------------------------------------------------------------
inline const TCHAR *ToWN(const NString &theString)
{
	return(theString.GetUTF16());
}

//=============================================================================
//		ToWN : Convert int64_t to LARGE_INTEGER.
//-----------------------------------------------------------------------------
inline LARGE_INTEGER ToWN(const int64_t &theValue)
{	LARGE_INTEGER	theResult;

	theResult.QuadPart = theValue;
	return(theResult);
}

//=============================================================================
//		ToWN : Convert uint64_t to ULARGE_INTEGER.
//-----------------------------------------------------------------------------
inline ULARGE_INTEGER ToWN(const uint64_t &theValue)
{	ULARGE_INTEGER	theResult;

	theResult.QuadPart = theValue;
	return(theResult);
}

//=============================================================================
//		ToWN : Convert uint64_t to DWORD + DWORD.
//-----------------------------------------------------------------------------
inline void ToWN(uint64_t theValue, DWORD &valueHigh, DWORD &valueLow)
{
	valueHigh = DWORD((theValue >> 32) & 0xFFFFFFFF);
	valueLow  = DWORD((theValue >>  0) & 0xFFFFFFFF);
}



//=============================================================================
//		ToWN : Convert POINT to NPoint.
//-----------------------------------------------------------------------------
inline NPoint ToNN(const POINT &thePoint)
{
	return(NPoint(float64_t(thePoint.x), float64_t(thePoint.y));
}

//=============================================================================
//		ToWN : Convert POINTS to NPoint.
//-----------------------------------------------------------------------------
inline NPoint ToNN(const POINTS &thePoint)
{
	return(NPoint(float64_t(thePoint.x), float64_t(thePoint.y));
}

//=============================================================================
//		ToWN : Convert POINTL to NPoint.
//-----------------------------------------------------------------------------
inline NPoint ToNN(const POINTL &thePoint)
{
	return(NPoint(float64_t(thePoint.x), float64_t(thePoint.y));
}

//=============================================================================
//		ToWN : Convert SIZE to NSize.
//-----------------------------------------------------------------------------
inline NSize ToNN(const SIZE &thePoint)
{
	return(NSize(float64_t(thePoint.cx), float64_t(thePoint.cy));
}

//=============================================================================
//		ToWN : Convert RECT to NRectangle.
//-----------------------------------------------------------------------------
inline NRectangle ToNN(const RECT &theRect)
{
	return(NRectangle(	float64_t(  theRect.left),
						float64_t(  theRect.top),
						float64_t( theRect.right  - theRect.left),
						float64_t( theRect.bottom - theRect.top)));
}

//=============================================================================
//		ToWN : Convert RECTL to NRectangle.
//-----------------------------------------------------------------------------
inline NRectangle ToNN(const RECTL &theRect)
{
	return(NRectangle(	float64_t(  theRect.left),
						float64_t(  theRect.top),
						float64_t (theRect.right  - theRect.left),
						float64_t (theRect.bottom - theRect.top)));
}

//=============================================================================
//		ToWN : Convert TCHAR* to NString.
//-----------------------------------------------------------------------------
inline NString ToNN(const TCHAR *theString)
{
	static_assert(sizeof(TCHAR) == sizeof(utf16_t));
	return(NString(reinterpret_casst<const utf16_t*>(theString)));
}

//=============================================================================
//		ToWN : Convert LARGE_INTEGER to int64_t.
//-----------------------------------------------------------------------------
inline int64_t ToNN(const LARGE_INTEGER &theValue)
{
	return(theValue.QuadPart);
}

//=============================================================================
//		ToWN : Convert ULARGE_INTEGER to uint64_t.
//-----------------------------------------------------------------------------
inline uint64_t ToNN(const ULARGE_INTEGER &theValue)
{
	return(theValue.QuadPart);
}

//=============================================================================
//		ToWN : Convert DWORD + DWORD to uint64_t.
//-----------------------------------------------------------------------------
inline uint64_t ToNN(DWORD valueHigh, DWORD valueLow)
{
	return uint64_t((uint64_t(valueHigh) << 32) | uint64_t(valueLow));
}







//============================================================================
//		NScopedCOM::NScopedCOM : Constructor.
//----------------------------------------------------------------------------
NScopedCOM::NScopedCOM(DWORD coInit)
{


	// Initialise COM
	HRESULT winErr = CoInitializeEx(nullptr, coInit);
	NN_EXPECT_SUCCESS(winErr);
}





//============================================================================
//		NScopedCOM::~NScopedCOM : Destructor.
//----------------------------------------------------------------------------
NScopedCOM::~NScopedCOM(void)
{


	// Clean up
	CoUninitialize();
}


