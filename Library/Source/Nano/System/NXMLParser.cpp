/*	NAME:
		NXMLParser.cpp

	DESCRIPTION:
		XML parser.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NXMLParser.h"

// Nano
#include "NData.h"
#include "NDictionary.h"
#include "NFileHandle.h"
#include "NanoConstants.h"
#include "expat.h"





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
NN_DIAGNOSTIC_IGNORE_CLANG("-Wdisabled-macro-expansion");





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr size_t kChunkSize                          = 10 * kNKilobyte;





//=============================================================================
//		NXMLParser::NXMLParser : Constructor.
//-----------------------------------------------------------------------------
NXMLParser::NXMLParser()
	: mParser(nullptr)
	, mOptions(kNXMLParserDefault)
	, mInsideCData(false)
	, mIsParsing(false)
	, mParsedText()
	, mFunctionDocumentType()
	, mFunctionElementStart()
	, mFunctionElementEnd()
	, mFunctionComment()
	, mFunctionText()

{
}





//=============================================================================
//		NXMLParser::~NXMLParser : Destructor.
//-----------------------------------------------------------------------------
NXMLParser::~NXMLParser()
{


	// Clean up
	DestroyParser();
}





//=============================================================================
//		NXMLParser::Clear : Clear the parser.
//-----------------------------------------------------------------------------
void NXMLParser::Clear()
{


	// Check our state
	if (mParser == nullptr)
	{
		return;
	}



	// Reset the parser
	XML_ParserReset(mParser, nullptr);

	XML_SetUserData(mParser, this);
	XML_SetStartDoctypeDeclHandler(mParser, ParsedDocumentType);
	XML_SetStartElementHandler(mParser, ParsedElementStart);
	XML_SetEndElementHandler(mParser, ParsedElementEnd);
	XML_SetCommentHandler(mParser, ParsedComment);
	XML_SetCharacterDataHandler(mParser, ParsedText);
	XML_SetStartCdataSectionHandler(mParser, ParsedCDataStart);
	XML_SetEndCdataSectionHandler(mParser, ParsedCDataEnd);



	// Reset our state
	mInsideCData = false;
	mIsParsing   = false;

	mParsedText.Clear();
}





//=============================================================================
//		NXMLParser::GetOptions : Get the options.
//-----------------------------------------------------------------------------
NXMLParserFlags NXMLParser::GetOptions() const
{


	// Get the options
	return mOptions;
}





//=============================================================================
//		NXMLParser::SetOptions : Set the options.
//-----------------------------------------------------------------------------
void NXMLParser::SetOptions(NXMLParserFlags setThese, NXMLParserFlags clearThese)
{


	// Set the options
	mOptions |= setThese;
	mOptions &= ~clearThese;
}





//=============================================================================
//		NXMLParser::Parse : Parse a document.
//-----------------------------------------------------------------------------
NStatus NXMLParser::Parse(const NFile& theFile)
{


	// Parse the document
	return Parse(NFileHandle::ReadData(theFile));
}





//=============================================================================
//		NXMLParser::Parse : Parse a document.
//-----------------------------------------------------------------------------
NStatus NXMLParser::Parse(const NString& theText)
{


	// Parse the document
	return Parse(theText.GetData(NStringEncoding::UTF8));
}





//=============================================================================
//		NXMLParser::Parse : Parse a document.
//-----------------------------------------------------------------------------
NStatus NXMLParser::Parse(const NData& theData)
{


	// Parse the document
	return Parse(theData.GetSize(), theData.GetData(), NXMLChunk::Last);
}





//=============================================================================
//		NXMLParser::Parse : Parse a fragment.
//-----------------------------------------------------------------------------
NStatus NXMLParser::Parse(size_t theSize, const void* thePtr, NXMLChunk theChunk)
{


	// Get the state we need
	const char* chunkPtr = reinterpret_cast<const char*>(thePtr);
	bool        isFinal  = (theChunk == NXMLChunk::Last);

	NStatus theErr   = NStatus::OK;
	size_t  sizeLeft = theSize;
	size_t  sizeDone = 0;



	// Prepare to parse
	if (mParser == nullptr)
	{
		theErr = CreateParser();
		if (theErr != NStatus::OK)
		{
			return theErr;
		}
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
	{
		theErr = BeginProgress(theChunk == NXMLChunk::Last ? kNProgressNone : kNProgressUnknown);
	}



	// Parse the fragment
	while (theErr == NStatus::OK)
	{
		// Parse the chunk
		size_t chunkSize  = std::min(kChunkSize, sizeLeft);
		bool   finalChunk = (isFinal && chunkSize == sizeLeft);

		XML_Status xmlErr = XML_Parse(mParser, chunkPtr, int(chunkSize), finalChunk);
		theErr            = ConvertXMLStatus(xmlErr);



		// Update our state
		if (theErr == NStatus::OK)
		{
			sizeLeft -= chunkSize;
			sizeDone += chunkSize;
			chunkPtr += chunkSize;

			if (isFinal)
			{
				theErr = UpdateProgress(sizeDone, theSize);
			}
			else
			{
				theErr = UpdateProgress(kNProgressUnknown);
			}
		}

		if (finalChunk)
		{
			break;
		}
	}



	// Update our state
	mIsParsing = true;

	if (theChunk == NXMLChunk::Last)
	{
		EndProgress();
	}

	if (theErr != NStatus::OK || isFinal)
	{
		Clear();
	}

	return theErr;
}





//=============================================================================
//		NXMLParser::SetProcessDocumentType : Set the document type function.
//-----------------------------------------------------------------------------
void NXMLParser::SetProcessDocumentType(const NFunctionXMLDocumentType& theFunction)
{


	// Set the function
	mFunctionDocumentType = theFunction;
}





//=============================================================================
//		NXMLParser::SetProcessElementStart : Set the element start function.
//-----------------------------------------------------------------------------
void NXMLParser::SetProcessElementStart(const NFunctionXMLElementStart& theFunction)
{


	// Set the function
	mFunctionElementStart = theFunction;
}





//=============================================================================
//		NXMLParser::SetProcessElementEnd : Set the element end function.
//-----------------------------------------------------------------------------
void NXMLParser::SetProcessElementEnd(const NFunctionXMLElementEnd& theFunction)
{


	// Set the function
	mFunctionElementEnd = theFunction;
}





//=============================================================================
//		NXMLParser::SetProcessComment : Set the comment function.
//-----------------------------------------------------------------------------
void NXMLParser::SetProcessComment(const NFunctionXMLComment& theFunction)
{


	// Set the function
	mFunctionComment = theFunction;
}





//=============================================================================
//		NXMLParser::SetProcessText : Set the text function.
//-----------------------------------------------------------------------------
void NXMLParser::SetProcessText(const NFunctionXMLText& theFunction)
{


	// Set the function
	mFunctionText = theFunction;
}





#pragma mark private
//=============================================================================
//		NXMLParser::CreateParser : Create the parser.
//-----------------------------------------------------------------------------
NStatus NXMLParser::CreateParser()
{


	// Validate our state
	NN_REQUIRE(mParser == nullptr);



	// Create the parser
	mParser = XML_ParserCreate(nullptr);
	if (mParser == nullptr)
	{
		return NStatus::Memory;
	}



	// Configure the parser
	Clear();

	return NStatus::OK;
}





//=============================================================================
//		NXMLParser::DestroyParser : Destroy the parser.
//-----------------------------------------------------------------------------
void NXMLParser::DestroyParser()
{


	// Destroy the parser
	if (mParser != nullptr)
	{
		XML_ParserFree(mParser);
		mParser = nullptr;
	}
}





//=============================================================================
//		NXMLParser::ConvertXMLStatus : Convert an XML_Status.
//-----------------------------------------------------------------------------
NStatus NXMLParser::ConvertXMLStatus(int32_t xmlErr)
{


	// Validate our state
	NN_REQUIRE(mParser != nullptr);



	// Convert the status
	switch (xmlErr)
	{
		case XML_STATUS_ERROR:
		case XML_STATUS_SUSPENDED:
			return NStatus::Malformed;
			break;

		case XML_STATUS_OK:
			return NStatus::OK;
			break;

		default:
			NN_LOG_ERROR("Unknown parsing error: {}", xmlErr);
			return NStatus::Param;
			break;
	}
}





//=============================================================================
//		NXMLParser::FlushText : Flush any parsed text.
//-----------------------------------------------------------------------------
bool NXMLParser::FlushText()
{


	// Check our state
	if (mParsedText.IsEmpty())
	{
		return true;
	}



	// Check for whitespace
	//
	// Whitespace within a CDATA is always preserved, as is any whitespace
	// surrounding a non-whitespace text section.
	if (!mInsideCData && (mOptions & kNXMLParserSkipWhitespace))
	{
		NString trimmedText = mParsedText;
		trimmedText.Trim();

		if (trimmedText.IsEmpty())
		{
			mParsedText.Clear();
			return true;
		}
	}



	// Flush the text
	bool keepParsing = ProcessText(mParsedText, mInsideCData);
	mParsedText.Clear();

	return keepParsing;
}





//=============================================================================
//		NXMLParser::StopParsing : Stop the parser.
//-----------------------------------------------------------------------------
void NXMLParser::StopParsing()
{


	// Stop parsing
	XML_Status xmlErr = XML_StopParser(mParser, false);
	NN_REQUIRE(xmlErr == XML_STATUS_OK);
}





//=============================================================================
//		NXMLParser::ProcessDocumentType : Process a document type.
//-----------------------------------------------------------------------------
bool NXMLParser::ProcessDocumentType(const NString& theName, const NXMLDocumentType& theInfo)
{


	// Process the item
	if (mFunctionDocumentType)
	{
		return mFunctionDocumentType(theName, theInfo);
	}

	return true;
}





//=============================================================================
//		NXMLParser::ProcessElementStart : Process an element start.
//-----------------------------------------------------------------------------
bool NXMLParser::ProcessElementStart(const NString& theName, const NDictionary& theAttributes)
{


	// Process the item
	if (mFunctionElementStart)
	{
		return mFunctionElementStart(theName, theAttributes);
	}

	return true;
}





//=============================================================================
//		NXMLParser::ProcessElementEnd : Process an element end.
//-----------------------------------------------------------------------------
bool NXMLParser::ProcessElementEnd(const NString& theName)
{


	// Process the item
	if (mFunctionElementEnd)
	{
		return mFunctionElementEnd(theName);
	}

	return true;
}





//=============================================================================
//		NXMLParser::ProcessComment : Process a comment.
//-----------------------------------------------------------------------------
bool NXMLParser::ProcessComment(const NString& theValue)
{


	// Process the item
	if (mFunctionComment)
	{
		return mFunctionComment(theValue);
	}

	return true;
}





//=============================================================================
//		NXMLParser::ProcessText : Process text.
//-----------------------------------------------------------------------------
bool NXMLParser::ProcessText(const NString& theValue, bool isCData)
{


	// Process the item
	if (mFunctionText)
	{
		return mFunctionText(theValue, isCData);
	}

	return true;
}





//=============================================================================
//		NXMLParser::ParsedDocumentType : Process a document type.
//-----------------------------------------------------------------------------
void NXMLParser::ParsedDocumentType(void*           userData,
									const XML_Char* itemName,
									const XML_Char* sysID,
									const XML_Char* pubID,
									int             hasInternal)
{


	// Get the state we need
	NXMLParser*      thisPtr = reinterpret_cast<NXMLParser*>(userData);
	NString          theName(itemName);
	NXMLDocumentType theInfo{};

	theInfo.hasInternal = (hasInternal != 0);
	theInfo.systemID    = NString(sysID);
	theInfo.publicID    = NString(pubID);



	// Process the item
	if (!thisPtr->FlushText() || !thisPtr->ProcessDocumentType(theName, theInfo))
	{
		thisPtr->StopParsing();
	}
}





//=============================================================================
//		NXMLParser::ParsedElementStart : Process an element start.
//-----------------------------------------------------------------------------
void NXMLParser::ParsedElementStart(void*            userData,
									const XML_Char*  itemName,
									const XML_Char** attributeList)
{


	// Get the state we need
	NXMLParser* thisPtr = reinterpret_cast<NXMLParser*>(userData);
	NString     theName(itemName);
	NDictionary theAttributes;

	while (*attributeList != nullptr)
	{
		NString theKey   = NString(*attributeList++);
		NString theValue = NString(*attributeList++);

		NN_REQUIRE(!theAttributes.HasKey(theKey));
		theAttributes[theKey] = theValue;
	}



	// Process the item
	if (!thisPtr->FlushText() || !thisPtr->ProcessElementStart(theName, theAttributes))
	{
		thisPtr->StopParsing();
	}
}





//=============================================================================
//		NXMLParser::ParsedElementEnd : Process an element end.
//-----------------------------------------------------------------------------
void NXMLParser::ParsedElementEnd(void* userData, const XML_Char* itemName)
{


	// Get the state we need
	NXMLParser* thisPtr = reinterpret_cast<NXMLParser*>(userData);
	NString     theName(itemName);



	// Process the item
	if (!thisPtr->FlushText() || !thisPtr->ProcessElementEnd(theName))
	{
		thisPtr->StopParsing();
	}
}





//=============================================================================
//		NXMLParser::ParsedComment : Process a comment.
//-----------------------------------------------------------------------------
void NXMLParser::ParsedComment(void* userData, const XML_Char* itemText)
{


	// Get the state we need
	NXMLParser* thisPtr = reinterpret_cast<NXMLParser*>(userData);
	NString     theText(itemText);



	// Process the item
	if (!thisPtr->FlushText() || !thisPtr->ProcessComment(theText))
	{
		thisPtr->StopParsing();
	}
}





//=============================================================================
//		NXMLParser::ParsedText : Process text.
//-----------------------------------------------------------------------------
void NXMLParser::ParsedText(void* userData, const XML_Char* itemText, int itemSize)
{


	// Validate our parameters
	NN_REQUIRE(itemSize >= 0);



	// Get the state we need
	NXMLParser* thisPtr = reinterpret_cast<NXMLParser*>(userData);
	NString     theText(NStringEncoding::UTF8, size_t(itemSize), itemText);

	NN_UNUSED(itemSize);



	// Process the item
	thisPtr->mParsedText += theText;
}





//=============================================================================
//		NXMLParser::ParsedCDataStart : Process a CDATA start.
//-----------------------------------------------------------------------------
void NXMLParser::ParsedCDataStart(void* userData)
{


	// Get the state we need
	NXMLParser* thisPtr = reinterpret_cast<NXMLParser*>(userData);
	NN_REQUIRE(!thisPtr->mInsideCData);



	// Process the item
	if (!thisPtr->FlushText())
	{
		thisPtr->StopParsing();
	}

	thisPtr->mInsideCData = true;
}





//=============================================================================
//		NXMLParser::ParsedCDataEnd : Process a CDATA end.
//-----------------------------------------------------------------------------
void NXMLParser::ParsedCDataEnd(void* userData)
{


	// Get the state we need
	NXMLParser* thisPtr = reinterpret_cast<NXMLParser*>(userData);
	NN_REQUIRE(thisPtr->mInsideCData);



	// Process the item
	if (!thisPtr->FlushText())
	{
		thisPtr->StopParsing();
	}

	thisPtr->mInsideCData = false;
}
