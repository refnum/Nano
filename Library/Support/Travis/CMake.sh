#!/bin/bash



# Get the arguments
TRAVIS_PROJECT="$1"
TRAVIS_PLATFORM="$2"



# Prepare to build
set -euo pipefail

mkdir -p Build
cd       Build



# Parse the platform
if [ "${TRAVIS_PLATFORM}" == "Android" ]; then
	wget https://dl.google.com/android/repository/android-ndk-r20b-linux-x86_64.zip -O android-ndk-r20b-linux-x86_64.zip
	unzip -q android-ndk-r20b-linux-x86_64.zip

	CMAKE_FLAGS="-DANDROID_ABI=arm64-v8a -DANDROID_NATIVE_API_LEVEL=26 -DCMAKE_TOOLCHAIN_FILE=\"${HOME}/Build/android-ndk-r20b/build/cmake/android.toolchain.cmake\""

elif [ "${TRAVIS_PLATFORM}" == "Linux" ]; then
	CMAKE_FLAGS=""

else
	echo "Unknown platform: ${TRAVIS_PLATFORM}"
	exit -1
fi



# Do the builds
for BUILD_CONFIG in "Debug" "Release"; do

	mkdir "${BUILD_CONFIG}"
	pushd "${BUILD_CONFIG}"

		JOB_NAME="${TRAVIS_PROJECT}_${TRAVIS_PLATFORM} - ${BUILD_CONFIG}"

		echo "${JOB_NAME}"
		printf -v _hr "%*s" ${#JOB_NAME} && echo ${_hr// /=}


		cmake ${CMAKE_FLAGS} -DCMAKE_BUILD_TYPE="${BUILD_CONFIG}" "${HOME}"
		echo ""

		make -j3 "${TRAVIS_PROJECT}"
		echo ""
	
	popd

done 



# Run the tests
if [ "${TRAVIS_PROJECT}" == "NanoTest" ]; then
	find . -name "NanoTest"
fi



