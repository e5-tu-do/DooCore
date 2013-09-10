#include "doocore/config/EasyConfig.cpp"

//#if __GNUG__
#define BOOST_NO_CXX11_SCOPED_ENUMS
//#endif

int main(int argc, char *argv[]){
	doocore::io::sout << "TestEasyConfig.cpp" << doocore::io::endmsg;	

	doocore::config::EasyConfig cfg(argc, argv);

	doocore::io::sinfo << "Testing default values" << doocore::io::endmsg;
	doocore::io::sout << "String default value: " << cfg.getString("Florida") << doocore::io::endmsg;
	doocore::io::sout << "Bool default value: " << cfg.getBool("Decision") << doocore::io::endmsg;
	doocore::io::sout << "Double default value: " << cfg.getDouble("pi") << doocore::io::endmsg;
	doocore::io::sout << "Integer default value: " << cfg.getInt("TheNumber") << doocore::io::endmsg;
}