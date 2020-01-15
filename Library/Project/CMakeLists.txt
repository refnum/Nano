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
		"${TARGET_SOURCE_DIR}/Core/NDebug.cpp"
		"${TARGET_SOURCE_DIR}/Core/NDebug.h"
		"${TARGET_SOURCE_DIR}/Core/NLog.cpp"
		"${TARGET_SOURCE_DIR}/Core/NLog.h"
		"${TARGET_SOURCE_DIR}/Core/NLogOutput.cpp"
		"${TARGET_SOURCE_DIR}/Core/NLogOutput.h"
		"${TARGET_SOURCE_DIR}/System/NByteSwap.cpp"
		"${TARGET_SOURCE_DIR}/System/NByteSwap.h"
		"${TARGET_SOURCE_DIR}/System/NByteSwap.inl"
		"${TARGET_SOURCE_DIR}/System/NDataDigest.cpp"
		"${TARGET_SOURCE_DIR}/System/NDataDigest.h"
		"${TARGET_SOURCE_DIR}/System/NDataDigest.inl"
		"${TARGET_SOURCE_DIR}/Text/NString.cpp"
		"${TARGET_SOURCE_DIR}/Text/NString.h"
		"${TARGET_SOURCE_DIR}/Text/NString.inl"
		"${TARGET_SOURCE_DIR}/Text/NStringEncoder.cpp"
		"${TARGET_SOURCE_DIR}/Text/NStringEncoder.h"
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
		"${TARGET_SOURCE_DIR}/Time/NTime.cpp"
		"${TARGET_SOURCE_DIR}/Time/NTime.h"
		"${TARGET_SOURCE_DIR}/Time/NTimeUtils.cpp"
		"${TARGET_SOURCE_DIR}/Time/NTimeUtils.h"
		"${TARGET_SOURCE_DIR}/Types/NData.cpp"
		"${TARGET_SOURCE_DIR}/Types/NData.h"
		"${TARGET_SOURCE_DIR}/Types/NMixinAttributes.h"
		"${TARGET_SOURCE_DIR}/Types/NMixinAttributes.inl"
		"${TARGET_SOURCE_DIR}/Types/NMixinComparable.h"
		"${TARGET_SOURCE_DIR}/Types/NMixinComparable.inl"
		"${TARGET_SOURCE_DIR}/Types/NMixinContainer.h"
		"${TARGET_SOURCE_DIR}/Types/NMixinContainer.inl"
		"${TARGET_SOURCE_DIR}/Types/NMixinHashable.h"
		"${TARGET_SOURCE_DIR}/Types/NMixinHashable.inl"
		"${TARGET_SOURCE_DIR}/Types/NRange.cpp"
		"${TARGET_SOURCE_DIR}/Types/NRange.h"
		"${TARGET_SOURCE_DIR}/Types/NRange.inl"
)

target_include_directories("${PROJECT_NAME}"
	PUBLIC
		"${TARGET_SOURCE_DIR}/Core"
		"${TARGET_SOURCE_DIR}/System"
		"${TARGET_SOURCE_DIR}/Text"
		"${TARGET_SOURCE_DIR}/Threads"
		"${TARGET_SOURCE_DIR}/Time"
		"${TARGET_SOURCE_DIR}/Types"
		"${TARGET_SOURCE_DIR}/Internal/Components/xxHash_0.7.2"
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Components/fourmilab.ch"
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
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNSemaphore.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNTimeUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.h"
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
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNSemaphore.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNTimeUtils.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.h"
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
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.h"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.h"
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
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedWindows.cpp"
			"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedWindows.h"
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
target_nano_build_library("${PROJECT_NAME}")



