#!/bin/sh



# Get the arguments
TRAVIS_PROJECT="$1"
TRAVIS_PLATFORM="$2"



# Parse the platform
mkdir -p Build
cd       Build

if [ "${TRAVIS_PLATFORM}" == "Android" ]; then
	wget https://dl.google.com/android/repository/android-ndk-r20b-linux-x86_64.zip -O android-ndk-r20b-linux-x86_64.zip
	unzip -q android-ndk-r20b-linux-x86_64.zip

	CMAKE_FLAGS=-DANDROID_ABI=arm64-v8a -DANDROID_NATIVE_API_LEVEL=26 -DCMAKE_TOOLCHAIN_FILE="./android-ndk-r20b/build/cmake/android.toolchain.cmake"

elif [ "${TRAVIS_PLATFORM}" == "Linux" ]; then
	CMAKE_FLAGS=""

else
	echo "Unknown platform: ${TRAVIS_PLATFORM}"
	exit -1
fi



# Do the builds
for BUILD_CONFIG in "Debug" "Release"; do

	JOB_NAME="${TRAVIS_PROJECT}_${TRAVIS_PLATFORM} - ${BUILD_CONFIG}"

	echo "${JOB_NAME}"
	printf -v _hr "%*s" ${#JOB_NAME} && echo ${_hr// /=}

	cmake ${CMAKE_FLAGS} -DCMAKE_BUILD_TYPE="${BUILD_CONFIG}" ..
	echo ""

	make -j3 "${TRAVIS_PROJECT}"
	echo ""

done 



# Run the tests
if [ "${TRAVIS_PROJECT}" == "NanoTest" ]; then
	find . -name "NanoTest"
fi



