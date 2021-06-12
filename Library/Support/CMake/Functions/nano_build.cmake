#==============================================================================
#	NAME:
#		nano_target_build.cmake
#
#	DESCRIPTION:
#		Build functions.
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
#		Internal Functions
#------------------------------------------------------------------------------
#		_nano_build_create_groups : Create the IDE source groups.
#------------------------------------------------------------------------------
function(_nano_build_create_groups)

	# Get the state we need
	get_target_property(PROJECT_SOURCES "${PROJECT_NAME}" SOURCES)

	get_filename_component(SOURCE_ROOT "${NN_SOURCE_DIR}" ABSOLUTE)


	# Group files under the source root
	foreach (FILE_PATH ${PROJECT_SOURCES})
		get_filename_component(FILE_PATH "${FILE_PATH}" ABSOLUTE)

		string(FIND "${FILE_PATH}" "${SOURCE_ROOT}" OFFSET_PREFIX)

		if (OFFSET_PREFIX EQUAL 0)
			source_group(TREE "${NN_SOURCE_DIR}" FILES ${FILE_PATH})
		endif()

	endforeach()

endfunction()





#==============================================================================
#		_nano_build_defaults : Set the default build options.
#------------------------------------------------------------------------------
function(_nano_build_defaults)

	# Features
	target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_17)
	target_compile_features("${PROJECT_NAME}" PUBLIC c_std_11)



	# Definitions
	target_compile_definitions("${PROJECT_NAME}" PRIVATE NN_DEBUG=$<BOOL:$<CONFIG:Debug>>)
	target_compile_definitions("${PROJECT_NAME}" PRIVATE NN_RELEASE=$<BOOL:$<CONFIG:Release>>)



	# Compiler
	get_property(TARGET_WARNINGS TARGET "${PROJECT_NAME}" PROPERTY NN_COMPILER_WARNINGS)
	if (TARGET_WARNINGS STREQUAL "")
		nano_build_warnings(MAXIMUM)
		get_property(TARGET_WARNINGS TARGET "${PROJECT_NAME}" PROPERTY NN_COMPILER_WARNINGS)
	endif()

	target_compile_options("${PROJECT_NAME}" PRIVATE ${TARGET_WARNINGS})
	target_compile_options("${PROJECT_NAME}" PRIVATE ${NN_COMPILER_FLAGS})

	if (${CMAKE_GENERATOR} STREQUAL "Ninja")
		if (NN_COMPILER_CLANG)
			target_compile_options("${PROJECT_NAME}" PRIVATE "-fcolor-diagnostics")
		
		elseif (NN_COMPILER_GCC)
			target_compile_options("${PROJECT_NAME}" PRIVATE "-fdiagnostics-color=always")
		endif()
	endif()



	# Linker
	target_link_options("${PROJECT_NAME}" PRIVATE ${NN_LINKER_FLAGS})

endfunction()





#==============================================================================
#		_nano_build_app : Build an app.
#------------------------------------------------------------------------------
function(_nano_build_app)

	_nano_build_create_groups()
	_nano_build_defaults()

endfunction()





#==============================================================================
#		_nano_build_library : Build a library.
#------------------------------------------------------------------------------
function(_nano_build_library)

	_nano_build_create_groups()
	_nano_build_defaults()

endfunction()





#==============================================================================
#		Public Functions
#------------------------------------------------------------------------------
#		nano_build_project : Build the project.
#------------------------------------------------------------------------------
function(nano_build_project)

	get_target_property(PROJECT_TYPE "${PROJECT_NAME}" TYPE)

	if ("${PROJECT_TYPE}" STREQUAL "EXECUTABLE")
		_nano_build_app()

	elseif ("${PROJECT_TYPE}" STREQUAL "STATIC_LIBRARY" OR
			"${PROJECT_TYPE}" STREQUAL "SHARED_LIBRARY")
		_nano_build_library()

	else()
		nano_log_error("Unknown project type '${PROJECT_TYPE}'")
	endif()

endfunction()
