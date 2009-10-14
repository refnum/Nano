/*	NAME:
		NHelpUtilities.cpp

	DESCRIPTION:
		Help utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHelpUtilities.h"





//============================================================================
//		NHelpUtilities::GetEmptyHelp : Get an empty help tag.
//----------------------------------------------------------------------------
HMHelpContentRec NHelpUtilities::GetEmptyHelp(void)
{	HMHelpContentRec	helpTag;



	// Get an empty help tag
	memset(&helpTag, 0x00, sizeof(helpTag));

	helpTag.version                = kMacHelpVersion;
	helpTag.content[0].contentType = kHMNoContent;
	helpTag.content[1].contentType = kHMNoContent;
	
	return(helpTag);
}





//============================================================================
//		NHelpUtilities::GetHelpString : Get a help string.
//----------------------------------------------------------------------------
NString NHelpUtilities::GetHelpString(const HMHelpContentRec &helpTag, bool isExtended)
{	const HMHelpContent		*theContent;
	NString					theValue;



	// Get the help string
	theContent = isExtended ? &helpTag.content[1] : &helpTag.content[0];
		
	switch (theContent->contentType) {
		case kHMCFStringContent:
			theValue = theContent->u.tagCFString;
			break;
			
		case kHMNoContent:
			break;
				
		default:
			NN_LOG("Unknown help content: %d", theContent->contentType);
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NHelpUtilities::SetHelpString : Set a help string.
//----------------------------------------------------------------------------
void NHelpUtilities::SetHelpString(HMHelpContentRec &helpTag, const NString &theValue, bool isExtended)
{	HMHelpContent	*theContent;



	// Set the help string
	theContent = isExtended ? &helpTag.content[1] : &helpTag.content[0];

	theContent->contentType   = kHMCFStringContent;
	theContent->u.tagCFString = theValue;
}

