#pragma once

#include <cassert>

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
	void TESTCASE_##NAME();													\
	class TEST_CASE_RUNNER_##NAME {											\
	public:																	\
		TEST_CASE_RUNNER_##NAME() {											\
			UnitTest::PushTest(&TEST_CASE_RUNNER_##NAME::RunTestUnit);		\
		}																	\
		static void RunTestUnit() {											\
			UnitTest::PrintMessage("TEST_CASE_"#NAME"_START");				\
			TESTCASE_##NAME();												\
			UnitTest::PrintMessage("TEST_CASE_"#NAME"_END\n");				\
		}																	\
	} TEST_CASE_RUNNER_##NAME##_INSTANCE;									\
	void TESTCASE_##NAME()													

#define ASSERT_EQ(lhs, rhs)													\
	assert(lhs == rhs)
#define ASSERT_NE(lhs, rhs)													\
	assert(lhs != rhs)
#define ASSERT_LT(lhs, rhs)													\
	assert(lhs < rhs)
#define ASSERT_GT(lhs, rhs)													\
	assert(lhs > rhs)
#define ASSERT_LTE(lhs, rhs)												\
	assert(lhs <= rhs)
#define ASSERT_GTE(lhs, rhs)												\
	assert(lhs >= rhs)