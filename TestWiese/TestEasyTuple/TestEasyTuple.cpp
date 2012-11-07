// from STL
#include <string>

// from RooFit
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"

// from project
#include "doocore/io/EasyTuple.h"

int main() {
  using namespace doocore::io;
  
  RooRealVar varMass("varMass", "varMass", 5000, 6000);
  RooFormulaVar varMassShift_formula("varMassShift", "varMassShift", "@0-5.0", RooArgList(varMass));
  
  // standard use case: open tuple as RooDataSet
  EasyTuple etuple("test.root", "Bs2Jpsif0", RooArgSet(varMass, varMassShift_formula));
  RooDataSet& data = etuple.ConvertToDataSet(std::string("varMass>5200"));
  
  RooRealVar& varMassShift = etuple.Var("varMassShift");
  varMassShift.Print();
  
  data.Print();
}