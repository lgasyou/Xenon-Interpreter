#include "UnitTest.h"
#include <iostream>

struct UnitTestLink {
	UnitTest::TestProc testProc = nullptr;
	UnitTestLink *next = nullptr;
};

UnitTestLink *head = nullptr;
UnitTestLink **tail = &head;

void UnitTest::PrintMessage(const char *msg) {
	std::cout << msg << "\n";
}

void UnitTest::PrintInfo(const char *info) {
	std::cout << info << "\n";
}

void UnitTest::PushTest(TestProc testProc) {
	auto link = new UnitTestLink;
	link->testProc = testProc;
	*tail = link;
	tail = &link->next;
}

void UnitTest::RunAndDisposeTests() {
	auto current = head;
	head = nullptr;
	tail = nullptr;

	while (current) {
		current->testProc();
		auto t = current;
		current = current->next;
		delete t;
	}
}
