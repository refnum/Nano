/*	NAME:
		NSpellSession.cpp

	DESCRIPTION:
		Spell check session.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAppKitUtilities.h"
#include "NSpellSession.h"





//============================================================================
//		NSpellSession::NSpellSession : Constructor.
//----------------------------------------------------------------------------
NSpellSession::NSpellSession(NSpellSessionID theID)
{


	// Initialize ourselves
	SetID(theID);
}





//============================================================================
//		NSpellSession::~NSpellSession : Destructor.
//----------------------------------------------------------------------------
NSpellSession::~NSpellSession(void)
{	StAutoReleasePool	thePool;



	// Clean up
	[[NSSpellChecker sharedSpellChecker] closeSpellDocumentWithTag:mID];
}





//============================================================================
//		NSpellSession::GetID : Get the session ID.
//----------------------------------------------------------------------------
NSpellSessionID NSpellSession::GetID(void) const
{


	// Get the session ID
	return(mID);
}





//============================================================================
//		NSpellSession::SetID : Set the session ID.
//----------------------------------------------------------------------------
void NSpellSession::SetID(NSpellSessionID theID)
{


	// Set the session ID
	mID = (theID == kNewSpellSession) ? CreateID() : theID;
}





//============================================================================
//		NSpellSession::GetIgnoredWords : Get the ignored words list.
//----------------------------------------------------------------------------
NArray NSpellSession::GetIgnoredWords(void) const
{	NArray				theResult;
	StAutoReleasePool	thePool;



	// Get the list
	theResult.Set([[NSSpellChecker sharedSpellChecker] ignoredWordsInSpellDocumentWithTag:mID], false);

	return(theResult);
}





//============================================================================
//		NSpellSession::SetIgnoredWords : Set the ignored words list.
//----------------------------------------------------------------------------
void NSpellSession::SetIgnoredWords(const NArray &theWords)
{	StAutoReleasePool	thePool;



	// Set the list
    [[NSSpellChecker sharedSpellChecker] setIgnoredWords:theWords inSpellDocumentWithTag:mID];
}





//============================================================================
//		NSpellSession::IgnoreWord : Add a word to the ignored words list.
//----------------------------------------------------------------------------
void NSpellSession::IgnoreWord(const NString &theWord)
{	StAutoReleasePool	thePool;



	// Update the list
    [[NSSpellChecker sharedSpellChecker] ignoreWord:theWord inSpellDocumentWithTag:mID];
}





//============================================================================
//		NSpellSession::operator NSpellSessionID : NSpellSessionID-cast operator.
//----------------------------------------------------------------------------
NSpellSession::operator NSpellSessionID(void) const
{


	// Get the ID
	return(mID);
}





//============================================================================
//		NSpellSession::CreateID : Create a unique session ID.
//----------------------------------------------------------------------------
NSpellSessionID NSpellSession::CreateID(void)
{	StAutoReleasePool	thePool;



	// Create a unique session ID
	return([NSSpellChecker uniqueSpellDocumentTag]);
}

