#include "kinematic.h"

// from ROOT
#include "TLorentzVector.h"

namespace doocore {
namespace physics {
namespace kinematic {

double TwoBodyWrongMassHypothesisMass(const TLorentzVector& daughter1,
                                      const TLorentzVector& daughter2,
                                      double new_mass) {
  TLorentzVector daughter1_new;
  daughter1_new.SetVectM(daughter1.Vect(), new_mass);
  
  TLorentzVector mother = daughter1_new + daughter2;
  
  return mother.M();
}

} // namespace kinematic
} // namespace physics
} // namespace doofit