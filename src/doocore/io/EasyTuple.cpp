#include "doocore/io/EasyTuple.h"

// from STL
#include <string>
#include <vector>

// from ROOT
#include "TFile.h"
#include "TTree.h"

// from RooFit
#include "RooArgSet.h"
#include "RooLinkedListIter.h"
#include "RooAbsArg.h"
#include "RooDataSet.h"
#include "RooFormulaVar.h"

// from project
#include "doocore/io/MsgStream.h"

using namespace ROOT;
using namespace RooFit;

doocore::io::EasyTuple::EasyTuple(const std::string& file_name, const std::string& tree_name, const RooArgSet& argset)
: file_(NULL),
  tree_(NULL),
  argset_(NULL),
  dataset_(NULL)
{
  file_ = new TFile(file_name.c_str());
  argset_ = new RooArgSet(argset);
  
  if (file_ == NULL || file_->IsZombie() || file_->GetNkeys() <= 0) {
    serr << "File " << file_name << " could not be opened properly." << endmsg;
    throw 1;
  }
  
  tree_ = dynamic_cast<TTree*>(file_->Get(tree_name.c_str()));
  if (tree_ == NULL) {
    serr << "Tree " << tree_name << " could not be opened properly." << endmsg;
    throw 2;
  }
  
  tree_->SetBranchStatus("*", 0);
  
  RooLinkedListIter* it  = (RooLinkedListIter*)argset.createIterator();
  RooAbsArg*         arg = NULL;
  
  while ((arg=(RooAbsArg*)it->Next())) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    
    if (var != NULL) {
      if (tree_->GetBranch(arg->GetName()) == NULL) {
        swarn << "Branch " << arg->GetName() << " not in tree. Ignoring." << endmsg;
      } else {
        tree_->SetBranchStatus(arg->GetName(), 1);
      }
    }
  }
  delete it;
}

doocore::io::EasyTuple::~EasyTuple() {
  if (dataset_ != NULL) delete dataset_;
  if (argset_ != NULL) delete argset_;
  if (tree_ != NULL) delete tree_;
  if (file_ != NULL) delete file_;
}

RooDataSet& doocore::io::EasyTuple::ConvertToDataSet(const std::string& cut) {
  if (argset_ == NULL) {
    serr << "Internal argset not set. Cannot convert to RooDataSet without this." << endmsg;
    throw 4;
  }
  
  return ConvertToDataSet(*argset_, cut);
}

RooDataSet& doocore::io::EasyTuple::ConvertToDataSet(const RooArgSet& argset, const std::string& cut) {
  if (dataset_ != NULL) {
    serr << "Dataset was converted before. Maybe you want to use doocore::io::EasyTuple::dataset()." << endmsg;
    throw 3;
  }
  
  // filter out formula vars
  RooLinkedListIter* it  = (RooLinkedListIter*)argset.createIterator();
  RooAbsArg*         arg = NULL;
  RooArgSet          new_set;
  std::vector<RooFormulaVar*> formulas;
  
  while ((arg=(RooAbsArg*)it->Next())) {
    RooFormulaVar* formula = dynamic_cast<RooFormulaVar*>(arg);
    
    if (formula == NULL) {
      new_set.add(*arg);
    } else {
      formulas.push_back(formula);
    }
  }
  delete it;
  
  if (cut.length() == 0) {
    dataset_ = new RooDataSet("dataset","dataset",new_set,Import(*tree_));
  } else {
    dataset_ = new RooDataSet("dataset","dataset",new_set,Cut(cut.c_str()),Import(*tree_));
  }
  
  for (std::vector<RooFormulaVar*>::const_iterator it = formulas.begin();
       it != formulas.end(); ++it) {
    sinfo << "Adding formula " << (*it)->GetName() << " to dataset." << endmsg;
    dataset_->addColumn(**it);
  }
  
  return *dataset_;
}
