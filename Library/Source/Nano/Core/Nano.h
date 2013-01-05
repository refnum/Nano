/*	NAME:
		Nano.h

	DESCRIPTION:
		Nano Library.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_HDR
#define NANO_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
// Bootstrap
#include "NanoTarget.h"
#include "NanoNotes.h"


// Mac
#if NN_TARGET_MAC
	#include <Carbon/Carbon.h>

	#if defined(__OBJC__)
		#include <Cocoa/Cocoa.h>
	#endif
#endif


// Windows
#if NN_TARGET_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	
	#ifndef UNICODE
	#define UNICODE
	#endif
	
	#ifndef _UNICODE
	#define _UNICODE
	#endif

	#include <Windows.h>
#endif


// iOS
#if NN_TARGET_IOS
	#include <CoreFoundation/CoreFoundation.h>
	#include <CoreGraphics/CoreGraphics.h>
	#include <QuartzCore/QuartzCore.h>

	#if defined(__OBJC__)
		#include <Foundation/Foundation.h>
		#include <UIKit/UIKit.h>
	#endif
#endif


// Nano
#include "NanoTypes.h"
#include "NanoMacros.h"
#include "NDebug.h"

#if defined(__cplusplus)
	#include "NCommandLine.h"
	#include "NPreferences.h"
	#include "NUndoManager.h"
	#include "NUndoSource.h"

	#include "NDBHandle.h"
	#include "NDBHandlePool.h"
	#include "NDBQuery.h"
	#include "NDBResult.h"

	#include "NBroadcaster.h"
	#include "NListener.h"
	#include "NTimer.h"

	#include "NBundle.h"
	#include "NFile.h"
	#include "NFileIterator.h"
	#include "NFileMap.h"
	#include "NPropertyList.h"

	#include "NColor.h"
	#include "NPoint.h"
	#include "NRectangle.h"
	#include "NShape.h"
	#include "NSize.h"
	#include "NTransform.h"
	#include "NVector.h"

	#include "NMessageClient.h"
	#include "NMessageServer.h"
	#include "NNetworkBrowser.h"
	#include "NNetworkManager.h"
	#include "NNetworkMessage.h"
	#include "NNetworkService.h"
	#include "NSocket.h"
	#include "NURLRequest.h"
	#include "NURLResponse.h"

	#include "NByteSwap.h"
	#include "NCache.h"
	#include "NCacheKey.h"
	#include "NCacheValue.h"
	#include "NDataCipher.h"
	#include "NDataCompressor.h"
	#include "NDataDigest.h"
	#include "NDataEncoder.h"
	#include "NJSONEncoder.h"
	#include "NLocale.h"
	#include "NTask.h"
	#include "NUnicodeParser.h"
	#include "NUTIRegistry.h"
	#include "NXMLEncoder.h"
	#include "NXMLNode.h"
	#include "NXMLParser.h"

	#include "NAtomicInt.h"
	#include "NAtomicList.h"
	#include "NLock.h"
	#include "NSemaphore.h"
	#include "NThread.h"
	#include "NThreadPool.h"
	#include "NThreadTask.h"

	#include "NArray.h"
	#include "NAttributes.h"
	#include "NBitVector.h"
	#include "NComparable.h"
	#include "NContainer.h"
	#include "NData.h"
	#include "NDate.h"
	#include "NDateFormatter.h"
	#include "NDictionary.h"
	#include "NEncodable.h"
	#include "NEncoder.h"
	#include "NFunctor.h"
	#include "NHashable.h"
	#include "NNumber.h"
	#include "NProgressable.h"
	#include "NPropertyStore.h"
	#include "NRange.h"
	#include "NRefCounted.h"
	#include "NSharedValue.h"
	#include "NSingleton.h"
	#include "NString.h"
	#include "NStringEncoder.h"
	#include "NStringFormatter.h"
	#include "NUncopyable.h"
	#include "NURL.h"
	#include "NUTI.h"
	#include "NVariant.h"

	#include "NFileUtilities.h"
	#include "NGeometryUtilities.h"
	#include "NMathUtilities.h"
	#include "NSTLUtilities.h"
	#include "NSystemUtilities.h"
	#include "NTextUtilities.h"
	#include "NThreadUtilities.h"
	#include "NTimeUtilities.h"
#endif



#endif // NANO_HDR


