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
	mProcessCDATAStart   = NULL;
	mProcessCDATAEnd     = NULL;
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

		XML_SetUserData(                mParser, this);
		XML_SetStartElementHandler(     mParser, ParsedElementStart);
		XML_SetEndElementHandler(       mParser, ParsedElementEnd);
		XML_SetCharacterDataHandler(    mParser, ParsedText);
		XML_SetCommentHandler(          mParser, ParsedComment);
		XML_SetStartCdataSectionHandler(mParser, ParsedCDATAStart);
		XML_SetEndCdataSectionHandler(  mParser, ParsedCDATAEnd);
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
NStatus NXMLParser::Parse(const NString &theText)
{


	// Parse the document
	return(Parse(theText.GetData()));
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
//		NXMLParser::SetProcessElementStart : Set the element start functor.
//----------------------------------------------------------------------------
void NXMLParser::SetProcessElementStart(const NXMLProcessElementStartFunctor &theFunctor)
{


	// Set the functor
	mProcessElementStart = theFunctor;
}





//============================================================================
//		NXMLParser::SetProcessElementEnd : Set the element end functor.
//----------------------------------------------------------------------------
void NXMLParser::SetProcessElementEnd(const NXMLProcessElementEndFunctor &theFunctor)
{


	// Set the functor
	mProcessElementEnd = theFunctor;
}





//============================================================================
//		NXMLParser::SetProcessText : Set the text functor.
//----------------------------------------------------------------------------
void NXMLParser::SetProcessText(const NXMLProcessTextFunctor &theFunctor)
{


	// Set the functor
	mProcessText = theFunctor;
}





//============================================================================
//		NXMLParser::SetProcessComment : Set the comment functor.
//----------------------------------------------------------------------------
void NXMLParser::SetProcessComment(const NXMLProcessCommentFunctor &theFunctor)
{


	// Set the functor
	mProcessComment = theFunctor;
}





//============================================================================
//		NXMLParser::SetProcessCDATAStart : Set the CDATA start functor.
//----------------------------------------------------------------------------
void NXMLParser::SetProcessCDATAStart(const NXMLProcessCDATAStartFunctor &theFunctor)
{


	// Set the functor
	mProcessCDATAStart = theFunctor;
}





//============================================================================
//		NXMLParser::SetProcessCDATAEnd : Set the CDATA end functor.
//----------------------------------------------------------------------------
void NXMLParser::SetProcessCDATAEnd(const NXMLProcessCDATAEndFunctor &theFunctor)
{


	// Set the functor
	mProcessCDATAEnd = theFunctor;
}





//============================================================================
//		NXMLParser::ProcessElementStart : Process an element start.
//----------------------------------------------------------------------------
#pragma mark -
bool NXMLParser::ProcessElementStart(const NString &theName, const NDictionary &theAttributes)
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
//		NXMLParser::ProcessCDATAStart : Process a CDATA start.
//----------------------------------------------------------------------------
bool NXMLParser::ProcessCDATAStart(void)
{


	// Process the item
	if (mProcessCDATAStart != NULL)
		return(mProcessCDATAStart());

	return(true);
}





//============================================================================
//		NXMLParser::ProcessCDATAEnd : Process a CDATA end.
//----------------------------------------------------------------------------
bool NXMLParser::ProcessCDATAEnd(void)
{


	// Process the item
	if (mProcessCDATAEnd != NULL)
		return(mProcessCDATAEnd());

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
void NXMLParser::ParsedElementStart(void *userData, const XML_Char *theName, const XML_Char **attributeList)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;
	NString			theKey, theValue;
	NDictionary		theAttributes;



	// Get the state we need
	while (*attributeList != NULL)
		{
		theKey   = NString(*attributeList++);
		theValue = NString(*attributeList++);
		
		NN_ASSERT(!theAttributes.HasKey(theKey));
		theAttributes.SetValue(theKey, theValue);
		}



	// Process the item
	if (!thisPtr->ProcessElementStart(theName, theAttributes))
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
//		NXMLParser::ParsedCDAtAStart : Process a CDATA start.
//----------------------------------------------------------------------------
void NXMLParser::ParsedCDATAStart(void *userData)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;



	// Process the item
	if (!thisPtr->ProcessCDATAStart())
		thisPtr->StopParsing();
}





//============================================================================
//		NXMLParser::ParsedCDAtAEnd : Process a CDATA end.
//----------------------------------------------------------------------------
void NXMLParser::ParsedCDATAEnd(void *userData)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;



	// Process the item
	if (!thisPtr->ProcessCDATAEnd())
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










