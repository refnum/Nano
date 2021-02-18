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
#		nano_target_build_library : Build a library target.
#------------------------------------------------------------------------------
function(nano_target_build_library theTarget)

	nano_target_compile_create_groups("${theTarget}")
	nano_target_compile_set_defaults( "${theTarget}")

endfunction()





#==============================================================================
#		nano_target_build_app : Build the current project as an app.
#------------------------------------------------------------------------------
function(nano_target_build_app theTarget)

	nano_target_compile_create_groups("${theTarget}")
	nano_target_compile_set_defaults( "${theTarget}")
	target_link_libraries(            "${theTarget}" PRIVATE "Nano")

	if (NN_TARGET_ANDROID)
		target_link_libraries("${theTarget}" PRIVATE "android")
		target_link_libraries("${theTarget}" PRIVATE "log")

	elseif (NN_TARGET_IOS)
		target_link_libraries("${theTarget}" PRIVATE "-framework CoreFoundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Foundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Security")

	elseif (NN_TARGET_LINUX)
		target_link_libraries("${theTarget}" PRIVATE "pthread")

	elseif (NN_TARGET_MACOS)
		target_link_libraries("${theTarget}" PRIVATE "-framework CoreFoundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Foundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Security")

	elseif (NN_TARGET_TVOS)
		target_link_libraries("${theTarget}" PRIVATE "-framework CoreFoundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Foundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Security")

	elseif (NN_TARGET_WINDOWS)
		target_link_libraries("${theTarget}" PRIVATE "bcrypt")

	endif()

endfunction()



