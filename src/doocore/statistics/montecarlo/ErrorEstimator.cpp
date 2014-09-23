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

using namespace doocore::io;

doocore::statistics::montecarlo::MultiVarGaussianSampleGenerator::MultiVarGaussianSampleGenerator(const RooArgList& values_expected, const TMatrixDSym& covariance)
: mvg_(NULL),
values_expected_(new RooArgSet()),
values_mu_(new RooArgList()),
last_generated_set_(nullptr),
min_generated_set_(nullptr),
max_generated_set_(nullptr),
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
  if (last_generated_set_ != nullptr) {
    delete last_generated_set_;
  }
  if (min_generated_set_ != nullptr) {
    delete min_generated_set_;
  }
  if (max_generated_set_ != nullptr) {
    delete max_generated_set_;
  }
}

const RooArgSet* doocore::statistics::montecarlo::MultiVarGaussianSampleGenerator::Generate() {
  using namespace doocore::io;
  
  if (last_generated_set_ != nullptr) {
    delete last_generated_set_;
    last_generated_set_ = nullptr;
  }
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
  
  last_generated_set_ = dataset_->get(pos_dataset_);
  return last_generated_set_;
}

void doocore::statistics::montecarlo::MultiVarGaussianSampleGenerator::SaveMinimumParameterSet() {
  if (min_generated_set_ != nullptr) {
    delete min_generated_set_;
  }
  min_generated_set_ = new RooArgSet();
  TIterator* it = last_generated_set_->createIterator();
  RooAbsArg* arg = NULL;
  while ((arg = dynamic_cast<RooAbsArg*>(it->Next()))) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    if (var != NULL) {
      RooRealVar* var_copy = new RooRealVar(*var, var->GetName());

      min_generated_set_->addOwned(*var_copy);
    }
  }
}

void doocore::statistics::montecarlo::MultiVarGaussianSampleGenerator::SaveMaximumParameterSet() {
  if (max_generated_set_ != nullptr) {
    delete max_generated_set_;
  }
  max_generated_set_ = new RooArgSet();
  TIterator* it = last_generated_set_->createIterator();
  RooAbsArg* arg = NULL;
  while ((arg = dynamic_cast<RooAbsArg*>(it->Next()))) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    if (var != NULL) {
      RooRealVar* var_copy = new RooRealVar(*var, var->GetName());

      max_generated_set_->addOwned(*var_copy);
    }
  }
}


doocore::statistics::montecarlo::VaryParameterErrorsGenerator::VaryParameterErrorsGenerator(const RooArgList& values_expected)
: values_(new RooArgList()),
last_generated_set_(nullptr),
min_generated_set_(nullptr),
max_generated_set_(nullptr) {
  using namespace doocore::io;

  TIterator* it = values_expected.createIterator();
  RooAbsArg* arg = NULL;
  
  while ((arg = dynamic_cast<RooAbsArg*>(it->Next()))) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    if (var != nullptr) {
      RooRealVar* var_copy = new RooRealVar(*var, var->GetName());
      values_->addOwned(*var_copy);
    }
  }
  delete it;
}

doocore::statistics::montecarlo::VaryParameterErrorsGenerator::~VaryParameterErrorsGenerator() {
  if (last_generated_set_ != nullptr) {
    delete last_generated_set_;
  }
  if (min_generated_set_ != nullptr) {
    delete min_generated_set_;
  }
  if (max_generated_set_ != nullptr) {
    delete max_generated_set_;
  }

  delete values_;
}

const RooArgSet* doocore::statistics::montecarlo::VaryParameterErrorsGenerator::Generate() {
  using namespace doocore::io;
  
  if (last_generated_set_ != nullptr) {
    delete last_generated_set_;
  }
  last_generated_set_ = new RooArgSet();

  TIterator* it = values_->createIterator();
  RooAbsArg* arg = NULL;
  while ((arg = dynamic_cast<RooAbsArg*>(it->Next()))) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    if (var != NULL) {
      double test(random_.Rndm());
      RooRealVar* var_copy = new RooRealVar(*var, var->GetName());

      if (test < 0.3333333333) {
        // do nothing, keep value
      } else if (test < 0.6666666667) {
        // vary parameter up
        var_copy->setVal(var->getVal() + var->getError());        
      } else {
        // vary parameter down
        var_copy->setVal(var->getVal() - var->getError());
      }

      last_generated_set_->addOwned(*var_copy);
    }
  }

  return last_generated_set_;
}

void doocore::statistics::montecarlo::VaryParameterErrorsGenerator::SaveMinimumParameterSet() {
  if (min_generated_set_ != nullptr) {
    delete min_generated_set_;
  }
  min_generated_set_ = new RooArgSet();
  TIterator* it = last_generated_set_->createIterator();
  RooAbsArg* arg = NULL;
  while ((arg = dynamic_cast<RooAbsArg*>(it->Next()))) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    if (var != NULL) {
      RooRealVar* var_copy = new RooRealVar(*var, var->GetName());

      min_generated_set_->addOwned(*var_copy);
    }
  }
}

void doocore::statistics::montecarlo::VaryParameterErrorsGenerator::SaveMaximumParameterSet() {
  if (max_generated_set_ != nullptr) {
    delete max_generated_set_;
  }
  max_generated_set_ = new RooArgSet();
  TIterator* it = last_generated_set_->createIterator();
  RooAbsArg* arg = NULL;
  while ((arg = dynamic_cast<RooAbsArg*>(it->Next()))) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    if (var != NULL) {
      RooRealVar* var_copy = new RooRealVar(*var, var->GetName());

      max_generated_set_->addOwned(*var_copy);
    }
  }
}
