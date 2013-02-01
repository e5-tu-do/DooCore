#include "doocore/io/Tools.h"

int main() {
	std::string filename = "/home/ccauet/LHCbB2JpsiKS/2012/build/DEBUG/FitResults.out";

	doocore::io::tools::ReplaceScientificNotationInFiles(filename);
}