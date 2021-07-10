#==============================================================================
#	NAME:
#		nano_project.cmake
#
#	DESCRIPTION:
#		Define the structure of a project.
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
#		Public Functions
#------------------------------------------------------------------------------
#		nano_project : Create the project.
#------------------------------------------------------------------------------
function(nano_project PROJECT_TYPE)

	# Add the project
	if (PROJECT_TYPE STREQUAL "STATIC")
		add_library("${PROJECT_NAME}" STATIC)

	elseif (PROJECT_TYPE STREQUAL "DYNAMIC")
		add_library("${PROJECT_NAME}" DYNAMIC)

	elseif (PROJECT_TYPE STREQUAL "INTERFACE")
		add_library("${PROJECT_NAME}" INTERFACE)

	elseif (PROJECT_TYPE STREQUAL "APP")
		add_executable("${PROJECT_NAME}")

	else()
		nano_log_error("Unknown project type '${PROJECT_TYPE}'")
	endif()



	# Define the project directories
	set(NN_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)

	if (EXISTS "${PROJECT_SOURCE_DIR}/../Source")
		set(NN_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../Source" PARENT_SCOPE)
	else()
		set(NN_SOURCE_DIR "${PROJECT_SOURCE_DIR}" PARENT_SCOPE)
	endif()

endfunction()





#==============================================================================
#		nano_project_prefix : Set the project prefix header.
#------------------------------------------------------------------------------
function(nano_project_prefix PREFIX_PATH)

	target_precompile_headers("${PROJECT_NAME}" PRIVATE "${PREFIX_PATH}")

endfunction()





#==============================================================================
#		nano_project_sources : Add the project source.
#------------------------------------------------------------------------------
function(nano_project_sources)
	
	target_sources("${PROJECT_NAME}" ${ARGV})

endfunction()





#==============================================================================
#		nano_project_warnings : Set the source warning level.
#------------------------------------------------------------------------------
#		The warning level can be set to MAXIMUM, MINIMUM, or NONE followed
#		by zero or more paths.
#
#		If no paths are provided after a level then the level becomes the
#		default level for source files.
#
#		If paths to files are provided after a level then the level is applied
#		to those files.
#
#		If paths to directories are provided after a level then the level is
#		applied to all files under that directory path.
#
#		The default warning level is MAXIMUM.
#
#		Usage:
#
#			nano_project_warnings(   <MAXIMUM|MINIMUM|NONE> [items1...]
#									[<MAXIMUM|MINIMUM|NONE> [items2...]
#									[<MAXIMUM|MINIMUM|NONE> [items...] )
#------------------------------------------------------------------------------
function(nano_project_warnings)

	cmake_parse_arguments(
		PARSE_ARGV
		0
        ARGUMENT
        ""
        ""
        "MAXIMUM;MINIMUM;NONE"
    )

	set_property(TARGET "${PROJECT_NAME}" APPEND PROPERTY NN_SOURCE_WARNINGS_MAXIMUM ${ARGUMENT_MAXIMUM})
	set_property(TARGET "${PROJECT_NAME}" APPEND PROPERTY NN_SOURCE_WARNINGS_MINIMUM ${ARGUMENT_MINIMUM})
	set_property(TARGET "${PROJECT_NAME}" APPEND PROPERTY NN_SOURCE_WARNINGS_NONE    ${ARGUMENT_NONE})


	if ("MAXIMUM" IN_LIST ARGV AND NOT DEFINED ARGUMENT_MAXIMUM)
		set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_DEFAULT_WARNINGS "MAXIMUM")

	elseif ("MINIMUM" IN_LIST ARGV AND NOT DEFINED ARGUMENT_MINIMUM)
		set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_DEFAULT_WARNINGS "MINIMUM")

	elseif ("NONE" IN_LIST ARGV AND NOT DEFINED ARGUMENT_NONE)
		set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_DEFAULT_WARNINGS "NONE")
	endif()

endfunction()





#==============================================================================
#		nano_project_language : Set the source language.
#------------------------------------------------------------------------------
#		The language for source files can be set to C, CPP, or OBJCPP.
#
#		The default language is determined by the file extension.
#
#		Usage:
#
#			nano_project_language(	  <C|CPP|OBJCPP> items1...
#									[[<C|CPP|OBJCPP> items2...] ...])
#------------------------------------------------------------------------------
function(nano_project_language)

	cmake_parse_arguments(
		PARSE_ARGV
		0
        ARGUMENT
        ""
        ""
        "C;CPP;OBJCPP"
    )

	set_property(TARGET "${PROJECT_NAME}" APPEND PROPERTY NN_SOURCE_LANGUAGE_C      ${ARGUMENT_C})
	set_property(TARGET "${PROJECT_NAME}" APPEND PROPERTY NN_SOURCE_LANGUAGE_CPP    ${ARGUMENT_CPP})
	set_property(TARGET "${PROJECT_NAME}" APPEND PROPERTY NN_SOURCE_LANGUAGE_OBJCPP ${ARGUMENT_OBJCPP})

endfunction()





#==============================================================================
#		nano_project_definitions : Set the project compiler definitions.
#------------------------------------------------------------------------------
function(nano_project_definitions)

	target_compile_definitions("${PROJECT_NAME}" PRIVATE ${ARGV})

endfunction()





#==============================================================================
#		nano_project_options : Set the project compiler options.
#------------------------------------------------------------------------------
#		Compiler-specific options can be set for the CLANG, GCC, or MSVC
#		compilers.
#
#		The special 'ALL' compiler applies the options to all compilers.
#
#		If the FILES keyword is provided then the options are applied to
#		those source files, if any.
#
#		If the FILES keyword is not provided the options apply globally.
#
#		Usage:
#
#			nano_project_options(   [ALL   options...]
#									[CLANG options...]
#									[GCC   options...]
#									[MSVC  options...]
#									[FILES items...])
#------------------------------------------------------------------------------
function(nano_project_options)

	cmake_parse_arguments(
		PARSE_ARGV
		0
        ARGUMENT
        ""
        ""
        "ALL;CLANG;GCC;MSVC;FILES"
    )


	if (ARGUMENT_FILES OR (FILES IN_LIST ARGUMENT_KEYWORDS_MISSING_VALUES))
		if (ARGUMENT_FILES)
			if (ARGUMENT_ALL)
				set_property(SOURCE ${ARGUMENT_FILES} APPEND PROPERTY COMPILE_OPTIONS ${ARGUMENT_ALL})

			elseif (ARGUMENT_CLANG AND NN_COMPILER_CLANG)
				set_property(SOURCE ${ARGUMENT_FILES} APPEND PROPERTY COMPILE_OPTIONS ${ARGUMENT_CLANG})

			elseif (ARGUMENT_GCC AND NN_COMPILER_GCC)
				set_property(SOURCE ${ARGUMENT_FILES} APPEND PROPERTY COMPILE_OPTIONS ${ARGUMENT_GCC})

			elseif (ARGUMENT_MSVC AND NN_COMPILER_MSVC)
				set_property(SOURCE ${ARGUMENT_FILES} APPEND PROPERTY COMPILE_OPTIONS ${ARGUMENT_MSVC})
			endif()
		endif()
	else()
		if (ARGUMENT_ALL)
			target_compile_options("${PROJECT_NAME}" PRIVATE ${ARGUMENT_ALL})

		elseif (ARGUMENT_CLANG AND NN_COMPILER_CLANG)
			target_compile_options("${PROJECT_NAME}" PRIVATE ${ARGUMENT_CLANG})

		elseif (ARGUMENT_GCC AND NN_COMPILER_GCC)
			target_compile_options("${PROJECT_NAME}" PRIVATE ${ARGUMENT_GCC})

		elseif (ARGUMENT_MSVC AND NN_COMPILER_MSVC)
			target_compile_options("${PROJECT_NAME}" PRIVATE ${ARGUMENT_MSVC})
		endif()
	endif()

endfunction()





#==============================================================================
#		nano_project_include_paths : Add the project include paths.
#------------------------------------------------------------------------------
function(nano_project_include_paths)

	target_include_directories("${PROJECT_NAME}" ${ARGV})

endfunction()





#==============================================================================
#		nano_project_libraries : Add the project libraries.
#------------------------------------------------------------------------------
function(nano_project_libraries)
	
	target_link_libraries("${PROJECT_NAME}" ${ARGV})

endfunction()






