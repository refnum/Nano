#==============================================================================
#	NAME:
#		nano_target_build.cmake
#
#	DESCRIPTION:
#		Build a project.
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
#		_nano_build_generator_ninja : Prepare a Ninja build.
#------------------------------------------------------------------------------
function(_nano_build_generator_ninja)

	if (NN_COMPILER_CLANG)
		target_compile_options("${PROJECT_NAME}" PRIVATE "-fcolor-diagnostics")

	elseif (NN_COMPILER_GCC)
		target_compile_options("${PROJECT_NAME}" PRIVATE "-fdiagnostics-color=always")
	endif()

endfunction()





#==============================================================================
#		_nano_build_generator_ide : Prepare an IDE build.
#------------------------------------------------------------------------------
function(_nano_build_generator_ide)

	# Get the state we need
	get_target_property(PROJECT_SOURCES "${PROJECT_NAME}" SOURCES)

	get_filename_component(SOURCE_ROOT "${NN_SOURCE_DIR}" ABSOLUTE)



	# Group files under the source root
	foreach (FILE_PATH IN LISTS PROJECT_SOURCES)
		get_filename_component(FILE_PATH "${FILE_PATH}" ABSOLUTE)

		string(FIND "${FILE_PATH}" "${SOURCE_ROOT}" OFFSET_PREFIX)

		if (OFFSET_PREFIX EQUAL 0)
			source_group(TREE "${NN_SOURCE_DIR}" FILES ${FILE_PATH})
		endif()

	endforeach()

endfunction()





#==============================================================================
#		_nano_build_generator : Prepare the build generator.
#------------------------------------------------------------------------------
function(_nano_build_generator)

	if (${CMAKE_GENERATOR} STREQUAL "Ninja")
		_nano_build_generator_ninja()
	else()
		_nano_build_generator_ide()
	endif()

endfunction()





#==============================================================================
#		_nano_build_add_compile_options : Add COMPILE_OPTIONS to a file.
#------------------------------------------------------------------------------
function(_nano_build_add_compile_options FILE_PATH NEW_OPTIONS)

	set_property(SOURCE "${FILE_PATH}" APPEND_STRING PROPERTY COMPILE_OPTIONS "${NEW_OPTIONS}")

endfunction()





#==============================================================================
#		_nano_build_get_file_warnings : Get the files with individual warning levels.
#------------------------------------------------------------------------------
function(_nano_build_get_file_warnings WARNING_FILES WARNING_LEVEL)

	get_target_property(FILE_PATHS "${PROJECT_NAME}" "${WARNING_LEVEL}")

	if (NOT FILE_PATHS STREQUAL "FILE_PATHS-NOTFOUND")
		foreach (FILE_PATH IN LISTS FILE_PATHS)
			file(REAL_PATH "${FILE_PATH}" FILE_PATH)
			list(APPEND REAL_FILE_PATHS "${FILE_PATH}")
		endforeach() 
	endif()

	set(${WARNING_FILES} "${REAL_FILE_PATHS}" PARENT_SCOPE)

endfunction()





#==============================================================================
#		_nano_build_apply_file_warnings : Apply the per-file warning flags.
#------------------------------------------------------------------------------
function(_nano_build_apply_file_warnings PATH_SOURCE WARNING_FILES WARNING_FLAGS)

	foreach (PATH_WARNING IN LISTS WARNING_FILES)

		string(FIND "${PATH_SOURCE}" "${PATH_WARNING}" FOUND_SOURCE)
		if (FOUND_SOURCE EQUAL 0)
			# Work around MSVC bug
			#
			# MSVC ignores the warning level set on an individual file in
			# favour of the warning level set on the precompiled header.
			#
			# To work around this we disable precompiled headers for any
			# files with a non-default warning level, and insert them as
			# a force-included header instead.
			if (NN_COMPILER_MSVC)
				set_property(SOURCE "${PATH_SOURCE}" PROPERTY SKIP_PRECOMPILE_HEADERS ON)

				get_target_property(PREFIX_HEADERS "${PROJECT_NAME}" PRECOMPILE_HEADERS)
				foreach (PATH_PREFIX IN LISTS PREFIX_HEADERS)
					_nano_build_add_compile_options("${PATH_SOURCE}" "/FI${PATH_PREFIX}")
				endforeach()
			endif()


			# Set the warning
			_nano_build_add_compile_options("${PATH_SOURCE}" "${WARNING_FLAGS}")
			break()
		endif()

	endforeach()

endfunction()





#==============================================================================
#		_nano_build_compile_warnings : Set the compiler warnings.
#------------------------------------------------------------------------------
function(_nano_build_compile_warnings)

	# Set the default warning level
	get_target_property(WARNING_DEFAULT "${PROJECT_NAME}" NN_WARNINGS_DEFAULT)

	if (WARNING_DEFAULT STREQUAL "MAXIMUM" OR WARNING_DEFAULT STREQUAL "WARNING_DEFAULT-NOTFOUND")
		target_compile_options("${PROJECT_NAME}" PRIVATE "${NN_COMPILER_WARNINGS_MAXIMUM}")

	elseif (WARNING_DEFAULT STREQUAL "MINIMUM")
		target_compile_options("${PROJECT_NAME}" PRIVATE "${NN_COMPILER_WARNINGS_MINIMUM}")

	elseif (WARNING_DEFAULT STREQUAL "NONE")
		target_compile_options("${PROJECT_NAME}" PRIVATE "${NN_COMPILER_WARNINGS_NONE}")

	else()
		nano_log_error("Unknown default warning level '${WARNING_DEFAULT}'!")
	endif()



	# Set any per-file warning levels
	_nano_build_get_file_warnings(SOURCES_MAXIMUM NN_WARNINGS_MAXIMUM)
	_nano_build_get_file_warnings(SOURCES_MINIMUM NN_WARNINGS_MINIMUM)
	_nano_build_get_file_warnings(SOURCES_NONE    NN_WARNINGS_NONE)

	if (SOURCES_MAXIMUM OR SOURCES_MINIMUM OR SOURCES_NONE)

		get_target_property(SOURCES_ALL "${PROJECT_NAME}" SOURCES)

		foreach (PATH_SOURCE IN LISTS SOURCES_ALL)
			file(REAL_PATH "${PATH_SOURCE}" PATH_SOURCE)
			_nano_build_apply_file_warnings("${PATH_SOURCE}" "${SOURCES_MAXIMUM}" "${NN_COMPILER_WARNINGS_MAXIMUM}")
			_nano_build_apply_file_warnings("${PATH_SOURCE}" "${SOURCES_MINIMUM}" "${NN_COMPILER_WARNINGS_MINIMUM}")
			_nano_build_apply_file_warnings("${PATH_SOURCE}" "${SOURCES_NONE}"    "${NN_COMPILER_WARNINGS_NONE}")
		endforeach()

	endif()

endfunction()





#==============================================================================
#		_nano_build_compile_language : Set the compiler language.
#------------------------------------------------------------------------------
function(_nano_build_compile_language)

	# Get the state we need
	get_target_property(SOURCES_C      "${PROJECT_NAME}" NN_LANGUAGE_C)
	get_target_property(SOURCES_CPP    "${PROJECT_NAME}" NN_LANGUAGE_CPP)
	get_target_property(SOURCES_OBJCPP "${PROJECT_NAME}" NN_LANGUAGE_OBJCPP)



	# Apply the languages
	foreach (FILE_PATH IN LISTS SOURCES_C)
		_nano_build_add_compile_options("${FILE_PATH}" "${NN_COMPILER_LANGUAGE_C}")
	endforeach()

	foreach (FILE_PATH IN LISTS SOURCES_CPP)
		_nano_build_add_compile_options("${FILE_PATH}" "${NN_COMPILER_LANGUAGE_CPP}")
	endforeach()

	foreach (FILE_PATH IN LISTS SOURCES_OBJCPP)
		_nano_build_add_compile_options("${FILE_PATH}" "${NN_COMPILER_LANGUAGE_OBJCPP}")
	endforeach()

endfunction()





#==============================================================================
#		_nano_build_prepare : Prepare the build.
#------------------------------------------------------------------------------
function(_nano_build_prepare)

	# Generator
	_nano_build_generator()



	# Compiler
	target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_17)
	target_compile_features("${PROJECT_NAME}" PUBLIC   c_std_11)

	target_compile_definitions("${PROJECT_NAME}" PRIVATE NN_DEBUG=$<BOOL:$<CONFIG:Debug>>)
	target_compile_definitions("${PROJECT_NAME}" PRIVATE NN_RELEASE=$<BOOL:$<CONFIG:Release>>)

	target_compile_options("${PROJECT_NAME}" PRIVATE ${NN_COMPILER_OPTIONS})
	_nano_build_compile_warnings()
	_nano_build_compile_language()



	# Linker
	target_link_options("${PROJECT_NAME}" PRIVATE ${NN_LINKER_OPTIONS})

endfunction()





#==============================================================================
#		_nano_build_app : Build an app.
#------------------------------------------------------------------------------
function(_nano_build_app)

	_nano_build_prepare()

endfunction()





#==============================================================================
#		_nano_build_library : Build a library.
#------------------------------------------------------------------------------
function(_nano_build_library)

	_nano_build_prepare()

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
