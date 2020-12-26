#==============================================================================
#		Project
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.12)

project(NanoTest VERSION 4.0)
add_executable("${PROJECT_NAME}")





#==============================================================================
#		Cross-Platform
#------------------------------------------------------------------------------
set(TARGET_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../Source")

target_sources("${PROJECT_NAME}"
	PRIVATE
		"${TARGET_SOURCE_DIR}/../../Library/Source/Nanites/Catch/NTestFixture.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Debugging/TDebug.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Debugging/TDebugger.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Debugging/TLog.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Debugging/TLogOutput.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Debugging/TUsageBucket.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Debugging/TUsageLog.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Files/TFile.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Files/TFileHandle.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Files/TFileInfo.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Files/TFilePath.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Files/TFileScanner.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Files/TFileUtils.cpp"
		"${TARGET_SOURCE_DIR}/Nano/System/TByteSwap.cpp"
		"${TARGET_SOURCE_DIR}/Nano/System/TDataDigest.cpp"
		"${TARGET_SOURCE_DIR}/Nano/System/TPOSIX.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Text/TFormat.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Text/TString.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Text/TStringComparator.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Text/TStringEncoder.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Text/TStringScanner.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Text/TStringTransformer.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Text/TTextUtils.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Text/TUnicodeView.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Threads/TMutex.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Threads/TSemaphore.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Threads/TSpinLock.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Threads/TThread.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Threads/TThreadID.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Time/TClock.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Time/TTime.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Time/TTimeUtils.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TData.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TMixinAppendable.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TMixinAttributes.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TMixinComparable.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TMixinContainer.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TMixinHashable.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TRange.cpp"
		"${TARGET_SOURCE_DIR}/NanoTest.cpp"
)

target_include_directories("${PROJECT_NAME}"
	PRIVATE
		"${TARGET_SOURCE_DIR}"
		"${TARGET_SOURCE_DIR}/../../Library/Source/Nanites/Catch"
)





#==============================================================================
#		Windows
#------------------------------------------------------------------------------
if (NN_TARGET_WINDOWS)
	target_sources("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/../../Library/Support/Visual Studio/Nano.natvis"
	)
endif()





#==============================================================================
#		Build
#------------------------------------------------------------------------------
nano_target_build_app("${PROJECT_NAME}")

