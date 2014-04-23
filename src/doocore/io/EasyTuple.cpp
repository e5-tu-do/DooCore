#include "doocore/io/EasyTuple.h"

// from STL
#include <string>
#include <sstream>
#include <vector>

// from Boost
#include <boost/assign/std/vector.hpp>
using namespace boost::assign;

// from ROOT
#include "TFile.h"
#include "TTree.h"

// from RooFit
#include "RooArgSet.h"
#include "RooLinkedListIter.h"
#include "RooAbsArg.h"
#include "RooDataSet.h"
#include "RooFormulaVar.h"
#include "RooCategory.h"
#include "RooRealVar.h"
#include "RooCmdArg.h"

// from project
#include "doocore/io/MsgStream.h"

using namespace ROOT;
using namespace RooFit;

doocore::io::EasyTuple::EasyTuple(const std::string& file_name, const std::string& tree_name, const RooArgSet& argset)
: file_(NULL),
  tree_(NULL),
  argset_(NULL),
  dataset_(NULL),
  tree_name_(tree_name),
  num_maximum_events_(-1),
  cut_variable_range_(kCutExclusive)
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
  
  if (argset.getSize() > 0) tree_->SetBranchStatus("*", 0);
  
  RooLinkedListIter* it  = (RooLinkedListIter*)argset.createIterator();
  RooAbsArg*         arg = NULL;
  
  while ((arg=(RooAbsArg*)it->Next())) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    RooCategory* cat = dynamic_cast<RooCategory*>(arg);
    
    if (var != NULL || cat != NULL) {
      if (tree_->GetBranch(arg->GetName()) == NULL) {
        swarn << "Branch " << arg->GetName() << " not in tree. Ignoring." << endmsg;
      } else {
        tree_->SetBranchStatus(arg->GetName(), 1);
      }
    }
  }
  delete it;
}

doocore::io::EasyTuple::EasyTuple(TTree* tree, const RooArgSet& argset)
: file_(NULL),
tree_(tree),
argset_(NULL),
dataset_(NULL),
num_maximum_events_(-1),
cut_variable_range_(kCutExclusive)
{
  argset_ = new RooArgSet(argset);
  
  if (argset.getSize() > 0) tree_->SetBranchStatus("*", 0);
  
  RooLinkedListIter* it  = (RooLinkedListIter*)argset.createIterator();
  RooAbsArg*         arg = NULL;
  
  while ((arg=(RooAbsArg*)it->Next())) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
    RooCategory* cat = dynamic_cast<RooCategory*>(arg);
    
    if (var != NULL || cat != NULL) {
      if (tree_->GetBranch(arg->GetName()) == NULL) {
        swarn << "Branch " << arg->GetName() << " not in tree. Ignoring." << endmsg;
      } else {
        tree_->SetBranchStatus(arg->GetName(), 1);
      }
    }
  }
  delete it;
}

doocore::io::EasyTuple::EasyTuple(const EasyTuple& other)
: file_(NULL),
tree_(NULL),
argset_(NULL),
dataset_(NULL),
tree_name_(other.tree_name_),
num_maximum_events_(other.num_maximum_events_),
cut_variable_range_(other.cut_variable_range_)
{
  argset_ = new RooArgSet(*other.argset_);
  
  if (other.file_ == NULL) {
    tree_ = other.tree_;
  } else {
    file_ = new TFile(other.file_->GetName());
    
    if (file_ == NULL || file_->IsZombie() || file_->GetNkeys() <= 0) {
      serr << "File could not be opened properly." << endmsg;
      throw 1;
    }
    
    tree_ = dynamic_cast<TTree*>(file_->Get(tree_name_.c_str()));
    if (tree_ == NULL) {
      serr << "Tree " << tree_name_ << " could not be opened properly." << endmsg;
      throw 2;
    }
    
    if (argset_->getSize() > 0) tree_->SetBranchStatus("*", 0);
    
    RooLinkedListIter* it  = (RooLinkedListIter*)argset_->createIterator();
    RooAbsArg*         arg = NULL;
    
    while ((arg=(RooAbsArg*)it->Next())) {
      RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
      RooCategory* cat = dynamic_cast<RooCategory*>(arg);
      
      if (var != NULL || cat != NULL) {
        if (tree_->GetBranch(arg->GetName()) == NULL) {
          swarn << "Branch " << arg->GetName() << " not in tree. Ignoring." << endmsg;
        } else {
          tree_->SetBranchStatus(arg->GetName(), 1);
        }
      }
    }
    delete it;

    if (num_maximum_events_>=0) {
      set_num_maximum_events(num_maximum_events_);
    }
    
    if (other.dataset_ != NULL) {
      dataset_ = new RooDataSet(*other.dataset_);
    }
  }
}

doocore::io::EasyTuple::~EasyTuple() {
  if (dataset_ != NULL) delete dataset_;
  if (argset_ != NULL) delete argset_;
  if (tree_ != NULL && file_!= NULL) delete tree_;
  if (file_ != NULL) delete file_;
}

RooDataSet& doocore::io::EasyTuple::ConvertToDataSet(const RooCmdArg& arg1,
                                                     const RooCmdArg& arg2,
                                                     const RooCmdArg& arg3,
                                                     const RooCmdArg& arg4,
                                                     const RooCmdArg& arg5,
                                                     const RooCmdArg& arg6,
                                                     const RooCmdArg& arg7) {
  if (argset_ == NULL) {
    serr << "Internal argset not set. Cannot convert to RooDataSet without this." << endmsg;
    throw 4;
  }
  
  return ConvertToDataSet(*argset_, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

RooDataSet& doocore::io::EasyTuple::ConvertToDataSet(const RooArgSet& argset,
                                                     const RooCmdArg& arg1,
                                                     const RooCmdArg& arg2,
                                                     const RooCmdArg& arg3,
                                                     const RooCmdArg& arg4,
                                                     const RooCmdArg& arg5,
                                                     const RooCmdArg& arg6,
                                                     const RooCmdArg& arg7) {
  if (dataset_ != NULL) {
    serr << "Dataset was converted before. Maybe you want to use doocore::io::EasyTuple::dataset()." << endmsg;
    throw 3;
  }
  
  // filter out formula vars and additionally cut on all variable ranges
  RooLinkedListIter* it  = (RooLinkedListIter*)argset.createIterator();
  RooAbsArg*         arg = NULL;
  RooArgSet          new_set;
  std::vector<RooFormulaVar*> formulas;
  std::string        cut_variables="";
  std::stringstream  stream_cut_variables;
  std::string        less_operator = cut_variable_range_==kCutInclusive ? "<=" : "<";
  std::string        greater_operator = cut_variable_range_==kCutInclusive ? ">=" : ">";
  
  while ((arg=(RooAbsArg*)it->Next())) {
    RooFormulaVar* formula = dynamic_cast<RooFormulaVar*>(arg);
    RooRealVar*    var     = dynamic_cast<RooRealVar*>(arg);
    
    if (formula == NULL) {
      new_set.add(*arg);
      
      if (var != NULL && cut_variable_range_ != kNoCuts) {
        if (var->hasMin()) stream_cut_variables << "&&" << var->GetName() << greater_operator << var->getMin();
        if (var->hasMax()) stream_cut_variables << "&&" << var->GetName() << less_operator << var->getMax();
      }
    } else {
      formulas.push_back(formula);
    }
  }
  delete it;
   
  // 	RooCmdArg(const char* name, Int_t i1 = 0, Int_t i2 = 0, Double_t d1 = 0, Double_t d2 = 0, const char* s1 = 0, const char* s2 = 0, const TObject* o1 = 0, const TObject* o2 = 0, const RooCmdArg* ca = 0, const char* s3 = 0, const RooArgSet* c1 = 0, const RooArgSet* c2 = 0)
  // RooCmdArg("CutSpec",0,0,0,0,cutSpec,0,0,0) ;
  
  std::vector<RooCmdArg> args;
  args += arg1, arg2, arg3, arg4, arg5, arg6, arg7;
  
  bool found_cut_arg = false;
  for (std::vector<RooCmdArg>::iterator it=args.begin(), end=args.end();
       it != end; ++it) {
    std::string name = it->GetName();
    if (name == "CutSpec") {
      if ((void*)it->getString(0) != NULL) {
        std::string user_cut_string(it->getString(0));
        if (user_cut_string.length() > 0) stream_cut_variables << "&&" << user_cut_string;
      }
      cut_variables = stream_cut_variables.str();
      if (cut_variables.length() > 2) {
        cut_variables = cut_variables.substr(2);
      }
      
      sinfo << "Converting dataset with cut '" << cut_variables << "'" <<  endmsg;
      *it = Cut(cut_variables.c_str());
      found_cut_arg = true;
    }
  }
  if (!found_cut_arg) {
    cut_variables = stream_cut_variables.str();
    if (cut_variables.length() > 0) {
      cut_variables = cut_variables.substr(2);
      std::string name = args[6].GetName();
      if (name != "") {
        swarn << "doocore::io::EasyTuple::ConvertToDataSet(...): Have to delete last passed RooCmdArg " << name << " to apply necessary cut." << endmsg;
      }
      sinfo << "Converting dataset with cut " << cut_variables << endmsg;
      args[6] = Cut(cut_variables.c_str());
    }
  }
  
  //temp: copy tree 
  //tree_ = tree_->CopyTree("", "", 800000);
  //tree_->SetEntries(300000);
 
  dataset_ = new RooDataSet("dataset","dataset",new_set,Import(*tree_), args[0],
                            args[1], args[2], args[3], args[4], args[5], args[6]);
  
  for (std::vector<RooFormulaVar*>::const_iterator it = formulas.begin();
       it != formulas.end(); ++it) {
    sinfo << "Adding formula " << (*it)->GetName() << " to dataset." << endmsg;
    dataset_->addColumn(**it);
  }
  
  return *dataset_;
}

RooRealVar& doocore::io::EasyTuple::Var(const std::string& name) {
  if (dataset_ != NULL && dataset_->get()->find(name.c_str()) != NULL) {
    RooRealVar* var = dynamic_cast<RooRealVar*>(dataset_->get()->find(name.c_str()));
    if (var != NULL) {
      return *var;
    } else {
      serr << "Variable " << name << " in dataset is not of type RooRealVar." << endmsg;
      throw 5;
    }
  } else {
    serr << "Variable " << name << " not in dataset or tuple not converted to dataset." << endmsg;
    throw 5;
  }
}

const RooRealVar& doocore::io::EasyTuple::Var(const std::string& name) const {
  return const_cast<doocore::io::EasyTuple*>(this)->Var(name);
}
