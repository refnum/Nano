/*	NAME:
		NTextUtilities.cpp

	DESCRIPTION:
		Text utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMutex.h"
#include "NTargetPOSIX.h"
#include "NTextUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Entities
static const NString kEntityNameQuot									= "&quot;";
static const NString kEntityNameAmp										= "&amp;";
static const NString kEntityNameApos									= "&apos;";
static const NString kEntityNameLt										= "&lt;";
static const NString kEntityNameGt										= "&gt;";

static const NString kEntityValueQuot									= "\"";
static const NString kEntityValueAmp									= "&";
static const NString kEntityValueApos									= "'";
static const NString kEntityValueLt										= "<";
static const NString kEntityValueGt										= ">";

static const NString kEntityNumberQuot									= "&#34;";
static const NString kEntityNumberAmp									= "&#38;";
static const NString kEntityNumberApos									= "&#39;";
static const NString kEntityNumberLt									= "&#60;";
static const NString kEntityNumberGt									= "&#62;";


// Hex
static const NIndex kHexLineWidth										= 16;
static const NIndex kHexBufferReserve									= 6;





//============================================================================
//      NTextUtilities::EncodeEntities : Encode XML entities.
//----------------------------------------------------------------------------
NString NTextUtilities::EncodeEntities(const NString &theValue, const NDictionary &extraEntities)
{	NDictionary		theEntities;
	NString			theResult;



	// Get the state we need
	theResult   = theValue;
	theEntities = GetEntityDictionary(extraEntities, true);



	// Process the entities
	//
	// Ampersands must be processed first, since they are part of the escape sequence.
	theEntities.RemoveKey(kEntityValueAmp);
	NTextUtilities::ProcessEntities(kEntityValueAmp, kEntityNameAmp, &theResult);

	theEntities.ForEach(BindFunction(NTextUtilities::ProcessEntities, _1, _2, &theResult));

	return(theResult);
}





//============================================================================
//      NTextUtilities::DecodeEntities : Decode XML entities.
//----------------------------------------------------------------------------
NString NTextUtilities::DecodeEntities(const NString &theValue, const NDictionary &extraEntities)
{	NDictionary		theEntities;
	NString			theResult;



	// Get the state we need
	theResult   = theValue;
	theEntities = GetEntityDictionary(extraEntities, false);



	// Process the entities
	theEntities.ForEach(BindFunction(NTextUtilities::ProcessEntities, _1, _2, &theResult));
	
	return(theResult);
}





//============================================================================
//      NTextUtilities::GetHexDump : Get a hex dump.
//----------------------------------------------------------------------------
NString NTextUtilities::GetHexDump(NIndex dataSize, const void *dataPtr, char flowSign, bool hexOnly)
{	NIndex		lineOffset, lineSize, linePad;
	NData		theResult;



	// Get the state we need
	lineOffset = 0;
	theResult.Reserve(dataSize * kHexBufferReserve);



	// Generate a hex dump
	while (lineOffset < dataSize)
		{
		lineSize = std::min(dataSize - lineOffset, kHexLineWidth);
		linePad  = kHexLineWidth - lineSize;
		
		theResult  += GetHexLine(lineOffset, lineSize, linePad, (const uint8_t *) dataPtr, flowSign, hexOnly);
		lineOffset += lineSize;
		}

	return(NString(theResult));
}





//============================================================================
//		NTextUtilities::ConvertLineEndings : Convert line endings.
//----------------------------------------------------------------------------
NString NTextUtilities::ConvertLineEndings(const NString &theString, const NString &newLine)
{	NString		theResult;



	// Convert the line endings
	theResult = theString;

	theResult.ReplaceAll(kNLineEndingDOS, kNLineEndingUnix);
	theResult.ReplaceAll(kNLineEndingMac, kNLineEndingUnix);

	if (newLine != kNLineEndingUnix)
		theResult.ReplaceAll(kNLineEndingUnix, newLine);
	
	return(theResult);
}





//============================================================================
//		NTextUtilities::GetArguments : Get a NULL-terminated argument list.
//----------------------------------------------------------------------------
NStringList NTextUtilities::GetArguments(const char *arg1, ...)
{	va_list			argList;
	NStringList		theArgs;



	// Get the arguments
	va_start(argList, arg1);
	theArgs = GetArguments(argList, arg1);
	va_end(argList);

	return(theArgs);
}





//============================================================================
//		NTextUtilities::GetArguments : Get a NULL-terminated argument list.
//----------------------------------------------------------------------------
NStringList NTextUtilities::GetArguments(va_list argList, const char *arg1)
{	const char		*argPtr;
	NStringList		theArgs;



	// Get the arguments
	argPtr = arg1;
	
	while (argPtr != NULL)
		{
		theArgs.push_back(NString(argPtr, kNStringLength));
		argPtr = va_arg(argList, const char *);
		}

	return(theArgs);
}





#pragma mark private
//============================================================================
//      NTextUtilities::GetEntityDictionary : Get the entity dictionary.
//----------------------------------------------------------------------------
NDictionary NTextUtilities::GetEntityDictionary(const NDictionary &extraEntities, bool forEncode)
{	static bool				sStandardInited = false;
	static NDictionary		sStandardEncode;
	static NDictionary		sStandardDecode;
	static NMutex			sStandardLock;

	NDictionary		theResult, theExtra;
	bool			didInvert;



	// Populate the standard entities
	//
	// When decoding, we also recognise the numeric form of the standard five.
	if (!sStandardInited)
		{
		sStandardLock.Lock();
		
		if (!sStandardInited)
			{
			sStandardEncode.SetValue(kEntityValueQuot,  kEntityNameQuot);
			sStandardEncode.SetValue(kEntityValueAmp,   kEntityNameAmp);
			sStandardEncode.SetValue(kEntityValueApos,  kEntityNameApos);
			sStandardEncode.SetValue(kEntityValueLt,    kEntityNameLt);
			sStandardEncode.SetValue(kEntityValueGt,    kEntityNameGt);
		
			sStandardDecode.SetValue(kEntityNameQuot,   kEntityValueQuot);
			sStandardDecode.SetValue(kEntityNameAmp,    kEntityValueAmp);
			sStandardDecode.SetValue(kEntityNameApos,   kEntityValueApos);
			sStandardDecode.SetValue(kEntityNameLt,     kEntityValueLt);
			sStandardDecode.SetValue(kEntityNameGt,     kEntityValueGt);

			sStandardDecode.SetValue(kEntityNumberQuot, kEntityValueQuot);
			sStandardDecode.SetValue(kEntityNumberAmp,  kEntityValueAmp);
			sStandardDecode.SetValue(kEntityNumberApos, kEntityValueApos);
			sStandardDecode.SetValue(kEntityNumberLt,   kEntityValueLt);
			sStandardDecode.SetValue(kEntityNumberGt,   kEntityValueGt);
			
			sStandardInited = true;
			}
		
		sStandardLock.Unlock();
		}



	// Get the state we need
	theExtra = extraEntities;
	if (!forEncode)
		{
		didInvert = theExtra.Invert();
		NN_ASSERT(didInvert);
		}



	// Get the entities
	theResult = forEncode ? sStandardEncode : sStandardDecode;
	theResult.Join(theExtra);

	return(theResult);
}





//============================================================================
//      NTextUtilities::ProcessEntities : Process entities.
//----------------------------------------------------------------------------
void NTextUtilities::ProcessEntities(const NString &theKey, const NVariant &theValue, NString *theResult)
{	NString		valueString;



	// Validate our parameters
	NN_ASSERT(theValue.IsType(valueString));



	// Update the result
	if (theValue.GetValue(valueString))
		theResult->ReplaceAll(theKey, valueString);
}





//============================================================================
//      NTextUtilities::GetHexLine : Get a line for a hex dump.
//----------------------------------------------------------------------------
NData NTextUtilities::GetHexLine(NIndex lineOffset, NIndex lineSize, NIndex linePad, const uint8_t *theData, char flowSign, bool hexOnly)
{	NIndex		n, m, p, numBytes;
	char		*linePtr;
	NData		theLine;
	uint8_t		theByte;



	// Get the state we need
	numBytes = lineSize + linePad;
	theLine.SetSize(numBytes * kHexBufferReserve);
	
	linePtr = (char *) theLine.GetData();
	m       = 0;



	// Add the header
	if (!hexOnly)
		{
		if (flowSign != 0x00)
			{
			linePtr[m++] = flowSign;
			linePtr[m++] = ' ';
			}

		p = NTargetPOSIX::snprintf(&linePtr[m], 10, "%.8lX ", lineOffset);
		if (p > 0)
			m += p;
		}



	// Add the hex data
	if (hexOnly)
		{
		for (n = 0; n < numBytes; n++)
			{
			if (n < lineSize)
				{
				p = NTargetPOSIX::snprintf(&linePtr[m], 3, "%.2X", theData[lineOffset + n]);
				if (p > 0)
					m += p;
				}
			}
		}
	else
		{
		for (n = 0; n < numBytes; n++)
			{
			if ((n % 4) == 0)
				linePtr[m++] = ' ';
		
			if (n < lineSize)
				{
				p = NTargetPOSIX::snprintf(&linePtr[m], 4, "%.2X ", theData[lineOffset + n]);
				if (p > 0)
					m += p;
				}
			else
				{
				linePtr[m++] = ' ';
				linePtr[m++] = ' ';
				linePtr[m++] = ' ';
				}
			}
		}



	// Add the trailer
	if (!hexOnly)
		{
		linePtr[m++] = ' ';

		for (n = 0; n < numBytes; n++)
			{
			if (n < lineSize)
				{
				theByte = theData[lineOffset + n];
			
				if (theByte == ' ' || !isprint(theByte))
					theByte = '.';
				}
			else
				theByte = ' ';

			linePtr[m++] = (char) theByte;
			}

		linePtr[m++] = '\n';
		}



	// Truncate the line
	NN_ASSERT(m < theLine.GetSize());
	theLine.SetSize(m);

	return(theLine);
}





