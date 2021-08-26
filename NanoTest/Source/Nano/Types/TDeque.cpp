/*	NAME:
		TDeque.cpp

	DESCRIPTION:
		NDeque tests.

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
#include "NDeque.h"
#include "NStdAlgorithm.h"
#include "NString.h"
#include "NTestFixture.h"
#include "NThread.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr size_t kNSizeSmall                         = 20;
static constexpr size_t kNSizeLarge                         = 2000;





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
struct TestAllocated
{
	NString value1;
	NString value2;
	NString value3;
};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TDeque)
{
	NDeque<size_t> theDeque;
	size_t         n;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "Default")
{
	// Perform the test
	REQUIRE(theDeque.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "GetSize/Small")
{


	// Perform the test
	for (n = 0; n < kNSizeSmall; n++)
	{
		theDeque.EmplaceFront(n);
	}

	REQUIRE(theDeque.GetSize() == kNSizeSmall);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "GetSize/Large")
{


	// Perform the test
	for (n = 0; n < kNSizeLarge; n++)
	{
		theDeque.EmplaceFront(n);
	}

	REQUIRE(theDeque.GetSize() == kNSizeLarge);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "Purge/Small")
{


	// Perform the test
	NDeque<TestAllocated> allocQueue;

	for (n = 0; n < kNSizeSmall; n++)
	{
		allocQueue.EmplaceFront(TestAllocated{});
	}

	allocQueue.Purge();
	REQUIRE(allocQueue.GetSize() == kNSizeSmall);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "Purge/Large")
{


	// Perform the test
	NDeque<TestAllocated> allocQueue;

	for (n = 0; n < kNSizeLarge; n++)
	{
		allocQueue.EmplaceFront(TestAllocated{});
	}

	allocQueue.Purge();
	REQUIRE(allocQueue.GetSize() == kNSizeLarge);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "EmplaceFront/Small")
{


	// Perform the test
	for (n = 0; n < kNSizeSmall; n++)
	{
		theDeque.EmplaceFront(n);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "EmplaceFront/Large")
{


	// Perform the test
	for (n = 0; n < kNSizeLarge; n++)
	{
		theDeque.EmplaceFront(n);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "PopFront/Small")
{


	// Perform the test
	for (n = 0; n < kNSizeSmall; n++)
	{
		theDeque.EmplaceFront(n);
	}

	REQUIRE(theDeque.GetSize() == kNSizeSmall);

	for (n = 0; n < kNSizeSmall; n++)
	{
		theDeque.PopFront();
	}

	REQUIRE(theDeque.GetSize() == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "PopFront/Large")
{


	// Perform the test
	for (n = 0; n < kNSizeLarge; n++)
	{
		theDeque.EmplaceFront(n);
	}

	REQUIRE(theDeque.GetSize() == kNSizeLarge);

	for (n = 0; n < kNSizeLarge; n++)
	{
		theDeque.PopFront();
	}

	REQUIRE(theDeque.GetSize() == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "PopBack/Small")
{


	// Perform the test
	for (n = 0; n < kNSizeSmall; n++)
	{
		theDeque.EmplaceFront(n);
	}

	REQUIRE(theDeque.GetSize() == kNSizeSmall);

	for (n = 0; n < kNSizeSmall; n++)
	{
		auto theItem = theDeque.PopBack();
		REQUIRE(theItem.has_value());
		REQUIRE(theItem.value() == n);
	}

	REQUIRE(theDeque.GetSize() == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "PopBack/Large")
{


	// Perform the test
	for (n = 0; n < kNSizeLarge; n++)
	{
		theDeque.EmplaceFront(n);
	}

	REQUIRE(theDeque.GetSize() == kNSizeLarge);

	for (n = 0; n < kNSizeLarge; n++)
	{
		auto theItem = theDeque.PopBack();
		REQUIRE(theItem.has_value());
		REQUIRE(theItem.value() == n);
	}

	REQUIRE(theDeque.GetSize() == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "ProducerConsumer")
{


	// Consumer
	NVectorSize theOutput;

	NUniqueThread theConsumer = NThread::Create("TDeque_ProducerConsumer",
	[&]()
	{
		while (theOutput.size() != kNSizeLarge)
		{
			auto theItem = theDeque.PopBack();
			if (theItem.has_value())
			{
				theOutput.push_back(theItem.value());
			}
			else
			{
				NThread::Sleep(0.010);
			}
		}
	});



	// Producer
	for (n = 0; n < kNSizeLarge; n++)
	{
		theDeque.EmplaceFront(n);
	}

	theConsumer->WaitForCompletion();



	// Perform the test
	REQUIRE(theOutput.size() == kNSizeLarge);
	nstd::sort(theOutput);

	for (n = 0; n < kNSizeLarge; n++)
	{
		REQUIRE(theOutput[n] == n);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "ProducerConsumers")
{


	// Consumer
	static constexpr size_t kNumConsumers  = 10;
	static constexpr size_t kNSizeConsumer = kNSizeLarge;
	static constexpr size_t kNSizeOutput   = kNSizeConsumer * kNumConsumers;

	NVectorSize                theOutputs[kNumConsumers];
	std::vector<NUniqueThread> theConsumers;

	for (n = 0; n < kNumConsumers; n++)
	{
		theConsumers.emplace_back(
			NThread::Create("TDeque::ProducerConsumers",
			[&theInput = theDeque, &theOutput = theOutputs[n]]()
			{
				while (theOutput.size() != kNSizeConsumer)
				{
					auto theItem = theInput.PopBack();
					if (theItem.has_value())
					{
						theOutput.push_back(theItem.value());
					}
					else
					{
						NThread::Sleep(0.010);
					}
				}
			}));
	}



	// Producer
	for (n = 0; n < kNSizeOutput; n++)
	{
		theDeque.EmplaceFront(n);
	}

	for (n = 0; n < kNumConsumers; n++)
	{
		theConsumers[n]->WaitForCompletion();
	}



	// Perform the test
	NVectorSize theOutput;

	for (n = 0; n < kNumConsumers; n++)
	{
		nstd::push_back(theOutput, theOutputs[n]);
	}

	REQUIRE(theOutput.size() == kNSizeOutput);
	nstd::sort(theOutput);

	for (n = 0; n < kNSizeOutput; n++)
	{
		REQUIRE(theOutput[n] == n);
	}
}





#pragma mark NMixinContainer
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDeque, "IsEmpty")
{


	// Perform the test
	REQUIRE(theDeque.IsEmpty());

	for (n = 0; n < kNSizeSmall; n++)
	{
		theDeque.EmplaceFront(n);
	}

	REQUIRE(!theDeque.IsEmpty());
}
