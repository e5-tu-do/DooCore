#include "doocore/config/EasyConfig.cpp"

int main(int argc, char *argv[]){
	doocore::config::EasyConfig cfg(argc, argv);

	doocore::io::sinfo << "Testing Config" << doocore::io::endmsg;
	doocore::io::sout << "Florida: " << cfg.getString("Florida") << doocore::io::endmsg;
	doocore::io::sout << "Decision: " << cfg.getBool("Decision") << doocore::io::endmsg;
	doocore::io::sout << "pi: " << cfg.getDouble("pi") << doocore::io::endmsg;
  
  doocore::io::sout << "pi (templating): " << cfg.Get<double>("pi", 3.14) << doocore::io::endmsg;
  
	doocore::io::sout << "TheNumber: " << cfg.getInt("TheNumber") << doocore::io::endmsg;

	doocore::io::sout << "Keys: " << cfg.getVoStringPairs("Keys") << doocore::io::endmsg;

}