/*	NAME:
		NanoTest.cpp

	DESCRIPTION:
		NanoTest app.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "Nano.h"

#include "TPoint.h"
#include "TSize.h"
#include "TRectangle.h"
#include "TRange.h"
#include "TArray.h"
#include "TData.h"
#include "TString.h"
#include "TDictionary.h"
#include "TBitVector.h"
#include "TMsgBroadcaster.h"
#include "TMsgListener.h"
#include "TCommandLine.h"
#include "TFile.h"
#include "TColor.h"





//============================================================================
//		main : Program entry point.
//----------------------------------------------------------------------------
int main(int /*argc*/, const char ** /*argv*/)
{


	// Execute the tests
	TPoint::Execute();
	TSize::Execute();
	TRectangle::Execute();
	TRange::Execute();

	TData::Execute();
	TArray::Execute();
	TDictionary::Execute();
	TString::Execute();

	TColor::Execute();
	TBitVector::Execute();

	TMsgBroadcaster::Execute();
	TMsgListener::Execute();

	TCommandLine::Execute();

	TFile::Execute();

	return(0);
}



