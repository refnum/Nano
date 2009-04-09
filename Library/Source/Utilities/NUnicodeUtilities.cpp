/*	NAME:
		NUnicodeUtilities.cpp

	DESCRIPTION:
		Unicode utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "ConvertUTF.h"

#include "NUnicodeUtilities.h"





//============================================================================
//		NUnicodeUtilities::ConvertUTF8ToUTF16 : Convert UTF8 to UTF16.
//----------------------------------------------------------------------------
NStatus NUnicodeUtilities::ConvertUTF8ToUTF16(const NData &srcData, NData &dstData)
{	const UTF8				*srcStart, *srcEnd;
	UTF16					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize() * 2);

	do
		{
		srcStart = (const UTF8 *) srcData.GetData();
		srcEnd   = srcStart +     srcData.GetSize();

		dstStart = (UTF16 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF8toUTF16(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NUnicodeUtilities::ConvertUTF8ToUTF32 : Convert UTF8 to UTF32.
//----------------------------------------------------------------------------
NStatus NUnicodeUtilities::ConvertUTF8ToUTF32(const NData &srcData, NData &dstData)
{	const UTF8				*srcStart, *srcEnd;
	UTF32					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize() * 4);

	do
		{
		srcStart = (const UTF8 *) srcData.GetData();
		srcEnd   = srcStart +     srcData.GetSize();

		dstStart = (UTF32 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF8toUTF32(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NUnicodeUtilities::ConvertUTF16ToUTF8 : Convert UTF16 to UTF8.
//----------------------------------------------------------------------------
NStatus NUnicodeUtilities::ConvertUTF16ToUTF8(const NData &srcData, NData &dstData)
{	const UTF16				*srcStart, *srcEnd;
	UTF8					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize());

	do
		{
		srcStart = (const UTF16 *) srcData.GetData();
		srcEnd   = srcStart +      srcData.GetSize();

		dstStart = (UTF8 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF16toUTF8(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NUnicodeUtilities::ConvertUTF16ToUTF32 : Convert UTF16 to UTF32.
//----------------------------------------------------------------------------
NStatus NUnicodeUtilities::ConvertUTF16ToUTF32(const NData &srcData, NData &dstData)
{	const UTF16				*srcStart, *srcEnd;
	UTF32					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize() * 2);

	do
		{
		srcStart = (const UTF16 *) srcData.GetData();
		srcEnd   = srcStart +      srcData.GetSize();

		dstStart = (UTF32 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF16toUTF32(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NUnicodeUtilities::ConvertUTF32ToUTF8 : Convert UTF32 to UTF8.
//----------------------------------------------------------------------------
NStatus NUnicodeUtilities::ConvertUTF32ToUTF8(const NData &srcData, NData &dstData)
{	const UTF32				*srcStart, *srcEnd;
	UTF8					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize());

	do
		{
		srcStart = (const UTF32 *) srcData.GetData();
		srcEnd   = srcStart +      srcData.GetSize();

		dstStart = (UTF8 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF32toUTF8(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NUnicodeUtilities::ConvertUTF32ToUTF16 : Convert UTF32 to UTF16.
//----------------------------------------------------------------------------
NStatus NUnicodeUtilities::ConvertUTF32ToUTF16(const NData &srcData, NData &dstData)
{	const UTF32				*srcStart, *srcEnd;
	UTF16					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize());

	do
		{
		srcStart = (const UTF32 *) srcData.GetData();
		srcEnd   = srcStart +      srcData.GetSize();

		dstStart = (UTF16 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF32toUTF16(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NUnicodeUtilities::ProcessUnicode : Process a Unicode conversion.
//----------------------------------------------------------------------------
#pragma mark -
NStatus NUnicodeUtilities::ProcessUnicode(NData &theData, const void *dataEnd, UInt32 theResult)
{	OSStatus	theErr;



	// Process the result
	switch ((ConversionResult) theResult) {
		case conversionOK:
			theData.SetSize(((const UInt8 *) dataEnd) - theData.GetData());
			theErr = kNoErr;
			break;
			
		case targetExhausted:
			theData.SetSize(theData.GetSize() * 2);
			theErr = kNErrExhaustedDst;
			break;
			
		case sourceExhausted:
			theErr = kNErrExhaustedSrc;
			break;

		case sourceIllegal:
			theErr = kNErrMalformed;
			break;

		default:
			NN_LOG("Unknown result: %d", theResult);
			theErr = kNErrInternal;
			break;
		}

	return(theErr);
}


