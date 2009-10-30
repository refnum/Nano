/*	NAME:
		NTextUtilities.cpp

	DESCRIPTION:
		Text utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <pwd.h>

#include "Nano Build.h"
#include "NCFBundle.h"
#include "NMathUtilities.h"
#include "NGeometryUtilities.h"
#include "NSystemUtilities.h"
#include "NTextUtilities.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
typedef struct {
	NString		number;
	NString		name;
	NString		value;
} EntityInfo;





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Formats
//
// These strings are encoded to property lists, and can never change.
//
// A version number has been included in case "never" has to be broken.
static const char *kFormatPoint								= "0.{%f, %f}";
static const char *kFormatSize								= "0.{%f, %f}";
static const char *kFormatRect								= "0.{{%f, %f}, {%f, %f}}";
static const char *kFormatColor								= "0.{%f, %f, %f, %f}";
static const char *kFormatPointer							= "0.{%p}";


// Misc
static const UInt32 kHexLineWidth							= 16;
static const UInt32 kHexBufferSize							= 128;


// Entities
static const EntityInfo kHTMLEntities[]						= {	{ "&#34;",   "&quot;",  "\"" },
																{ "&#38;",   "&amp;",   "&"  },
																{ "&#39;",   "&apos;",  "'"  },
																{ "&#60;",   "&lt;",    "<"  },
																{ "&#62;",   "&gt;",    ">"  },
																{ "&#160;",  "&nbsp;",  " "  } };





//============================================================================
//		NTextUtilities::GetOSType : Convert from a string.
//----------------------------------------------------------------------------
OSType NTextUtilities::GetOSType(const NString &theString)
{	OSType	theValue;



	// Convert the string
	//
	// OSType values are encoded to strings in big-endian order.
	theValue = 0;
	
	if (theString.GetSize() == 4)
		memcpy(&theValue, theString.GetUTF8(), 4);

	theValue = (OSType) EndianU32_BtoN((UInt32) theValue);
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetSInt64 : Convert from a string.
//----------------------------------------------------------------------------
SInt64 NTextUtilities::GetSInt64(const NString &theString)
{	SInt64	theValue;



	// Convert the string
	if (sscanf(theString.GetUTF8(), "%lld",   &theValue) == 0 &&
		sscanf(theString.GetUTF8(), "%llx",   &theValue) == 0 &&
		sscanf(theString.GetUTF8(), "0x%llx", &theValue) == 0 &&
		sscanf(theString.GetUTF8(), "0X%llx", &theValue) == 0)
		{
		theValue = 0;
		}
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetFloat64 : Convert from a string.
//----------------------------------------------------------------------------
Float64 NTextUtilities::GetFloat64(const NString &theString)
{	Float64	theValue;



	// Convert the string
	theValue = CFStringGetDoubleValue(theString);
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetStr255 : Convert from a string.
//----------------------------------------------------------------------------
void NTextUtilities::GetStr255(const NString &theString, Str255 &theValue, CFStringEncoding cfEncoding)
{


	// Convert the string
	if (!CFStringGetPascalString(theString, theValue, 256, cfEncoding))
		theValue[0] = 0x00;
}





//============================================================================
//		NTextUtilities::GetStr255Hnd : Convert from a string.
//----------------------------------------------------------------------------
//		Note :	Returns a Str255-in-a-Handle, a type used to build QuickTime
//				data references.
//----------------------------------------------------------------------------
Handle NTextUtilities::GetStr255Hnd(const NString &theString)
{	Str255		theValue;
	Handle		theHnd;
	OSStatus	theErr;



	// Convert the string
	GetStr255(theString, theValue);



	// Convert to a handle
	theHnd = NULL;
	theErr = PtrToHand(theValue, &theHnd, theValue[0] + 1);
	NN_ASSERT_NOERR(theErr);
	
	return(theHnd);
}





//============================================================================
//		NTextUtilities::GetPoint : Convert from a string.
//----------------------------------------------------------------------------
HIPoint NTextUtilities::GetPoint(const NString &theString)
{	HIPoint		theValue;



	// Convert the string
	if (sscanf(theString.GetUTF8(), kFormatPoint, &theValue.x, &theValue.y) != 2)
		theValue = kHIPointZero;
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetSize : Convert from a string.
//----------------------------------------------------------------------------
HISize NTextUtilities::GetSize(const NString &theString)
{	HISize		theValue;



	// Convert the string
	if (sscanf(theString.GetUTF8(), kFormatSize, &theValue.width, &theValue.height) != 2)
		theValue = kHISizeZero;
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetRect : Convert from a string.
//----------------------------------------------------------------------------
HIRect NTextUtilities::GetRect(const NString &theString)
{	HIRect		theValue;



	// Get the value
	if (sscanf(theString.GetUTF8(), kFormatRect, &theValue.origin.x,   &theValue.origin.y,
												 &theValue.size.width, &theValue.size.height) != 4)
		theValue = kHIRectZero;
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetColor : Convert from a string.
//----------------------------------------------------------------------------
NColor NTextUtilities::GetColor(const NString &theString)
{	float		r, g, b, a;
	NColor		theValue;



	// Convert the string
	if (sscanf(theString.GetUTF8(), kFormatColor, &r, &g, &b, &a) == 4)
		theValue.SetColor(r, g, b, a);
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetData : Convert from a string.
//----------------------------------------------------------------------------
NData NTextUtilities::GetData(const NString &theString)
{	UInt32			n, theSize;
	NData			theValue;
	UInt8			*dataPtr;
	const char		*textPtr;



	// Validate our parameters
	NN_ASSERT(theString.IsEmpty() || (theString.GetSize() % 2) == 0);



	// Get the state we need
	theSize = theString.GetSize() / 2;
	dataPtr = theValue.AppendData(theSize);
	textPtr = theString.GetUTF8();

	if (theSize == 0 || dataPtr == NULL || textPtr == NULL)
		return(theValue);



	// Convert the string
	for (n = 0; n < theSize; n++)
		{
		sscanf(textPtr, "%2hhx", &dataPtr[n]);
		textPtr += 2;
		}
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::GetPointer : Convert from a pointer.
//----------------------------------------------------------------------------
void *NTextUtilities::GetPointer(const NString &theString)
{	void	*theValue;



	// Convert the strin
	if (sscanf(theString.GetUTF8(), kFormatPointer, &theValue) != 1)
		theValue = NULL;
	
	return(theValue);
}





//============================================================================
//		NTextUtilities::FromOSType : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromOSType(OSType theValue)
{	NString		theString;
	OSType		bigValue;



	// Convert the value
	//
	// OSType values are encoded to strings in big-endian order.
	bigValue  = (OSType) EndianU32_BtoN((UInt32) theValue);
	theString = NString((char *) &bigValue, 4);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromSInt64 : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromSInt64(SInt64 theValue)
{	NString		theString;



	// Convert the value
	theString.Format("%qi", theValue);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromFloat64 : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromFloat64(Float64 theValue)
{	NString		theString;



	// Convert the value
	theString.Format("%lf", theValue);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromStr255 : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromStr255(ConstStr255Param theValue, CFStringEncoding cfEncoding)
{	NString		theString;



	// Convert the value
	theString = NString((const char *) &theValue[1], theValue[0], cfEncoding);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromPoint : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromPoint(const HIPoint &theValue)
{	NString		theString;



	// Convert the value
	theString.Format(kFormatPoint, theValue.x, theValue.y);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromSize : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromSize(const HISize &theValue)
{	NString		theString;



	// Convert the value
	theString.Format(kFormatSize, theValue.width, theValue.height);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromRect : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromRect(const HIRect &theValue)
{	NString		theString;



	// Convert the value
	theString.Format(kFormatRect, theValue.origin.x,   theValue.origin.y,
								  theValue.size.width, theValue.size.height);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromColor : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromColor(const NColor &theValue)
{	NString		theString;



	// Convert the value
	theString.Format(kFormatColor, theValue.GetRed(),  theValue.GetGreen(),
								   theValue.GetBlue(), theValue.GetAlpha());
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromData : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromData(const NData &theValue)
{	UInt32			n, theSize;
	NData			tmpBuffer;
	NString			theString;
	const UInt8		*dataPtr;
	char			*textPtr;



	// Get the state we need
	theSize = theValue.GetSize();
	dataPtr = theValue.GetData();
	textPtr = (char *) tmpBuffer.AppendData((theSize * 2) + 1);

	if (theSize == 0 || dataPtr == NULL || textPtr == NULL)
		return(theString);



	// Convert to a string
	for (n = 0; n < theSize; n++)
		{
		sprintf(textPtr, "%.2X", dataPtr[n]);
		textPtr += 2;
		}
	
	theString = NString(tmpBuffer);
	
	return(theString);
}





//============================================================================
//		NTextUtilities::FromPointer : Convert to a string.
//----------------------------------------------------------------------------
NString NTextUtilities::FromPointer(void *theValue)
{	NString		theString;



	// Convert the value
	theString.Format(kFormatPointer, theValue);

	return(theString);
}





//============================================================================
//		NTextUtilities::EncodeURL : Encode a URL.
//----------------------------------------------------------------------------
NString NTextUtilities::EncodeURL(const NString &theURL, CFStringRef leaveEscaped, CFStringEncoding escapeEncoding)
{	NString		theValue;



	// Encode the URL
	theValue.Set(CFURLCreateStringByAddingPercentEscapes(kCFAllocatorNano, theURL, leaveEscaped, NULL, escapeEncoding));

	return(theValue);
}





//============================================================================
//		NTextUtilities::DecodeURL : Decode a URL.
//----------------------------------------------------------------------------
NString NTextUtilities::DecodeURL(const NString &theURL, CFStringRef leaveEscaped, CFStringEncoding escapeEncoding)
{	NString		theValue;



	// Encode the URL
	theValue.Set(CFURLCreateStringByReplacingPercentEscapesUsingEncoding(kCFAllocatorNano, theURL, leaveEscaped, escapeEncoding));

	return(theValue);
}





//============================================================================
//		NTextUtilities::EncodeEntities : Encode HTML entities.
//----------------------------------------------------------------------------
NString NTextUtilities::EncodeEntities(const NString &theString)
{	NString		theResult;
	UInt32		n;



	// Encode the entities
	theResult = theString;
	
	for (n = 0; n < GET_ARRAY_SIZE(kHTMLEntities); n++)
		theResult.ReplaceAll(kHTMLEntities[n].value, kHTMLEntities[n].name);
	
	return(theResult);
}





//============================================================================
//		NTextUtilities::DecodeEntities : Decode HTML entities.
//----------------------------------------------------------------------------
NString NTextUtilities::DecodeEntities(const NString &theString)
{	NString		theResult;
	UInt32		n;



	// Decode the entities
	theResult = theString;
	
	for (n = 0; n < GET_ARRAY_SIZE(kHTMLEntities); n++)
		{
		theResult.ReplaceAll(kHTMLEntities[n].name,   kHTMLEntities[n].value);
		theResult.ReplaceAll(kHTMLEntities[n].number, kHTMLEntities[n].value);
		}
	
	return(theResult);
}





//============================================================================
//		NTextUtilities::ExpandTilde : Expand a tilde prefix.
//----------------------------------------------------------------------------
NString NTextUtilities::ExpandTilde(const NString &thePath)
{	NString					theResult, pathTilde, pathRemainder;
	const struct passwd		*userInfo;
	CFRange					theRange;



	// Expand the tilde prefix
	if (thePath.StartsWith("~"))
		{
		// Break down the path
		theRange      = thePath.Find("/");
		pathTilde     = thePath.GetString(0, theRange.location);
		pathRemainder = thePath.GetString(theRange.location);



		// Expand the tilde
		if (pathTilde.GetSize() == 1)
			pathTilde = getenv("HOME");
		else
			{
			pathTilde = pathTilde.GetString(1);
			userInfo  = getpwnam(pathTilde.GetUTF8());
			pathTilde = (userInfo != NULL) ? userInfo->pw_dir : "";
			}
		}



	// Build the path
	if (pathTilde.IsNotEmpty())
		theResult = pathTilde + pathRemainder;
	else
		theResult = thePath;
	
	return(theResult);
}





//============================================================================
//		NTextUtilities::NormalizeLineEndings : Normalize line endings.
//----------------------------------------------------------------------------
NString NTextUtilities::NormalizeLineEndings(const NString &theString, const NString &newLine)
{	NString		theResult;



	// Normalize the line endings
	theResult = theString;

	theResult.ReplaceAll(kLineEndingDOS, kLineEndingUnix);
	theResult.ReplaceAll(kLineEndingMac, kLineEndingUnix);
	
	if (newLine != kLineEndingUnix)
		theResult.ReplaceAll(kLineEndingUnix, newLine);
	
	return(theResult);
}





//============================================================================
//		NTextUtilities::CollectArguments : Collect a NULL-terminated argument list.
//----------------------------------------------------------------------------
NStringList NTextUtilities::CollectArguments(const char *arg1, ...)
{	va_list			argList;
	NStringList		theArgs;



	// Collect the arguments
	va_start(argList, arg1);
	theArgs = CollectArguments(argList, arg1);
	va_end(argList);
	
	return(theArgs);
}





//============================================================================
//		NTextUtilities::CollectArguments : Collect a NULL-terminated argument list.
//----------------------------------------------------------------------------
NStringList NTextUtilities::CollectArguments(va_list argList, const char *arg1)
{	const char		*argPtr;
	NStringList		theArgs;



	// Collect the arguments
	argPtr = arg1;
	do
		{
		theArgs.push_back(argPtr);
		argPtr = va_arg(argList, const char *);
		}
	while (argPtr != NULL);

	return(theArgs);
}





//============================================================================
//		NTextUtilities::GetTransferInfo : Get a transfer info string.
//----------------------------------------------------------------------------
NString NTextUtilities::GetTransferInfo(UInt32 dataDone, UInt32 dataTotal, UInt32 bytesPerSec, EventTime timeLeft)
{	NString		theInfo, infoData, infoRate, infoTime;
	NString		theFormat, infoDone, infoTotal;
	float		valueFloat, dataDivisor;
	UInt32		valueInt;
	bool		dataInMb;



	// Get the state we need
	dataInMb    = (dataDone >= kMegabyte || dataTotal >= kMegabyte);
	dataDivisor = (float) dataInMb ? kMegabyte : kKilobyte;



	// Calculate the data
	theFormat  = NBundleString(dataInMb ? kStringTransferProgressMB : kStringTransferProgressKB, "", kNanoStrings);
	valueFloat = ((float) dataDone) / dataDivisor;
	infoDone.Format(theFormat, valueFloat);

	if (dataTotal == (UInt32) -1)
		infoData = infoDone;
	else
		{
		theFormat  = NBundleString(dataInMb ? kStringTransferProgressMB : kStringTransferProgressKB, "", kNanoStrings);
		valueFloat = ((float) dataTotal) / dataDivisor;
		infoTotal.Format(theFormat, valueFloat);

		theFormat = NBundleString(kStringTransferProgressXofY, "", kNanoStrings);
		infoData.Format(theFormat, infoDone, infoTotal);
		}
		
	infoData.ReplaceAll(".0", "");



	// Calculate the rate
	if (bytesPerSec != (UInt32) -1)
		{
		if (bytesPerSec > kMegabyte)
			{
			theFormat = NBundleString(kStringTransferRateMBSec, "", kNanoStrings);
			valueInt  = bytesPerSec / kMegabyte;
			}
		else
			{
			theFormat = NBundleString(kStringTransferRateKBSec, "", kNanoStrings);
			valueInt  = bytesPerSec / kKilobyte;
			}
	
		infoRate.Format(theFormat, valueInt);
		}



	// Calculate the time
	if (NMathUtilities::NotZero(timeLeft))
		{
		if (NMathUtilities::AreEqual(timeLeft, kEventDurationForever))
			{
			theFormat = NBundleString(kStringTransferTimeStalled, "", kNanoStrings);
			valueInt  = 1;
			}
	
		else if (timeLeft >= 90.0f)
			{
			theFormat = NBundleString(kStringTransferTimeMinutes, "", kNanoStrings);
			valueInt  = (UInt32) ceil(timeLeft / 60.0);
			}

		else if (timeLeft >= 50.0f)
			{
			theFormat = NBundleString(kStringTransferTimeMinute, "", kNanoStrings);
			valueInt  = 1;
			}
		
		else if (timeLeft >= 2.0)
			{
			theFormat = NBundleString(kStringTransferTimeSeconds, "", kNanoStrings);
			valueInt  = (UInt32) timeLeft;
			}

		else
			{
			theFormat = NBundleString(kStringTransferTimeSecond, "", kNanoStrings);
			valueInt  = (UInt32) timeLeft;
			}

		infoTime.Format(theFormat, valueInt);
		}



	// Build the string
	theInfo = infoData.GetUTF8();

	if (infoRate.IsNotEmpty())
		theInfo.Format("%@ (%@)", theInfo, infoRate);

	if (infoTime.IsNotEmpty())
		theInfo.Format("%@ - %@", theInfo, infoTime);

	return(theInfo);
}





//============================================================================
//      NTextUtilities::GetHexDump : Get a hex dump.
//----------------------------------------------------------------------------
NString NTextUtilities::GetHexDump(UInt32 theSize, const void *thePtr, char flowSign, bool hexOnly)
{	UInt32			lineOffset, lineSize, linePad;
	NString			theResult;



	// Generate a hex dump
	lineOffset = 0;

	while (lineOffset < theSize)
		{
		lineSize = std::min(theSize - lineOffset, kHexLineWidth);
		linePad  = kHexLineWidth - lineSize;
		
		theResult  += GetHexLine(lineOffset, lineSize, linePad, (const UInt8 *) thePtr, flowSign, hexOnly);
		lineOffset += lineSize;
		}

	return(theResult);
}





//============================================================================
//      NTextUtilities::CombineStrings : Combine a list of strings.
//----------------------------------------------------------------------------
NString NTextUtilities::CombineStrings(const NStringList &theValues, const NString &joinWith, bool skipEmpty)
{	NString						theResult;
	NStringListConstIterator	theIter;



	// Combine the strings
	for (theIter = theValues.begin(); theIter != theValues.end(); theIter++)
		{
		if (skipEmpty && theIter->IsEmpty())
			continue;
		
		theResult += *theIter;

		if ((theIter + 1) != theValues.end())
			theResult += joinWith;
		}
	
	return(theResult);
}





//============================================================================
//      NTextUtilities::GetHexLine : Get a line for a hex dump.
//----------------------------------------------------------------------------
#pragma mark - 
NString NTextUtilities::GetHexLine(UInt32 lineOffset, UInt32 lineSize, UInt32 linePad, const UInt8 *theData, char flowSign, bool hexOnly)
{	char		theBuffer[kHexBufferSize];
	UInt32		n, numBytes;
	NString		theLine;
	char		theByte;



	// Get the state we need
	numBytes = lineSize + linePad;



	// Add the header
	if (!hexOnly)
		{
		if (flowSign != 0x00)
			theLine.Format("%c ", (UInt8) flowSign);

		snprintf(theBuffer, kHexBufferSize, "%.8lX ", lineOffset);

		theLine += theBuffer;
		}



	// Add the hex data
	for (n = 0; n < numBytes; n++)
		{
		if ((n % 4) == 0)
			theLine += " ";
		
		if (n < lineSize)
			{
			snprintf(theBuffer, kHexBufferSize, "%.2X ", theData[lineOffset + n]);
			theLine += theBuffer;
			}
		else
			theLine += "   ";
		}
	
	if (hexOnly)
		theLine.ReplaceAll(" ", "");



	// Add the trailer
	if (!hexOnly)
		{
		theLine += " ";
		
		for (n = 0; n < numBytes; n++)
			{
			if (n < lineSize)
				{
				theByte = theData[lineOffset + n];
			
				if (!isprint(theByte) || theByte == ' ')
					theByte = '.';
				}
			else
				theByte = ' ';

			theLine += theByte;
			}

		theLine += "\n";
		}

	return(theLine);
}





