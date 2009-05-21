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

#include "TCFObject.h"
#include "TCFString.h"

#include "TCommandLine.h"

#include "TDebug.h"

#include "TBroadcaster.h"
#include "TListener.h"

#include "TFile.h"

#include "TTargetDebug.h"
#include "TTargetFile.h"
#include "TTargetLock.h"
#include "TTargetMath.h"
#include "TTargetSystem.h"
#include "TTargetThread.h"
#include "TTargetTime.h"

#include "TByteSwap.h"
#include "TChecksum.h"
#include "TUnicodeParser.h"

#include "TAtomicInt.h"
#include "TLock.h"

#include "TArray.h"
#include "TBitVector.h"
#include "TColor.h"
#include "TData.h"
#include "TDate.h"
#include "TDictionary.h"
#include "TFunctor.h"
#include "TNumber.h"
#include "TPoint.h"
#include "TRange.h"
#include "TRectangle.h"
#include "TSize.h"
#include "TString.h"
#include "TStringEncoder.h"
#include "TStringFormatter.h"
#include "TVariant.h"

#include "TFileUtilities.h"
#include "TMathUtilities.h"
#include "TSTLUtilities.h"
#include "TSystemUtilities.h"
#include "TThreadUtilities.h"
#include "TTimeUtilities.h"





//============================================================================
//		main : Program entry point.
//----------------------------------------------------------------------------
int main(int /*argc*/, const char ** /*argv*/)
{


	// Application
	TCommandLine::Execute();


	// Core
	TDebug::Execute();



	// Events
	TBroadcaster::Execute();
	TListener::Execute();
	
	
	// Files
	TFile::Execute();


	// Internal/Targets
	TTargetDebug::Execute();
	TTargetFile::Execute();
	TTargetLock::Execute();
	TTargetMath::Execute();
	TTargetSystem::Execute();
	TTargetThread::Execute();
	TTargetTime::Execute();
	
	
	// System
	TByteSwap::Execute();
	TChecksum::Execute();
	TUnicodeParser::Execute();



	// Threads
	TAtomicInt::Execute();
	TLock::Execute();



	// Types
	TArray::Execute();
	TBitVector::Execute();
	TColor::Execute();
	TData::Execute();
	TDate::Execute();
	TDictionary::Execute();
	TFunctor::Execute();
	TNumber::Execute();
	TPoint::Execute();
	TRange::Execute();
	TRectangle::Execute();
	TSize::Execute();
	TString::Execute();
	TStringEncoder::Execute();
	TStringFormatter::Execute();
	TVariant::Execute();



	// Utilities
	TFileUtilities::Execute();
	TMathUtilities::Execute();
	TSTLUtilities::Execute();
	TSystemUtilities::Execute();
	TThreadUtilities::Execute();
	TTimeUtilities::Execute();


	// Nanite - CoreFoundation
	TCFObject::Execute();
	TCFString::Execute();
	
	return(0);
}



