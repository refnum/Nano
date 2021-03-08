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
#		nano_target_compile_create_groups : Create IDE source groups.
#------------------------------------------------------------------------------
function(nano_target_compile_create_groups theTarget)

	get_target_property(theSources "${theTarget}" SOURCES)

	foreach (theFile ${theSources})

		get_filename_component(thePath "${theFile}" ABSOLUTE)
		get_filename_component(thePath "${thePath}" PATH)

		if ("${theFile}" MATCHES "${PROJECT_BINARY_DIR}")
			file(RELATIVE_PATH "${thePath}" "${PROJECT_BINARY_DIR}" "${thePath}")
		else()
			file(RELATIVE_PATH "${thePath}" "${PROJECT_SOURCE_DIR}" "${thePath}")
		endif()

		string(REGEX REPLACE "/" "\\\\" winPath "${thePath}")
		source_group("${winPath}" REGULAR_EXPRESSION "${thePath}/[^/\\]+\\..*")

	endforeach()

endfunction()





#==============================================================================
#		nano_target_compile_set_defaults : Set the default compiler options.
#------------------------------------------------------------------------------
function(nano_target_compile_set_defaults theTarget)

	# Features
	target_compile_features("${theTarget}" PUBLIC "cxx_std_17")
	target_compile_features("${theTarget}" PUBLIC "c_std_11")


	# Flags
	if (NN_COMPILER_WARNINGS STREQUAL "")
		nano_target_compile_set_warnings_maximum(${theTarget})
	endif()

	target_compile_options("${theTarget}" PRIVATE ${NN_COMPILER_WARNINGS})
	target_compile_options("${theTarget}" PRIVATE ${NN_COMPILER_FLAGS})
	target_link_options(   "${theTarget}" PRIVATE ${NN_LINKER_FLAGS})

endfunction()





#==============================================================================
#		nano_target_compile_set_prefix : Set the compiler prefix header.
#------------------------------------------------------------------------------
function(nano_target_compile_set_prefix theTarget thePrefix)

	if (NN_COMPILER_MSVC)
		target_compile_options("${theTarget}" PRIVATE       /FI"${thePrefix}")
	else()
		target_compile_options("${theTarget}" PRIVATE -include "${thePrefix}")
	endif()

endfunction()





#==============================================================================
#		nano_target_compile_set_warnings_maximum : Set maximum warnings.
#------------------------------------------------------------------------------
function(nano_target_compile_set_warnings_maximum theTarget)

	set(NN_COMPILER_WARNINGS ${NN_COMPILER_WARNINGS_MAXIMUM})

endfunction()





#==============================================================================
#		nano_target_compile_set_warnings_none : Set no warnings.
#------------------------------------------------------------------------------
#		Note : Applies to specific files within the target.
#------------------------------------------------------------------------------
function(nano_target_compile_set_warnings_none theTarget thePattern)

	get_target_property(theSources "${theTarget}" SOURCES)

	foreach (theFile ${theSources})

		if ("${theFile}" MATCHES "${thePattern}")
			set_source_files_properties("${theFile}" PROPERTIES COMPILE_FLAGS "${NN_COMPILER_WARNINGS_NONE}")
		endif()

 	endforeach()

endfunction()



