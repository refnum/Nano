#==============================================================================
#	NAME:
#		NanoCompilers.cmake
#
#	DESCRIPTION:
#		Nano compilers.
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
#		Architectures
#------------------------------------------------------------------------------
if (NN_TARGET_MACOS)
	set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)

elseif (NN_TARGET_IOS OR NN_TARGET_TVOS)
	set(CMAKE_OSX_ARCHITECTURES arm64)

endif()





#==============================================================================
#		Compiler Warnings
#------------------------------------------------------------------------------
set(NN_COMPILER_WARNINGS_MAXIMUM "")
set(NN_COMPILER_WARNINGS_NONE    "")
set(NN_COMPILER_WARNINGS         "")

#------------------------------------------------------------------------------
if (NN_COMPILER_CLANG)

	# Maximum
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Weverything)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Werror)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wblock-capture-autoreleasing)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wbool-conversion)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wcomma)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wconstant-conversion)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wconversion)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wempty-body)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wenum-conversion)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wexit-time-destructors)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wfloat-conversion)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wfour-char-constants)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Winfinite-recursion)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wint-conversion)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wmissing-braces)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wmissing-field-initializers)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wmissing-prototypes)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wmove)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wnewline-eof)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wnon-literal-null-conversion)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wnon-virtual-dtor)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wobjc-literal-conversion)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Woverloaded-virtual)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wrange-loop-analysis)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wreturn-type)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wsemicolon-before-method-body)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wshadow)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wshorten-64-to-32)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wsign-conversion)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wstrict-prototypes)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wtrigraphs)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wuninitialized)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wunknown-pragmas)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wunused-function)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wunused-label)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wunused-parameter)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wunused-variable)

	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-atomic-implicit-seq-cst)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-c++2a-compat)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-c++17-extensions)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-c++98-compat-pedantic)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-c++98-compat)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-c99-extensions)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-documentation-unknown-command)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-exit-time-destructors)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-float-equal)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-format-non-iso)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-format-nonliteral)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-global-constructors)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-gnu-zero-variadic-macro-arguments)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-nested-anon-types)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-packed)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-padded)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-return-std-move-in-c++11)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-switch-enum)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-unreachable-code-break)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-weak-vtables)


	# None
	list(APPEND NN_COMPILER_WARNINGS_NONE -w)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_GCC)

	# Maximum
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wall)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Werror)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wextra)

	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -Wno-unknown-pragmas)


	# None
	list(APPEND NN_COMPILER_WARNINGS_NONE -w)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_MSVC)

	# Maximum
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /Wall)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /WX)			# Warnings as errors

	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4061)		# Enumerator in switch of enum is not explicitly handled by a case label
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4068)		# Unknown pragma
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4371)		# Class layout may vary previous version of the compiler
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4514)		# Unreferenced inline function has been removed 
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4571)		# Structured exceptions are no longer caught
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4619)		# Unrecognised warning pragma
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4623)		# Default constructor was implicitly defined as deleted
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4625)		# Copy constructor was implicitly defined as deleted
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4626)		# Assignment operator was implicitly defined as deleted
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4710)		# Function not inlined
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4711)		# Function selected for automatic inlining
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4715)		# Not all control paths return a value (enforced by clang/gcc -Wswitch-enum)
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4774)		# Format string is not a string literal
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd4820)		# Padding added after data member 
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd5026)		# Move constructor was implicitly defined as deleted 
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd5027)		# Move assignment operator was implicitly defined as deleted
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd5039)		# Pointer to potentially throwing function passed to C function
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd5045)		# Spectre mitigation suggested
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM /wd5105)		# Macro expansion producing 'defined' has undefined behavior
	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM -D_CRT_SECURE_NO_WARNINGS)


	# None
	list(APPEND NN_COMPILER_WARNINGS_NONE /W0)

endif()





#==============================================================================
#		Compiler Flags
#------------------------------------------------------------------------------
set(NN_COMPILER_FLAGS "")

#------------------------------------------------------------------------------
if (NN_COMPILER_CLANG)

	list(APPEND NN_COMPILER_FLAGS -DNN_DEBUG=${NN_DEBUG})
	list(APPEND NN_COMPILER_FLAGS -DNN_RELEASE=${NN_RELEASE})
	list(APPEND NN_COMPILER_FLAGS -fconstant-cfstrings)
	list(APPEND NN_COMPILER_FLAGS -fno-common)

	if (NN_DEBUG)
		list(APPEND NN_COMPILER_FLAGS -fstack-protector-all)
		list(APPEND NN_COMPILER_FLAGS -O0)
		list(APPEND NN_COMPILER_FLAGS -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2)
	else()
		list(APPEND NN_COMPILER_FLAGS -fno-stack-protector)
		list(APPEND NN_COMPILER_FLAGS -O3)
	endif()


#------------------------------------------------------------------------------
elseif (NN_COMPILER_GCC)

	list(APPEND NN_COMPILER_FLAGS -DNN_DEBUG=${NN_DEBUG})
	list(APPEND NN_COMPILER_FLAGS -DNN_RELEASE=${NN_RELEASE})

	if (NN_DEBUG)
		list(APPEND NN_COMPILER_FLAGS -O0)
	else()
		list(APPEND NN_COMPILER_FLAGS -O3)
	endif()


#------------------------------------------------------------------------------
elseif (NN_COMPILER_MSVC)

	list(APPEND NN_COMPILER_FLAGS /DNN_DEBUG=${NN_DEBUG})
	list(APPEND NN_COMPILER_FLAGS /DNN_RELEASE=${NN_RELEASE})
	list(APPEND NN_COMPILER_FLAGS /DNOMINMAX)
	list(APPEND NN_COMPILER_FLAGS /DUNICODE)
	list(APPEND NN_COMPILER_FLAGS /DVC_EXTRALEAN)
	list(APPEND NN_COMPILER_FLAGS /DWIN32_LEAN_AND_MEAN)

	if (NN_DEBUG)
		list(APPEND NN_COMPILER_FLAGS /Od)
	else()
		list(APPEND NN_COMPILER_FLAGS /O2i)
	endif()

endif()





#==============================================================================
#		Linker Flags
#------------------------------------------------------------------------------
set(NN_LINKER_FLAGS "")

#------------------------------------------------------------------------------
if (NN_COMPILER_CLANG)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_GCC)

	list(APPEND NN_LINKER_FLAGS -rdynamic)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_MSVC)

	list(APPEND NN_LINKER_FLAGS dbghelp.lib)

endif()


