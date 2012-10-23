
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
  RooFormulaVar varMassShift("varMassShift", "varMassShift", "@0-5.0", RooArgList(varMass));
  
  // standard use case: open tuple as RooDataSet
  EasyTuple etuple("test.root", "Bs2Jpsif0", RooArgSet(varMass, varMassShift));
  RooDataSet& data = etuple.ConvertToDataSet();
  
  data.Print();
}