/*	NAME:
		NDBStatement.cpp

	DESCRIPTION:
		Database statement.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDBStatement.h"





//============================================================================
//		NDBStatement::NDBStatement : Constructor.
//----------------------------------------------------------------------------
NDBStatement::NDBStatement(const NString &theValue)
{


	// Initialize ourselves
	SetValue(theValue);
}





//============================================================================
//		NDBStatement::NDBStatement : Constructor.
//----------------------------------------------------------------------------
NDBStatement::NDBStatement(const char *theValue)
{


	// Initialize ourselves
	SetValue(theValue);
}





//============================================================================
//		NDBStatement::NDBStatement : Constructor.
//----------------------------------------------------------------------------
NDBStatement::NDBStatement(void)
{
}





//============================================================================
//		NDBStatement::~NDBStatement : Destructor.
//----------------------------------------------------------------------------
NDBStatement::~NDBStatement(void)
{
}





//============================================================================
//		NDBStatement::GetValue : Get the value.
//----------------------------------------------------------------------------
NString NDBStatement::GetValue(void) const
{


	// Get the value
	return(mValue);
}





//============================================================================
//		NDBStatement::SetValue : Set the value.
//----------------------------------------------------------------------------
void NDBStatement::SetValue(const NString &theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue.IsNotEmpty());



	// Set the value
	mValue = theValue;
}





//============================================================================
//		NDBStatement::GetParameters : Get the parameters.
//----------------------------------------------------------------------------
NDictionary NDBStatement::GetParameters(void) const
{


	// Get the parameters
	return(mParameters);
}





//============================================================================
//		NDBStatement::SetParameters : Set the parameters.
//----------------------------------------------------------------------------
void NDBStatement::SetParameters(const NDictionary &theValue)
{


	// Set the parameters
	mParameters = theValue;
}

