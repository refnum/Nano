/*	NAME:
		NFileMap.cpp

	DESCRIPTION:
		Memory-mapped file.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFileMap.h"

// Nano
#include "NFile.h"
#include "NFileInfo.h"
#include "NScopedLock.h"
#include "NStdAlgorithm.h"





//=============================================================================
//		NFileMap::NFileMap : Constructor.
//-----------------------------------------------------------------------------
NFileMap::NFileMap()
	: mLock()
	, mPath()
	, mHandle(std::nullopt)
	, mFileSize(0)
{
}





//=============================================================================
//		NFileMap::~NFileMap : Destructor.
//-----------------------------------------------------------------------------
NFileMap::~NFileMap()
{


	// Clean up
	if (IsOpen())
	{
		Close();
	}
}





//=============================================================================
//		NFileMap::IsOpen : Is the file open?
//-----------------------------------------------------------------------------
bool NFileMap::IsOpen() const
{


	// Get our state
	NScopedLock acquireLock(mLock);

	return mHandle.has_value();
}





//=============================================================================
//		NFileMap::GetPath : Get the path.
//-----------------------------------------------------------------------------
NFilePath NFileMap::GetPath() const
{


	// Get the path
	NScopedLock acquireLock(mLock);

	return mPath;
}





//=============================================================================
//		NFileMap::Open : Open the file map.
//-----------------------------------------------------------------------------
NStatus NFileMap::Open(const NFile& theFile, NMapAccess theAccess)
{


	// Open the file mape
	return Open(theFile.GetPath(), theAccess);
}





//=============================================================================
//		NFileMap::Open : Open the file map.
//-----------------------------------------------------------------------------
NStatus NFileMap::Open(const NFilePath& thePath, NMapAccess theAccess)
{


	// Validate our state
	NN_REQUIRE(!IsOpen());



	// Check our parameters
	NFileInfo fileInfo(thePath);

	if (!fileInfo.IsFile())
	{
		return NStatus::NotFound;
	}



	// Open the file
	NScopedLock acquireLock(mLock);

	NN_REQUIRE(!mPath.IsValid());
	NN_REQUIRE(mMappings.empty());

	mHandle = MapOpen(thePath, theAccess);
	if (IsOpen())
	{
		mPath     = thePath;
		mFileSize = fileInfo.GetFileSize();

		return NStatus::OK;
	}

	return NStatus::Permission;
}





//=============================================================================
//		NFileMap::Close : Close the file map.
//-----------------------------------------------------------------------------
void NFileMap::Close()
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Close the file
	NScopedLock acquireLock(mLock);

	for (const auto& ptrMapping : mMappings)
	{
		MapDiscard(mHandle, ptrMapping.second);
	}

	MapClose(mHandle);



	// Reset our state
	mPath.Clear();
	mHandle.reset();
	mMappings.clear();
	mFileSize = 0;
}





//=============================================================================
//		NFileMap::Map : Map a page.
//-----------------------------------------------------------------------------
const void* NFileMap::Map(uint64_t theOffset, size_t theSize, NMapAccess theAccess)
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Get the state we need
	//
	// A size of 0 maps the entire file.
	//
	// The requested offset + size are expanded to cover the required
	// pages, then the size clamped to the maximum size of the file.
	//
	// Although some platforms will allow mapping beyond the end of
	// the file some require an exact trim.
	if (theSize == 0)
	{
		theSize = mFileSize;
	}

	size_t pageSize = MapPageSize();

	size_t byteStart = theOffset;
	size_t byteEnd   = theOffset + theSize;

	size_t mapStart = byteStart - (byteStart % pageSize);
	size_t mapEnd   = byteEnd - (byteEnd % pageSize) + pageSize;

	if (mapEnd > mFileSize)
	{
		mapEnd = mFileSize;
	}



	// Map the pages
	NScopedLock  acquireLock(mLock);
	NFileMapping theMapping{};

	theMapping.theOffset = mapStart;
	theMapping.theSize   = mapEnd - mapStart;
	theMapping.theAccess = theAccess;

	MapFetch(mHandle, theMapping);
	NN_EXPECT(theMapping.thePtr != nullptr);



	// Calculate the pointer
	//
	// The requested pointer is at some offset relative to the mapped page.
	const uint8_t* bytePtr = nullptr;

	if (theMapping.thePtr != nullptr)
	{
		NN_REQUIRE(!nstd::contains(mMappings, bytePtr));
		NN_REQUIRE(theMapping.thePtr != nullptr);

		bytePtr            = theMapping.thePtr + (byteStart - mapStart);
		mMappings[bytePtr] = theMapping;
	}

	return bytePtr;
}





//=============================================================================
//		NFileMap::Unmap : Unmap a page.
//-----------------------------------------------------------------------------
void NFileMap::Unmap(const void* thePtr)
{


	// Validate our parameters and state
	NN_REQUIRE(thePtr != nullptr);
	NN_REQUIRE(IsOpen());



	// Unmap the pages
	NScopedLock acquireLock(mLock);

	const uint8_t* bytePtr = static_cast<const uint8_t*>(thePtr);
	auto           theIter = mMappings.find(bytePtr);
	NN_REQUIRE(theIter != mMappings.end());

	MapDiscard(mHandle, theIter->second);
	mMappings.erase(theIter);
}
