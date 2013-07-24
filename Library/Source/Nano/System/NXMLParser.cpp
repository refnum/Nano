/*	NAME:
		NXMLParser.cpp

	DESCRIPTION:
		XML parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
//		Internal constants
//----------------------------------------------------------------------------
static const NIndex kChunkSize									= 10 * kNKilobyte;





//============================================================================
//		NXMLParser::NXMLParser : Constructor.
//----------------------------------------------------------------------------
NXMLParser::NXMLParser(void)
{


	// Initialise ourselves
	mParser  = NULL;
	mOptions = kNXMLParserDefault;
	
	mInsideCData = false;
	mIsParsing   = false;

	mProcessDocumentType = NULL;
	mProcessElementStart = NULL;
	mProcessElementEnd   = NULL;
	mProcessComment      = NULL;
	mProcessText         = NULL;
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


	// Check our state
	if (mParser == NULL)
		return;



	// Reset the parser
	XML_ParserReset(mParser, NULL);

	XML_SetUserData(                mParser, this);
	XML_SetStartDoctypeDeclHandler( mParser, ParsedDocumentType);
	XML_SetStartElementHandler(     mParser, ParsedElementStart);
	XML_SetEndElementHandler(       mParser, ParsedElementEnd);
	XML_SetCommentHandler(          mParser, ParsedComment);
	XML_SetCharacterDataHandler(    mParser, ParsedText);
	XML_SetStartCdataSectionHandler(mParser, ParsedCDataStart);
	XML_SetEndCdataSectionHandler(  mParser, ParsedCDataEnd);



	// Reset our state
	mInsideCData = false;
	mIsParsing   = false;

	mParsedText.Clear();
}





//============================================================================
//		NXMLParser::GetOptions : Get the options.
//----------------------------------------------------------------------------
NXMLParserOptions NXMLParser::GetOptions(void) const
{


	// Get the options
	return(mOptions);
}





//============================================================================
//		NXMLParser::SetOptions : Set the options.
//----------------------------------------------------------------------------
void NXMLParser::SetOptions(NXMLParserOptions setThese, NXMLParserOptions clearThese)
{


	// Set the options
	mOptions |=    setThese;
	mOptions &= ~clearThese;
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
//		NXMLParser::Parse : Parse a fragment.
//----------------------------------------------------------------------------
NStatus NXMLParser::Parse(NIndex theSize, const void *thePtr, bool isFinal)
{	NIndex			sizeLeft, sizeDone, chunkSize;
	bool			finalChunk;
	const char		*chunkPtr;
	XML_Status		xmlErr;
	NStatus			theErr;



	// Get the state we need
	chunkPtr   = (const char *) thePtr;
	finalChunk = false;

	sizeDone = 0;
	sizeLeft = theSize;
	theErr   = kNoErr;



	// Prepare to parse
	if (mParser == NULL)
		{
		theErr = CreateParser();
		if (theErr != kNoErr)
			return(theErr);
		}



	// Update the progress
	//
	// If we're parsing a single chunk then we can emit a percentage progress
	// from the start since we know the total chunk size.
	//
	// Otherwise we need to start with indeterminate progress, and wait until we
	// receive the final chunk before we can display a percentage progress (for
	// that case the percentage progress will be that of the final chunk, which
	// will probably be quite small, but this is the best we can do).
	if (!mIsParsing)
		theErr = BeginProgress(isFinal ? kNProgressNone : kNProgressUnknown);



	// Parse the fragment
	while (theErr == kNoErr && !finalChunk)
		{
		// Parse the chunk
		chunkSize  = std::min(kChunkSize, sizeLeft);
		finalChunk = (isFinal && chunkSize == sizeLeft);

		xmlErr = XML_Parse(mParser, chunkPtr, chunkSize, finalChunk);
		theErr = ConvertXMLStatus(xmlErr);
		
		

		// Update our state
		if (theErr == kNoErr)
			{
			sizeDone += chunkSize;
			sizeLeft -= chunkSize;
			chunkPtr += chunkSize;

			if (isFinal)
				theErr = ContinueProgress(sizeDone, theSize);
			else
				theErr = ContinueProgress(kNProgressUnknown);
			}
		}



	// Update our state
	mIsParsing = true;

	if (isFinal)
		EndProgress();

	if (theErr != kNoErr || isFinal)
		Clear();

	return(theErr);
}





//============================================================================
//		NXMLParser::SetProcessDocumentType : Set the document type functor.
//----------------------------------------------------------------------------
void NXMLParser::SetProcessDocumentType(const NXMLProcessDocumentTypeFunctor &theFunctor)
{


	// Set the functor
	mProcessDocumentType = theFunctor;
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
//		NXMLParser::SetProcessComment : Set the comment functor.
//----------------------------------------------------------------------------
void NXMLParser::SetProcessComment(const NXMLProcessCommentFunctor &theFunctor)
{


	// Set the functor
	mProcessComment = theFunctor;
}





//============================================================================
//		NXMLParser::SetProcessText : Set the text functor.
//----------------------------------------------------------------------------
void NXMLParser::SetProcessText(const NXMLProcessTextFunctor &theFunctor)
{


	// Set the functor
	mProcessText = theFunctor;
}





#pragma mark protected
//============================================================================
//		NXMLParser::ProcessDocumentType : Process a document type.
//----------------------------------------------------------------------------
bool NXMLParser::ProcessDocumentType(const NString &theName, const NXMLDocumentTypeInfo &theInfo)
{


	// Process the item
	if (mProcessDocumentType != NULL)
		return(mProcessDocumentType(theName, theInfo));

	return(true);
}





//============================================================================
//		NXMLParser::ProcessElementStart : Process an element start.
//----------------------------------------------------------------------------
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
//		NXMLParser::ProcessText : Process text.
//----------------------------------------------------------------------------
bool NXMLParser::ProcessText(const NString &theValue, bool isCData)
{


	// Process the item
	if (mProcessText != NULL)
		return(mProcessText(theValue, isCData));

	return(true);
}





#pragma mark private
//============================================================================
//		NXMLParser::CreateParser : Create the parser.
//----------------------------------------------------------------------------
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
NStatus NXMLParser::ConvertXMLStatus(int32_t xmlErr)
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
			theErr = kNErrParam;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NXMLParser::FlushText : Flush any parsed text.
//----------------------------------------------------------------------------
bool NXMLParser::FlushText(void)
{	bool		keepParsing;
	NString		trimmedText;



	// Check our state
	if (mParsedText.IsEmpty())
		return(true);



	// Check for whitespace
	//
	// Whitespace within a CDATA is always preserved, as is any whitespace
	// surrounding a non-whitespace text section.
	if (!mInsideCData && (mOptions & kNXMLParserSkipWhitespace))
		{
		trimmedText = mParsedText;
		trimmedText.Trim();

		if (trimmedText.IsEmpty())
			{
			mParsedText.Clear();
			return(true);
			}
		}



	// Flush the text
	keepParsing = ProcessText(mParsedText, mInsideCData);
	mParsedText.Clear();

	return(keepParsing);
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





//============================================================================
//		NXMLParser::ParsedDocumentType : Process a document type.
//----------------------------------------------------------------------------
void NXMLParser::ParsedDocumentType(void *userData, const XML_Char *itemName, const XML_Char *sysID, const XML_Char *pubID, int hasInternal)
{	NXMLParser					*thisPtr = (NXMLParser *) userData;
	NString						theName;
	NXMLDocumentTypeInfo		theInfo;
	


	// Get the state we need
	theName             = NString(itemName, kNStringLength);
	theInfo.systemID    = NString(sysID,    kNStringLength);
	theInfo.publicID    = NString(pubID,    kNStringLength);
	theInfo.hasInternal = (hasInternal != 0);



	// Process the item
	if (!thisPtr->FlushText() || !thisPtr->ProcessDocumentType(theName, theInfo))
		thisPtr->StopParsing();
}





//============================================================================
//		NXMLParser::ParsedElementStart : Process an element start.
//----------------------------------------------------------------------------
void NXMLParser::ParsedElementStart(void *userData, const XML_Char *itemName, const XML_Char **attributeList)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;
	NString			theName, theKey, theValue;
	NDictionary		theAttributes;



	// Get the state we need
	theName = NString(itemName, kNStringLength);

	while (*attributeList != NULL)
		{
		theKey   = NString(*attributeList++, kNStringLength);
		theValue = NString(*attributeList++, kNStringLength);
		
		NN_ASSERT(!theAttributes.HasKey(theKey));
		theAttributes.SetValue(theKey, theValue);
		}



	// Process the item
	if (!thisPtr->FlushText() || !thisPtr->ProcessElementStart(theName, theAttributes))
		thisPtr->StopParsing();
}





//============================================================================
//		NXMLParser::ParsedElementEnd : Process an element end.
//----------------------------------------------------------------------------
void NXMLParser::ParsedElementEnd(void *userData, const XML_Char *itemName)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;
	NString			theName;



	// Get the state we need
	theName = NString(itemName, kNStringLength);



	// Process the item
	if (!thisPtr->FlushText() || !thisPtr->ProcessElementEnd(theName))
		thisPtr->StopParsing();
}





//============================================================================
//		NXMLParser::ParsedComment : Process a comment.
//----------------------------------------------------------------------------
void NXMLParser::ParsedComment(void *userData, const XML_Char *itemText)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;
	NString			theText;
	
	
	
	// Get the state we need
	theText = NString(itemText, kNStringLength);



	// Process the item
	if (!thisPtr->FlushText() || !thisPtr->ProcessComment(theText))
		thisPtr->StopParsing();
}





//============================================================================
//		NXMLParser::ParsedText : Process text.
//----------------------------------------------------------------------------
void NXMLParser::ParsedText(void *userData, const XML_Char *itemText, int itemSize)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;
	NString			theText;
	
	
	
	// Get the state we need
	theText = NString(itemText, itemSize);



	// Process the item
	thisPtr->mParsedText += theText;
}





//============================================================================
//		NXMLParser::ParsedCDataStart : Process a CDATA start.
//----------------------------------------------------------------------------
void NXMLParser::ParsedCDataStart(void *userData)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;



	// Validate our state
	NN_ASSERT(!thisPtr->mInsideCData);
	


	// Process the item
	if (!thisPtr->FlushText())
		thisPtr->StopParsing();

	thisPtr->mInsideCData = true;
}





//============================================================================
//		NXMLParser::ParsedCDataEnd : Process a CDATA end.
//----------------------------------------------------------------------------
void NXMLParser::ParsedCDataEnd(void *userData)
{	NXMLParser		*thisPtr = (NXMLParser *) userData;



	// Validate our state
	NN_ASSERT(thisPtr->mInsideCData);



	// Process the item
	if (!thisPtr->FlushText())
		thisPtr->StopParsing();

	thisPtr->mInsideCData = false;
}



