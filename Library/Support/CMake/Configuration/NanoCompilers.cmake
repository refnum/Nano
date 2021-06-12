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
#		Compiler Warnings
#------------------------------------------------------------------------------
if (NN_COMPILER_CLANG)

	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM
		-Werror
		-Weverything

		-Wblock-capture-autoreleasing
		-Wbool-conversion
		-Wcomma
		-Wconstant-conversion
		-Wconversion
		-Wempty-body
		-Wenum-conversion
		-Wexit-time-destructors
		-Wfloat-conversion
		-Wfour-char-constants
		-Winfinite-recursion
		-Wint-conversion
		-Wmissing-braces
		-Wmissing-field-initializers
		-Wmissing-prototypes
		-Wmove
		-Wnewline-eof
		-Wnon-literal-null-conversion
		-Wnon-virtual-dtor
		-Wobjc-literal-conversion
		-Woverloaded-virtual
		-Wrange-loop-analysis
		-Wreturn-type
		-Wsemicolon-before-method-body
		-Wshadow
		-Wshorten-64-to-32
		-Wsign-conversion
		-Wstrict-prototypes
		-Wtrigraphs
		-Wuninitialized
		-Wunknown-pragmas
		-Wunused-function
		-Wunused-label
		-Wunused-parameter
		-Wunused-variable

		-Wno-atomic-implicit-seq-cst
		-Wno-c++2a-compat
		-Wno-c++17-extensions
		-Wno-c++98-compat
		-Wno-c++98-compat-pedantic
		-Wno-c99-extensions
		-Wno-documentation-unknown-command
		-Wno-exit-time-destructors
		-Wno-float-equal
		-Wno-format-non-iso
		-Wno-format-nonliteral
		-Wno-global-constructors
		-Wno-gnu-zero-variadic-macro-arguments
		-Wno-nested-anon-types
		-Wno-packed
		-Wno-padded
		-Wno-return-std-move-in-c++11
		-Wno-switch-enum
		-Wno-unreachable-code-break
		-Wno-weak-vtables
	)

	list(APPEND NN_COMPILER_WARNINGS_NONE
		-w
	)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_GCC)

	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM
		-Wall
		-Werror
		-Wextra

		-Wno-unknown-pragmas
	)

	list(APPEND NN_COMPILER_WARNINGS_NONE
		-w
	)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_MSVC)

	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM
		/Wall
		/WX								# Warnings as errors

		/wd4061							# Enumerator in switch of enum is not explicitly handled by a case label
		/wd4068							# Unknown pragma
		/wd4371							# Class layout may vary previous version of the compiler
		/wd4514							# Unreferenced inline function has been removed 
		/wd4571							# Structured exceptions are no longer caught
		/wd4619							# Unrecognised warning pragma
		/wd4623							# Default constructor was implicitly defined as deleted
		/wd4625							# Copy constructor was implicitly defined as deleted
		/wd4626							# Assignment operator was implicitly defined as deleted
		/wd4710							# Function not inlined
		/wd4711							# Function selected for automatic inlining
		/wd4715							# Not all control paths return a value (enforced by clang/gcc -Wswitch-enum)
		/wd4774							# Format string is not a string literal
		/wd4820							# Padding added after data member 
		/wd5026							# Move constructor was implicitly defined as deleted 
		/wd5027							# Move assignment operator was implicitly defined as deleted
		/wd5039							# Pointer to potentially throwing function passed to C function
		/wd5045							# Spectre mitigation suggested
		/wd5105							# Macro expansion producing 'defined' has undefined behavior
		-D_CRT_SECURE_NO_WARNINGS
	)

	list(APPEND NN_COMPILER_WARNINGS_NONE
		/W0
	)


#------------------------------------------------------------------------------
else()
	nn_log_error("Unknown compiler!")
endif()





#==============================================================================
#		Compiler Language
#------------------------------------------------------------------------------
if (NN_COMPILER_CLANG)

	set(NN_COMPILER_LANGUAGE_C			"-x c             -std=c11")
	set(NN_COMPILER_LANGUAGE_CPP		"-x c++           -std=c++17")
	set(NN_COMPILER_LANGUAGE_OBJCPP		"-x objective-c++ -std=c++17")


#------------------------------------------------------------------------------
elseif (NN_COMPILER_GCC)

	set(NN_COMPILER_LANGUAGE_C			"-x c")
	set(NN_COMPILER_LANGUAGE_CPP		"-x c++")
	set(NN_COMPILER_LANGUAGE_OBJCPP		"-x objective-c++")


#------------------------------------------------------------------------------
elseif (NN_COMPILER_MSVC)

	set(NN_COMPILER_LANGUAGE_C			"/Tc")
	set(NN_COMPILER_LANGUAGE_CPP		"/Tp")
	set(NN_COMPILER_LANGUAGE_OBJCPP		"NN_ERROR_OBJCPP_NOT_SUPPORTED")


#------------------------------------------------------------------------------
else()
	nn_log_error("Unknown compiler!")
endif()





#==============================================================================
#		Compiler Flags
#------------------------------------------------------------------------------
if (NN_COMPILER_CLANG)

	list(APPEND NN_COMPILER_FLAGS
		-fconstant-cfstrings
		-fno-common
	)

	list(APPEND NN_COMPILER_FLAGS_Debug
		-fstack-protector-all
		-O0
		-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2
	)

	list(APPEND NN_COMPILER_FLAGS_Release
		-fno-stack-protector
		-O3
	)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_GCC)

	list(APPEND NN_COMPILER_FLAGS
	)

	list(APPEND NN_COMPILER_FLAGS_Debug
		-Og
	)

	list(APPEND NN_COMPILER_FLAGS_Release
		-O3
	)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_MSVC)

	list(APPEND NN_COMPILER_FLAGS
		/DNOMINMAX
		/DUNICODE
		/DVC_EXTRALEAN
		/DWIN32_LEAN_AND_MEAN
	)

	list(APPEND NN_COMPILER_FLAGS_Debug
		/Od
	)

	list(APPEND NN_COMPILER_FLAGS_Release
		/O2i
	)


#------------------------------------------------------------------------------
else()
	nn_log_error("Unknown compiler!")
endif()

list(APPEND NN_COMPILER_FLAGS $<$<CONFIG:Debug>:${NN_COMPILER_FLAGS_Debug}>)
list(APPEND NN_COMPILER_FLAGS $<$<CONFIG:Release>:${NN_COMPILER_FLAGS_Release}>)





#==============================================================================
#		Linker Flags
#------------------------------------------------------------------------------
if (NN_COMPILER_CLANG)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_GCC)

	list(APPEND NN_LINKER_FLAGS
		-rdynamic
	)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_MSVC)

	list(APPEND NN_LINKER_FLAGS
		dbghelp.lib
	)


#------------------------------------------------------------------------------
else()
	nn_log_error("Unknown compiler!")
endif()


