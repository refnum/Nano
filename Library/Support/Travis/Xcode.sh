#!/bin/bash



# Get the arguments
set -euo pipefail

TRAVIS_PROJECT="$1"
TRAVIS_PLATFORM="$2"



# Parse the project
if [[ "${TRAVIS_PROJECT}" == "Nano" ]]; then
	XCODE_PROJECT="Library/Project/Nano.xcodeproj"

elif [[ "${TRAVIS_PROJECT}" == "NanoTest" ]]; then
	XCODE_PROJECT="NanoTest/Project/NanoTest.xcodeproj"

else
	echo "Unknown project: '${TRAVIS_PROJECT}'"
	exit -1
fi



# Parse the platform
if [[ "${TRAVIS_PLATFORM}" == "macOS" ]]; then
	XCODE_DESTINATION="arch=x86_64"

elif [[ "${TRAVIS_PLATFORM}" == "iOS" ]]; then
	XCODE_DESTINATION="generic/platform=iOS"

elif [[ "${TRAVIS_PLATFORM}" == "tvOS" ]]; then
	XCODE_DESTINATION="generic/platform=tvOS"

else
	echo "Unknown platform: ${TRAVIS_PLATFORM}"
	exit -1
fi



# Do the builds
for BUILD_CONFIG in "Debug" "Release"; do

	# Prepare to build
	XCODE_SCHEME="${TRAVIS_PROJECT}_${TRAVIS_PLATFORM} - ${BUILD_CONFIG}"
	XCODE_PARAMS="-jobs 3 CODE_SIGNING_REQUIRED=NO Nano=${TRAVIS_BUILD_DIR}"


	echo "${XCODE_SCHEME}"
	printf -v _hr "%*s" ${#XCODE_SCHEME} && echo ${_hr// /=}


	# Perform the build
	xcodebuild build ${XCODE_PARAMS} -project "$XCODE_PROJECT" -scheme "$XCODE_SCHEME" -destination "$XCODE_DESTINATION" | xcpretty
	echo ""


	# Run the tests
	if [[ "${TRAVIS_PROJECT}" == "NanoTest" && "${TRAVIS_PLATFORM}" == "macOS" ]]; then
		./NanoTest/Project/NanoTest -d yes
	fi

done 
