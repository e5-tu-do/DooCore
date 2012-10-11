#include "doocore/io/EasyTuple.h"

// from STL
#include <string>

// from ROOT
#include "TFile.h"
#include "TTree.h"

// from RooFit
#include "RooArgSet.h"
#include "RooLinkedListIter.h"
#include "RooAbsArg.h"
#include "RooDataSet.h"

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
  
  if (file_ == NULL || file_->IsZombie() || file_->GetNkeys() <= 1) {
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
    tree_->SetBranchStatus(arg->GetName(), 1);
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
  
  if (cut.length() == 0) {
    dataset_ = new RooDataSet("dataset","dataset",argset,Import(*tree_));
  } else {
    dataset_ = new RooDataSet("dataset","dataset",argset,Cut(cut.c_str()),Import(*tree_));
  }
  
  return *dataset_;
}
