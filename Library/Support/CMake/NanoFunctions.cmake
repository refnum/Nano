#==============================================================================
#		nano_target_source_make_groups : Create source groupings for IDEs.
#------------------------------------------------------------------------------
function(nano_target_source_make_groups theTarget)

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
#		nano_target_compile_set_defaults : Set the compiler's default options.
#------------------------------------------------------------------------------
function(nano_target_compile_set_defaults theTarget)

	# Features
	target_compile_features("${theTarget}" PUBLIC "cxx_std_17")
	target_compile_features("${theTarget}" PUBLIC "c_std_11")


	# Flags
	target_compile_options("${theTarget}" PRIVATE ${NN_COMPILER_FLAGS})
	target_compile_options("${theTarget}" PRIVATE ${NN_COMPILER_WARNINGS})

endfunction()





#==============================================================================
#		nano_target_link_add_library : Find and link to a library.
#------------------------------------------------------------------------------
function(nano_target_link_add_library theTarget theLibrary)

	find_library("_NN_LIBRARY_${theLibrary}" "${theLibrary}")

	target_link_libraries("${theTarget}" PRIVATE "${_NN_LIBRARY_${theLibrary}}")

endfunction()





#==============================================================================
#		nano_target_build_library : Build a library target.
#------------------------------------------------------------------------------
function(nano_target_build_library theTarget)

	nano_target_compile_set_defaults(	"${theTarget}")
	nano_target_source_make_groups(		"${theTarget}")

endfunction()





#==============================================================================
#		nano_target_build_app : Build the current project as an app.
#------------------------------------------------------------------------------
function(nano_target_build_app theTarget)

	nano_target_compile_set_defaults(	"${theTarget}")
	nano_target_source_make_groups(		"${theTarget}")
	target_link_libraries(				"${theTarget}" PRIVATE "Nano")


	if (NN_TARGET_ANDROID)
		target_link_libraries("${theTarget}" PRIVATE "android")
		target_link_libraries("${theTarget}" PRIVATE "log")

	elseif (NN_TARGET_IOS)
		nano_target_link_add_library("${theTarget}" "CoreFoundation")

	elseif (NN_TARGET_LINUX)
		target_link_libraries("${theTarget}" PRIVATE "pthread")

	elseif (NN_TARGET_MACOS)
		nano_target_link_add_library("${theTarget}" "CoreFoundation")

	elseif (NN_TARGET_TVOS)
		nano_target_link_add_library("${theTarget}" "CoreFoundation")

	elseif (NN_TARGET_WINDOWS)

	endif()

endfunction()



