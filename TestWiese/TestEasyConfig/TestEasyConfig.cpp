#include "doocore/config/EasyConfig.cpp"

int main(int argc, char *argv[]){
	doocore::io::sout << "TestEasyConfig.cpp" << doocore::io::endmsg;	

	doocore::config::EasyConfig cfg(argc, argv);
}