/*	NAME:
		NXMLParser.h

	DESCRIPTION:
		XML parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
//		Types
//----------------------------------------------------------------------------
// Functors
//
// Processing functors should return true to continue processing.
typedef nfunctor<bool (const NString &theName, const NDictionary &theAttributes)>		NXMLProcessElementStartFunctor;
typedef nfunctor<bool (const NString &theName)>											NXMLProcessElementEndFunctor;
typedef nfunctor<bool (const NString &theValue)>										NXMLProcessTextFunctor;
typedef nfunctor<bool (const NString &theValue)>										NXMLProcessCommentFunctor;
typedef nfunctor<bool (void)>															NXMLProcessCDATAStartFunctor;
typedef nfunctor<bool (void)>															NXMLProcessCDATAEndFunctor;


// Internal
typedef struct XML_ParserStruct		*XML_Parser;
typedef char						XML_Char;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NXMLParser {
public:
										 NXMLParser(void);
	virtual								~NXMLParser(void);


	// Clear the parser
	void								Clear(void);


	// Parse a document
	NStatus								Parse(const NFile   &theFile);
	NStatus								Parse(const NString &theText);
	NStatus								Parse(const NData   &theData);


	// Parse a fragment
	//
	// The final fragment must be closed by setting isFinal to true.
	NStatus								Parse(NIndex theSize, const void *thePtr, bool isFinal);


	// Set processing functors
	//
	// Documents can be parsed by sub-classing and overriding ProcessXXX, or
	// by assigning processing functors. If set, these functors are invoked
	// by the default implementation of ProcessXXX.
	void								SetProcessElementStart(const NXMLProcessElementStartFunctor &theFunctor);
	void								SetProcessElementEnd(  const NXMLProcessElementEndFunctor   &theFunctor);
	void								SetProcessText(        const NXMLProcessTextFunctor         &theFunctor);
	void								SetProcessComment(     const NXMLProcessCommentFunctor      &theFunctor);
	void								SetProcessCDATAStart(  const NXMLProcessCDATAStartFunctor   &theFunctor);
	void								SetProcessCDATAEnd(    const NXMLProcessCDATAEndFunctor     &theFunctor);


protected:
	// Process the items
	//
	// Processing methods should return true to continue processing.
	virtual bool						ProcessElementStart(const NString &theName, const NDictionary &theAttributes);
	virtual bool						ProcessElementEnd(  const NString &theName);
	virtual bool						ProcessText(        const NString &theValue);
	virtual bool						ProcessComment(     const NString &theValue);
	virtual bool						ProcessCDATAStart(void);
	virtual bool						ProcessCDATAEnd(  void);
	

private:
	NStatus								CreateParser( void);
	void								DestroyParser(void);

	NStatus								ConvertXMLStatus(SInt32 xmlErr);
	void								StopParsing(void);

	static void							ParsedElementStart(void *userData, const XML_Char *theName, const XML_Char **attributeList);
	static void							ParsedElementEnd(  void *userData, const XML_Char *theName);
	static void							ParsedText(        void *userData, const XML_Char *theText, int theSize);
	static void							ParsedComment(     void *userData, const XML_Char *theText);
	static void							ParsedCDATAStart(  void *userData);
	static void							ParsedCDATAEnd(    void *userData);
	

private:
	XML_Parser							mParser;

	NXMLProcessElementStartFunctor		mProcessElementStart;
	NXMLProcessElementEndFunctor		mProcessElementEnd;
	NXMLProcessTextFunctor				mProcessText;
	NXMLProcessCommentFunctor			mProcessComment;
	NXMLProcessCDATAStartFunctor		mProcessCDATAStart;
	NXMLProcessCDATAEndFunctor			mProcessCDATAEnd;
};



#endif // NXMLPARSER_HDR



