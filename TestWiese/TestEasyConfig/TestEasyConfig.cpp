#include "doocore/config/EasyConfig.cpp"

#include <string>

int main(int argc, char *argv[]){
	doocore::config::EasyConfig cfg(argc, argv);

  doocore::io::sout << "binning: " << cfg.KeyExists("fullfit.flexibuilder.acceptance.binning.bssig") << doocore::io::endmsg;
  doocore::io::sout << "binning2: " << cfg.KeyExists("fullfit.flexibuilder") << doocore::io::endmsg;
  
	doocore::io::sout << "Testing Config" << doocore::io::endmsg;
	doocore::io::sout << "Florida: " << cfg.getString("Florida") << doocore::io::endmsg;
	doocore::io::sout << "Decision: " << cfg.getBool("Decision") << doocore::io::endmsg;
	doocore::io::sout << "pi: " << cfg.getDouble("pi") << doocore::io::endmsg;
  
  doocore::io::sout << "pi (templating): " << cfg.Get<double>("pi", 3.14) << doocore::io::endmsg;
  
	doocore::io::sout << "TheNumber: " << cfg.getInt("TheNumber") << doocore::io::endmsg;

	doocore::io::sout << "Keys: " << cfg.getVoStringPairs("Keys") << doocore::io::endmsg;
  
  doocore::io::sout << "doubles: " << cfg.GetVector<double>("doubles") << doocore::io::endmsg;
  doocore::io::sout << "keyvals: " << cfg.GetVectorPairs<std::string,bool>("keyvals") << doocore::io::endmsg;

  doocore::io::sout << "real key exists: " << cfg.KeyExists("doubles") << doocore::io::endmsg;
  doocore::io::sout << "absurd key exists: " << cfg.KeyExists("YoMamaKey") << doocore::io::endmsg;

  
  cfg.Print();
}