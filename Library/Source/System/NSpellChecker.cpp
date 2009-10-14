/*	NAME:
		NSpellChecker.cpp

	DESCRIPTION:
		NSpellChecker provides access to the system spell check library.
		
		See NSpellingPanel for spelling panel support.
		
		Objects that wish to spell check some text can create an NSpellChecker,
		and use it to find mistakes within the text (or provide suggestions,
		completions, or a word count).
		
		Objects that can visually represent spell-checked text would typically
		derive from NSpellableView, which uses an NSpellChecker to support the
		standard user interface for spelling and allows mistakes in the text to
		be overlaid with the standard "red underline" appearance.
		
		These objects can also use NSpellingPanel to support the standard
		spelling panel.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSpellChecker.h"





//============================================================================
//		NSpellChecker::NSpellChecker : Constructor.
//----------------------------------------------------------------------------
NSpellChecker::NSpellChecker(void)
{


	// Initialize ourselves
	mSpellChecker = [NSSpellChecker sharedSpellChecker];
}





//============================================================================
//		NSpellChecker::~NSpellChecker : Destructor.
//----------------------------------------------------------------------------
NSpellChecker::~NSpellChecker(void)
{
}





//============================================================================
//		NSpellChecker::GetLanguage : Get the language.
//----------------------------------------------------------------------------
NString NSpellChecker::GetLanguage(void) const
{	NString				theResult;
	StAutoReleasePool	thePool;



	// Get the language
	theResult.Set([mSpellChecker language], false);
	
	return(theResult);
}





//============================================================================
//		NSpellChecker::SetLanguage : Set the language.
//----------------------------------------------------------------------------
void NSpellChecker::SetLanguage(const NString &languageID)
{	StAutoReleasePool	thePool;



	// Set the language
	[mSpellChecker setLanguage:languageID];
}





//============================================================================
//		NSpellChecker::GetWordCount : Get the number of words.
//----------------------------------------------------------------------------
UInt32 NSpellChecker::GetWordCount(const NString &theText, const NString &languageID) const
{	UInt32				theResult;
	StAutoReleasePool	thePool;



	// Count the words
	theResult = [mSpellChecker countWordsInString:theText language:GetLanguage(languageID)];
	
	return(theResult);
}





//============================================================================
//		NSpellChecker::GetSuggestions : Get the suggestions for a word.
//----------------------------------------------------------------------------
NArray NSpellChecker::GetSuggestions(const NString &theWord) const
{	NArray				theResult;
	StAutoReleasePool	thePool;



	// Get the suggestions
	theResult.Set([mSpellChecker guessesForWord:theWord], false);
	
	return(theResult);
}





//============================================================================
//		NSpellChecker::GetCompletions : Get the completions for a word.
//----------------------------------------------------------------------------
NArray NSpellChecker::GetCompletions(const NString &theText, const CFRange &theRange, NSpellSessionID sessionID, const NString &languageID) const
{	NArray				theResult;
	StAutoReleasePool	thePool;
	NSRange				nsRange;



	// Convert the range
	if (theRange == kCFRangeAll)
		nsRange = NSMakeRange(0, theText.GetSize());
	else
		nsRange = NSMakeRange(theRange.location, theRange.length);



	// Get the completions
	theResult.Set([mSpellChecker completionsForPartialWordRange:nsRange
													   inString:theText
													   language:GetLanguage(languageID)
										 inSpellDocumentWithTag:sessionID], false);

	return(theResult);
}





//============================================================================
//		NSpellChecker::GetMistakes : Get the mistakes.
//----------------------------------------------------------------------------
CFRangeList NSpellChecker::GetMistakes(const NString &theText, NSpellSessionID sessionID, const NString &languageID) const
{	CFRangeList			theResult;
	UInt32				theOffset;
	CFRange				theRange;
	StAutoReleasePool	thePool;



	// Get the mistakes
	theOffset = 0;
	do
		{
		theRange = GetMistake(theText, theOffset, false, sessionID, languageID);
		if (theRange != kCFRangeNone)
			{
			theResult.push_back(theRange);
			theOffset = theRange.location + theRange.length;
			}
		}
	while (theRange != kCFRangeNone);
	
	return(theResult);
}





//============================================================================
//		NSpellChecker::GetMistake : Get a mistake.
//----------------------------------------------------------------------------
CFRange NSpellChecker::GetMistake(const NString &theText, UInt32 theOffset, bool wrapAtEnd, NSpellSessionID sessionID, const NString &languageID) const
{	CFRange				theResult;
	StAutoReleasePool	thePool;
	NSRange				nsRange;



	// Get the next mistake
	nsRange = [mSpellChecker checkSpellingOfString:theText
										startingAt:theOffset
										  language:GetLanguage(languageID)
											  wrap:wrapAtEnd
							inSpellDocumentWithTag:sessionID
										 wordCount:NULL];

	if (nsRange.location >= theText.GetSize())
		theResult = kCFRangeNone;
	else
		theResult = CFRangeMake(nsRange);
	
	return(theResult);
}





//============================================================================
//		NSpellChecker::GetLanguage : Get the language.
//----------------------------------------------------------------------------
#pragma mark -
NSString *NSpellChecker::GetLanguage(const NString &languageID) const
{


	// Get the language
	//
	// NSSpellChecker requires a NULL string to indicate the default language.
	if (languageID.IsEmpty())
		return(NULL);
	
	return(languageID);
}


