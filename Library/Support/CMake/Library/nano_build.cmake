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
#		_nano_build_get_real_paths : Get a list of real paths.
#------------------------------------------------------------------------------
function(_nano_build_get_real_paths OUTPUT_PATHS INPUT_PATHS)

	if (INPUT_PATHS)
		foreach (INPUT_PATH IN LISTS INPUT_PATHS)
			file(REAL_PATH "${INPUT_PATH}" INPUT_PATH)
			list(APPEND REAL_PATHS "${INPUT_PATH}")
		endforeach()
	endif()

	set(${OUTPUT_PATHS} "${REAL_PATHS}" PARENT_SCOPE)

endfunction()





#==============================================================================
#		_nano_build_get_matching_paths : Get a list of matching paths.
#------------------------------------------------------------------------------
#		Takes a list of file paths and a list of paths to match against that
#		list.
#
#		INPUT_PATHS contains a list of file paths.
#
#		MATCHING_PATHS contains a list of file or directory paths.
#
#		File paths will match if they exist in the input paths, directory
#		paths will match any file under that directory in the intput paths.
#
#		The input paths are assumed to have been expanded to real paths which
#		allows us to identify a match by comparing the prefix.
#------------------------------------------------------------------------------
function(_nano_build_get_matching_paths OUTPUT_PATHS INPUT_PATHS MATCHING_PATHS)

	if (MATCHING_PATHS AND INPUT_PATHS)
		foreach (MATCH_PATH IN LISTS MATCHING_PATHS)
			file(REAL_PATH "${MATCH_PATH}" MATCH_PATH)

			foreach (INPUT_PATH IN LISTS INPUT_PATHS)
		
				string(FIND "${INPUT_PATH}" "${MATCH_PATH}" FOUND_MATCH)
				if (FOUND_MATCH EQUAL 0)
					list(APPEND MATCHED_PATHS "${INPUT_PATH}")
				endif()
		
			endforeach()
		endforeach()
	endif()

	set(${OUTPUT_PATHS} "${MATCHED_PATHS}" PARENT_SCOPE)

endfunction()





#==============================================================================
#		_nano_build_generator_cmdline : Prepare a command-line build.
#------------------------------------------------------------------------------
function(_nano_build_generator_cmdline)

	# Enable colour output
	nano_project_options(
		CLANG
			"-fcolor-diagnostics"
		GCC
			"-fdiagnostics-color=always"
	)



	# Work around precompiled header bug
	#
	# The Ninja / Makefile generators implement precompiled headers by
	# creating a set of language-specific headers that include the prefix
	# headers and precompiling those headers for each language.
	#
	# The precompiled prefix header used for a given source file is selected
	# based on the extension of that source file, rather than the language
	# used to compile that file.
	#
	# This fails if the langauge has been overriden, as the precompiled
	# headers are language-specific.
	#
	#
	# To work around this we disable precompiled headers for any files whose
	# language has been overriden.
	#
	# Disabling precompiled headers also disables their inclusion as a prefix
	# and so we need to manually force their inclusion.
	get_target_property(PREFIX_FILES "${PROJECT_NAME}" NN_PREFIX_FILES)

	if (PREFIX_FILES)
		get_target_property(SOURCE_LANGUAGE_C		"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_C)
		get_target_property(SOURCE_LANGUAGE_CPP		"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_CPP)
		get_target_property(SOURCE_LANGUAGE_OBJCPP	"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_OBJCPP)

		list(APPEND SOURCE_SKIP_PRECOMPILE
			${SOURCE_LANGUAGE_C}
			${SOURCE_LANGUAGE_CPP}
			${SOURCE_LANGUAGE_OBJCPP}
		)


		set_source_files_properties(${SOURCE_SKIP_PRECOMPILE} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

		foreach (PREFIX_PATH IN LISTS PREFIX_FILES)
			nano_project_options(ALL -include "${PREFIX_PATH}" FILES ${SOURCE_SKIP_PRECOMPILE})
		endforeach()
	endif()



	# Work around language override bug
	#
	# Compiling .c files as C++ produces a warning from clang:
	#
	#	error: treating 'c' input as 'c++' when in C++ mode, this
	#	       behavior is deprecated [-Werror,-Wdeprecated]
	#
	# This warning is not emitted when clang is invoked by Xcode so we
	# only suppress the warning for the Ninja / Makefile generators.
	nano_project_options(CLANG "-Wno-deprecated" FILES ${SOURCE_LANGUAGE_CPP})

endfunction()





#==============================================================================
#		_nano_build_generator_ide : Prepare an IDE build.
#------------------------------------------------------------------------------
function(_nano_build_generator_ide)

	# Create source groups
	get_filename_component(SOURCE_ROOT  "${NN_SOURCE_DIR}" ABSOLUTE)
	get_target_property(   SOURCE_FILES "${PROJECT_NAME}"  NN_SOURCE_FILES)

	foreach (SOURCE_PATH IN LISTS SOURCE_FILES)

		string(FIND "${SOURCE_PATH}" "${SOURCE_ROOT}" FOUND_ROOT)
		if (FOUND_ROOT EQUAL 0)
			source_group(TREE "${NN_SOURCE_DIR}" FILES "${SOURCE_PATH}")
		endif()

	endforeach()

endfunction()





#==============================================================================
#		_nano_build_generator_make : Prepare a Make build.
#------------------------------------------------------------------------------
function(_nano_build_generator_make)

	_nano_build_generator_cmdline()

endfunction()





#==============================================================================
#		_nano_build_generator_msvc : Prepare an MSVC build.
#------------------------------------------------------------------------------
function(_nano_build_generator_msvc)

	# Work around precompiled header bugs
	#
	# The MSVC generator implements precompiled headers by creating a set of
	# language-specific headers that include the prefix headers and
	# precompiling those headers for each language.
	#
	# The precompiled prefix header used for a given source file is selected
	# based on the extension of that source file, rather than the language
	# used to compile that file.
	#
	# This fails if the langauge has been overriden, as the precompiled
	# headers are language-specific.
	#
	# In addition MSVC ignores the warning level set on individual files in
	# favour of the warning level set on the precompiled prefix header.
	#
	#
	# To work around this we disable precompiled headers for any files whose
	# language has been overriden, or which have a custom warning level.
	#
	# Disabling precompiled headers also disables their inclusion as a prefix
	# and so we need to manually force their inclusion.
	get_target_property(PREFIX_FILES "${PROJECT_NAME}" NN_PREFIX_FILES)

	if (PREFIX_FILES)
		get_target_property(SOURCE_WARNINGS_MAXIMUM "${PROJECT_NAME}" NN_SOURCE_WARNINGS_MAXIMUM)
		get_target_property(SOURCE_WARNINGS_MINIMUM "${PROJECT_NAME}" NN_SOURCE_WARNINGS_MINIMUM)
		get_target_property(SOURCE_WARNINGS_NONE    "${PROJECT_NAME}" NN_SOURCE_WARNINGS_NONE)

		get_target_property(SOURCE_LANGUAGE_C		"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_C)
		get_target_property(SOURCE_LANGUAGE_CPP		"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_CPP)
		get_target_property(SOURCE_LANGUAGE_OBJCPP	"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_OBJCPP)

		list(APPEND SOURCE_SKIP_PRECOMPILE
			${SOURCE_WARNINGS_MAXIMUM}
			${SOURCE_WARNINGS_MINIMUM}
			${SOURCE_WARNINGS_NONE}
			${SOURCE_LANGUAGE_C}
			${SOURCE_LANGUAGE_CPP}
			${SOURCE_LANGUAGE_OBJCPP}
		)


		set_source_files_properties(${SOURCE_SKIP_PRECOMPILE} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

		foreach (PREFIX_PATH IN LISTS PREFIX_FILES)
			nano_project_options(MSVC "/FI${PREFIX_PATH}" FILES ${SOURCE_SKIP_PRECOMPILE})
		endforeach()
	endif()



	# IDE generator
	_nano_build_generator_ide()

endfunction()





#==============================================================================
#		_nano_build_generator_ninja : Prepare a Ninja build.
#------------------------------------------------------------------------------
function(_nano_build_generator_ninja)

	_nano_build_generator_cmdline()

endfunction()





#==============================================================================
#		_nano_build_generator_xcode : Prepare an Xcode build.
#------------------------------------------------------------------------------
function(_nano_build_generator_xcode)

	# Work around precompiled header bug
	#
	# The Xcode generator implements precompiled headers by creating a set of
	# language-specific headers that include the prefix headers then setting
	# the Xcode project to use one of those headers as the prefix header.
	#
	# As Xcode only supports a single precompiled prefix header this means
	# that this header is only visible to one language.
	#
	#
	# To work around this we set Xcode's prefix header directly, and let Xcode
	# generate its own language-specific precompiled prefix headers.
	#
	# This approach assumes we only have a single prefix header.
	#
	# To support multiple prefix headers we would need to include them from a
	# generated source file then tell Xcode to use that generated files as the
	# prefix header.
	get_target_property(PREFIX_FILES "${PROJECT_NAME}" NN_PREFIX_FILES)
	list(LENGTH PREFIX_FILES PREFIX_COUNT)

	if (PREFIX_FILES AND PREFIX_COUNT EQUAL 1)

		set_target_properties("${PROJECT_NAME}" PROPERTIES
			XCODE_ATTRIBUTE_GCC_PREFIX_HEADER				"${PREFIX_FILES}"
			XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER	"YES")

	elseif (PREFIX_COUNT GREATER 1)
		nano_log_error("Multiple prefix headers not supported!")
	endif()



	# IDE generator
	_nano_build_generator_ide()

endfunction()





#==============================================================================
#		_nano_build_generator : Prepare the build generator.
#------------------------------------------------------------------------------
function(_nano_build_generator)

	if (NN_GENERATOR_MAKE)
		_nano_build_generator_make()

	elseif (NN_GENERATOR_MSVC)
		_nano_build_generator_msvc()

	elseif (NN_GENERATOR_NINJA)
		_nano_build_generator_ninja()

	elseif (NN_GENERATOR_XCODE)
		_nano_build_generator_xcode()
	
	else()
		nano_log_error("Unknown build generator")
	endif()

endfunction()





#==============================================================================
#		_nano_build_compiler_warnings : Set the compiler warnings.
#------------------------------------------------------------------------------
function(_nano_build_compiler_warnings)

	# Set the default warning level
	get_target_property(DEFAULT_WARNINGS "${PROJECT_NAME}" NN_DEFAULT_WARNINGS)

	if (DEFAULT_WARNINGS STREQUAL "MAXIMUM" OR NOT DEFAULT_WARNINGS)
		target_compile_options("${PROJECT_NAME}" PRIVATE "${NN_COMPILER_WARNINGS_MAXIMUM}"
								$<$<COMPILE_LANGUAGE:C>:      ${NN_COMPILER_WARNINGS_MAXIMUM_C}>
								$<$<COMPILE_LANGUAGE:CXX>:    ${NN_COMPILER_WARNINGS_MAXIMUM_CPP}>
								$<$<COMPILE_LANGUAGE:OBJC>:   ${NN_COMPILER_WARNINGS_MAXIMUM_OBJC}>
								$<$<COMPILE_LANGUAGE:OBJCXX>: ${NN_COMPILER_WARNINGS_MAXIMUM_CPP}>
								$<$<COMPILE_LANGUAGE:OBJCXX>: ${NN_COMPILER_WARNINGS_MAXIMUM_OBJC}>)

	elseif (DEFAULT_WARNINGS STREQUAL "MINIMUM")
		target_compile_options("${PROJECT_NAME}" PRIVATE "${NN_COMPILER_WARNINGS_MINIMUM}"
								$<$<COMPILE_LANGUAGE:C>:      ${NN_COMPILER_WARNINGS_MINIMUM_C}>
								$<$<COMPILE_LANGUAGE:CXX>:    ${NN_COMPILER_WARNINGS_MINIMUM_CPP}>
								$<$<COMPILE_LANGUAGE:OBJC>:   ${NN_COMPILER_WARNINGS_MINIMUM_OBJC}>
								$<$<COMPILE_LANGUAGE:OBJCXX>: ${NN_COMPILER_WARNINGS_MINIMUM_CPP}>
								$<$<COMPILE_LANGUAGE:OBJCXX>: ${NN_COMPILER_WARNINGS_MINIMUM_OBJC}>)

	elseif (DEFAULT_WARNINGS STREQUAL "NONE")
		target_compile_options("${PROJECT_NAME}" PRIVATE "${NN_COMPILER_WARNINGS_NONE}")

	else()
		nano_log_error("Unknown default warning level '${WARNING_DEFAULT}'!")
	endif()



	# Set the per-file warning levels
	get_target_property(SOURCE_WARNINGS_MAXIMUM	"${PROJECT_NAME}" NN_SOURCE_WARNINGS_MAXIMUM)
	get_target_property(SOURCE_WARNINGS_MINIMUM	"${PROJECT_NAME}" NN_SOURCE_WARNINGS_MINIMUM)
	get_target_property(SOURCE_WARNINGS_NONE	"${PROJECT_NAME}" NN_SOURCE_WARNINGS_NONE)

	nano_project_options(ALL ${NN_COMPILER_WARNINGS_MAXIMUM} FILES ${SOURCE_WARNINGS_MAXIMUM})
	nano_project_options(ALL ${NN_COMPILER_WARNINGS_MINIMUM} FILES ${SOURCE_WARNINGS_MINIMUM})
	nano_project_options(ALL ${NN_COMPILER_WARNINGS_NONE}    FILES ${SOURCE_WARNINGS_NONE})

endfunction()





#==============================================================================
#		_nano_build_compiler_language : Set the compiler language.
#------------------------------------------------------------------------------
function(_nano_build_compiler_language)

	# Set the per-file language
	get_target_property(SOURCE_LANGUAGE_C			"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_C)
	get_target_property(SOURCE_LANGUAGE_CPP			"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_CPP)
	get_target_property(SOURCE_LANGUAGE_OBJCPP		"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_OBJCPP)

	set_source_files_properties(${SOURCE_LANGUAGE_C}      PROPERTIES LANGUAGE C)
	set_source_files_properties(${SOURCE_LANGUAGE_CPP}    PROPERTIES LANGUAGE CXX)
	set_source_files_properties(${SOURCE_LANGUAGE_OBJCPP} PROPERTIES LANGUAGE OBJCXX)

endfunction()





#==============================================================================
#		_nano_build_compiler_source : Set the compiler source options.
#------------------------------------------------------------------------------
function(_nano_build_compiler_source)

	# Create the paths
	#
	# To simplify path comparisons we normalise all paths to real paths,
	# and match any file-or-directory file lists into a list of files.
	get_target_property(SOURCE_FILES				"${PROJECT_NAME}" SOURCES)
	get_target_property(PREFIX_FILES				"${PROJECT_NAME}" PRECOMPILE_HEADERS)

	get_target_property(SOURCE_WARNINGS_MAXIMUM		"${PROJECT_NAME}" NN_SOURCE_WARNINGS_MAXIMUM)
	get_target_property(SOURCE_WARNINGS_MINIMUM		"${PROJECT_NAME}" NN_SOURCE_WARNINGS_MINIMUM)
	get_target_property(SOURCE_WARNINGS_NONE		"${PROJECT_NAME}" NN_SOURCE_WARNINGS_NONE)

	get_target_property(SOURCE_LANGUAGE_C			"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_C)
	get_target_property(SOURCE_LANGUAGE_CPP			"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_CPP)
	get_target_property(SOURCE_LANGUAGE_OBJCPP		"${PROJECT_NAME}" NN_SOURCE_LANGUAGE_OBJCPP)


	_nano_build_get_real_paths(SOURCE_FILES					"${SOURCE_FILES}")
	_nano_build_get_real_paths(PREFIX_FILES					"${PREFIX_FILES}")

	_nano_build_get_matching_paths(SOURCE_WARNINGS_MAXIMUM	"${SOURCE_FILES}" "${SOURCE_WARNINGS_MAXIMUM}")
	_nano_build_get_matching_paths(SOURCE_WARNINGS_MINIMUM	"${SOURCE_FILES}" "${SOURCE_WARNINGS_MINIMUM}")
	_nano_build_get_matching_paths(SOURCE_WARNINGS_NONE		"${SOURCE_FILES}" "${SOURCE_WARNINGS_NONE}")

	_nano_build_get_matching_paths(SOURCE_LANGUAGE_C		"${SOURCE_FILES}" "${SOURCE_LANGUAGE_C}")
	_nano_build_get_matching_paths(SOURCE_LANGUAGE_CPP		"${SOURCE_FILES}" "${SOURCE_LANGUAGE_CPP}")
	_nano_build_get_matching_paths(SOURCE_LANGUAGE_OBJCPP	"${SOURCE_FILES}" "${SOURCE_LANGUAGE_OBJCPP}")


	set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_SOURCE_FILES				"${SOURCE_FILES}")
	set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_PREFIX_FILES				"${PREFIX_FILES}")

	set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_SOURCE_WARNINGS_MAXIMUM	"${SOURCE_WARNINGS_MAXIMUM}")
	set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_SOURCE_WARNINGS_MINIMUM	"${SOURCE_WARNINGS_MINIMUM}")
	set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_SOURCE_WARNINGS_NONE		"${SOURCE_WARNINGS_NONE}")

	set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_SOURCE_LANGUAGE_C			"${SOURCE_LANGUAGE_C}")
	set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_SOURCE_LANGUAGE_CPP		"${SOURCE_LANGUAGE_CPP}")
	set_property(TARGET "${PROJECT_NAME}" PROPERTY NN_SOURCE_LANGUAGE_OBJCPP	"${SOURCE_LANGUAGE_OBJCPP}")



	# Set the source options
	_nano_build_compiler_warnings()
	_nano_build_compiler_language()

endfunction()





#==============================================================================
#		_nano_build_compiler_options : Set the compiler options.
#------------------------------------------------------------------------------
function(_nano_build_compiler_options)

	# Options
	nano_project_options(ALL ${NN_COMPILER_OPTIONS})

	set_property(TARGET "${PROJECT_NAME}" PROPERTY POSITION_INDEPENDENT_CODE ON)

	set_property(TARGET "${PROJECT_NAME}" PROPERTY CXX_STANDARD          17)
	set_property(TARGET "${PROJECT_NAME}" PROPERTY CXX_EXTENSIONS        OFF)
	set_property(TARGET "${PROJECT_NAME}" PROPERTY CXX_STANDARD_REQUIRED ON)

	set_property(TARGET "${PROJECT_NAME}" PROPERTY C_STANDARD          11)
	set_property(TARGET "${PROJECT_NAME}" PROPERTY C_EXTENSIONS        OFF)
	set_property(TARGET "${PROJECT_NAME}" PROPERTY C_STANDARD_REQUIRED ON)



	# Definitions
	nano_project_definitions(NN_DEBUG=$<BOOL:$<CONFIG:Debug>>)
	nano_project_definitions(NN_RELEASE=$<BOOL:$<CONFIG:Release>>)

endfunction()





#==============================================================================
#		_nano_build_linker_options : Set the linker options.
#------------------------------------------------------------------------------
function(_nano_build_linker_options)

	target_link_options("${PROJECT_NAME}" PRIVATE ${NN_LINKER_OPTIONS})

endfunction()





#==============================================================================
#		_nano_build_prepare : Prepare the build.
#------------------------------------------------------------------------------
function(_nano_build_prepare)

	_nano_build_compiler_source()
	_nano_build_compiler_options()
	_nano_build_linker_options()
	_nano_build_generator()

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
