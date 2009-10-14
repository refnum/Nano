/*	NAME:
		NSpellChecker.h

	DESCRIPTION:
		Spell checker.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSPELLCHECKER_HDR
#define NSPELLCHECKER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAppKitUtilities.h"
#include "NSpellSession.h"
#include "NArray.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NString kLanguageDefault									= "";
static const NString kLanguageEnglishUSA								= "en";
static const NString kLanguageEnglishIntl								= "en_gb";
static const NString kLanguageFrench									= "fr";
static const NString kLanguageGerman									= "de";
static const NString kLanguageJapanese									= "ja";





//============================================================================
//		Types
//----------------------------------------------------------------------------
DECLARE_OBJC_CLASS(NSSpellChecker);





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSpellChecker {
public:
										NSpellChecker(void);
	virtual								~NSpellChecker(void);


	// Get/set the language
	NString								GetLanguage(void) const;
	void								SetLanguage(const NString &languageID);


	// Get the word count
	UInt32								GetWordCount(const NString &theText,
													 const NString &languageID="") const;


	// Get the suggestions for a word
	NArray								GetSuggestions(const NString &theWord) const;


	// Get the completions for a word
	NArray								GetCompletions(const NString		&theText,
														const CFRange		&theRange   = kCFRangeAll,
														NSpellSessionID		sessionID   = kNewSpellSession,
														const NString		&languageID = "") const;


	// Get the mistakes
	//
	// Returns the entire set of mistakes in the text, or the next mistake from a given offset.
	CFRangeList							GetMistakes(const NString			&theText,
														NSpellSessionID		sessionID   = kNewSpellSession,
														const NString		&languageID = "") const;

	CFRange								GetMistake(const NString			&theText,
														UInt32				theOffset   = 0,
														bool				wrapAtEnd   = false,
														NSpellSessionID		sessionID   = kNewSpellSession,
														const NString		&languageID = "") const;


private:
	NSString							*GetLanguage(const NString &languageID) const;


private:
	NSSpellChecker						*mSpellChecker;
};






#endif // NSPELLCHECKER_HDR


