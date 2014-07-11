// from STL
#include <string>

// from RooFit
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooCategory.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"


// from project
#include "doocore/io/EasyTuple.h"

int main() {
  using namespace doocore::io;
  
  RooRealVar varMass("varMass", "varMass", 5000, 6000);
  RooRealVar mean("mean", "mean", 5500, 5000, 6000);
  RooRealVar sigma("sigma", "sigma", 10, 0, 50);
  RooCategory cat("cat", "cat");

  cat.defineType("bla", 1);
  cat.defineType("blub", 0);

  RooGaussian pdf("pdf", "pdf", varMass, mean, sigma);
  RooDataSet* data_gen = pdf.generate(RooArgSet(varMass, cat), 10000);

  EasyTuple etuple1(*data_gen);
  etuple1.dataset().Print();
  etuple1.WriteDataSetToTree("test.root", "Bs2Jpsif0");

  RooFormulaVar varMassShift_formula("varMassShift", "varMassShift", "@0-5.0", RooArgList(varMass));
  
  // standard use case: open tuple as RooDataSet
  EasyTuple etuple("test.root", "Bs2Jpsif0", RooArgSet(varMass, varMassShift_formula));
  RooDataSet& data = etuple.ConvertToDataSet(RooFit::Cut("varMass>5200"));
  
  RooRealVar& varMassShift = etuple.Var("varMassShift");
  varMassShift.Print();
  
  data.Print();
}