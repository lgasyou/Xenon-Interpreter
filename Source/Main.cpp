//#include "Stable.h"
//#include "Script.h"
//#include "Utils/FileReader.h"
//
//int main(int argc, char *argv[]) {
//#if DEBUG == 0
//	if (argc == 1) {
//		return 0;
//	}
//	const char *fileName = argv[1];
//#else
//	const char *fileName = "test.txt";
//#endif // DEBUG == 0
//	DBG_PRINT << "fileName: " << fileName << '\n';
//
//	FileReader reader{ fileName };
//	std::string source = reader.readAll();
//	DBG_PRINT << "source:\n" << source;
//
//	Script script = Script::Compile(source);
//	script.Run();
//
//	system("pause");
//	return 0;
//	}
