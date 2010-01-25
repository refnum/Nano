/*	NAME:
		TLocale.cpp

	DESCRIPTION:
		NLocale tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLocale.h"

#include "TLocale.h"





//============================================================================
//		TLocale::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TLocale::Execute(void)
{	NLocale		theLocale;
	bool		valueBool;



	// Execute the tests
	valueBool = theLocale.IsMetric();
	NN_ASSERT(valueBool == true || valueBool == false);

}









