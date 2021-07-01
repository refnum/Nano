#==============================================================================
#	NAME:
#		NanoTargets.cmake
#
#	DESCRIPTION:
#		Nano target detection.
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
	nano_log_error("Unable to identify OS from '${CMAKE_SYSTEM_NAME}'")
endif()





#==============================================================================
#		Compiler
#------------------------------------------------------------------------------
set(NN_COMPILER_CLANG										0)
set(NN_COMPILER_GCC											0)
set(NN_COMPILER_MSVC										0)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	set(NN_COMPILER_CLANG									1)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(NN_COMPILER_GCC										1)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(NN_COMPILER_MSVC									1)

else()
	nano_log_error("Unable to identify compiler from '${CMAKE_CXX_COMPILER_ID}'")
endif()





#==============================================================================
#		Generator
#------------------------------------------------------------------------------
set(NN_GENERATOR_MAKE										0)
set(NN_GENERATOR_MSVC										0)
set(NN_GENERATOR_NINJA										0)
set(NN_GENERATOR_XCODE										0)

if (CMAKE_GENERATOR STREQUAL "Unix Makefiles")
	set(NN_GENERATOR_MAKE									1)

elseif (CMAKE_GENERATOR MATCHES "^Visual Studio [0-9]+")
	set(NN_GENERATOR_MSVC									1)

elseif (CMAKE_GENERATOR STREQUAL "Ninja")
	set(NN_GENERATOR_NINJA									1)

elseif (CMAKE_GENERATOR STREQUAL "Xcode")
	set(NN_GENERATOR_XCODE									1)

else()
	nano_log_error("Unable to identify generator from '${CMAKE_GENERATOR}'")
endif()







