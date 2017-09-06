#include "Stable.h"
#include "Script.h"
#include "Utils/FileReader.h"
#include "Utils/UnitTest.h"

int main(int argc, char *argv[]) {
#if UNIT_TEST == 1
	UnitTest::RunAndDisposeTests();

#elif DEBUG == 1
	std::string srcs[] = { "逻辑运算测试用例.txt", "in函数测试用例.txt", "out函数测试用例.txt", "赋值语句测试用例.txt", "四则运算表达式测试用例.txt", "字符串操作测试用例.txt" };
	for (int i = 0; i != 6; ++i) {
		FileReader reader{ "TestCases/" + srcs[i] };
		std::string source = reader.readAll();
		Script script = Script::Compile(source);
		script.Run();
	}

#else
	if (argc == 1) {
		return 0;
	}
	const char *fileName = argv[1];
	DBG_PRINT << "fileName: " << fileName << '\n';

	FileReader reader{ fileName };
	std::string source = reader.readAll();
	DBG_PRINT << "source:\n" << source;

	Script script = Script::Compile(source);
	script.Run();

#endif // UNIT_TEST == 1
	system("pause");
	return 0;
}
