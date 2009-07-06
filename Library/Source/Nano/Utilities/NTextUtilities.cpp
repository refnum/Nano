/*	NAME:
		NTextUtilities.cpp

	DESCRIPTION:
		Text utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
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
static const NIndex kHexBufferSize										= 128;





//============================================================================
//      NTextUtilities::EscapeEntities : Escape XML entities.
//----------------------------------------------------------------------------
NString NTextUtilities::EscapeEntities(const NString &theValue, const NDictionary &extraEntities)
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
//      NTextUtilities::UnescapeEntities : Unescape XML entities.
//----------------------------------------------------------------------------
NString NTextUtilities::UnescapeEntities(const NString &theValue, const NDictionary &extraEntities)
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
{	NIndex			lineOffset, lineSize, linePad;
	NString			theResult;



	// Generate a hex dump
	lineOffset = 0;

	while (lineOffset < dataSize)
		{
		lineSize = std::min(dataSize - lineOffset, kHexLineWidth);
		linePad  = kHexLineWidth - lineSize;
		
		theResult  += GetHexLine(lineOffset, lineSize, linePad, (const UInt8 *) dataPtr, flowSign, hexOnly);
		lineOffset += lineSize;
		}

	return(theResult);
}





//============================================================================
//      NTextUtilities::GetEntityDictionary : Get the entity dictionary.
//----------------------------------------------------------------------------
#pragma mark -
NDictionary NTextUtilities::GetEntityDictionary(const NDictionary &extraEntities, bool forEscape)
{	NDictionary		theResult;
	bool			didInvert;



	// Get the state we need
	theResult = extraEntities;

	if (!forEscape)
		{
		didInvert = theResult.Invert();
		NN_ASSERT(didInvert);
		}



	// Add the standard entities
	//
	// When unescaping, we also recognise the numeric form of the standard five.
	if (forEscape)
		{
		theResult.SetValue(kEntityValueQuot, kEntityNameQuot);
		theResult.SetValue(kEntityValueAmp,  kEntityNameAmp);
		theResult.SetValue(kEntityValueApos, kEntityNameApos);
		theResult.SetValue(kEntityValueLt,   kEntityNameLt);
		theResult.SetValue(kEntityValueGt,   kEntityNameGt);
		}
	else
		{
		theResult.SetValue(kEntityNameQuot, kEntityValueQuot);
		theResult.SetValue(kEntityNameAmp,  kEntityValueAmp);
		theResult.SetValue(kEntityNameApos, kEntityValueApos);
		theResult.SetValue(kEntityNameLt,   kEntityValueLt);
		theResult.SetValue(kEntityNameGt,   kEntityValueGt);

		theResult.SetValue(kEntityNumberQuot, kEntityValueQuot);
		theResult.SetValue(kEntityNumberAmp,  kEntityValueAmp);
		theResult.SetValue(kEntityNumberApos, kEntityValueApos);
		theResult.SetValue(kEntityNumberLt,   kEntityValueLt);
		theResult.SetValue(kEntityNumberGt,   kEntityValueGt);
		}
	
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
NString NTextUtilities::GetHexLine(NIndex lineOffset, NIndex lineSize, NIndex linePad, const UInt8 *theData, char flowSign, bool hexOnly)
{	char		theBuffer[kHexBufferSize];
	NIndex		n, numBytes;
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

			theLine += NString(&theByte, 1);
			}

		theLine += "\n";
		}

	return(theLine);
}





