#pragma once

#include "UnitTest.h"

class UnitTest {
public:
	typedef void(*TestProc)();

	static void PrintMessage(const char *msg);

	static void PrintInfo(const char *info);

	static void PushTest(TestProc testProc);

	// Run all test cases.
	static void RunAndDisposeTests();

};

#define TEST_CASE(NAME)														\
	extern void TESTCASE_##NAME();											\
	class TEST_CASE_RUNNER_##NAME {											\
	public:																	\
		TEST_CASE_RUNNER_##NAME() {											\
			UnitTest::PushTest(&TEST_CASE_RUNNER_##NAME::RunTestUnit);		\
		}																	\
		static void RunTestUnit() {											\
			UnitTest::PrintMessage("TEST_CASE_"#NAME":");					\
			TESTCASE_##NAME();												\
		}																	\
	} TEST_CASE_RUNNER_##NAME_INSTANCE;										\
	void TESTCASE_##NAME()													