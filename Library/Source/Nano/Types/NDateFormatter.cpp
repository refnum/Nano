/*	NAME:
		NDateFormatter.cpp

	DESCRIPTION:
		Date formatter.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDateFormatter.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Literals
//
// Must be #defines to ensure formatting can be perfored during static initialization.
#define kFormatTypes													"GyYuQqMLwWdDFgEecahHKkmsSAzZv'"

#define kSingleQuote													'\''





//============================================================================
//		NDateFormatter::NDateFormatter : Constructor.
//----------------------------------------------------------------------------
NDateFormatter::NDateFormatter(void)
{
}





//============================================================================
//		NDateFormatter::~NDateFormatter : Destructor.
//----------------------------------------------------------------------------
NDateFormatter::~NDateFormatter(void)
{
}





//============================================================================
//		NDateFormatter::Format : Format a date.
//----------------------------------------------------------------------------
NString NDateFormatter::Format(const NGregorianDate &theDate, const NString &theFormat) const
{	const char			*textUTF8, *tokenStart, *tokenEnd;
	NString				theResult, theToken;
	bool				areDone;



	// Get the state we need
	textUTF8 = theFormat.GetUTF8();
	areDone  = false;



	// Format the date
	while (!areDone)
		{
		// Find the next token
		tokenStart = strpbrk(textUTF8, kFormatTypes);
		if (tokenStart == NULL)
			break;

		tokenEnd = GetTokenEnd(tokenStart);
		theToken = NString(tokenStart, tokenEnd - tokenStart + 1);



		// Append the text to this point
		theResult += NString(textUTF8, tokenStart - textUTF8);
		textUTF8   = tokenEnd + 1;



		// Append the token
		switch (*tokenStart) {
			case kSingleQuote:
				theResult += GetLiteral(theDate, theToken);
				break;

			case 'G':
				theResult += GetEra(theDate, theToken);
				break;

			default:
				NN_LOG("Unknown token: %c", *tokenStart);
				areDone = true;
				break;
			}
		}



	// Append any remaining text
	if (*textUTF8 != 0x00)
		theResult += NString(textUTF8);

	return(theResult);
}





//============================================================================
//		NDateFormatter::GetTokenEnd : Get the end of a token.
//----------------------------------------------------------------------------
#pragma mark -
const char *NDateFormatter::GetTokenEnd(const char *tokenStart) const
{	bool			isQuote, areDone;
	const char		*tokenEnd;



	// Validate our parameters
	NN_ASSERT(*tokenStart != 0x00);



	// Get the state we need
	tokenEnd = tokenStart;
	isQuote  = (*tokenStart == kSingleQuote);
	areDone  = (*tokenStart == 0x00);



	// Get the end token
	//
	// Date tokens are a continuous run of the same character, and quoted text
	// is zero or more arbitrary characters bracketed by a single quote.
	//
	// We return a pointer to the last character of the token, which may be the
	// last character in the string.
	while (!areDone)
		{
		tokenEnd++;

		if (isQuote)
			{
			areDone = (*tokenEnd == kSingleQuote || *tokenEnd == 0x00);
			if (*tokenEnd == 0x00)
				tokenEnd--;
			}

		else
			{
			areDone = (*tokenEnd != *tokenStart);
			if (areDone)
				tokenEnd--;
			}
		}

	return(tokenEnd);
}





//============================================================================
//		NDateFormatter::GetLiteral : Get literal text.
//----------------------------------------------------------------------------
NString NDateFormatter::GetLiteral(const NGregorianDate &/*theDate*/, const NString &theToken) const
{	NString		theText;



	// Validate our parameters
	NN_ASSERT(theToken.GetSize() >= 2);



	// Get the text
	theText = theToken;
	theText.Trim(kSingleQuote, kNStringNone);

	if (theText.IsEmpty())
		theText = NString(kSingleQuote);
	
	return(theText);
}





//============================================================================
//		NDateFormatter::GetEra : Get an era.
//----------------------------------------------------------------------------
NString NDateFormatter::GetEra(const NGregorianDate &theDate, const NString &theToken) const
{	NString		theText;
	NIndex		theSize;



	// Get the state we need
	theSize = theToken.GetSize();
	NN_ASSERT(theSize >= 1 && theSize <= 5);



	// Get the text
	switch (theSize) {
		case 5:
			theText = (theDate.year >= 0) ? "A" : "B";
			break;

		case 4:
			theText = (theDate.year >= 0) ? "Anno Domini" : "Before Christ";
			break;
		
		default:
			theText = (theDate.year >= 0) ? "AD" : "BC";
			break;
		}

	return(theText);
}







