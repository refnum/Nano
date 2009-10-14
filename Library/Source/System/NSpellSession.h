/*	NAME:
		NSpellSession.h

	DESCRIPTION:
		Spell check session.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSPELLSESSION_HDR
#define NSPELLSESSION_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NArray.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
typedef SInt32 NSpellSessionID;

static const NSpellSessionID kNewSpellSession							= 0;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSpellSession {
public:
										NSpellSession(NSpellSessionID theID=kNewSpellSession);
	virtual								~NSpellSession(void);


	// Get/set the session ID
	NSpellSessionID						GetID(void) const;
	void								SetID(NSpellSessionID theID);


	// Get/set the ignored words list
	NArray								GetIgnoredWords(void) const;
	void								SetIgnoredWords(const NArray &theWords);


	// Add a word to the ignore list
	void								IgnoreWord(const NString &theValue);


	// Operators
	operator							NSpellSessionID(void) const;


	// Create a unique session ID
	static NSpellSessionID				CreateID(void);


private:
	NSpellSessionID						mID;
};




#endif // NSPELLSESSION_HDR


