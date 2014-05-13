
#include <gtest/gtest.h>

#include "doocore/config/EasyConfig.cpp"

TEST(TestDooConfig, CreateCfg) {

	auto* cfg = new doocore::config::EasyConfig("bla", false);
    EXPECT_EQ(cfg, nullptr);

}
