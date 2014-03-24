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
values_expected_(new RooArgSet(values_expected)),
num_generated_(0),
pos_dataset_(0),
dataset_(NULL) {
  using namespace doocore::io;

  TIterator* it = values_expected.createIterator();
  RooAbsArg* arg = NULL;
  
  RooArgList values_mu;
  while ((arg = dynamic_cast<RooAbsArg*>(it->Next()))) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    if (var != NULL) {
      var->Print();
      std::string name_copy(std::string(var->GetName())+"_mu");
      RooRealVar* var_copy = new RooRealVar(*var, name_copy.c_str());
      var_copy->Print();
      values_mu.addOwned(*var_copy);
    }
  }
  delete it;
  
  sdebug << *values_expected_ << endmsg;
  sdebug << values_mu << endmsg;
  values_mu.Print();
  covariance.Print();
  
  mvg_ = new RooMultiVarGaussian("mvg", "mvg", *values_expected_, values_mu, covariance);
}

const RooArgSet* doocore::statistics::montecarlo::MultiVarGaussianSampleGenerator::Generate() {
  using namespace doocore::io;
  
  const RooArgSet* values;
  pos_dataset_++;
  if (dataset_ == NULL || pos_dataset_ >= dataset_->numEntries()) {
    pos_dataset_ = 0;
    int num_generate = std::min(100, 2*static_cast<int>(num_generated_));
    sdebug << "Will generate " << num_generate << " events." << endmsg;
    num_generated_ += num_generate;
    if (dataset_ != NULL) {
      delete dataset_;
    }
    dataset_ = mvg_->generate(*values_expected_, num_generate);
  }
  
  values = dataset_->get(pos_dataset_);
  return values;
}