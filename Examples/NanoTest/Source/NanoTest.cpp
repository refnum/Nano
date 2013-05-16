/*	NAME:
		NanoTest.cpp

	DESCRIPTION:
		NanoTest app.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#if NN_TARGET_MAC
	#include "NNSAutoReleasePool.h"
	#include "TNSAutoReleasePool.h"
	#include "TNSSingleton.h"

	#include "TCFArray.h"
	#include "TCFData.h"
	#include "TCFDate.h"
	#include "TCFDictionary.h"
	#include "TCFNumber.h"
	#include "TCFObject.h"
	#include "TCFString.h"
	#include "TCFURL.h"
#endif


#include "NCommandLine.h"

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
#include "TFileIterator.h"
#include "TFileMap.h"
#include "TPropertyList.h"

#include "TColor.h"
#include "TPoint.h"
#include "TRectangle.h"
#include "TShape.h"
#include "TSize.h"
#include "TTransform.h"
#include "TVector.h"

#include "TMessageClient.h"
#include "TMessageServer.h"
#include "TNetworkBrowser.h"
#include "TNetworkManager.h"
#include "TNetworkMessage.h"
#include "TNetworkService.h"
#include "TSocket.h"
#include "TURLRequest.h"
#include "TURLResponse.h"

#include "TByteSwap.h"
#include "TCache.h"
#include "TCacheKey.h"
#include "TCacheValue.h"
#include "TDataCipher.h"
#include "TDataCompressor.h"
#include "TDataDigest.h"
#include "TDataEncoder.h"
#include "TJSONEncoder.h"
#include "TLocale.h"
#include "TUnicodeParser.h"
#include "TUTIRegistry.h"
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
#include "TAttributes.h"
#include "TBitVector.h"
#include "TData.h"
#include "TDate.h"
#include "TDateFormatter.h"
#include "TDictionary.h"
#include "TEncoder.h"
#include "TFunctor.h"
#include "TNumber.h"
#include "TPropertyStore.h"
#include "TRange.h"
#include "TSingleton.h"
#include "TString.h"
#include "TStringEncoder.h"
#include "TStringFormatter.h"
#include "TURL.h"
#include "TUTI.h"
#include "TVariant.h"

#include "TFileUtilities.h"
#include "TGeometryUtilities.h"
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
	NCommandLine::Get()->SetArguments(argc, argv);



	// Mac
#if NN_TARGET_MAC
	StAutoReleasePool		thePool;


	// Nanite - Cocoa
	TNSAutoReleasePool::Execute();
	TNSSingleton::Execute();



	// Nanite - CoreFoundation
	TCFArray::Execute();
	TCFData::Execute();
	TCFDate::Execute();
	TCFDictionary::Execute();
	TCFNumber::Execute();
	TCFObject::Execute();
	TCFString::Execute();
	TCFURL::Execute();
#endif // NN_TARGET_MAC



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
	TFileIterator::Execute();
	TFileMap::Execute();
	TPropertyList::Execute();



	// Geometry
	TColor::Execute();
	TPoint::Execute();
	TRectangle::Execute();
	TShape::Execute();
	TSize::Execute();
	TTransform::Execute();
	TVector::Execute();



	// Network
	TMessageClient::Execute();
	TMessageServer::Execute();
	TNetworkBrowser::Execute();
	TNetworkManager::Execute();
	TNetworkMessage::Execute();
	TNetworkService::Execute();
	TSocket::Execute();
	TURLRequest::Execute();
	TURLResponse::Execute();



	// System
	TByteSwap::Execute();
	TCache::Execute();
	TCacheKey::Execute();
	TCacheValue::Execute();
	TDataCipher::Execute();
	TDataCompressor::Execute();
	TDataDigest::Execute();
	TDataEncoder::Execute();
	TJSONEncoder::Execute();
	TLocale::Execute();
	TUnicodeParser::Execute();
	TUTIRegistry::Execute();
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
	TAttributes::Execute();
	TBitVector::Execute();
	TData::Execute();
	TDate::Execute();
	TDateFormatter::Execute();
	TDictionary::Execute();
	TEncoder::Execute();
	TFunctor::Execute();
	TNumber::Execute();
	TPropertyStore::Execute();
	TRange::Execute();
	TSingleton::Execute();
	TString::Execute();
	TStringEncoder::Execute();
	TStringFormatter::Execute();
	TURL::Execute();
	TUTI::Execute();
	TVariant::Execute();



	// Utilities
	TFileUtilities::Execute();
	TGeometryUtilities::Execute();
	TMathUtilities::Execute();
	TSTLUtilities::Execute();
	TSystemUtilities::Execute();
	TTextUtilities::Execute();
	TThreadUtilities::Execute();
	TTimeUtilities::Execute();

	return(0);
}



