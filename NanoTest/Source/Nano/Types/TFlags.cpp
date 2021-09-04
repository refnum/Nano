/*	NAME:
		TFlags.cpp

	DESCRIPTION:
		NFlags tests.

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
#include "NFlags.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
#define NN_CHECK_INVALID                                    0





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
enum UnscopedValue
{
	kUnscopedValueA,
	kUnscopedValueB,
	kUnscopedValueC
};

NN_FLAG_ENUM(UnscopedValue, UnscopedValueFlags);

enum UnscopedBit
{
	kUnscopedBitA                                           = 1 << 3,
	kUnscopedBitB                                           = 1 << 5,
	kUnscopedBitC                                           = 1 << 7
};

NN_FLAG_ENUM_MASK(UnscopedBit, UnscopedBitFlags);

enum class ScopedValue
{
	A,
	B,
	C
};

NN_FLAG_ENUM(ScopedValue, ScopedValueFlags);

enum class ScopedBit
{
	A                                                       = 1 << 3,
	B                                                       = 1 << 5,
	C                                                       = 1 << 7,
};

NN_FLAG_ENUM_MASK(ScopedBit, ScopedBitFlags);





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFlags){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Constructor")
{
	// Perform the test
	UnscopedValueFlags flagsUV1(kUnscopedValueA);
	UnscopedBitFlags   flagsUB1(kUnscopedBitA);
	ScopedValueFlags   flagsSV1(ScopedValue::A);
	ScopedBitFlags     flagsSB1(ScopedBit::A);

	UnscopedValueFlags flagsUV2;
	UnscopedBitFlags   flagsUB2;
	ScopedValueFlags   flagsSV2;
	ScopedBitFlags     flagsSB2;

	REQUIRE(flagsUV1.IsSet(kUnscopedValueA));
	REQUIRE(flagsUB1.IsSet(kUnscopedBitA));
	REQUIRE(flagsSV1.IsSet(ScopedValue::A));
	REQUIRE(flagsSB1.IsSet(ScopedBit::A));

	REQUIRE(!flagsUV2.IsSet(kUnscopedValueA));
	REQUIRE(!flagsUB2.IsSet(kUnscopedBitA));
	REQUIRE(!flagsSV2.IsSet(ScopedValue::A));
	REQUIRE(!flagsSB2.IsSet(ScopedBit::A));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Clear/All")
{


	// Perform the test
	UnscopedValueFlags flagsUV(kUnscopedValueA);
	UnscopedBitFlags   flagsUB(kUnscopedBitA);
	ScopedValueFlags   flagsSV(ScopedValue::A);
	ScopedBitFlags     flagsSB(ScopedBit::A);

	REQUIRE(flagsUV.IsSet(kUnscopedValueA));
	REQUIRE(flagsUB.IsSet(kUnscopedBitA));
	REQUIRE(flagsSV.IsSet(ScopedValue::A));
	REQUIRE(flagsSB.IsSet(ScopedBit::A));

	flagsUV.Clear();
	flagsUB.Clear();
	flagsSV.Clear();
	flagsSB.Clear();

	REQUIRE(!flagsUV.IsSet(kUnscopedValueA));
	REQUIRE(!flagsUB.IsSet(kUnscopedBitA));
	REQUIRE(!flagsSV.IsSet(ScopedValue::A));
	REQUIRE(!flagsSB.IsSet(ScopedBit::A));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Clear/Bit")
{


	// Perform the test
	UnscopedValueFlags flagsUV(kUnscopedValueA);
	UnscopedBitFlags   flagsUB(kUnscopedBitA);
	ScopedValueFlags   flagsSV(ScopedValue::A);
	ScopedBitFlags     flagsSB(ScopedBit::A);

	REQUIRE(flagsUV.IsSet(kUnscopedValueA));
	REQUIRE(flagsUB.IsSet(kUnscopedBitA));
	REQUIRE(flagsSV.IsSet(ScopedValue::A));
	REQUIRE(flagsSB.IsSet(ScopedBit::A));

	flagsUV.Clear(kUnscopedValueA);
	flagsUB.Clear(kUnscopedBitA);
	flagsSV.Clear(ScopedValue::A);
	flagsSB.Clear(ScopedBit::A);

	REQUIRE(!flagsUV.IsSet(kUnscopedValueA));
	REQUIRE(!flagsUB.IsSet(kUnscopedBitA));
	REQUIRE(!flagsSV.IsSet(ScopedValue::A));
	REQUIRE(!flagsSB.IsSet(ScopedBit::A));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "IsSet")
{


	// Perform the test
	UnscopedValueFlags flagsUV(kUnscopedValueA);
	UnscopedBitFlags   flagsUB(kUnscopedBitA);
	ScopedValueFlags   flagsSV(ScopedValue::A);
	ScopedBitFlags     flagsSB(ScopedBit::A);

	REQUIRE(flagsUV.IsSet(kUnscopedValueA));
	REQUIRE(flagsUB.IsSet(kUnscopedBitA));
	REQUIRE(flagsSV.IsSet(ScopedValue::A));
	REQUIRE(flagsSB.IsSet(ScopedBit::A));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Set")
{


	// Perform the test
	UnscopedValueFlags flagsUV;
	UnscopedBitFlags   flagsUB;
	ScopedValueFlags   flagsSV;
	ScopedBitFlags     flagsSB;

	REQUIRE(!flagsUV.IsSet(kUnscopedValueA));
	REQUIRE(!flagsUB.IsSet(kUnscopedBitA));
	REQUIRE(!flagsSV.IsSet(ScopedValue::A));
	REQUIRE(!flagsSB.IsSet(ScopedBit::A));

	flagsUV.Set(kUnscopedValueA);
	flagsUB.Set(kUnscopedBitA);
	flagsSV.Set(ScopedValue::A);
	flagsSB.Set(ScopedBit::A);

	REQUIRE(flagsUV.IsSet(kUnscopedValueA));
	REQUIRE(flagsUB.IsSet(kUnscopedBitA));
	REQUIRE(flagsSV.IsSet(ScopedValue::A));
	REQUIRE(flagsSB.IsSet(ScopedBit::A));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Toggle")
{


	// Perform the test
	UnscopedValueFlags flagsUV;
	UnscopedBitFlags   flagsUB;
	ScopedValueFlags   flagsSV;
	ScopedBitFlags     flagsSB;

	REQUIRE(!flagsUV.IsSet(kUnscopedValueA));
	REQUIRE(!flagsUB.IsSet(kUnscopedBitA));
	REQUIRE(!flagsSV.IsSet(ScopedValue::A));
	REQUIRE(!flagsSB.IsSet(ScopedBit::A));

	flagsUV.Toggle(kUnscopedValueA);
	flagsUB.Toggle(kUnscopedBitA);
	flagsSV.Toggle(ScopedValue::A);
	flagsSB.Toggle(ScopedBit::A);

	REQUIRE(flagsUV.IsSet(kUnscopedValueA));
	REQUIRE(flagsUB.IsSet(kUnscopedBitA));
	REQUIRE(flagsSV.IsSet(ScopedValue::A));
	REQUIRE(flagsSB.IsSet(ScopedBit::A));

	flagsUV.Toggle(kUnscopedValueA);
	flagsUB.Toggle(kUnscopedBitA);
	flagsSV.Toggle(ScopedValue::A);
	flagsSB.Toggle(ScopedBit::A);

	REQUIRE(!flagsUV.IsSet(kUnscopedValueA));
	REQUIRE(!flagsUB.IsSet(kUnscopedBitA));
	REQUIRE(!flagsSV.IsSet(ScopedValue::A));
	REQUIRE(!flagsSB.IsSet(ScopedBit::A));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Operators/Not")
{


	// Perform the test
	UnscopedValueFlags flagsUV;
	UnscopedBitFlags   flagsUB;
	ScopedValueFlags   flagsSV;
	ScopedBitFlags     flagsSB;

	REQUIRE(~flagsUV);
	REQUIRE(~flagsUB);
	REQUIRE(~flagsSV);
	REQUIRE(~flagsSB);

	REQUIRE(!(~(~flagsUV)));
	REQUIRE(!(~(~flagsUB)));
	REQUIRE(!(~(~flagsSV)));
	REQUIRE(!(~(~flagsSB)));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Operators/FlagEnum")
{


	// Perform the test
	UnscopedValueFlags flagsUV;
	UnscopedBitFlags   flagsUB;
	ScopedValueFlags   flagsSV;
	ScopedBitFlags     flagsSB;

	REQUIRE(!(flagsUV & kUnscopedValueA));
	REQUIRE(!(flagsUB & kUnscopedBitA));
	REQUIRE(!(flagsSV & ScopedValue::A));
	REQUIRE(!(flagsSB & ScopedBit::A));

	REQUIRE((flagsUV | kUnscopedValueA));
	REQUIRE((flagsUB | kUnscopedBitA));
	REQUIRE((flagsSV | ScopedValue::A));
	REQUIRE((flagsSB | ScopedBit::A));

	REQUIRE((flagsUV ^ kUnscopedValueA));
	REQUIRE((flagsUB ^ kUnscopedBitA));
	REQUIRE((flagsSV ^ ScopedValue::A));
	REQUIRE((flagsSB ^ ScopedBit::A));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Operators/EnumFlag")
{


	// Perform the test
	UnscopedValueFlags flagsUV;
	UnscopedBitFlags   flagsUB;
	ScopedValueFlags   flagsSV;
	ScopedBitFlags     flagsSB;

	REQUIRE(!(kUnscopedValueA & flagsUV));
	REQUIRE(!(kUnscopedBitA & flagsUB));
	REQUIRE(!(ScopedValue::A & flagsSV));
	REQUIRE(!(ScopedBit::A & flagsSB));

	REQUIRE((kUnscopedValueA | flagsUV));
	REQUIRE((kUnscopedBitA | flagsUB));
	REQUIRE((ScopedValue::A | flagsSV));
	REQUIRE((ScopedBit::A | flagsSB));

	REQUIRE((kUnscopedValueA ^ flagsUV));
	REQUIRE((kUnscopedBitA ^ flagsUB));
	REQUIRE((ScopedValue::A ^ flagsSV));
	REQUIRE((ScopedBit::A ^ flagsSB));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Operators/EnumEnum")
{


	// Perform the test
	UnscopedValueFlags flagsUV;
	UnscopedBitFlags   flagsUB;
	ScopedValueFlags   flagsSV;
	ScopedBitFlags     flagsSB;

	flagsUV = kUnscopedValueA | kUnscopedValueB;
	flagsUB = kUnscopedBitA | kUnscopedBitB;
	flagsSV = ScopedValue::A | ScopedValue::B;
	flagsSB = ScopedBit::A | ScopedBit::B;

	REQUIRE((flagsUV.IsSet(kUnscopedValueA) && flagsUV.IsSet(kUnscopedValueB)));
	REQUIRE((flagsUB.IsSet(kUnscopedBitA) && flagsUB.IsSet(kUnscopedBitB)));
	REQUIRE((flagsSV.IsSet(ScopedValue::A) && flagsSV.IsSet(ScopedValue::B)));
	REQUIRE((flagsSB.IsSet(ScopedBit::A) && flagsSB.IsSet(ScopedBit::B)));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Operators/FlagFlag")
{


	// Perform the test
	UnscopedValueFlags flagsUV;
	UnscopedBitFlags   flagsUB;
	ScopedValueFlags   flagsSV;
	ScopedBitFlags     flagsSB;

	REQUIRE(!(flagsUV & UnscopedValueFlags(kUnscopedValueA)));
	REQUIRE(!(flagsUB & UnscopedBitFlags(kUnscopedBitA)));
	REQUIRE(!(flagsSV & ScopedValueFlags(ScopedValue::A)));
	REQUIRE(!(flagsSB & ScopedBitFlags(ScopedBit::A)));

	REQUIRE((flagsUV | UnscopedValueFlags(kUnscopedValueA)));
	REQUIRE((flagsUB | UnscopedBitFlags(kUnscopedBitA)));
	REQUIRE((flagsSV | ScopedValueFlags(ScopedValue::A)));
	REQUIRE((flagsSB | ScopedBitFlags(ScopedBit::A)));

	REQUIRE((flagsUV ^ UnscopedValueFlags(kUnscopedValueA)));
	REQUIRE((flagsUB ^ UnscopedBitFlags(kUnscopedBitA)));
	REQUIRE((flagsSV ^ ScopedValueFlags(ScopedValue::A)));
	REQUIRE((flagsSB ^ ScopedBitFlags(ScopedBit::A)));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Assignment/Enum")
{


	// Perform the test
	UnscopedValueFlags flagsUV;
	UnscopedBitFlags   flagsUB;
	ScopedValueFlags   flagsSV;
	ScopedBitFlags     flagsSB;

	flagsUV &= kUnscopedValueA;
	flagsUB &= kUnscopedBitA;
	flagsSV &= ScopedValue::A;
	flagsSB &= ScopedBit::A;

	REQUIRE(!(flagsUV & kUnscopedValueA));
	REQUIRE(!(flagsUB & kUnscopedBitA));
	REQUIRE(!(flagsSV & ScopedValue::A));
	REQUIRE(!(flagsSB & ScopedBit::A));

	flagsUV |= kUnscopedValueA;
	flagsUB |= kUnscopedBitA;
	flagsSV |= ScopedValue::A;
	flagsSB |= ScopedBit::A;

	REQUIRE((flagsUV & kUnscopedValueA));
	REQUIRE((flagsUB & kUnscopedBitA));
	REQUIRE((flagsSV & ScopedValue::A));
	REQUIRE((flagsSB & ScopedBit::A));

	flagsUV ^= kUnscopedValueA;
	flagsUB ^= kUnscopedBitA;
	flagsSV ^= ScopedValue::A;
	flagsSB ^= ScopedBit::A;

	REQUIRE(!(flagsUV & kUnscopedValueA));
	REQUIRE(!(flagsUB & kUnscopedBitA));
	REQUIRE(!(flagsSV & ScopedValue::A));
	REQUIRE(!(flagsSB & ScopedBit::A));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Assignment/Flags")
{


	// Perform the test
	UnscopedValueFlags flagsUV;
	UnscopedBitFlags   flagsUB;
	ScopedValueFlags   flagsSV;
	ScopedBitFlags     flagsSB;

	flagsUV &= UnscopedValueFlags(kUnscopedValueA);
	flagsUB &= UnscopedBitFlags(kUnscopedBitA);
	flagsSV &= ScopedValueFlags(ScopedValue::A);
	flagsSB &= ScopedBitFlags(ScopedBit::A);

	REQUIRE(!(flagsUV & UnscopedValueFlags(kUnscopedValueA)));
	REQUIRE(!(flagsUB & UnscopedBitFlags(kUnscopedBitA)));
	REQUIRE(!(flagsSV & ScopedValueFlags(ScopedValue::A)));
	REQUIRE(!(flagsSB & ScopedBitFlags(ScopedBit::A)));

	flagsUV |= UnscopedValueFlags(kUnscopedValueA);
	flagsUB |= UnscopedBitFlags(kUnscopedBitA);
	flagsSV |= ScopedValueFlags(ScopedValue::A);
	flagsSB |= ScopedBitFlags(ScopedBit::A);

	REQUIRE((flagsUV & UnscopedValueFlags(kUnscopedValueA)));
	REQUIRE((flagsUB & UnscopedBitFlags(kUnscopedBitA)));
	REQUIRE((flagsSV & ScopedValueFlags(ScopedValue::A)));
	REQUIRE((flagsSB & ScopedBitFlags(ScopedBit::A)));

	flagsUV ^= UnscopedValueFlags(kUnscopedValueA);
	flagsUB ^= UnscopedBitFlags(kUnscopedBitA);
	flagsSV ^= ScopedValueFlags(ScopedValue::A);
	flagsSB ^= ScopedBitFlags(ScopedBit::A);

	REQUIRE(!(flagsUV & UnscopedValueFlags(kUnscopedValueA)));
	REQUIRE(!(flagsUB & UnscopedBitFlags(kUnscopedBitA)));
	REQUIRE(!(flagsSV & ScopedValueFlags(ScopedValue::A)));
	REQUIRE(!(flagsSB & ScopedBitFlags(ScopedBit::A)));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Cast")
{


	// Perform the test
	UnscopedValueFlags flagsUV(kUnscopedValueA);
	UnscopedBitFlags   flagsUB(kUnscopedBitA);
	ScopedValueFlags   flagsSV(ScopedValue::A);
	ScopedBitFlags     flagsSB(ScopedBit::A);

	REQUIRE(flagsUV);
	REQUIRE(flagsUB);
	REQUIRE(flagsSV);
	REQUIRE(flagsSB);

	flagsUV.Clear();
	flagsUB.Clear();
	flagsSV.Clear();
	flagsSB.Clear();

	REQUIRE(!flagsUV);
	REQUIRE(!flagsUB);
	REQUIRE(!flagsSV);
	REQUIRE(!flagsSB);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFlags, "Invalid")
{


	// Perform the test
#if NN_CHECK_INVALID
	UnscopedValueFlags flagsUV;
	UnscopedBitFlags   flagsUB;
	ScopedValueFlags   flagsSV;
	ScopedBitFlags     flagsSB;

	flagsUV = kUnscopedValueA & kUnscopedValueB;    // Not allowed (always 0)
	flagsUB = kUnscopedBitA & kUnscopedBitB;        // Not allowed (always 0)
	flagsSV = ScopedValue::A & ScopedValue::B;      // Not allowed (always 0)
	flagsSB = ScopedBit::A & ScopedBit::B;          // Not allowed (always 0)

	flagsUV = kUnscopedValueA ^ kUnscopedValueB;    // Not allowed (always 0)
	flagsUB = kUnscopedBitA ^ kUnscopedBitB;        // Not allowed (always 0)
	flagsSV = ScopedValue::A ^ ScopedValue::B;      // Not allowed (always 0)
	flagsSB = ScopedBit::A ^ ScopedBit::B;          // Not allowed (always 0)

	(void) (flagsUV == kUnscopedValueA);    // Not allowed (but can't prevent if 0, as 0==false)
	(void) (flagsUV == kUnscopedValueB);    // Not allowed (but can't prevent if 1, as !==true)
	(void) (flagsUV == kUnscopedValueC);    // Not allowed (test bits with &, compare flags with ==)

	(void) (flagsUB == kUnscopedBitA);     // Not allowed (test bits with &, compare flags with ==)
	(void) (flagsSV == ScopedValue::A);    // Not allowed (test bits with &, compare flags with ==)
	(void) (flagsSB == ScopedBit::A);      // Not allowed (test bits with &, compare flags with ==)
#endif // NN_CHECK_INVALID
}
