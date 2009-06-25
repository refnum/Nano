/*	NAME:
		NXMLParser.cpp

	DESCRIPTION:
		XML parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "expat.h"

#include "NFileUtilities.h"
#include "NXMLParser.h"





//============================================================================
//		NXMLParser::NXMLParser : Constructor.
//----------------------------------------------------------------------------
NXMLParser::NXMLParser(void)
{


	// Initialise ourselves
	mParser = NULL;

	mProcessElementStart = NULL;
	mProcessElementEnd   = NULL;
	mProcessText         = NULL;
	mProcessComment      = NULL;
}





//============================================================================
//		NXMLParser::~NXMLParser : Destructor.
//----------------------------------------------------------------------------
NXMLParser::~NXMLParser(void)
{


	// Clean up
	DestroyParser();
}





//============================================================================
//		NXMLParser::Clear : Clear the parser.
//----------------------------------------------------------------------------
void NXMLParser::Clear(void)
{


	// Reset the parser
	if (mParser != NULL)
		{
		XML_ParserReset(mParser, NULL);

		XML_SetUserData(            mParser, this);
		XML_SetStartElementHandler( mParser, ParsedElementStart);
		XML_SetEndElementHandler(   mParser, ParsedElementEnd);
		XML_SetCharacterDataHandler(mParser, ParsedText);
		XML_SetCommentHandler(      mParser, ParsedComment);
		}
}





//============================================================================
//		NXMLParser::Parse : Parse a document.
//----------------------------------------------------------------------------
NStatus NXMLParser::Parse(const NFile &theFile)
{


	// Parse the document
	return(Parse(NFileUtilities::GetFileData(theFile)));
}





//============================================================================
//		NXMLParser::Parse : Parse a document.
//----------------------------------------------------------------------------
NStatus NXMLParser::Parse(const NData &theData)
{


	// Parse the document
	return(Parse(theData.GetSize(), theData.GetData(), true));
}





//============================================================================
//		NXMLParser::Parse : Parse a document.
//----------------------------------------------------------------------------
NStatus NXMLParser::Parse(NIndex theSize, const void *thePtr, bool isFinal)
{	XML_Status		xmlErr;
	NStatus			theErr;



	// Prepare to parse
	if (mParser == NULL)
		{
		theErr = CreateParser();
		if (theErr != noErr)
			return(theErr);
		}



	// Parse the document
	xmlErr = XML_Parse(mParser, (const char *) thePtr, theSize, isFinal);
	theErr = ConvertXMLStatus(xmlErr);



	// Clean up
	if (isFinal)
		Clear();
	
	return(theErr);
}





//============================================================================
//		NXMLParser::ProcessElementStart : Process an element start.
//----------------------------------------------------------------------------
bool NXMLParser::ProcessElementStart(const NString &theName, const NStringList &theAttributes)
{


	// Process the item
	if (mProcessElementStart != NULL)
		return(mProcessElementStart(theName, theAttributes));

	return(true);
}





//============================================================================
//		NXMLParser::ProcessElementEnd : Process an element end.
//----------------------------------------------------------------------------
bool NXMLParser::ProcessElementEnd(const NString &theName)
{


	// Process the item
	if (mProcessElementEnd != NULL)
		return(mProcessElementEnd(theName));

	return(true);
}





//============================================================================
//		NXMLParser::ProcessText : Process text.
//----------------------------------------------------------------------------
bool NXMLParser::ProcessText(const NString &theValue)
{


	// Process the item
	if (mProcessText != NULL)
		return(mProcessText(theValue));

	return(true);
}





//============================================================================
//		NXMLParser::ProcessComment : Process a comment.
//----------------------------------------------------------------------------
bool NXMLParser::ProcessComment(const NString &theValue)
{


	// Process the item
	if (mProcessComment != NULL)
		return(mProcessComment(theValue));

	return(true);
}





//============================================================================
//		NXMLParser::CreateParser : Create the parser.
//----------------------------------------------------------------------------
#pragma mark -
NStatus NXMLParser::CreateParser(void)
{


	// Validate our state
	NN_ASSERT(mParser == NULL);
	
	
	
	// Create the parser
	mParser = XML_ParserCreate(NULL);
	if (mParser == NULL)
		return(kNErrMemory);



	// Configure the parser
	Clear();

	return(kNoErr);
}





//============================================================================
//		NXMLParser::DestroyParser : Destroy the parser.
//----------------------------------------------------------------------------
void NXMLParser::DestroyParser(void)
{


	// Destroy the parser
	if (mParser != NULL)
		{
		XML_ParserFree(mParser);
		mParser = NULL;
		}
}





//============================================================================
//		NXMLParser::ConvertXMLStatus : Convert an XML_Status.
//----------------------------------------------------------------------------
NStatus NXMLParser::ConvertXMLStatus(SInt32 xmlErr)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(mParser != NULL);



	// Convert the status
	switch (xmlErr) {
		case XML_STATUS_ERROR:
		case XML_STATUS_SUSPENDED:
			theErr = kNErrMalformed;
			break;
		
		case XML_STATUS_OK:
			theErr = kNoErr;
			break;
		
		default:
			NN_LOG("Unknown error: %d", xmlErr);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NXMLParser::ParsedElementStart : Process an element start.
//----------------------------------------------------------------------------
void NXMLParser::ParsedElementStart(void *userData, const XML_Char *theName, const XML_Char **theAttributes)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;
	NStringList		attributeList;



	// Get the state we need
	while (theAttributes != NULL)
		{
		attributeList.push_back(NString(*theAttributes));
		theAttributes++;
		}



	// Process the item
	if (!thisPtr->ProcessElementStart(theName, attributeList))
		thisPtr->StopParsing();
}





//============================================================================
//		NXMLParser::ParsedElementEnd : Process an element end.
//----------------------------------------------------------------------------
void NXMLParser::ParsedElementEnd(void *userData, const XML_Char *theName)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;



	// Process the item
	if (!thisPtr->ProcessElementEnd(theName))
		thisPtr->StopParsing();
}





//============================================================================
//		NXMLParser::ParsedText : Process text.
//----------------------------------------------------------------------------
void NXMLParser::ParsedText(void *userData, const XML_Char *theText, int theSize)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;



	// Process the item
	if (!thisPtr->ProcessText(NString(theText, theSize)))
		thisPtr->StopParsing();
}





//============================================================================
//		NXMLParser::ParsedComment : Process a comment.
//----------------------------------------------------------------------------
void NXMLParser::ParsedComment(void *userData, const XML_Char *theText)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;



	// Process the item
	if (!thisPtr->ProcessComment(theText))
		thisPtr->StopParsing();
}





//============================================================================
//		NXMLParser::StopParsing : Stop the parser.
//----------------------------------------------------------------------------
void NXMLParser::StopParsing(void)
{	XML_Status		xmlErr;



	// Stop parsing
	xmlErr = XML_StopParser(mParser, false);
	NN_ASSERT(xmlErr == XML_STATUS_OK);
}










