#==============================================================================
#	NAME:
#		NanoCompilers.cmake
#
#	DESCRIPTION:
#		Nano compiler definitions.
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

		-Wno-atomic-implicit-seq-cst
		-Wno-c++2a-compat
		-Wno-c++98-compat
		-Wno-c++98-compat-pedantic
		-Wno-date-time
		-Wno-documentation-unknown-command
		-Wno-empty-translation-unit
		-Wno-exit-time-destructors
		-Wno-float-equal
		-Wno-global-constructors
		-Wno-gnu-zero-variadic-macro-arguments
		-Wno-nested-anon-types
		-Wno-padded
		-Wno-switch-enum
		-Wno-unreachable-code-break
		-Wno-unused-command-line-argument				# Required for Android toolchain
		-Wno-weak-vtables
	)

	# Restore Xcode suppressions
	#
	# Xcode adds multiple -Wno-xxxx flags to clang's command line, placing
	# them before any externally provided warning flags.
	#
	# As clang evaluates -Weverything before any other warning flags these
	# -Wno-xxxx flags override any subsequent -Weverything flag.
	#
	# As such Xcode projects must manually re-enable these warnings.
	if (NN_GENERATOR_XCODE)
		list(APPEND NN_COMPILER_WARNINGS_MAXIMUM
			-Wblock-capture-autoreleasing
			-Wbool-conversion
			-Wc++11-extensions
			-Wcomma
			-Wconstant-conversion
			-Wconversion
			-Wempty-body
			-Wenum-conversion
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
		)
	endif()

	list(APPEND NN_COMPILER_WARNINGS_MINIMUM
		-Wall
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

		-Walloc-zero
		-Walloca
		-Wcast-align
		-Wcast-qual
		-Wconversion
		-Wdisabled-optimization
		-Wdouble-promotion
		-Wduplicated-branches
		-Wduplicated-cond
		-Wformat-overflow
		-Wformat-signedness
		-Wformat-truncation
		-Wimplicit-fallthrough=5
		-Winit-self
		-Winvalid-pch
		-Wlogical-op
		-Wmisleading-indentation
		-Wmissing-declarations
		-Wmissing-format-attribute
		-Wmissing-include-dirs
		-Wmissing-noreturn
		-Wno-aggressive-loop-optimizations
		-Wnull-dereference
		-Woverlength-strings
		-Wpointer-arith
		-Wredundant-decls
		-Wshadow
		-Wsign-conversion
		-Wstack-protector
		-Wstrict-aliasing
		-Wsuggest-final-methods
		-Wsuggest-final-types
		-Wunreachable-code
		-Wunused
		-Wvector-operation-performance
		-Wwrite-strings

		-Wno-unknown-pragmas
	)

	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM_CPP
		-Wclass-memaccess
		-Wctor-dtor-privacy
		-Wdelete-non-virtual-dtor
		-Wdeprecated-copy
		-Wdeprecated-copy-dtor
		-Wextra-semi
		-Wnon-virtual-dtor
		-Woverloaded-virtual
		-Wpessimizing-move
		-Wredundant-move
		-Wreorder
		-Wstrict-null-sentinel
		-Wvirtual-inheritance
		-Wzero-as-null-pointer-constant
	)

	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM_OBJC
		-Wselector
		-Wstrict-selector-match
		-Wundeclared-selector
	)

	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM_C
		-Wbad-function-cast
		-Wjump-misses-init
		-Wmissing-prototypes
		-Wnested-externs
		-Wold-style-definition
		-Wstrict-prototypes
	)

	list(APPEND NN_COMPILER_WARNINGS_MINIMUM
		-Wall
	)

	list(APPEND NN_COMPILER_WARNINGS_NONE
		-w -fcompare-debug-second
	)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_MSVC)

	list(APPEND NN_COMPILER_WARNINGS_MAXIMUM
		/Wall
		/WX												# Warnings as errors

		-D_CRT_SECURE_NO_WARNINGS
		/w14242											# Possible loss of data in conversion
		/w14254											# Possible loss of data in conversion
		/w14263											# Override does not override any base class virtual function
		/w14265											# Class has virtual functions, but destructor is not virtual
		/w14287											# Unsigned/negative constant mismatch
		/w14296											# Expression is always true/false
		/w14311											# Pointer truncation from 'type1' to 'type2'
		/w14545											# Expression before comma evaluates to a function without arguments
		/w14546											# Function call before comma missing argument list
		/w14547											# Operator before comma has no effect
		/w14549											# Operator before comma has no effect
		/w14555											# Expression has no effect
		/w14619											# Unknown warning pragma
		/w14640											# Thread un-safe static member initialization
		/w14826											# Conversion is sign-extended
		/w14905											# Casting wide string literal to LPSTR
		/w14906											# Casting string literal to LPWSTR
		/w14928											# Illegal copy-initialization

		/wc4289											# Loop control variable used outside for loop
		/wd4061											# Enumerator in switch of enum is not explicitly handled by a case label
		/wd4068											# Unknown pragma
		/wd4206											# Translation unit is empty
		/wd4324											# Structure was padded due to alignment specifier 
		/wd4371											# Class layout may vary previous version of the compiler
		/wd4514											# Unreferenced inline function has been removed 
		/wd4571											# Structured exceptions are no longer caught
		/wd4619											# Unrecognised warning pragma
		/wd4623											# Default constructor was implicitly defined as deleted
		/wd4625											# Copy constructor was implicitly defined as deleted
		/wd4626											# Assignment operator was implicitly defined as deleted
		/wd4710											# Function not inlined
		/wd4711											# Function selected for automatic inlining
		/wd4715											# Not all control paths return a value (enforced by clang/gcc -Wswitch-enum)
		/wd4774											# Format string is not a string literal
		/wd4820											# Padding added after data member 
		/wd5026											# Move constructor was implicitly defined as deleted 
		/wd5027											# Move assignment operator was implicitly defined as deleted
		/wd5039											# Pointer to potentially throwing function passed to C function
		/wd5045											# Spectre mitigation suggested
		/wd5105											# Macro expansion producing 'defined' has undefined behavior
	)

	list(APPEND NN_COMPILER_WARNINGS_MINIMUM
		/W2
	)

	list(APPEND NN_COMPILER_WARNINGS_NONE
		/W0
	)


#------------------------------------------------------------------------------
else()
	nn_log_error("Unknown compiler!")
endif()





#==============================================================================
#		Compiler Options
#------------------------------------------------------------------------------
if (NN_COMPILER_CLANG)

	list(APPEND NN_COMPILER_OPTIONS
		-fconstant-cfstrings
		-fno-common
	)

	list(APPEND NN_COMPILER_OPTIONS_Debug
		-fstack-protector-all
		-O0
		-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2
	)

	list(APPEND NN_COMPILER_OPTIONS_Release
		-fno-stack-protector
		-O3
	)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_GCC)

	list(APPEND NN_COMPILER_OPTIONS
	)

	list(APPEND NN_COMPILER_OPTIONS_Debug
		-Og
	)

	list(APPEND NN_COMPILER_OPTIONS_Release
		-O3
	)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_MSVC)

	list(APPEND NN_COMPILER_OPTIONS
		/DNOMINMAX
		/DUNICODE
		/DVC_EXTRALEAN
		/DWIN32_LEAN_AND_MEAN
	)

	list(APPEND NN_COMPILER_OPTIONS_Debug
		/Od
	)

	list(APPEND NN_COMPILER_OPTIONS_Release
		/O2i
	)


#------------------------------------------------------------------------------
else()
	nn_log_error("Unknown compiler!")
endif()

list(APPEND NN_COMPILER_OPTIONS $<$<CONFIG:Debug>:  ${NN_COMPILER_OPTIONS_Debug}>)
list(APPEND NN_COMPILER_OPTIONS $<$<CONFIG:Release>:${NN_COMPILER_OPTIONS_Release}>)





#==============================================================================
#		Linker Options
#------------------------------------------------------------------------------
if (NN_COMPILER_CLANG)

	list(APPEND NN_LINKER_OPTIONS
	)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_GCC)

	list(APPEND NN_LINKER_OPTIONS
		-rdynamic
	)


#------------------------------------------------------------------------------
elseif (NN_COMPILER_MSVC)

	list(APPEND NN_LINKER_OPTIONS
		dbghelp.lib
	)


#------------------------------------------------------------------------------
else()
	nn_log_error("Unknown compiler!")
endif()


