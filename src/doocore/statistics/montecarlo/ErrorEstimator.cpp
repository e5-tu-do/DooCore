#include "ErrorEstimator.h"

// from STL
#include <algorithm>

// from RooFit
#include "RooMultiVarGaussian.h"
#include "TMatrixDSym.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooAbsArg.h"
#include "RooRealVar.h"
#include "RooDataSet.h"

// from DooCore
#include "doocore/io/MsgStream.h"
#include "doocore/statistics/general.h"

doocore::statistics::montecarlo::MultiVarGaussianSampleGenerator::MultiVarGaussianSampleGenerator(const RooArgList& values_expected, const TMatrixDSym& covariance)
: mvg_(NULL),
values_expected_(new RooArgSet()),
values_mu_(new RooArgList()),
num_generated_(0),
pos_dataset_(0),
dataset_(NULL) {
  using namespace doocore::io;
  

  TIterator* it = values_expected.createIterator();
  RooAbsArg* arg = NULL;
  
  while ((arg = dynamic_cast<RooAbsArg*>(it->Next()))) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    if (var != NULL) {
      RooRealVar* var_copy = new RooRealVar(*var, var->GetName());
      values_expected_->addOwned(*var_copy);

      std::string name_copy(std::string(var->GetName())+"_mu");
      RooRealVar* var_copy_mu = new RooRealVar(*var, name_copy.c_str());
      values_mu_->addOwned(*var_copy_mu);
    }
  }
  delete it;
  
  mvg_ = new RooMultiVarGaussian("mvg", "mvg", *values_expected_, *values_mu_, covariance);
}

doocore::statistics::montecarlo::MultiVarGaussianSampleGenerator::~MultiVarGaussianSampleGenerator() {
  delete mvg_;
  delete values_expected_;
  delete values_mu_;
  if (dataset_ != NULL) {
    delete dataset_;
  }
}

const RooArgSet* doocore::statistics::montecarlo::MultiVarGaussianSampleGenerator::Generate() {
  using namespace doocore::io;
  
  const RooArgSet* values;
  pos_dataset_++;
  if (dataset_ == NULL || pos_dataset_ >= static_cast<unsigned int>(dataset_->numEntries())) {
    pos_dataset_ = 0;
    int num_generate = std::max(100, 2*static_cast<int>(num_generated_));
    num_generated_ += num_generate;
    if (dataset_ != NULL) {
      delete dataset_;
    }
    
//    values_expected_->Print();
//    values_mu_->Print();
    dataset_ = mvg_->generate(*values_expected_, num_generate);
  }
  
  values = dataset_->get(pos_dataset_);
  return values;
}