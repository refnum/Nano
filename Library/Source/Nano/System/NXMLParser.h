/*	NAME:
		NXMLParser.h

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
#ifndef NXMLPARSER_H
#define NXMLPARSER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NProgressable.h"
#include "NString.h"

// System
#include <functional>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Parser flags
using NXMLParserFlags                                       = uint8_t;

inline constexpr NXMLParserFlags kNXMLParserNone            = 0;
inline constexpr NXMLParserFlags kNXMLParserSkipWhitespace  = (1 << 0);
inline constexpr NXMLParserFlags kNXMLParserDefault         = kNXMLParserSkipWhitespace;


// Parsing progress
enum class NXMLChunk
{
	Next,
	Last
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NFile;
class NDictionary;
class NData;


// Document info
struct NXMLDocumentType
{
	bool    hasInternal;
	NString systemID;
	NString publicID;
};


// Process functions
//
// Processing functions should return true to continue processing.
using NFunctionProgress = std::function<NStatus(NProgress theState, float theValue)>;

using NFunctionXMLDocumentType =
	std::function<bool(const NString& theName, const NXMLDocumentType& theType)>;

using NFunctionXMLElementStart =
	std::function<bool(const NString& theName, const NDictionary& theAttributes)>;

using NFunctionXMLElementEnd = std::function<bool(const NString& theName)>;

using NFunctionXMLComment = std::function<bool(const NString& theValue)>;

using NFunctionXMLText = std::function<bool(const NString& theValue, bool isCData)>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NXMLParser final : public NProgressable
{
public:
										NXMLParser();
									   ~NXMLParser();

										NXMLParser(const NXMLParser& otherParser) = delete;
	NXMLParser&                         operator=( const NXMLParser& otherParser) = delete;

										NXMLParser(NXMLParser&& otherParser) = delete;
	NXMLParser&                         operator=( NXMLParser&& otherParser) = delete;


	// Clear the parser state
	void                                Clear();


	// Get/set the options
	NXMLParserFlags                     GetOptions() const;
	void                                SetOptions(NXMLParserFlags setThese, NXMLParserFlags clearThese = kNXMLParserNone);


	// Parse a document
	NStatus                             Parse(const NFile& theFile);
	NStatus                             Parse(const NString& theText);
	NStatus                             Parse(const NData& theData);


	// Parse a chunk
	//
	// Parsing concludes when NXMLChunk::Last is submitted.
	NStatus                             Parse(size_t theSize, const void* thePtr, NXMLChunk theChunk);


	// Set the processing functions
	void                                SetProcessDocumentType(const NFunctionXMLDocumentType& theFunction);
	void                                SetProcessElementStart(const NFunctionXMLElementStart& theFunction);
	void                                SetProcessElementEnd(  const NFunctionXMLElementEnd&   theFunction);
	void                                SetProcessComment(     const NFunctionXMLComment&      theFunction);
	void                                SetProcessText(        const NFunctionXMLText&         theFunction);


private:
	NStatus                             CreateParser();
	void                                DestroyParser();

	NStatus                             ConvertXMLStatus(int32_t xmlErr);

	bool                                FlushText();
	void                                StopParsing();

	bool                                ProcessDocumentType(const NString& theName, const NXMLDocumentType& theType);
	bool                                ProcessElementStart(const NString& theName, const NDictionary& theAttributes);
	bool                                ProcessElementEnd(  const NString& theName);
	bool                                ProcessComment(     const NString& theValue);
	bool                                ProcessText(        const NString& theValue, bool isCData);

	static void                         ParsedDocumentType(void*         userData,
														   const utf8_t* itemName,
														   const utf8_t* sysID,
														   const utf8_t* pubID,
														   int           hasInternal);

	static void                         ParsedElementStart(void*          userData,
														   const utf8_t*  itemName,
														   const utf8_t** attributeList);

	static void                         ParsedElementEnd(void* userData, const utf8_t* itemName);
	static void                         ParsedComment(   void* userData, const utf8_t* itemText);
	static void                         ParsedText(      void* userData, const utf8_t* itemText, int itemSize);
	static void                         ParsedCDataStart(void* userData);
	static void                         ParsedCDataEnd(  void* userData);


private:
	struct XML_ParserStruct*            mParser;
	NXMLParserFlags                     mOptions;

	bool                                mInsideCData;
	bool                                mIsParsing;
	NString                             mParsedText;

	NFunctionXMLDocumentType            mFunctionDocumentType;
	NFunctionXMLElementStart            mFunctionElementStart;
	NFunctionXMLElementEnd              mFunctionElementEnd;
	NFunctionXMLComment                 mFunctionComment;
	NFunctionXMLText                    mFunctionText;
};



#endif // NXMLPARSER_H
