/*	NAME:
		NAlias.h

	DESCRIPTION:
		File alias.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NALIAS_HDR
#define NALIAS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NFile.h"
#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NAlias;

typedef std::vector<NAlias>										NAliasList;
typedef NAliasList::iterator									NAliasListIterator;
typedef NAliasList::const_iterator								NAliasListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NAlias {
public:
										NAlias(const NFile  &theFile);
										NAlias(const NAlias &theAlias);
										
										NAlias(void);
	virtual								~NAlias(void);


	// Is the alias valid?
	//
	// An alias is valid when it tracks a file system object which exists.
	bool								IsValid(void) const;
	

	// Get/set the file
	//
	// An NAlias can be assigned a file which does not exist, and have the
	// same file retrieved via GetFile.
	//
	// This tracking does not have the normal alias characteristics, and the
	// file will only be tracked as an alias if it points to a file system
	// object which exists when the file is assigned or retrieved.
	NFile								GetFile(void) const;
	void								SetFile(const NFile &theFile);


	// Get/set the alias data
	//
	// The alias must refer to a file system object which exists in order to
	// be converted to an NData.
	NData								GetData(void) const;
	void								SetData(const NData &theData);


	// Operators
	const NAlias&						operator = (const NAlias &theFile);
	operator							AliasHandle(void) const;


private:
	void								DisposeAlias(void);
	

private:
	mutable NFile						mFile;
	AliasHandle							mAlias;
};





#endif // NALIAS_HDR
