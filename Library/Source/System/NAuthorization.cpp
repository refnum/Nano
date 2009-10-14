/*	NAME:
		NAuthorization.cpp

	DESCRIPTION:
		Authorization object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTextUtilities.h"
#include "NAuthorization.h"





//============================================================================
//		NAuthorization::NAuthorization : Constructor.
//----------------------------------------------------------------------------
NAuthorization::NAuthorization(void)
{


	// Initialize ourselves
	mAuthorization = NULL;
}





//============================================================================
//		NAuthorization::~NAuthorization : Destructor.
//----------------------------------------------------------------------------
NAuthorization::~NAuthorization(void)
{


	// Clean up
	if (IsValid())
		Deauthorize();
}





//============================================================================
//		NAuthorization::IsValid : Is the authorization valid?
//----------------------------------------------------------------------------
bool NAuthorization::IsValid(void) const
{


	// Check our state
	return(mAuthorization != NULL);
}





//============================================================================
//		NAuthorization::Authorize : Acquire authorization rights.
//----------------------------------------------------------------------------
OSStatus NAuthorization::Authorize(const NAuthorizationItemList &theItems, AuthorizationFlags theFlags)
{	AuthorizationRights			theRights;
	AuthorizationItemList		tmpItems;
	OSStatus					theErr;



	// Get the state we need
	tmpItems = ConvertAuthorizationItems(theItems);
	
	theRights.count = tmpItems.size();
	theRights.items = tmpItems.empty() ? NULL : &tmpItems[0];



	// Acquire the authorization rights
	theErr = CreateAuthorization();
	if (theErr == noErr)
		theErr = AuthorizationCopyRights(mAuthorization, &theRights, kAuthorizationEmptyEnvironment, theFlags, NULL);
	
	return(theErr);
}





//============================================================================
//		NAuthorization::Deauthorize : Relinquish authorization rights.
//----------------------------------------------------------------------------
void NAuthorization::Deauthorize(void)
{


	// Validate our state
	NN_ASSERT(IsValid());
	


	// Release the authorization
	DestroyAuthorization();
}





//============================================================================
//		NAuthorization::ExecuteTool : Execute a tool as root.
//----------------------------------------------------------------------------
OSStatus NAuthorization::ExecuteTool(const NString &thePath, FILE **thePipe, const char *arg1, ...)
{	NStringList						argStrings;
	NStringListConstIterator		theIter;
	std::vector<const char *>		theArgs;
	va_list							argList;
	OSStatus						theErr;



	// Collect the arguments
	va_start(argList, arg1);
	argStrings = NTextUtilities::CollectArguments(argList, arg1);
	va_end(argList);
	
	for (theIter = argStrings.begin(); theIter != argStrings.end(); theIter++)
		theArgs.push_back(theIter->GetUTF8());

	theArgs.push_back(NULL);



	// Acquire the authorization
	//
	// The most common usage, of acquiring authorization simply to execute a tool, will be
	// achieved automatically if no authorization has been acquired.
	//
	// Alternatively, a specific authorization can be obtained prior to executing the tool.
	theErr = noErr;

	if (!IsValid())
		theErr = Authorize(CreateItem(kAuthorizationRightExecute, thePath.GetUTF8()));



	// Execute the tool
	if (theErr == noErr)
		theErr = AuthorizationExecuteWithPrivileges(mAuthorization, thePath.GetUTF8(), kAuthorizationFlagDefaults, (char * const *) &theArgs[0], thePipe);
	
	return(theErr);
}





//============================================================================
//		NAuthorization::CreateItem : Construct an authorization item.
//----------------------------------------------------------------------------
NAuthorizationItemList NAuthorization::CreateItem(const NString &theName, const NString &theValue)
{	NAuthorizationItemList	theItems;
	NAuthorizationItem		theItem;



	// Construct the item
	theItem.theName = theName;

	if (theValue.IsNotEmpty())
		theItem.theData.AppendData(theValue.GetSize(), theValue.GetUTF8());
	
	theItems.push_back(theItem);

	return(theItems);
}





//============================================================================
//		NAuthorization::CreateAuthorization : Create the authorization.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NAuthorization::CreateAuthorization(void)
{	OSStatus	theErr;



	// Check our state
	if (mAuthorization != NULL)
		return(noErr);



	// Create the authorization
	theErr = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &mAuthorization);
	NN_ASSERT_NOERR(theErr);

	return(theErr);
}





//============================================================================
//		NAuthorization::DestroyAuthorization : Destroy the authorization.
//----------------------------------------------------------------------------
void NAuthorization::DestroyAuthorization(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Destroy the authorization
	theErr = AuthorizationFree(mAuthorization, kAuthorizationFlagDefaults);
	NN_ASSERT_NOERR(theErr);
		
	mAuthorization = NULL;
}





//============================================================================
//		NAuthorization::ConvertAuthorizationItems : Convert a list of authorization items.
//----------------------------------------------------------------------------
AuthorizationItemList NAuthorization::ConvertAuthorizationItems(const NAuthorizationItemList &theItems)
{	AuthorizationItemList					theResult;
	AuthorizationItem						theItem;
	NAuthorizationItemListConstIterator		theIter;



	// Construct the list
	for (theIter = theItems.begin(); theIter != theItems.end(); theIter++)
		{
		NN_ASSERT(theIter->theName.IsNotEmpty());
		memset(&theItem, 0x00, sizeof(theItem));
		
		theItem.name        = theIter->theName.GetUTF8();
		theItem.valueLength = theIter->theData.GetSize();
		
		if (theItem.valueLength != 0)
			theItem.value = (void *) theIter->theData.GetData();
		
		theResult.push_back(theItem);
		}
	
	return(theResult);
}





