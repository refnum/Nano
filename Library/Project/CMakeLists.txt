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
		"${TARGET_SOURCE_DIR}/Debugging/NLog.cpp"
		"${TARGET_SOURCE_DIR}/Debugging/NLog.h"
		"${TARGET_SOURCE_DIR}/Debugging/NLogOutput.cpp"
		"${TARGET_SOURCE_DIR}/Debugging/NLogOutput.h"
		"${TARGET_SOURCE_DIR}/Files/NFile.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFile.h"
		"${TARGET_SOURCE_DIR}/Files/NFileHandle.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFileHandle.h"
		"${TARGET_SOURCE_DIR}/Files/NFileInfo.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFileInfo.h"
		"${TARGET_SOURCE_DIR}/Files/NFilePath.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFilePath.h"
		"${TARGET_SOURCE_DIR}/Files/NFilePath.inl"
		"${TARGET_SOURCE_DIR}/Files/NFileScanner.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFileScanner.h"
		"${TARGET_SOURCE_DIR}/Files/NFileUtils.cpp"
		"${TARGET_SOURCE_DIR}/Files/NFileUtils.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt_2020_03_04/core.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt_2020_03_04/format-inl.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt_2020_03_04/format.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt_2020_03_04/Nano_fmt.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt_2020_03_04/ostream.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt_2020_03_04/printf.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/config.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/dftables.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_auto_possess.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_chartables.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_compile.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_config.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_context.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_convert.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_dfa_match.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_error.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_extuni.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_find_bracket.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_fuzzsupport.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_internal.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_intmodedep.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_jit_compile.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_jit_neon_inc.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_jit_simd_inc.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_maketables.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_match_data.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_match.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_newline.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_ord2utf.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_pattern_info.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_script_run.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_serialize.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_string_utils.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_study.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_substitute.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_substring.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_tables.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_ucd.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_ucp.h"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_valid_utf.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2_xclass.c"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34/pcre2.h"
		"${TARGET_SOURCE_DIR}/Std/NStdAlgorithm.h"
		"${TARGET_SOURCE_DIR}/Std/NStdContainer.h"
		"${TARGET_SOURCE_DIR}/System/NByteSwap.cpp"
		"${TARGET_SOURCE_DIR}/System/NByteSwap.h"
		"${TARGET_SOURCE_DIR}/System/NByteSwap.inl"
		"${TARGET_SOURCE_DIR}/System/NDataDigest.cpp"
		"${TARGET_SOURCE_DIR}/System/NDataDigest.h"
		"${TARGET_SOURCE_DIR}/System/NDataDigest.inl"
		"${TARGET_SOURCE_DIR}/System/NPOSIX.cpp"
		"${TARGET_SOURCE_DIR}/System/NPOSIX.h"
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
		"${TARGET_SOURCE_DIR}/Text/NStringFormatter.h"
		"${TARGET_SOURCE_DIR}/Text/NStringFormatter.inl"
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
		"${TARGET_SOURCE_DIR}/Threads/NScopedLock.h"
		"${TARGET_SOURCE_DIR}/Threads/NSemaphore.cpp"
		"${TARGET_SOURCE_DIR}/Threads/NSemaphore.h"
		"${TARGET_SOURCE_DIR}/Threads/NSpinLock.h"
		"${TARGET_SOURCE_DIR}/Threads/NSpinLock.inl"
		"${TARGET_SOURCE_DIR}/Threads/NThread.cpp"
		"${TARGET_SOURCE_DIR}/Threads/NThread.h"
		"${TARGET_SOURCE_DIR}/Threads/NThread.inl"
		"${TARGET_SOURCE_DIR}/Time/NClock.cpp"
		"${TARGET_SOURCE_DIR}/Time/NClock.h"
		"${TARGET_SOURCE_DIR}/Time/NTime.inl"
		"${TARGET_SOURCE_DIR}/Time/NTime.h"
		"${TARGET_SOURCE_DIR}/Time/NTimeUtils.cpp"
		"${TARGET_SOURCE_DIR}/Time/NTimeUtils.h"
		"${TARGET_SOURCE_DIR}/Types/NData.cpp"
		"${TARGET_SOURCE_DIR}/Types/NData.h"
		"${TARGET_SOURCE_DIR}/Types/NData.inl"
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
		"${TARGET_SOURCE_DIR}/Types/NRange.h"
		"${TARGET_SOURCE_DIR}/Types/NRange.inl"
)

target_include_directories("${PROJECT_NAME}"
	PUBLIC
		"${TARGET_SOURCE_DIR}/Core"
		"${TARGET_SOURCE_DIR}/Debugging"
		"${TARGET_SOURCE_DIR}/Files"
		"${TARGET_SOURCE_DIR}/Std"
		"${TARGET_SOURCE_DIR}/System"
		"${TARGET_SOURCE_DIR}/Text"
		"${TARGET_SOURCE_DIR}/Threads"
		"${TARGET_SOURCE_DIR}/Time"
		"${TARGET_SOURCE_DIR}/Types"
		"${TARGET_SOURCE_DIR}/Internal/Components/fmt_2020_03_04"
		"${TARGET_SOURCE_DIR}/Internal/Components/xxHash_0.7.2"
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Components/fourmilab.ch"
		"${TARGET_SOURCE_DIR}/Internal/Components/pcre2_10.34"
		"${TARGET_SOURCE_DIR}/Internal/Components/sha2_1.1a"
		"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Shared"
)




 
#==============================================================================
#		Android
#------------------------------------------------------------------------------
if (NN_TARGET_ANDROID)
	target_sources("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNFileUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNSemaphore.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNTimeUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedLinux.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedLinux.h"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.h"
	)

	target_include_directories("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/Android"
	)
endif()





#==============================================================================
#		iOS
#------------------------------------------------------------------------------
if (NN_TARGET_IOS)
	target_sources("${PROJECT_NAME}"
		PRIVATE
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNFileUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNSemaphore.cpp"
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
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNFileUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNSemaphore.cpp"
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
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNFileUtils.mm"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNSemaphore.cpp"
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
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNFileUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNSemaphore.cpp"
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
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNFileHandle.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNFileInfo.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNFileUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNSemaphore.cpp"
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




