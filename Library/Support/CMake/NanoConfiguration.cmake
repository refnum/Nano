#==============================================================================
#		Configuration
#------------------------------------------------------------------------------
set(NN_DEBUG												0)
set(NN_RELEASE												0)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(NN_DEBUG											1)

elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	set(NN_RELEASE											1)

else()
	message(FATAL_ERROR "[Nano] Unable to identify configuration from '${CMAKE_BUILD_TYPE}'")
endif()





#==============================================================================
#		Compiler - Configuration
#------------------------------------------------------------------------------
set(        NN_COMPILER_FLAGS "")
list(APPEND NN_COMPILER_FLAGS -DNN_DEBUG=0)
list(APPEND NN_COMPILER_FLAGS -DNN_RELEASE=0)


if (NN_DEBUG)
	list(APPEND NN_COMPILER_FLAGS -UNN_DEBUG   -DNN_DEBUG=1)
else()
	list(APPEND NN_COMPILER_FLAGS -UNN_RELEASE -DNN_RELEASE=1)
endif()


if (NN_COMPILER_CLANG)
	list(APPEND NN_COMPILER_FLAGS -fconstant-cfstrings)
	list(APPEND NN_COMPILER_FLAGS -fno-common)

	if (NN_DEBUG)
		list(APPEND NN_COMPILER_FLAGS -O0)
		list(APPEND NN_COMPILER_FLAGS -fstack-protector-all)
		list(APPEND NN_COMPILER_FLAGS -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2)
	else()
		list(APPEND NN_COMPILER_FLAGS -O3)
		list(APPEND NN_COMPILER_FLAGS -fno-stack-protector)
	endif()


elseif (NN_COMPILER_GCC)

	if (NN_DEBUG)
		list(APPEND NN_COMPILER_FLAGS -O0)
	else()
		list(APPEND NN_COMPILER_FLAGS -O3)
	endif()


elseif (NN_COMPILER_MSVC)

	list(APPEND NN_COMPILER_FLAGS /DNOMINMAX)
	list(APPEND NN_COMPILER_FLAGS /DUNICODE)
	list(APPEND NN_COMPILER_FLAGS /DVC_EXTRALEAN)
	list(APPEND NN_COMPILER_FLAGS /DWIN32_LEAN_AND_MEAN)

endif()





#==============================================================================
#		Compiler - Warnings
#------------------------------------------------------------------------------
set(NN_COMPILER_WARNINGS "")

if (NN_COMPILER_CLANG)
	# Maximum warnings
	list(APPEND NN_COMPILER_WARNINGS -Weverything)
	list(APPEND NN_COMPILER_WARNINGS -Werror)

	# Revert Xcode suppressions)
	list(APPEND NN_COMPILER_WARNINGS -Wblock-capture-autoreleasing)
	list(APPEND NN_COMPILER_WARNINGS -Wbool-conversion)
	list(APPEND NN_COMPILER_WARNINGS -Wcomma)
	list(APPEND NN_COMPILER_WARNINGS -Wconstant-conversion)
	list(APPEND NN_COMPILER_WARNINGS -Wconversion)
	list(APPEND NN_COMPILER_WARNINGS -Wempty-body)
	list(APPEND NN_COMPILER_WARNINGS -Wenum-conversion)
	list(APPEND NN_COMPILER_WARNINGS -Wexit-time-destructors)
	list(APPEND NN_COMPILER_WARNINGS -Wfloat-conversion)
	list(APPEND NN_COMPILER_WARNINGS -Wfour-char-constants)
	list(APPEND NN_COMPILER_WARNINGS -Winfinite-recursion)
	list(APPEND NN_COMPILER_WARNINGS -Wint-conversion)
	list(APPEND NN_COMPILER_WARNINGS -Wmissing-braces)
	list(APPEND NN_COMPILER_WARNINGS -Wmissing-field-initializers)
	list(APPEND NN_COMPILER_WARNINGS -Wmissing-prototypes)
	list(APPEND NN_COMPILER_WARNINGS -Wmove)
	list(APPEND NN_COMPILER_WARNINGS -Wnewline-eof)
	list(APPEND NN_COMPILER_WARNINGS -Wnon-literal-null-conversion)
	list(APPEND NN_COMPILER_WARNINGS -Wnon-virtual-dtor)
	list(APPEND NN_COMPILER_WARNINGS -Wobjc-literal-conversion)
	list(APPEND NN_COMPILER_WARNINGS -Woverloaded-virtual)
	list(APPEND NN_COMPILER_WARNINGS -Wrange-loop-analysis)
	list(APPEND NN_COMPILER_WARNINGS -Wreturn-type)
	list(APPEND NN_COMPILER_WARNINGS -Wsemicolon-before-method-body)
	list(APPEND NN_COMPILER_WARNINGS -Wshadow)
	list(APPEND NN_COMPILER_WARNINGS -Wshorten-64-to-32)
	list(APPEND NN_COMPILER_WARNINGS -Wsign-conversion)
	list(APPEND NN_COMPILER_WARNINGS -Wstrict-prototypes)
	list(APPEND NN_COMPILER_WARNINGS -Wtrigraphs)
	list(APPEND NN_COMPILER_WARNINGS -Wuninitialized)
	list(APPEND NN_COMPILER_WARNINGS -Wunknown-pragmas)
	list(APPEND NN_COMPILER_WARNINGS -Wunused-function)
	list(APPEND NN_COMPILER_WARNINGS -Wunused-label)
	list(APPEND NN_COMPILER_WARNINGS -Wunused-parameter)
	list(APPEND NN_COMPILER_WARNINGS -Wunused-variable)

	# Discard unwanted warnings
	list(APPEND NN_COMPILER_WARNINGS -Wno-atomic-implicit-seq-cst)
	list(APPEND NN_COMPILER_WARNINGS -Wno-c++17-extensions)
	list(APPEND NN_COMPILER_WARNINGS -Wno-c++98-compat)
	list(APPEND NN_COMPILER_WARNINGS -Wno-c++98-compat-pedantic)
	list(APPEND NN_COMPILER_WARNINGS -Wno-c99-extensions)
	list(APPEND NN_COMPILER_WARNINGS -Wno-documentation-unknown-command)
	list(APPEND NN_COMPILER_WARNINGS -Wno-exit-time-destructors)
	list(APPEND NN_COMPILER_WARNINGS -Wno-float-equal)
	list(APPEND NN_COMPILER_WARNINGS -Wno-format-nonliteral)
	list(APPEND NN_COMPILER_WARNINGS -Wno-global-constructors)
	list(APPEND NN_COMPILER_WARNINGS -Wno-gnu-zero-variadic-macro-arguments)
	list(APPEND NN_COMPILER_WARNINGS -Wno-nested-anon-types)
	list(APPEND NN_COMPILER_WARNINGS -Wno-packed)
	list(APPEND NN_COMPILER_WARNINGS -Wno-padded)
	list(APPEND NN_COMPILER_WARNINGS -Wno-return-std-move-in-c++11)
	list(APPEND NN_COMPILER_WARNINGS -Wno-switch-enum)
	list(APPEND NN_COMPILER_WARNINGS -Wno-unreachable-code-break)
	list(APPEND NN_COMPILER_WARNINGS -Wno-weak-vtables)


elseif (NN_COMPILER_GCC)
	# Maximum warnings
	list(APPEND NN_COMPILER_WARNINGS -Wall)
	list(APPEND NN_COMPILER_WARNINGS -Wextra)
	list(APPEND NN_COMPILER_WARNINGS -Werror)

	# Discard unwanted warnings
	list(APPEND NN_COMPILER_WARNINGS -Wno-unknown-pragmas)


elseif (NN_COMPILER_MSC)
	list(APPEND NN_COMPILER_WARNINGS /Wall)
	list(APPEND NN_COMPILER_WARNINGS /WX)			# Warnings as errors

	# Discard unwanted warnings
	list(APPEND NN_COMPILER_WARNINGS /wd4068)		# Unknown pragma


endif()





