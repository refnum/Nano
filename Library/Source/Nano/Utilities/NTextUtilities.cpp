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
// Misc
static const UInt32 kHexLineWidth							= 16;
static const UInt32 kHexBufferSize							= 128;





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

			theLine += NString(&theByte, 1);
			}

		theLine += "\n";
		}

	return(theLine);
}





