#==============================================================================
#	NAME:
#		CMakeLists.txt
#
#	DESCRIPTION:
#		NanoTest project.
#
#	COPYRIGHT:
#		Copyright (c) 2006-2021, refNum Software
#		All rights reserved.
#
#		Redistribution and use in source and binary forms, with or without
#		modification, are permitted provided that the following conditions
#		are met:
#		
#		1. Redistributions of source code must retain the above copyright
#		notice, this list of conditions and the following disclaimer.
#		
#		2. Redistributions in binary form must reproduce the above copyright
#		notice, this list of conditions and the following disclaimer in the
#		documentation and/or other materials provided with the distribution.
#		
#		3. Neither the name of the copyright holder nor the names of its
#		contributors may be used to endorse or promote products derived from
#		this software without specific prior written permission.
#		
#		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#==============================================================================
#		Project
#------------------------------------------------------------------------------
project(NanoTest VERSION 4.0)

nano_project(APP)





#==============================================================================
#		Common
#------------------------------------------------------------------------------
nano_project_sources(
	PRIVATE
		"${NN_SOURCE_DIR}/../../Library/Source/Nanites/Catch/NTestFixture.cpp"
		"${NN_SOURCE_DIR}/Benchmarks/BThreadPool.cpp"
		"${NN_SOURCE_DIR}/Nano/Application/TCommandLine.cpp"
		"${NN_SOURCE_DIR}/Nano/Application/TUndoManager.cpp"
		"${NN_SOURCE_DIR}/Nano/Application/TUndoSource.cpp"
		"${NN_SOURCE_DIR}/Nano/Data/TData.cpp"
		"${NN_SOURCE_DIR}/Nano/Data/TDataCipher.cpp"
		"${NN_SOURCE_DIR}/Nano/Data/TDataCompressor.cpp"
		"${NN_SOURCE_DIR}/Nano/Data/TDataDigest.cpp"
		"${NN_SOURCE_DIR}/Nano/Data/TDataEncoder.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/TDebug.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/TDebugger.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/TLog.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/TLogOutput.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/TUsageBucket.cpp"
		"${NN_SOURCE_DIR}/Nano/Debugging/TUsageLog.cpp"
		"${NN_SOURCE_DIR}/Nano/Events/TBroadcaster.cpp"
		"${NN_SOURCE_DIR}/Nano/Events/TListener.cpp"
		"${NN_SOURCE_DIR}/Nano/Events/TProgressable.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TBundle.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TDBHandle.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TFile.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TFileHandle.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TFileInfo.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TFileMap.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TFilePath.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TFileScanner.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TFileUtils.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TPropertyList.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TUTI.cpp"
		"${NN_SOURCE_DIR}/Nano/Files/TUTIRegistry.cpp"
		"${NN_SOURCE_DIR}/Nano/Graphics/TColor.cpp"
		"${NN_SOURCE_DIR}/Nano/Graphics/TGeomUtils.cpp"
		"${NN_SOURCE_DIR}/Nano/Graphics/TImage.cpp"
		"${NN_SOURCE_DIR}/Nano/Graphics/TPoint.cpp"
		"${NN_SOURCE_DIR}/Nano/Graphics/TRectangle.cpp"
		"${NN_SOURCE_DIR}/Nano/Graphics/TShape.cpp"
		"${NN_SOURCE_DIR}/Nano/Graphics/TSize.cpp"
		"${NN_SOURCE_DIR}/Nano/Graphics/TTransform.cpp"
		"${NN_SOURCE_DIR}/Nano/Graphics/TVector.cpp"
		"${NN_SOURCE_DIR}/Nano/Maths/TMathUtils.cpp"
		"${NN_SOURCE_DIR}/Nano/Maths/TRandom.cpp"
		"${NN_SOURCE_DIR}/Nano/Maths/TRandomGenerator.cpp"
		"${NN_SOURCE_DIR}/Nano/Maths/TUInt128.cpp"
		"${NN_SOURCE_DIR}/Nano/Maths/TUInt128.imp"
		"${NN_SOURCE_DIR}/Nano/Std/TStdAlgorithm.cpp"
		"${NN_SOURCE_DIR}/Nano/Std/TStdContainer.cpp"
		"${NN_SOURCE_DIR}/Nano/Std/TStdMath.cpp"
		"${NN_SOURCE_DIR}/Nano/Std/TStdMemory.cpp"
		"${NN_SOURCE_DIR}/Nano/Std/TStdUtility.cpp"
		"${NN_SOURCE_DIR}/Nano/System/TByteSwap.cpp"
		"${NN_SOURCE_DIR}/Nano/System/TMachine.cpp"
		"${NN_SOURCE_DIR}/Nano/System/TPOSIX.cpp"
		"${NN_SOURCE_DIR}/Nano/System/TProcess.cpp"
		"${NN_SOURCE_DIR}/Nano/System/TSystem.cpp"
		"${NN_SOURCE_DIR}/Nano/System/TVersion.cpp"
		"${NN_SOURCE_DIR}/Nano/System/TXMLEncoder.cpp"
		"${NN_SOURCE_DIR}/Nano/System/TXMLNode.cpp"
		"${NN_SOURCE_DIR}/Nano/System/TXMLParser.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/TFormat.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/TString.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/TStringComparator.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/TStringEncoder.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/TStringScanner.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/TStringTransformer.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/TTextUtils.cpp"
		"${NN_SOURCE_DIR}/Nano/Text/TUnicodeView.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TExecute.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TLockable.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TMutex.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TRunLoop.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TRunLoopTask.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TScopedLock.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TSemaphore.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TSpinLock.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TThread.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TThreadGroup.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TThreadID.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TThreadPool.cpp"
		"${NN_SOURCE_DIR}/Nano/Threads/TThreadTask.cpp"
		"${NN_SOURCE_DIR}/Nano/Time/TClock.cpp"
		"${NN_SOURCE_DIR}/Nano/Time/TDate.cpp"
		"${NN_SOURCE_DIR}/Nano/Time/TDateFormatter.cpp"
		"${NN_SOURCE_DIR}/Nano/Time/TTime.cpp"
		"${NN_SOURCE_DIR}/Nano/Time/TTimeUtils.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TAny.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TArray.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TBitVector.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TCache.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TDeque.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TDictionary.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TFunction.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TMap.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TMixinAppendable.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TMixinAttributes.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TMixinComparable.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TMixinContainer.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TMixinHashable.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TNumber.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TRange.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TSet.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TURL.cpp"
		"${NN_SOURCE_DIR}/Nano/Types/TVariant.cpp"
		"${NN_SOURCE_DIR}/NanoTest.cpp"
)

nano_project_include_paths(
	PRIVATE
		"${NN_SOURCE_DIR}"
		"${NN_SOURCE_DIR}/../../Library/Source/Nanites/Catch"
)

nano_project_libraries(
	PRIVATE
		Nano
)





#==============================================================================
#		ANDROID
#------------------------------------------------------------------------------
if (NN_TARGET_ANDROID)
	nano_project_libraries(
		PRIVATE
			"android"
			"log"
	)
endif()





#==============================================================================
#		Linux
#------------------------------------------------------------------------------
if (NN_TARGET_LINUX)
	nano_project_libraries(
		PRIVATE
			"pthread"
	)
endif()





#==============================================================================
#		macOS
#------------------------------------------------------------------------------
if (NN_TARGET_MACOS)
	nano_project_sources(
		PRIVATE
			"${NN_SOURCE_DIR}/Nanites/CoreFoundation/TCFArray.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreFoundation/TCFData.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreFoundation/TCFDate.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreFoundation/TCFDictionary.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreFoundation/TCFNumber.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreFoundation/TCFObject.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreFoundation/TCFString.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreFoundation/TCFURL.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreFoundation/TCoreFoundation.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreGraphics/TCGColor.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreGraphics/TCGContext.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreGraphics/TCGImage.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreGraphics/TCGShading.cpp"
			"${NN_SOURCE_DIR}/Nanites/CoreGraphics/TCoreGraphics.cpp"
	)

	nano_project_libraries(
		PRIVATE
			"-framework CoreFoundation"
			"-framework CoreGraphics"
			"-framework CoreServices"
			"-framework Foundation"
			"-framework ImageIO"
			"-framework Security"
	)
endif()





#==============================================================================
#		Windows
#------------------------------------------------------------------------------
if (NN_TARGET_WINDOWS)
	nano_project_sources(
		PRIVATE
			"${NN_SOURCE_DIR}/../../Library/Support/Visual Studio/Nano.natvis"
			"${NN_SOURCE_DIR}/Nanites/Windows/TWindows.cpp"
	)

	nano_project_libraries(
		PRIVATE
			"Bcrypt"
			"Windowscodecs"
	)
endif()





#==============================================================================
#		Build
#------------------------------------------------------------------------------
nano_build_project()

