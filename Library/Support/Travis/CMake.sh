#!/bin/bash



# Get the arguments
set -euo pipefail

TRAVIS_PROJECT="$1"
TRAVIS_PLATFORM="$2"




# Prepare to build
mkdir -p "${TRAVIS_BUILD_DIR}/Build"
cd       "${TRAVIS_BUILD_DIR}/Build"

if [[ "${TRAVIS_PLATFORM}" == "Android" ]]; then

	wget https://dl.google.com/android/repository/android-ndk-r20b-linux-x86_64.zip -O android-ndk-r20b-linux-x86_64.zip
	unzip -q android-ndk-r20b-linux-x86_64.zip

	CMAKE_URL="https://github.com/Kitware/CMake/releases/download/v3.17.1/cmake-3.17.1-Linux-x86_64.tar.gz"
	wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C "${TRAVIS_BUILD_DIR}/Build/cmake"
	export PATH="${TRAVIS_BUILD_DIR}/Build/cmake/bin:${PATH}"

	CMAKE_GENERATOR="Unix Makefiles"
	CMAKE_PARAMS="-DANDROID_ABI=arm64-v8a -DANDROID_NATIVE_API_LEVEL=26 -DCMAKE_TOOLCHAIN_FILE=${TRAVIS_BUILD_DIR}/Build/android-ndk-r20b/build/cmake/android.toolchain.cmake"

elif [[ "${TRAVIS_PLATFORM}" == "Linux" ]]; then

	CMAKE_URL="https://github.com/Kitware/CMake/releases/download/v3.17.1/cmake-3.17.1-Linux-x86_64.tar.gz"
	wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C "${TRAVIS_BUILD_DIR}/Build/cmake"
	export PATH="${TRAVIS_BUILD_DIR}/Build/cmake/bin:${PATH}"

	CMAKE_GENERATOR="Unix Makefiles"
	CMAKE_PARAMS=""

elif [[ "${TRAVIS_PLATFORM}" == "Windows" ]]; then

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
	cmake --version
	cmake -G "${CMAKE_GENERATOR}" ${CMAKE_PARAMS} -DCMAKE_BUILD_TYPE="${BUILD_CONFIG}" "${TRAVIS_BUILD_DIR}"
	echo ""

	if [[ "${TRAVIS_PLATFORM}" != "Windows" ]]; then
		make -j3 "${TRAVIS_PROJECT}"
	else
		cmake --build . --parallel --config "${BUILD_CONFIG}" --target "${TRAVIS_PROJECT}"
	fi

	echo ""



	# Run the tests
	if [[ "${TRAVIS_PROJECT}" == "NanoTest" ]]; then

		if [[ "${TRAVIS_PLATFORM}" == "Linux" ]]; then
			./NanoTest/Project/NanoTest -d yes

		elif [[ "${TRAVIS_PLATFORM}" == "Windows" ]]; then
			"./NanoTest/Project/${BUILD_CONFIG}/NanoTest.exe" -d yes
		fi

	fi

done 



