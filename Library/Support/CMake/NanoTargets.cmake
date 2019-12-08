#==============================================================================
#		Target
#------------------------------------------------------------------------------
set(NN_TARGET_ANDROID										0)
set(NN_TARGET_IOS											0)
set(NN_TARGET_LINUX											0)
set(NN_TARGET_MACOS											0)
set(NN_TARGET_TVOS											0)
set(NN_TARGET_WINDOWS										0)

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
	set(NN_TARGET_ANDROID									1)

elseif (CMAKE_SYSTEM_NAME STREQUAL "iOS")
	set(NN_TARGET_IOS										1)

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(NN_TARGET_LINUX										1)

elseif (CMAKE_SYSTEM_NAME STREQUAL "macOS" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(NN_TARGET_MACOS										1)

elseif (CMAKE_SYSTEM_NAME STREQUAL "tvOS")
	set(NN_TARGET_TVOS										1)

elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(NN_TARGET_WINDOWS									1)

else()
	message(FATAL_ERROR "[Nano] Unable to identify OS from '${CMAKE_SYSTEM_NAME}'")
endif()





#==============================================================================
#		Architecture
#------------------------------------------------------------------------------
set(NN_ARCH_ARM												0)
set(NN_ARCH_INTEL											0)

if (NN_TARGET_ANDROID)
	set(NN_ARCH_ARM											1)

elseif (NN_TARGET_IOS)
	set(NN_ARCH_ARM											1)

elseif (NN_TARGET_LINUX)
	set(NN_ARCH_INTEL										1)

elseif (NN_TARGET_MACOS)
	set(NN_ARCH_INTEL										1)

elseif (NN_TARGET_TVOS)
	set(NN_ARCH_ARM											1)

elseif (NN_TARGET_WINDOWS)
	set(NN_ARCH_INTEL										1)

else()
	message(FATAL_ERROR "[Nano] Unable to identify target architecture")
endif()





#==============================================================================
#		Compiler
#------------------------------------------------------------------------------
set(NN_COMPILER_CLANG										0)
set(NN_COMPILER_GCC											0)
set(NN_COMPILER_MSVC										0)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	set(NN_COMPILER_CLANG									1)
	set(NN_COMPILER											"Clang")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(NN_COMPILER_GCC										1)
	set(NN_COMPILER											"GCC")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(NN_COMPILER_MSVC									1)
	set(NN_COMPILER											"MSVC")

else()
	message(FATAL_ERROR "[Nano] Unable to identify compiler from '${CMAKE_CXX_COMPILER_ID}'")
endif()



