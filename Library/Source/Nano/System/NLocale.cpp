/*	NAME:
		NLocale.cpp

	DESCRIPTION:
		System locale.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLocale.h"





//============================================================================
//		NLocale::NLocale : Constructor.
//----------------------------------------------------------------------------
NLocale::NLocale(const NString &theID)
{


	// Initialise ourselves
	mID = theID;
}





//============================================================================
//		NLocale::~NLocale : Destructor.
//----------------------------------------------------------------------------
NLocale::~NLocale(void)
{
}





//============================================================================
//		NLocale::IsMetric : Is the measurement system metric?
//----------------------------------------------------------------------------
bool NLocale::IsMetric(void) const
{


	// To do
	NN_LOG("NLocale::IsMetric - NOT IMPLEMENTED");

	return(true);
}
