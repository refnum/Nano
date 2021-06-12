#==============================================================================
#	NAME:
#		nano_target_compile.cmake
#
#	DESCRIPTION:
#		target_compile functions.
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
#		Interanl functions
#------------------------------------------------------------------------------
#		_nano_project_add_compile_flags : Append to a file's COMPILE_FLAGS.
#------------------------------------------------------------------------------
function(_nano_project_add_compile_flags FILE_PATH FLAGS)

	set_property(SOURCE "${FILE_PATH}" APPEND_STRING PROPERTY COMPILE_FLAGS " ${FLAGS}")

endfunction()





#==============================================================================
#		Public Functions
#------------------------------------------------------------------------------
#		nano_project : Set the project type.
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
function(nano_project_prefix PREFIX)

	if (NN_COMPILER_MSVC)
		target_compile_options("${PROJECT_NAME}" PRIVATE       /FI"${PREFIX}")
	else()
		target_compile_options("${PROJECT_NAME}" PRIVATE -include "${PREFIX}")
	endif()

endfunction()





#==============================================================================
#		nano_project_sources : Add the project source.
#------------------------------------------------------------------------------
function(nano_project_sources)
	
	target_sources("${PROJECT_NAME}" ${ARGV})

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





#==============================================================================
#		nano_project_warnings : Set the source warning level.
#------------------------------------------------------------------------------
#		Usage:
#			nano_project_warnings([MAXMIMUM] [NONE items1...])
#------------------------------------------------------------------------------
function(nano_project_warnings)

	cmake_parse_arguments(
		PARSE_ARGV
		0
        ARGUMENT
        "MAXIMUM"
        ""
        "NONE"
    )



	# Set the warning level
	if (ARGUMENT_MAXIMUM)
		set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_COMPILER_WARNINGS ${NN_COMPILER_WARNINGS_MAXIMUM})

	elseif(NOT ARGUMENT_NONE)
		nano_log_error("Unknown warning level '${ARGV}'")
	endif()



	# Suppress warnings
	if (ARGUMENT_NONE)
		get_target_property(PROJECT_SOURCES "${PROJECT_NAME}" SOURCES)

		foreach (FILE_PATH ${PROJECT_SOURCES})
			if ("${FILE_PATH}" IN_LIST ARGUMENT_NONE)
				_nano_project_add_compile_flags("${FILE_PATH}" "${NN_COMPILER_WARNINGS_NONE}")
			endif()
		endforeach()
	endif()

endfunction()





#==============================================================================
#		nano_project_language : Set the source language.
#------------------------------------------------------------------------------
#		Usage:
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



	# Set the language
	foreach (FILE_PATH ${ARGUMENT_C})
		_nano_project_add_compile_flags("${FILE_PATH}" "${NN_COMPILER_LANGUAGE_C}")
	endforeach()

	foreach (FILE_PATH ${ARGUMENT_CPP})
		_nano_project_add_compile_flags("${FILE_PATH}" "${NN_COMPILER_LANGUAGE_CPP}")
	endforeach()

	foreach (FILE_PATH ${ARGUMENT_OBJCPP})
		_nano_project_add_compile_flags("${FILE_PATH}" "${NN_COMPILER_LANGUAGE_OBJCPP}")
	endforeach()

endfunction()





#==============================================================================
#		nano_project_definitions : Set the source definitions.
#------------------------------------------------------------------------------
#		Usage:
#			nano_project_definitions(	  <INTERFACE|PUBLIC|PRIVATE> items1...
#										[[<INTERFACE|PUBLIC|PRIVATE> items2...] ...])
#------------------------------------------------------------------------------
function(nano_project_definitions)

	target_compile_definitions("${PROJECT_NAME}" PRIVATE "${ARGV}")

endfunction()
