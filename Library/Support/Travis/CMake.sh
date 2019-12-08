#!/bin/bash



# Get the arguments
set -euo pipefail

TRAVIS_PROJECT="$1"
TRAVIS_PLATFORM="$2"




# Parse the platform
if [[ "${TRAVIS_PLATFORM}" == "Android" ]]; then

	mkdir -p "${TRAVIS_BUILD_DIR}/Build"
	cd       "${TRAVIS_BUILD_DIR}/Build"

	wget https://dl.google.com/android/repository/android-ndk-r20b-linux-x86_64.zip -O android-ndk-r20b-linux-x86_64.zip
	unzip -q android-ndk-r20b-linux-x86_64.zip

	CMAKE_GENERATOR="Ninja"
	CMAKE_PARAMS="-DANDROID_ABI=arm64-v8a -DANDROID_NATIVE_API_LEVEL=26 -DCMAKE_TOOLCHAIN_FILE=${TRAVIS_BUILD_DIR}/Build/android-ndk-r20b/build/cmake/android.toolchain.cmake"

elif [[ "${TRAVIS_PLATFORM}" == "Linux" ]]; then
	CMAKE_GENERATOR="Ninja"
	CMAKE_PARAMS=""

elif [[ "${TRAVIS_PLATFORM}" == "Windows" ]]; then
#	MSBUILD_PATH="/C/Program Files (x86)/Microsoft Visual Studio/2017/BuildTools/MSBuild/15.0/Bin"
#	PATH=$MSBUILD_PATH:$PATH

	CMAKE_GENERATOR="Visual Studio 15 2017 Win64"
	CMAKE_PARAMS=""

else
	echo "Unknown platform: ${TRAVIS_PLATFORM}"
	exit 1
fi




# Do the builds
for BUILD_CONFIG in "Debug" "Release"; do

	# Prepare to build
	mkdir -p "${TRAVIS_BUILD_DIR}/Build/${BUILD_CONFIG}"
	cd       "${TRAVIS_BUILD_DIR}/Build/${BUILD_CONFIG}"

	JOB_NAME="${TRAVIS_PROJECT}_${TRAVIS_PLATFORM} - ${BUILD_CONFIG}"

	echo "${JOB_NAME}"
	printf -v _hr "%*s" ${#JOB_NAME} && echo ${_hr// /=}



	# Perform the build
	cmake -G "${CMAKE_GENERATOR}" ${CMAKE_PARAMS} -DCMAKE_BUILD_TYPE="${BUILD_CONFIG}" "${TRAVIS_BUILD_DIR}"
	echo ""

	if [[ "${TRAVIS_PLATFORM}" != "Windows" ]]; then
		make -j3 "${TRAVIS_PROJECT}"
	else
#		MSBuild.exe Nano.sln //p:Configuration=${BUILD_CONFIG} //m //nologo //verbosity:minimal 
		cmake --build --config ${BUILD_CONFIG}

	fi

	echo ""



	# Run the tests
	if [[ "${TRAVIS_PROJECT}" == "NanoTest" && "${TRAVIS_PLATFORM}" == "Linux" ]]; then
		./NanoTest/Project/NanoTest -d yes
	fi

done 



