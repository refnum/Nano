/*	NAME:
		NXMLParser.h

	DESCRIPTION:
		XML parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NXMLPARSER_HDR
#define NXMLPARSER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NFile.h"
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// NXMLParser options
//
//		kNXMLParserSkipWhitespace
//			Skips all-whitespace text sections found outside of a CDATA section.
//
//		kNXMLParserDefault
//			Default options.
//
typedef NBitfield NXMLParserOptions;

static const NXMLParserOptions kNXMLParserNone							= 0;
static const NXMLParserOptions kNXMLParserAll							= 0xFFFFFFFF;

static const NXMLParserOptions kNXMLParserSkipWhitespace				= (1 << 0);
static const NXMLParserOptions kNXMLParserDefault						= kNXMLParserSkipWhitespace;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Document info
typedef struct {
	NString			systemID;
	NString			publicID;
	bool			hasInternal;
} NXMLDocumentTypeInfo;


// Functors
//
// Processing functors should return true to continue processing.
typedef nfunctor<bool (const NString &theName, const NXMLDocumentTypeInfo &theInfo)>	NXMLProcessDocumentTypeFunctor;
typedef nfunctor<bool (const NString &theName, const NDictionary &theAttributes)>		NXMLProcessElementStartFunctor;
typedef nfunctor<bool (const NString &theName)>											NXMLProcessElementEndFunctor;
typedef nfunctor<bool (const NString &theValue)>										NXMLProcessCommentFunctor;
typedef nfunctor<bool (const NString &theValue, bool isCData)>							NXMLProcessTextFunctor;


// Internal
typedef struct XML_ParserStruct		*XML_Parser;
typedef char						XML_Char;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NXMLParser {
public:
										NXMLParser(void);
	virtual							   ~NXMLParser(void);


	// Clear the parser state
	void								Clear(void);


	// Get/set the options
	NXMLParserOptions					GetOptions(void) const;
	void								SetOptions(NXMLParserOptions setThese, NXMLParserOptions clearThese=kNXMLParserNone);


	// Parse a document
	NStatus								Parse(const NFile   &theFile);
	NStatus								Parse(const NString &theText);
	NStatus								Parse(const NData   &theData);


	// Parse a fragment
	//
	// The final fragment is indicated by setting isFinal to true.
	NStatus								Parse(NIndex theSize, const void *thePtr, bool isFinal);


	// Set processing functors
	//
	// Documents can be parsed by sub-classing and overriding ProcessXXX, or by
	// assigning processing functors. If set, these functors are invoked by the
	// default implementation of ProcessXXX.
	void								SetProcessDocumentType(const NXMLProcessDocumentTypeFunctor &theFunctor);
	void								SetProcessElementStart(const NXMLProcessElementStartFunctor &theFunctor);
	void								SetProcessElementEnd(  const NXMLProcessElementEndFunctor   &theFunctor);
	void								SetProcessComment(     const NXMLProcessCommentFunctor      &theFunctor);
	void								SetProcessText(        const NXMLProcessTextFunctor         &theFunctor);


protected:
	// Process the items
	//
	// Processing methods should return true to continue processing.
	virtual bool						ProcessDocumentType(const NString &theName, const NXMLDocumentTypeInfo &theInfo);
	virtual bool						ProcessElementStart(const NString &theName, const NDictionary          &theAttributes);
	virtual bool						ProcessElementEnd(  const NString &theName);
	virtual bool						ProcessComment(     const NString &theValue);
	virtual bool						ProcessText(        const NString &theValue, bool isCData);


private:
	NStatus								CreateParser( void);
	void								DestroyParser(void);

	NStatus								ConvertXMLStatus(SInt32 xmlErr);
	
	bool								FlushText(  void);
	void								StopParsing(void);

	static void							ParsedDocumentType(void *userData, const XML_Char *itemName, const XML_Char *sysID, const XML_Char *pubID, int hasInternal);
	static void							ParsedElementStart(void *userData, const XML_Char *itemName, const XML_Char **attributeList);
	static void							ParsedElementEnd(  void *userData, const XML_Char *itemName);
	static void							ParsedComment(     void *userData, const XML_Char *itemText);
	static void							ParsedText(        void *userData, const XML_Char *itemText, int itemSize);
	static void							ParsedCDataStart(  void *userData);
	static void							ParsedCDataEnd(    void *userData);
	

private:
	XML_Parser							mParser;
	NXMLParserOptions					mOptions;

	bool								mInsideCData;
	NString								mParsedText;

	NXMLProcessDocumentTypeFunctor		mProcessDocumentType;
	NXMLProcessElementStartFunctor		mProcessElementStart;
	NXMLProcessElementEndFunctor		mProcessElementEnd;
	NXMLProcessCommentFunctor			mProcessComment;
	NXMLProcessTextFunctor				mProcessText;
};



#endif // NXMLPARSER_HDR



