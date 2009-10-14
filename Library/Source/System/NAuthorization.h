/*	NAME:
		NAuthorization.h

	DESCRIPTION:
		Authorization object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NAUTHORIZATION_HDR
#define NAUTHORIZATION_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <Security/Security.h>

#include "NString.h"
#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Authorization item
typedef struct {
	NString		theName;
	NData		theData;
} NAuthorizationItem;


// Lists
typedef std::vector<NAuthorizationItem>							NAuthorizationItemList;
typedef NAuthorizationItemList::iterator						NAuthorizationItemListIterator;
typedef NAuthorizationItemList::const_iterator					NAuthorizationItemListConstIterator;

typedef std::vector<AuthorizationItem>							AuthorizationItemList;
typedef AuthorizationItemList::iterator							AuthorizationItemListIterator;
typedef AuthorizationItemList::const_iterator					AuthorizationItemListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NAuthorization {
public:
										NAuthorization(void);
	virtual								~NAuthorization(void);


	// Is the authorization valid?
	bool								IsValid(void) const;
	

	// Acquire/release authorisation rights
	//
	// Acquired rights will be released automatically when the object is destroyed.
	OSStatus							Authorize(const NAuthorizationItemList &theItems = NAuthorizationItemList(),
													     AuthorizationFlags		theFlags = kAuthorizationFlagDefaults     |
																						   kAuthorizationFlagExtendRights |
																						   kAuthorizationFlagInteractionAllowed);
	void								Deauthorize(void);


	// Execute a tool as root
	//
	// If an authorization has not yet been acquired, a kAuthorizationRightExecute
	// will be obtained that allows execution of the specified tool.
	//
	// Tool arguments are supplied as a NULL-terminated list of UTF8 strings.
	OSStatus							ExecuteTool(const NString &thePath, FILE **thePipe=NULL, const char *arg1=NULL, ...);


	// Construct an authorization item
	NAuthorizationItemList				CreateItem(const NString &theName, const NString &theValue="");


private:
	OSStatus							CreateAuthorization(void);
	void								DestroyAuthorization(void);
	
	AuthorizationItemList				ConvertAuthorizationItems(const NAuthorizationItemList &theItems);


private:
	AuthorizationRef					mAuthorization;
};





#endif // NAUTHORIZATION_HDR


