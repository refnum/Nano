/*	NAME:
		NDBStatement.h

	DESCRIPTION:
		Database statement.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDBSTATEMENT_HDR
#define NDBSTATEMENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Misc
typedef SInt32  NDBStatus;
typedef void   *NDBHandleRef;
typedef void   *NDBStatementRef;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDBStatement {
public:
										NDBStatement(const NString &theValue);
										NDBStatement(const char    *theValue);

										NDBStatement( void);
	virtual								~NDBStatement(void);


	// Get/set the statement
	NString								GetValue(void) const;
	void								SetValue(const NString &theValue);


	// Get/set the parameters
	NDictionary							GetParameters(void) const;
	void								SetParameters(const NDictionary &theValue);


private:
	NString								mValue;
	NDictionary							mParameters;
};




#endif // NDBSTATEMENT_HDR


