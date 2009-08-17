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
#include "NCommandLine.h"

#include "TNSSingleton.h"

#include "TCFArray.h"
#include "TCFData.h"
#include "TCFDate.h"
#include "TCFDictionary.h"
#include "TCFNumber.h"
#include "TCFObject.h"
#include "TCFString.h"

#include "TCommandLine.h"
#include "TPreferences.h"
#include "TUndoManager.h"
#include "TUndoSource.h"

#include "TDebug.h"

#include "TDBHandle.h"
#include "TDBHandlePool.h"

#include "TBroadcaster.h"
#include "TListener.h"
#include "TTimer.h"

#include "TBundle.h"
#include "TFile.h"
#include "TFileMap.h"
#include "TPropertyList.h"

#include "TPoint.h"
#include "TRectangle.h"
#include "TSize.h"
#include "TVector.h"

#include "TB64Encoder.h"
#include "TByteSwap.h"
#include "TChecksum.h"
#include "TDataCompressor.h"
#include "TLocale.h"
#include "TUnicodeParser.h"
#include "TXMLEncoder.h"
#include "TXMLParser.h"
#include "TXMLNode.h"

#include "TAtomicInt.h"
#include "TAtomicList.h"
#include "TLock.h"
#include "TSemaphore.h"
#include "TThread.h"
#include "TThreadPool.h"

#include "TArray.h"
#include "TBitVector.h"
#include "TColor.h"
#include "TData.h"
#include "TDate.h"
#include "TDateFormatter.h"
#include "TDictionary.h"
#include "TEncoder.h"
#include "TFunctor.h"
#include "TNumber.h"
#include "TPropertyStore.h"
#include "TRange.h"
#include "TString.h"
#include "TStringEncoder.h"
#include "TStringFormatter.h"
#include "TUTI.h"
#include "TVariant.h"

#include "TFileUtilities.h"
#include "TMathUtilities.h"
#include "TSTLUtilities.h"
#include "TSystemUtilities.h"
#include "TTextUtilities.h"
#include "TThreadUtilities.h"
#include "TTimeUtilities.h"





//============================================================================
//		main : Program entry point.
//----------------------------------------------------------------------------
int main(int argc, const char **argv)
{


	// Initialise ourselves
	*NCommandLine::Get() = NCommandLine(argc, argv);



	// Nanite - Cocoa
	TNSSingleton::Execute();



	// Nanite - CoreFoundation
	TCFArray::Execute();
	TCFData::Execute();
	TCFDate::Execute();
	TCFDictionary::Execute();
	TCFNumber::Execute();
	TCFObject::Execute();
	TCFString::Execute();



	// Application
	TCommandLine::Execute();
	TPreferences::Execute();
	TUndoManager::Execute();
	TUndoSource::Execute();



	// Core
	TDebug::Execute();



	// Database
	TDBHandle::Execute();
	TDBHandlePool::Execute();



	// Events
	TBroadcaster::Execute();
	TListener::Execute();
	TTimer::Execute();
	

	
	// Files
	TBundle::Execute();
	TFile::Execute();
	TFileMap::Execute();
	TPropertyList::Execute();



	// Geometry
	TPoint::Execute();
	TRectangle::Execute();
	TSize::Execute();
	TVector::Execute();



	// System
	TB64Encoder::Execute();
	TByteSwap::Execute();
	TChecksum::Execute();
	TDataCompressor::Execute();
	TLocale::Execute();
	TUnicodeParser::Execute();
	TXMLEncoder::Execute();
	TXMLParser::Execute();
	TXMLNode::Execute();



	// Threads
	TAtomicInt::Execute();
	TAtomicList::Execute();
	TLock::Execute();
	TSemaphore::Execute();
	TThread::Execute();
	TThreadPool::Execute();



	// Types
	TArray::Execute();
	TBitVector::Execute();
	TColor::Execute();
	TData::Execute();
	TDate::Execute();
	TDateFormatter::Execute();
	TDictionary::Execute();
	TEncoder::Execute();
	TFunctor::Execute();
	TNumber::Execute();
	TPropertyStore::Execute();
	TRange::Execute();
	TString::Execute();
	TStringEncoder::Execute();
	TStringFormatter::Execute();
	TUTI::Execute();
	TVariant::Execute();



	// Utilities
	TFileUtilities::Execute();
	TMathUtilities::Execute();
	TSTLUtilities::Execute();
	TSystemUtilities::Execute();
	TTextUtilities::Execute();
	TThreadUtilities::Execute();
	TTimeUtilities::Execute();
	
	return(0);
}



