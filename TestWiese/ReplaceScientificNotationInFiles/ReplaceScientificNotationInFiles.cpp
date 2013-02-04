#include "doocore/io/Tools.h"
#include "doocore/system/Tools.h"

int main() {
	std::string filename = "FitResults.out";
	doocore::io::tools::ReplaceScientificNotationInFile(filename);
}