/*	NAME:
		NTestFixture.h

	DESCRIPTION:
		Nano test fixture for catch.
		
		Catch is a header-only library, distributed under the boost licence.

		It has been included directly with this Nanite as this is compatible
		with the Nano licence.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTESTFIXTURE_HDR
#define NTESTFIXTURE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "catch.hpp"

#include "NString.h"





//============================================================================
//		Catch
//----------------------------------------------------------------------------
// Classes
namespace Catch {
	class Fixture {
	public:
		         Fixture(void) { }
		virtual ~Fixture(void) { }
		
		// TestCaseInfo::invoke could pass the TestCaseInfo in to the test
		// invoker to allow it to obtain info about the current test.
		virtual void setCurrentTest(const std::string &/*testName*/ ) { }

		// ITestCase could declare pre/post setup/teardown methods allowing
		// SETUP/TEARDOWN macros that do not require the class name.
		virtual void willSetUp(void) { }
		virtual void     setUp(void) { }
		virtual void  didSetUp(void) { }

		virtual void willTearDown(void) { }
		virtual void     tearDown(void) { }
		virtual void  didTearDown(void) { }
	};
}


// Macros
#define TEST_CASE_FIXTURE( ClassName, TestName, ... )\
	namespace{ \
		struct INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ ) : ClassName{ \
			         INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )(void) { } \
			virtual ~INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )(void) { } \
			\
            void invokeFixture(); \
			void test(); \
		}; \
		Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME( autoRegistrar ) ( &INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )::invokeFixture, #ClassName, Catch::NameAndDesc( TestName, ##__VA_ARGS__ ), CATCH_INTERNAL_LINEINFO ); \
	} \
	\
	void INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )::invokeFixture() \
	{ \
		setCurrentTest(TestName); \
		\
		willSetUp(); \
		setUp(); \
		didSetUp(); \
		\
		test(); \
		\
		willTearDown(); \
		tearDown(); \
		didTearDown(); \
	} \
	\
	void INTERNAL_CATCH_UNIQUE_NAME( ____C_A_T_C_H____T_E_S_T____ )::test()

#define SETUP														virtual void setUp(void)

#define TEARDOWN													virtual void tearDown(void)





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Fixture macros
//
// A fixture derived from NTestFixture can be defined with:
//
//		FIXTURE_NANO(SomeFixture)
//		{
//			UInt8 *someData;
//
//			SETUP
//			{
//				someData = NULL;
//			}
//
//			TEARDOWN
//			{
//				if (someData != NULL)
//					free(someData);
//			}
//		}
//
// Both SETUP and TEARDOWN sections are optional.
//
//
// A test that uses this fixture can be defined with:
//
//		TEST_NANO(SomeFixture, "Allocate")
//		{
//			someData = (UInt8 *) malloc(10);
//			REQUIRE(someData != NULL);
//		}
//
// This will create a "Nano/SomeFixture/Allocate" test, based on SomeFixture.
//
//
// A list of []-delimited tags can also be provided:
//
//		TEST_NANO(SomeFixture, "Allocate", "[malloc][mem]")
//		{
//			someData = (UInt8 *) malloc(10);
//			REQUIRE(someData != NULL);
//		}
//
// Tags may be used to group tests for execution by CATCH.
#define FIXTURE_NANO(_class)										struct _class : public NTestFixture
#define TEST_NANO(_class, ...)										TEST_CASE_FIXTURE(_class, "Nano/" #_class "/" __VA_ARGS__)


// Assertions
#define REQUIRE_NOERR(_err)											REQUIRE((_err) == kNoErr)





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTestFixture : public Catch::Fixture {
public:
										NTestFixture(void);
	virtual							   ~NTestFixture(void);


	// Get the current test ID
	//
	// The test ID is a /-separated unique identifier for the test.
	NString								GetTestID(void) const;


	// Reset the time
	void								ResetTime(void);


	// Get the elapsed time
	NTime								GetElapsedTime(void) const;


	// Test the time
	bool								TimeUnder(NTime theTime) const;
	bool								TimeOver( NTime theTime) const;


protected:
	// Pre/post setup/teardown hooks
	//
	// Although NFixture can be used directly, it may also be sub-classed by a
	// class that acts as a common base class for other fixtures.
	//
	// These sub-classes can override these methods to obtain a hook before and
	// after the leaf fixtures invoke SETUP and TEARDOWN.
	virtual void						WillSetUp(void);
	virtual void						DidSetUp( void);


	// Pre/post teardown hook
	virtual void						WillTearDown(void);
	virtual void						DidTearDown( void);


protected:
	void								setCurrentTest(const std::string &testName);

	void								willSetUp(void);
	void								didSetUp( void);

	void								willTearDown(void);
	void								didTearDown( void);


private:
	NString								mID;
	NTime								mTimeStart;
};





#endif // NTESTFIXTURE_HDR

