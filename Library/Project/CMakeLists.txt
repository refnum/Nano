cmake_minimum_required(VERSION 3.12)


project(Nano VERSION 4.0 LANGUAGES CXX C)
add_library(Nano "")

set(TARGET_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../Source/Nano")



target_sources(Nano
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
	"${TARGET_SOURCE_DIR}/Internal/Components/fourmilab.ch/fm_md5.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/fourmilab.ch/fm_md5.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/xxHash_0.7.2/Nano_xxHash.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/xxHash_0.7.2/xxh3.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/xxHash_0.7.2/xxhash.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/xxHash_0.7.2/xxhash.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/adler32.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/compress.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/crc32.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/crc32.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/deflate.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/deflate.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/gzio.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/infback.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/inffast.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/inffast.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/inffixed.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/inflate.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/inflate.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/inftrees.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/inftrees.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/nano_zlib.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/trees.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/trees.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/uncompr.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/zconf.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/zlib_1.2.3.h"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/zutil.c"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3/zutil.h"
	"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.cpp"
	"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedPOSIX.h"
	"${TARGET_SOURCE_DIR}/System/NDataDigest.cpp"
	"${TARGET_SOURCE_DIR}/System/NDataDigest.h"
	"${TARGET_SOURCE_DIR}/System/NDataDigest.inl"
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
	"${TARGET_SOURCE_DIR}/Types/NString.cpp"
	"${TARGET_SOURCE_DIR}/Types/NString.h"
	"${TARGET_SOURCE_DIR}/Types/NString.inl"
)

target_include_directories(Nano
PUBLIC
	"${TARGET_SOURCE_DIR}/Core"
	"${TARGET_SOURCE_DIR}/System"
	"${TARGET_SOURCE_DIR}/Threads"
	"${TARGET_SOURCE_DIR}/Time"
	"${TARGET_SOURCE_DIR}/Types"
PRIVATE
	"${TARGET_SOURCE_DIR}/Internal/Components/fourmilab.ch"
	"${TARGET_SOURCE_DIR}/Internal/Components/sha2_1.1a"
	"${TARGET_SOURCE_DIR}/Internal/Components/xxHash_0.7.2"
	"${TARGET_SOURCE_DIR}/Internal/Components/zlib_1.2.3"
	"${TARGET_SOURCE_DIR}/Internal/Targets/Shared"
)



if (CMAKE_SYSTEM_NAME STREQUAL "Android")
	target_sources(Nano
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNSemaphore.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Android/AndroidNTimeUtils.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedLinux.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedLinux.h"
	)

	target_include_directories(Nano
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Targets/Android"
	)
endif()


if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_sources(Nano
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNSemaphore.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Linux/LinuxNTimeUtils.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedLinux.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedLinux.h"
	)

	target_include_directories(Nano
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Targets/Android"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Linux"
	)
endif()


if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	target_sources(Nano
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNSemaphore.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/iOS/iOSNTimeUtils.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNSemaphore.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/macOS/macOSNTimeUtils.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedDarwin.h"
		"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNSemaphore.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS/tvOSNTimeUtils.cpp"
	)

	target_include_directories(Nano
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Targets/iOS"
		"${TARGET_SOURCE_DIR}/Internal/Targets/macOS"
		"${TARGET_SOURCE_DIR}/Internal/Targets/tvOS"
	)
endif()


if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_sources(Nano
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedWindows.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Shared/NSharedWindows.h"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNSemaphore.cpp"
		"${TARGET_SOURCE_DIR}/Internal/Targets/Windows/WindowsNTimeUtils.cpp"
	)

	target_include_directories(Nano
	PRIVATE
		"${TARGET_SOURCE_DIR}/Internal/Targets/Windows"
	)
endif()





target_compile_features(Nano
PUBLIC
	cxx_std_17
	c_std_11
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	target_compile_options(Nano
	PRIVATE
		-Werror
	)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(Nano
	PRIVATE
		-Werror
	)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Windows")
	target_compile_options(Nano
	PRIVATE
		-Werror
	)

else()
	message(FATAL_ERROR "Unknown compiler '${CMAKE_CXX_COMPILER_ID}'")
endif()






