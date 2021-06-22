/*	NAME:
		NCFData.cpp

	DESCRIPTION:
		CFDataRef wrapper.

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
#include "NCFData.h"





//=============================================================================
//		NCFData::GetData : Get the data.
//-----------------------------------------------------------------------------
NData NCFData::GetData(NCFSource theSource) const
{


	// Get the data
	CFDataRef cfData = *this;
	NData     theData;

	if (cfData != nullptr)
	{
		size_t      theSize = size_t(CFDataGetLength(cfData));
		const void* thePtr  = CFDataGetBytePtr(cfData);

		if (theSource == NCFSource::Copy)
		{
			theData.SetData(theSize, thePtr, NDataSource::Copy);
		}
		else
		{
			theData.SetData(theSize, thePtr, NDataSource::View);
		}
	}

	return theData;
}





//=============================================================================
//		NCFData::SetData : Set the data.
//-----------------------------------------------------------------------------
bool NCFData::SetData(const NData& theData, NCFSource theSource)
{


	// Set the data
	CFIndex      theSize = CFIndex(theData.GetSize());
	const UInt8* thePtr  = theData.GetData();
	bool         wasOK   = false;

	if (theSource == NCFSource::Copy)
	{
		wasOK = Set(CFDataCreate(kCFAllocatorDefault, thePtr, theSize));
	}
	else
	{
		wasOK = Set(
			CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, thePtr, theSize, kCFAllocatorNull));
	}

	return wasOK;
}



#pragma mark NCFMutableData





//=============================================================================
//		NCFMutableData::GetData : Get the data.
//-----------------------------------------------------------------------------
NData NCFMutableData::GetData(NCFSource theSource) const
{


	// Get the data
	//
	// A CFMutableDataRef can always be cast to a CFDataRef.
	NCFData cfData;

	cfData.Set(CFDataRef(CFRetain(*this)));
	NN_REQUIRE(IsValid() == cfData.IsValid());

	return cfData.GetData(theSource);
}





//=============================================================================
//		NCFMutableData::SetData : Set the data.
//-----------------------------------------------------------------------------
bool NCFMutableData::SetData(const NData& theData)
{


	// Set the data
	NCFData cfData;

	cfData.SetData(theData, NCFSource::View);

	return Set(CFDataCreateMutableCopy(kCFAllocatorDefault, 0, cfData));
}
