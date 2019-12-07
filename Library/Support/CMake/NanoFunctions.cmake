#==============================================================================
#		target_nano_create_groups : Organise source into groups for IDEs.
#------------------------------------------------------------------------------
function(target_nano_create_groups target)

	get_target_property(theSources "${target}" SOURCES)

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
#		target_nano_compile_options : Set the compiler options.
#------------------------------------------------------------------------------
function(target_nano_compile_options theTarget)

	# Features
	target_compile_features("${theTarget}" PUBLIC "cxx_std_17")
	target_compile_features("${theTarget}" PUBLIC "c_std_11")


	# Flags
	target_compile_options("${theTarget}" PRIVATE ${NN_COMPILER_FLAGS})
	target_compile_options("${theTarget}" PRIVATE ${NN_COMPILER_WARNINGS})

endfunction()






#==============================================================================
#		target_nano_find_link_library : Find and link a library.
#------------------------------------------------------------------------------
function(target_nano_find_link_library theTarget theLibrary)

	find_library("NN_LIBRARY_${theLibrary}" "${theLibrary}")
	target_link_libraries("${theTarget}" PRIVATE "${NN_LIBRARY_${theLibrary}}")

endfunction()





#==============================================================================
#		target_nano_build_library : Build a library target.
#------------------------------------------------------------------------------
function(target_nano_build_library theTarget)

	target_nano_compile_options("${theTarget}")
	target_nano_create_groups(  "${theTarget}")

endfunction()





#==============================================================================
#		nano_target_build_app : Build the current project as an app.
#------------------------------------------------------------------------------
function(nano_target_build_app theTarget)

	target_nano_compile_options("${theTarget}")
	target_nano_create_groups(	"${theTarget}")
	target_link_libraries(		"${theTarget}" PRIVATE "Nano")


	if (NN_TARGET_ANDROID)
		target_link_libraries("${theTarget}" PRIVATE "android")
		target_link_libraries("${theTarget}" PRIVATE "log")
		target_link_libraries("${theTarget}" PRIVATE "pthread")

	elseif (NN_TARGET_IOS)
		target_nano_find_link_library("${theTarget}" "CoreFoundation")

	elseif (NN_TARGET_LINUX)
		target_link_libraries("${theTarget}" PRIVATE "pthread")

	elseif (NN_TARGET_MACOS)
		target_nano_find_link_library("${theTarget}" "CoreFoundation")

	elseif (NN_TARGET_TVOS)
		target_nano_find_link_library("${theTarget}" "CoreFoundation")

	elseif (NN_TARGET_WINDOWS)

	endif()

endfunction()



