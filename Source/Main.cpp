#include "Stable.h"
#include "Utils/FileReader.h"

int main(int argc, char *argv[]) {
	if (argc != 1) {
		char *fileName = argv[1];
		FileReader reader{ fileName };
		std::string source =  reader.readAll();
		DBG_PRINT << source;
	}
	system("pause");
    return 0;
}
