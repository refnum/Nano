/*	NAME:
		TMixinHashable.cpp

	DESCRIPTION:
		NMixinHashable tests.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
// Nano
#include "NMixinHashable.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
class TestHashable : public NMixinHashable<TestHashable>
{
public:
	TestHashable()
		: mHash(kNHashNone)
	{
	}

	bool HasHash() const
	{
		return mHash != kNHashNone;
	}

	size_t& FetchHash(bool updateHash) const
	{
		if (updateHash)
		{
			mHash = 1234;
		}

		return mHash;
	}

private:
	mutable size_t                      mHash;
};





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TMixinHashable)
{
	TestHashable                        theObject;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TMixinHashable, "Default")
{
	// Perform the test
										REQUIRE(!theObject.HasHash());

	size_t theHash = theObject.GetHash();
										REQUIRE(theHash != kNHashNone);
										REQUIRE(theObject.HasHash());

	theObject.ClearHash();
										REQUIRE(!theObject.HasHash());
}
