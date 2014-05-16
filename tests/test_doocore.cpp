
#include <gtest/gtest.h>
#include <iostream>

#include <doocore/statistics/general.h>

TEST(TestDoocoreStatisticsGeneral_ValueWithError, CorrectOutputCheck) {
    
    doocore::statistics::general::ValueWithError<double_t> p1(0.99, 0.109);
    doocore::statistics::general::ValueWithError<double_t> p2(0.339, 0.40, 3);
    
    EXPECT_EQ("0.99 +/- 0.11", p1.FormatString());
    EXPECT_EQ("0.3 +/- 0.4", p2.FormatString());
    
}

TEST(TestDoocoreStatisticsGeneral_EfficiencyBionomialError, CorrectOutputCheck) {
    double_t binomeError = doocore::statistics::general::EfficiencyBinomialError(1337, 4200);
    
    EXPECT_DOUBLE_EQ(0.007187906878292545812890080560554374851754525774, binomeError);
}

TEST(TestDoocoreStatisticsGeneral_WeightedAverage, CorrectOutputCheck){
    std::vector<double_t> values{1,2,3};
    std::vector<double_t> weights{0.5, 0.4, 0.1};
    std::vector<double_t> errors{0.2, 0.3, 0.4};
    
    doocore::statistics::general::ValueWithError<double_t> testWeightedAverage = doocore::statistics::general::WeightedAverage<double_t>(values.begin(), values.end(), weights.begin(), errors.begin());
    
    EXPECT_EQ("1.60 +/- 0.16", testWeightedAverage.FormatString());

}