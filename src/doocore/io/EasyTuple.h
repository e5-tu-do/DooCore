#ifndef DOOCORE_IO_EASYTUPLE_H
#define DOOCORE_IO_EASYTUPLE_H

// from STL
#include <string>
#include <vector>

// from RooFit
#include "RooCmdArg.h"

// forward decalarations
class RooArgSet;
class TFile;
class TTree;
class RooDataSet;
class RooRealVar;

namespace doocore {
namespace io {
/*! @class doocore::io::EasyTuple
 * @brief Easy tuple loading into TTree/RooDataSet without boilerplate code
 *
 * EasyTuple allows loading of a TTree from a TFile without the ususal 
 * boilerplate code. It also supports automatic deactivation of unnecessary 
 * branches to speedup RooDataSet import from non-fit tuples.
 * 
 * EasyTuple can be used as a stack object and will take care of cleanup at 
 * destruction. Main focus is convenient and easy usage.
 *
 * @section et_usage Usage
 *
 * Usage of EasyTuple is simple. Consider this example:
 * 
 * @code
 * #include "doocore/io/EasyTuple.h"
 *
 * int main() {
 *   using namespace doocore::io;
 *
 *   // define a RooRealVar and a RooFormulaVar which will later be added 
 *   // automatically
 *   RooRealVar varMass("varMass", "varMass", 5000, 6000);
 *   RooFormulaVar varMassShift("varMassShift", "varMassShift", 
 *                              "@0-5.0", RooArgList(varMass));
 *   // define more variables...
 *
 *   // standard use case: open tuple as RooDataSet
 *   EasyTuple etuple("tuplefile.root", "Bs2Jpsif0", 
 *                    RooArgSet(varMass,varMassShift,varOmega,cutVar));
 *   RooDataSet& data = etuple.ConvertToDataSet("cutVar == 1");
 *
 *   // print the tuple - notice that the RooFormulaVar varMassShift has 
 *   // automatically been added and that the accessed variable is of type
 *   // RooRealVar.
 *   data.Print();
 *   etuple.Var("varMassShift").Print();
 *
 *   // other use case: open tuple and get TTree to iterate by hand
 *   EasyTuple etuple2("tuplefile.root", "Bs2Jpsif0",
 *                     RooArgSet(varMass,varProptime,varOmega,cutVar));
 *   TTree& tree = etuple2.tree();
 * }
 * @endcode
 *
 * What is EasyTuple doing behind the curtain? First, the ROOT file 
 * @c tuplefile.root is being opened and the TTree @c Bs2Jpsif0 inside is 
 * opened. Afterwards, all branches not in the import RooArgSet are being 
 * deactivated for performance reasons (see Performance section below). 
 *
 * In the next call to doocore::io::EasyTuple::ConvertToDataSet() the tuple is 
 * converted to a RooDataSet while it is further reduced by the supplied cut 
 * string. The RooDataSet is returned as reference.
 * 
 * EasyTuple takes care of internal pointers and deletes them upon destruction 
 * so the user does not have to bother with bookkeeping. It's as simple as that.
 *
 * @section et_performance Performance
 *
 * As EasyTuple is deactivating all unnecessary branches, looping over the tree
 * or converting to a RooDataSet is quite fast. In some real case studies, the 
 * conversion to a RooDataSet took a few seconds compared to several minutes 
 * when not deactivating the branches (and compared to a few split seconds when
 * directly using a reduced fit tuple without unnecessary branches).
 */
class EasyTuple {
 public:
  /**
   *  @brief Constructor for EasyTuple with RooArgSet for branches to use
   *
   *  Based on the supplied file name and tree name in this file, EasyTuple will
   *  load the TTree and deactivate all branches not in the supplied RooArgSet.
   *
   *  @param file_name file name of TFile to open
   *  @param tree_name tree name in TFile to open
   *  @param argset RooArgSet of parameters to activate
   */
  EasyTuple(const std::string& file_name, const std::string& tree_name, const RooArgSet& argset);
  
  /**
   *  @brief Constructor for EasyTuple based on a given TTree
   *
   *  Based on the supplied TTree, EasyTuple will deactivate all branches not in
   *  the supplied RooArgSet. EasyTuple will not assume ownership over the 
   *  supplied TTree but may deactivate branches in it.
   *
   *  @param tree TTree to use
   *  @param argset RooArgSet of parameters to activate
   */
  EasyTuple(TTree* tree, const RooArgSet& argset);
  
  /**
   *  @brief Destructor for EasyTuple
   */
  ~EasyTuple();
  
  /**
   *  @brief Get opened TTree
   *
   *  @return the TTree reference
   */
  const TTree& tree() const {return *tree_;}

  /**
   *  @brief Get opened TTree
   *
   *  @return the TTree reference
   */
  TTree& tree() {return *tree_;}
  
  /**
   *  @brief Get previously converted RooDataSet
   *
   *  @return the RooDataSet reference
   */
  const RooDataSet& dataset() const {return *dataset_;}
  
  /**
   *  @brief Get previously converted RooDataSet
   *
   *  @return the RooDataSet reference
   */
  RooDataSet& dataset() {return *dataset_;}
  
  /**
   *  @brief Get RooDataSet based on opened TTree and supplied optional RooCmdArgs
   *
   *  The internal tree will be converted into a RooDataSet and this RooDataSet
   *  returned. The internally stored argset will be used. Optional cuts or 
   *  arguments can be supplied as RooCmdArgs.
   *
   *  RooFormulaVars in the argset will automatically be added to the dataset
   *  and therewith converted to real variables in the dataset.
   *
   *  If the dataset has already been converted, this function will throw an
   *  exception (reconverting the dataset is not supported). If you just want to
   *  get the already converted dataset again, use dataset().
   *
   *  @param arg1 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg2 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg3 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg4 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg5 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg6 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg7 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @return the converted dataset
   */
  RooDataSet& ConvertToDataSet(const RooCmdArg& arg1 = RooCmdArg(),
                               const RooCmdArg& arg2 = RooCmdArg(),
                               const RooCmdArg& arg3 = RooCmdArg(),
                               const RooCmdArg& arg4 = RooCmdArg(),
                               const RooCmdArg& arg5 = RooCmdArg(),
                               const RooCmdArg& arg6 = RooCmdArg(),
                               const RooCmdArg& arg7 = RooCmdArg());
  
  /**
   *  @brief (DEPRECATED) Get RooDataSet based on opened TTree and supplied optional cut
   *
   *  The internal tree will be converted into a RooDataSet and this RooDataSet
   *  returned. The internally stored argset will be used. An optional cut can
   *  be supplied to further reduce the dataset.
   *
   *  RooFormulaVars in the argset will automatically be added to the dataset
   *  and therewith converted to real variables in the dataset.
   *
   *  If the dataset has already been converted, this function will throw an
   *  exception (reconverting the dataset is not supported). If you just want to
   *  get the already converted dataset again, use dataset().
   *
   *  This function is deprecated and will be removed in a future version.
   *
   *  @param cut optional cut to reduce dataset
   *  @return the converted dataset
   */
  RooDataSet& ConvertToDataSet(const std::string& cut);
  
  /**
   *  @brief Get RooDataSet based on opened TTree and supplied argset and optional RooCmdArgs
   *
   *  The internal tree will be converted into a RooDataSet and this RooDataSet
   *  returned. The supplied argset has to be a subset of the variables used for
   *  (de)activation upon construction. Optional cuts or arguments can be 
   *  supplied as RooCmdArgs.
   *
   *  RooFormulaVars in the argset will automatically be added to the dataset
   *  and therewith converted to real variables in the dataset.
   *
   *  If the dataset has already been converted, this function will throw an
   *  exception (reconverting the dataset is not supported). If you just want to
   *  get the already converted dataset again, use dataset().
   *
   *  @param argset the RooArgSet used for conversion
   *  @param arg1 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg2 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg3 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg4 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg5 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg6 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @param arg7 optional RooCmdArg as in RooDataSet::RooDataSet(...)
   *  @return the converted dataset
   */
  RooDataSet& ConvertToDataSet(const RooArgSet& argset,
                               const RooCmdArg& arg1 = RooCmdArg(),
                               const RooCmdArg& arg2 = RooCmdArg(),
                               const RooCmdArg& arg3 = RooCmdArg(),
                               const RooCmdArg& arg4 = RooCmdArg(),
                               const RooCmdArg& arg5 = RooCmdArg(),
                               const RooCmdArg& arg6 = RooCmdArg(),
                               const RooCmdArg& arg7 = RooCmdArg());
  
  /**
   *  @brief (DEPRECATED) Get RooDataSet based on opened TTree and supplied argset and cut
   *
   *  The internal tree will be converted into a RooDataSet and this RooDataSet
   *  returned. The supplied argset has to be a subset of the variables used for
   *  (de)activation upon construction. An optional cut can be supplied to
   *  further reduce the dataset.
   *
   *  RooFormulaVars in the argset will automatically be added to the dataset
   *  and therewith converted to real variables in the dataset.
   *
   *  If the dataset has already been converted, this function will throw an
   *  exception (reconverting the dataset is not supported). If you just want to
   *  get the already converted dataset again, use dataset().
   *
   *  This function is deprecated and will be removed in a future version.
   *
   *  @param argset the RooArgSet used for conversion
   *  @param cut optional cut to reduce dataset
   *  @return the converted dataset
   */
  RooDataSet& ConvertToDataSet(const RooArgSet& argset, const std::string& cut);
  
  /**
   *  @brief Access variable in dataset
   *
   *  After the tuple has been converted to a RooDataSet, use this function to 
   *  access any RooRealVar in the RooDataSet. If not found or tuple not 
   *  converted to a dataset, an exception is thrown.
   *
   *  @param name name of the variable
   *  @return reference to the appropriate RooRealVar in the dataset
   */
  const RooRealVar& Var(const std::string& name) const;

  /**
   *  @brief Access variable in dataset
   *
   *  After the tuple has been converted to a RooDataSet, use this function to
   *  access any RooRealVar in the RooDataSet. If not found or tuple not
   *  converted to a dataset, an exception is thrown.
   *
   *  @param name name of the variable
   *  @return reference to the appropriate RooRealVar in the dataset
   */
  RooRealVar& Var(const std::string& name);
  
 protected:
  
 private:
  /**
   *  @brief Internal TFile pointer
   */
  TFile* file_;
  /**
   *  @brief Internal TTree pointer
   */
  TTree* tree_;
  /**
   *  @brief Internal RooArgSet pointer
   */
  RooArgSet* argset_;
  /**
   *  @brief Internal RooDataSet pointer
   */
  RooDataSet* dataset_;
}; // class EasyTuple
} // namespace utils
} // namespace doofit

#endif // DOOCORE_IO_EASYTUPLE_H
