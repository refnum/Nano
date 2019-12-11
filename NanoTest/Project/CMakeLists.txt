#==============================================================================
#		Project
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.12)

project(NanoTest VERSION 4.0)
add_executable("${PROJECT_NAME}")





#==============================================================================
#		Cross-Platform
#------------------------------------------------------------------------------
set(TARGET_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../Source")

target_sources("${PROJECT_NAME}"
	PRIVATE
		"${TARGET_SOURCE_DIR}/../../Library/Source/Nanites/Catch/NTestFixture.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Core/TDebug.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Core/TLog.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Core/TLogOutput.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Threads/TMutex.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Threads/TSemaphore.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Threads/TSpinLock.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Threads/TThread.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Time/TTimeUtils.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TData.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TMixinAttributes.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TMixinComparable.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TMixinContainer.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TMixinHashable.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TRange.cpp"
		"${TARGET_SOURCE_DIR}/Nano/Types/TString.cpp"
		"${TARGET_SOURCE_DIR}/NanoTest.cpp"
)

target_include_directories("${PROJECT_NAME}"
	PRIVATE
		"${TARGET_SOURCE_DIR}"
		"${TARGET_SOURCE_DIR}/../../Library/Source/Nanites/Catch"
)





#==============================================================================
#		Build
#------------------------------------------------------------------------------
nano_target_build_app("${PROJECT_NAME}")

