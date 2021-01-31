#==============================================================================
#		Project
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.12)

project(Nano VERSION 4.0)
add_library("${PROJECT_NAME}" STATIC)





#==============================================================================
#		Cross-Platform
#------------------------------------------------------------------------------
set(TARGET_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../Source/Nano")

target_sources("${PROJECT_NAME}"
	PRIVATE
		"${TARGET_SOURCE_DIR}/Core/NanoConstants.h"
		"${TARGET_SOURCE_DIR}/Core/NanoMacros.h"
		"${TARGET_SOURCE_DIR}/Core/NanoTargets.h"
		"${TARGET_SOURCE_DIR}/Core/NanoTypes.h"
		"${TARGET_SOURCE_DIR}/Debugging/NDebug.cpp"
		"${TARGET_SOURCE_DIR}/Debugging/NDebug.h"
		"${TARGET_SOURCE_DIR}/Debugging/NDebug.inl"
		"${TARGET_SOURCE_DIR}/Debugging/NDebugger.cpp"
		"${TARGET_SOURCE_DIR}/Debugging/NDebugger.h"
		"${TARGET_SOURCE_DIR}/Debugging/NLog.cpp"
		"${TARGET_SOURCE_DIR}/Debugging/NLog.h"
		"${TARGET_SOURCE_DIR}/Debugging/NLogOutput.cpp"
		"${TARGET_SOURCE_DIR}/Debugging/NLogOutput.h"
		"${TARGET_SOURCE_DIR}/Debugging/NUsageBucket.cpp"
		"${TARGET_SOURCE_DIR}/Debugging/NUsageBucket.h"
		"${TARGET_SOURCE_DIR}/Debugging/NUsageLog.cpp"
		"${TARGET_SOURCE_DIR}/Debugging/NUsageLog.h"
		"${TARGET_SOURCE_DIR}/Files/NFile.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFile.h"
		"${TARGET_SOURCE_DIR}/Files/NFileHandle.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFileHandle.h"
		"${TARGET_SOURCE_DIR}/Files/NFileInfo.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFileInfo.h"
		"${TARGET_SOURCE_DIR}/Files/NFilePath.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFilePath.h"
		"${TARGET_SOURCE_DIR}/Files/NFileScanner.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFileScanner.h"
		"${TARGET_SOURCE_DIR}/Files/NFileUtils.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFileUtils.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/ascii.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/asciitab.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/expat_external.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/expat.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/iasciitab.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/internal.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/latin1tab.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/nametab.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/Nano_expat_config.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/siphash.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/utf8tab.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/winconfig.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/xmlparse.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/xmlrole.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/xmlrole.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/xmltok_impl.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/xmltok.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10/xmltok.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt-7.1.3/fmt_core.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt-7.1.3/fmt_format-inl.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt-7.1.3/fmt_format.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt-7.1.3/fmt_ostream.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt-7.1.3/fmt_printf.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt-7.1.3/Nano_fmt_format.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/json-parser-1.1.0/json.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/json-parser-1.1.0/json.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/config.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_auto_possess.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_chartables.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_compile.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_config.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_context.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_convert.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_dfa_match.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_error.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_extuni.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_find_bracket.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_fuzzsupport.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_internal.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_intmodedep.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_jit_compile.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_jit_neon_inc.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_jit_simd_inc.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_maketables.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_match_data.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_match.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_newline.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_ord2utf.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_pattern_info.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_script_run.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_serialize.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_string_utils.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_study.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_substitute.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_substring.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_tables.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_ucd.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_ucp.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_valid_utf.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2_xclass.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36/pcre2.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/sqlite-3.34.0/Nano_sqlite3.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/sqlite-3.34.0/sqlite3.34.0.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/sqlite-3.34.0/sqlite3.34.0.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Aes.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Aes.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Md5.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Md5.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Sha1.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Sha1.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Sha256.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/WjCryptLib-2.3.0/WjCryptLib_Sha256.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/xxHash-0.8.0/Nano_xxhash.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/xxHash-0.8.0/xxhash.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/adler32.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/compress.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/crc32.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/crc32.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/deflate.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/deflate.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/gzclose.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/gzguts.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/gzlib.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/gzread.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/gzwrite.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/infback.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/inffast.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/inffast.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/inffixed.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/inflate.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/inflate.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/inftrees.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/inftrees.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/Nano_zlib.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/trees.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/trees.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/uncompr.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/zconf.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/zlib.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/zutil.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11/zutil.h"
		"${TARGET_SOURCE_DIR}/Maths/NMathUtils.cpp"
		"${TARGET_SOURCE_DIR}/Maths/NMathUtils.h"
		"${TARGET_SOURCE_DIR}/Maths/NMathUtils.inl"
		"${TARGET_SOURCE_DIR}/Maths/NRandom.cpp"
		"${TARGET_SOURCE_DIR}/Maths/NRandom.h"
		"${TARGET_SOURCE_DIR}/Maths/NRandom.inl"
		"${TARGET_SOURCE_DIR}/Maths/NRandomGenerator.h"
		"${TARGET_SOURCE_DIR}/Maths/NRandomGenerator.inl"
		"${TARGET_SOURCE_DIR}/Maths/NUInt128.h"
		"${TARGET_SOURCE_DIR}/Maths/NUInt128.inl"
		"${TARGET_SOURCE_DIR}/Std/NStdAlgorithm.h"
		"${TARGET_SOURCE_DIR}/Std/NStdContainer.h"
		"${TARGET_SOURCE_DIR}/Std/NStdUtility.h"
		"${TARGET_SOURCE_DIR}/System/NByteSwap.cpp"
		"${TARGET_SOURCE_DIR}/System/NByteSwap.h"
		"${TARGET_SOURCE_DIR}/System/NByteSwap.inl"
		"${TARGET_SOURCE_DIR}/System/NDataDigest.cpp"
		"${TARGET_SOURCE_DIR}/System/NDataDigest.h"
		"${TARGET_SOURCE_DIR}/System/NDataDigest.inl"
		"${TARGET_SOURCE_DIR}/System/NMachine.cpp"
		"${TARGET_SOURCE_DIR}/System/NMachine.h"
		"${TARGET_SOURCE_DIR}/System/NPOSIX.cpp"
		"${TARGET_SOURCE_DIR}/System/NPOSIX.h"
		"${TARGET_SOURCE_DIR}/System/NProcess.cpp"
		"${TARGET_SOURCE_DIR}/System/NProcess.h"
		"${TARGET_SOURCE_DIR}/System/NSystem.cpp"
		"${TARGET_SOURCE_DIR}/System/NSystem.h"
		"${TARGET_SOURCE_DIR}/Text/NFormat.cpp"
		"${TARGET_SOURCE_DIR}/Text/NFormat.h"
		"${TARGET_SOURCE_DIR}/Text/NFormat.inl"
		"${TARGET_SOURCE_DIR}/Text/NString.cpp"
		"${TARGET_SOURCE_DIR}/Text/NString.h"
		"${TARGET_SOURCE_DIR}/Text/NString.inl"
		"${TARGET_SOURCE_DIR}/Text/NStringComparator.cpp"
		"${TARGET_SOURCE_DIR}/Text/NStringComparator.h"
		"${TARGET_SOURCE_DIR}/Text/NStringEncoder.cpp"
		"${TARGET_SOURCE_DIR}/Text/NStringEncoder.h"
		"${TARGET_SOURCE_DIR}/Text/NStringEncoder.inl"
		"${TARGET_SOURCE_DIR}/Text/NStringEncodings.h"
		"${TARGET_SOURCE_DIR}/Text/NStringScanner.cpp"
		"${TARGET_SOURCE_DIR}/Text/NStringScanner.h"
		"${TARGET_SOURCE_DIR}/Text/NStringTransformer.cpp"
		"${TARGET_SOURCE_DIR}/Text/NStringTransformer.h"
		"${TARGET_SOURCE_DIR}/Text/NTextUtils.cpp"
		"${TARGET_SOURCE_DIR}/Text/NTextUtils.h"
		"${TARGET_SOURCE_DIR}/Text/NTextUtils.inl"
		"${TARGET_SOURCE_DIR}/Text/NUnicodeView.cpp"
		"${TARGET_SOURCE_DIR}/Text/NUnicodeView.h"
		"${TARGET_SOURCE_DIR}/Threads/NMutex.cpp"
		"${TARGET_SOURCE_DIR}/Threads/NMutex.h"
		"${TARGET_SOURCE_DIR}/Threads/NMutex.inl"
		"${TARGET_SOURCE_DIR}/Threads/NRunLoop.cpp"
		"${TARGET_SOURCE_DIR}/Threads/NRunLoop.h"
		"${TARGET_SOURCE_DIR}/Threads/NScopedLock.h"
		"${TARGET_SOURCE_DIR}/Threads/NScopedLock.inl"
		"${TARGET_SOURCE_DIR}/Threads/NSemaphore.cpp"
		"${TARGET_SOURCE_DIR}/Threads/NSemaphore.h"
		"${TARGET_SOURCE_DIR}/Threads/NSpinLock.h"
		"${TARGET_SOURCE_DIR}/Threads/NSpinLock.inl"
		"${TARGET_SOURCE_DIR}/Threads/NThread.cpp"
		"${TARGET_SOURCE_DIR}/Threads/NThread.h"
		"${TARGET_SOURCE_DIR}/Threads/NThread.inl"
		"${TARGET_SOURCE_DIR}/Threads/NThreadID.h"
		"${TARGET_SOURCE_DIR}/Threads/NThreadID.inl"
		"${TARGET_SOURCE_DIR}/Time/NClock.cpp"
		"${TARGET_SOURCE_DIR}/Time/NClock.h"
		"${TARGET_SOURCE_DIR}/Time/NTime.h"
		"${TARGET_SOURCE_DIR}/Time/NTime.inl"
		"${TARGET_SOURCE_DIR}/Time/NTimeUtils.cpp"
		"${TARGET_SOURCE_DIR}/Time/NTimeUtils.h"
		"${TARGET_SOURCE_DIR}/Types/NAny.cpp"
		"${TARGET_SOURCE_DIR}/Types/NAny.h"
		"${TARGET_SOURCE_DIR}/Types/NAny.inl"
		"${TARGET_SOURCE_DIR}/Types/NArray.cpp"
		"${TARGET_SOURCE_DIR}/Types/NArray.h"
		"${TARGET_SOURCE_DIR}/Types/NArray.inl"
		"${TARGET_SOURCE_DIR}/Types/NData.cpp"
		"${TARGET_SOURCE_DIR}/Types/NData.h"
		"${TARGET_SOURCE_DIR}/Types/NData.inl"
		"${TARGET_SOURCE_DIR}/Types/NDictionary.cpp"
		"${TARGET_SOURCE_DIR}/Types/NDictionary.h"
		"${TARGET_SOURCE_DIR}/Types/NDictionary.inl"
		"${TARGET_SOURCE_DIR}/Types/NMixinAppendable.h"
		"${TARGET_SOURCE_DIR}/Types/NMixinAppendable.inl"
		"${TARGET_SOURCE_DIR}/Types/NMixinAttributes.h"
		"${TARGET_SOURCE_DIR}/Types/NMixinAttributes.inl"
		"${TARGET_SOURCE_DIR}/Types/NMixinComparable.h"
		"${TARGET_SOURCE_DIR}/Types/NMixinComparable.inl"
		"${TARGET_SOURCE_DIR}/Types/NMixinContainer.h"
		"${TARGET_SOURCE_DIR}/Types/NMixinContainer.inl"
		"${TARGET_SOURCE_DIR}/Types/NMixinHashable.h"
		"${TARGET_SOURCE_DIR}/Types/NMixinHashable.inl"
		"${TARGET_SOURCE_DIR}/Types/NNumber.cpp"
		"${TARGET_SOURCE_DIR}/Types/NNumber.h"
		"${TARGET_SOURCE_DIR}/Types/NNumber.inl"
		"${TARGET_SOURCE_DIR}/Types/NRange.h"
		"${TARGET_SOURCE_DIR}/Types/NRange.inl"
		"${TARGET_SOURCE_DIR}/Types/NVariant.h"
		"${TARGET_SOURCE_DIR}/Types/NVariant.inl"
)

target_include_directories("${PROJECT_NAME}"
	PUBLIC
		"${TARGET_SOURCE_DIR}/Core"
		"${TARGET_SOURCE_DIR}/Debugging"
		"${TARGET_SOURCE_DIR}/Files"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt-7.1.3"
		"${TARGET_SOURCE_DIR}/Internal/Components/xxHash-0.8.0"
		"${TARGET_SOURCE_DIR}/Maths"
		"${TARGET_SOURCE_DIR}/Std"
		"${TARGET_SOURCE_DIR}/System"
		"${TARGET_SOURCE_DIR}/Text"
		"${TARGET_SOURCE_DIR}/Threads"
		"${TARGET_SOURCE_DIR}/Time"
		"${TARGET_SOURCE_DIR}/Types"
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Components/expat-2.2.10"
		"${TARGET_SOURCE_DIR}/Internal/Components/json-parser-1.1.0"
		"${TARGET_SOURCE_DIR}/Internal/Components/libb64-1.2"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2-10.36"
		"${TARGET_SOURCE_DIR}/Internal/Components/sqlite-3.34.0"
		"${TARGET_SOURCE_DIR}/Internal/Components/WjCryptLib-2.3.0"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib-1.2.11"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Shared"
)




 
#==============================================================================
#		Android
#------------------------------------------------------------------------------
if (NN_TARGET_ANDROID)
	target_sources("${PROJECT_NAME}"
		PRIVATE
			"${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNDebugger.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNFileUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNMachine.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNProcess.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNRandom.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNRunLoop.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNSemaphore.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNSystem.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNThread.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNTimeUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedLinux.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedLinux.h"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.h"
	)

	target_include_directories("${PROJECT_NAME}"
		PRIVATE
			"${ANDROID_NDK}/sources/android"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android"
	)

	nano_target_compile_disable_warnings("${PROJECT_NAME}" "${ANDROID_NDK}/sources/android")
endif()





#==============================================================================
#		iOS
#------------------------------------------------------------------------------
if (NN_TARGET_IOS)
	target_sources("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNDebugger.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNFileUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNMachine.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNProcess.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNRandom.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNRunLoop.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNSemaphore.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNSystem.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNThread.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNTimeUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.h"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.mm"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.h"
	)

	target_include_directories("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS"
	)
endif()





#==============================================================================
#		Linux
#------------------------------------------------------------------------------
if (NN_TARGET_LINUX)
	target_sources("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNDebugger.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNFileUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNMachine.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNProcess.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNRandom.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNRunLoop.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNSemaphore.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNSystem.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNThread.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNTimeUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedLinux.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedLinux.h"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.h"
	)

	target_include_directories("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux"
	)
endif()





#==============================================================================
#		macOS
#------------------------------------------------------------------------------
if (NN_TARGET_MACOS)
	target_sources("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNDebugger.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNFileUtils.mm"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNMachine.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNProcess.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNRandom.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNRunLoop.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNSemaphore.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNSystem.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNThread.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNTimeUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.h"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.mm"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.h"
	)

	target_include_directories("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS"
	)
endif()





#==============================================================================
#		tvOS
#------------------------------------------------------------------------------
if (NN_TARGET_TVOS)
	target_sources("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.h"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.mm"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.h"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNDebugger.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNFileUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNMachine.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNProcess.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNRandom.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNRunLoop.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNSemaphore.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNSystem.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNThread.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNTimeUtils.cpp"
		)

	target_include_directories("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS"
		)
endif()





#==============================================================================
#		Windows
#------------------------------------------------------------------------------
if (NN_TARGET_WINDOWS)
	target_sources("${PROJECT_NAME}"
		PRIVATE
			"${PROJECT_SOURCE_DIR}/../Support/Visual Studio/Nano.natvis"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedWindows.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedWindows.h"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNDebugger.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNFileUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNMachine.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNProcess.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNRandom.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNRunLoop.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNSemaphore.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNSystem.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNThread.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNTimeUtils.cpp"
	)

	target_include_directories("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows"
	)
endif()





#==============================================================================
#		Build
#------------------------------------------------------------------------------
nano_target_compile_set_prefix(			"${PROJECT_NAME}" "${TARGET_SOURCE_DIR}/Nano Prefix.h")
nano_target_compile_disable_warnings(	"${PROJECT_NAME}" "Internal/Components")

nano_target_build_library("${PROJECT_NAME}")




