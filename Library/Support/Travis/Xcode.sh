#!/bin/bash



# Get the arguments
TRAVIS_PROJECT="$1"
TRAVIS_PLATFORM="$2"



# Prepare to build
set -euo pipefail



# Parse the project
if [ "${TRAVIS_PROJECT}" == "Nano" ]; then
	XCODE_PROJECT="Library/Project/Nano.xcodeproj"

elif [ "${TRAVIS_PROJECT}" == "NanoTest" ]; then
	XCODE_PROJECT="NanoTest/Project/NanoTest.xcodeproj"

else
	echo "Unknown project: '${TRAVIS_PROJECT}'"
	exit -1
fi



# Parse the platform
if [ "${TRAVIS_PLATFORM}" == "macOS" ]; then
	XCODE_DESTINATION="arch=x86_64"

elif [ "${TRAVIS_PLATFORM}" == "iOS" ]; then
	XCODE_DESTINATION="generic/platform=iOS"

elif [ "${TRAVIS_PLATFORM}" == "tvOS" ]; then
	XCODE_DESTINATION="generic/platform=tvOS"

else
	echo "Unknown platform: ${TRAVIS_PLATFORM}"
	exit -1
fi



# Do the builds
for BUILD_CONFIG in "Debug" "Release"; do

	XCODE_SCHEME="${TRAVIS_PROJECT}_${TRAVIS_PLATFORM} - ${BUILD_CONFIG}"

	echo "${XCODE_SCHEME}"
	printf -v _hr "%*s" ${#XCODE_SCHEME} && echo ${_hr// /=}

	xcodebuild build -jobs 3 -project "$XCODE_PROJECT" -scheme "$XCODE_SCHEME" -destination "$XCODE_DESTINATION" | xcpretty
	echo ""

done 



# Run the tests
if [ "${TRAVIS_PROJECT}" == "NanoTest" ]; then
	find . -name "NanoTest"
fi



