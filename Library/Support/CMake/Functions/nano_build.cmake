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
	foreach (FILE_PATH ${PROJECT_SOURCES})
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
#		_nano_build_compile_warnings : Set the compiler warnings.
#------------------------------------------------------------------------------
function(_nano_build_compile_warnings)

	# Get the state we need
	get_target_property(PROJECT_SOURCES "${PROJECT_NAME}" SOURCES)

	get_target_property(WARNING_DEFAULT "${PROJECT_NAME}" NN_WARNINGS_DEFAULT)
	if (WARNING_DEFAULT STREQUAL "WARNING_DEFAULT-NOTFOUND")
		set(WARNING_DEFAULT "MAXIMUM")
	endif()

	get_target_property(SOURCES_MAXIMUM "${PROJECT_NAME}" NN_WARNINGS_MAXIMUM)
	get_target_property(SOURCES_MINIMUM "${PROJECT_NAME}" NN_WARNINGS_MINIMUM)
	get_target_property(SOURCES_NONE    "${PROJECT_NAME}" NN_WARNINGS_NONE)



	# Apply the warnings
	foreach (RAW_PATH_SOURCE ${PROJECT_SOURCES})
		file(REAL_PATH "${RAW_PATH_SOURCE}" PATH_SOURCE)

		set(FOUND_SOURCE -1)


		# Maximum
		if (FOUND_SOURCE EQUAL -1)
			foreach (RAW_PATH_WARNING ${SOURCES_MAXIMUM})
				file(REAL_PATH "${RAW_PATH_WARNING}" PATH_WARNING)

				string(FIND "${PATH_SOURCE}" "${PATH_WARNING}" FOUND_SOURCE)
				if (FOUND_SOURCE EQUAL 0)
					_nano_build_add_compile_options("${PATH_SOURCE}" "${NN_COMPILER_WARNINGS_MAXIMUM}")
					list(REMOVE_ITEM SOURCES_MAXIMUM "${PATH_WARNING}")
					break()
				endif()
			endforeach()
		endif()


		# Minimum
		if (FOUND_SOURCE EQUAL -1)
			foreach (RAW_PATH_WARNING ${SOURCES_MINIMUM})
				file(REAL_PATH "${RAW_PATH_WARNING}" PATH_WARNING)

				string(FIND "${PATH_SOURCE}" "${PATH_WARNING}" FOUND_SOURCE)
				if (FOUND_SOURCE EQUAL 0)
					_nano_build_add_compile_options("${PATH_SOURCE}" "${NN_COMPILER_WARNINGS_MINIMUM}")
					list(REMOVE_ITEM SOURCES_MINIMUM "${PATH_WARNING}")
					break()
				endif()
			endforeach()
		endif()


		# None
		if (FOUND_SOURCE EQUAL -1)
			foreach (RAW_PATH_WARNING ${SOURCES_NONE})
				file(REAL_PATH "${RAW_PATH_WARNING}" PATH_WARNING)

				string(FIND "${PATH_SOURCE}" "${PATH_WARNING}" FOUND_SOURCE)
				if (FOUND_SOURCE EQUAL 0)
					_nano_build_add_compile_options("${PATH_SOURCE}" "${NN_COMPILER_WARNINGS_NONE}")
					list(REMOVE_ITEM SOURCES_NONE "${PATH_WARNING}")
					break()
				endif()
			endforeach()
		endif()


		# Default
		if (FOUND_SOURCE EQUAL -1)
			if (WARNING_DEFAULT STREQUAL "MAXIMUM")
				_nano_build_add_compile_options("${PATH_SOURCE}" "${NN_COMPILER_WARNINGS_MAXIMUM}")

			elseif (WARNING_DEFAULT STREQUAL "MINIMUM")
				_nano_build_add_compile_options("${PATH_SOURCE}" "${NN_COMPILER_WARNINGS_MINIMUM}")

			elseif (WARNING_DEFAULT STREQUAL "NONE")
				_nano_build_add_compile_options("${PATH_SOURCE}" "${NN_COMPILER_WARNINGS_NONE}")

			else()
				nano_log_error("Unknown default warning level '${WARNING_DEFAULT}'!")
			endif()
		endif()

	endforeach()

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
	foreach (FILE_PATH ${SOURCES_C})
		_nano_build_add_compile_options("${FILE_PATH}" "${NN_COMPILER_LANGUAGE_C}")
	endforeach()

	foreach (FILE_PATH ${SOURCES_CPP})
		_nano_build_add_compile_options("${FILE_PATH}" "${NN_COMPILER_LANGUAGE_CPP}")
	endforeach()

	foreach (FILE_PATH ${SOURCES_OBJCPP})
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
	target_link_options("${PROJECT_NAME}" PRIVATE ${NN_LINKER_FLAGS})

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
