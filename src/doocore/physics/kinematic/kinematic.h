#ifndef DOOCORE_PHYSICS_KINEMATIC_H
#define DOOCORE_PHYSICS_KINEMATIC_H

// from STL

// from ROOT

// from RooFit

// forward decalarations
class TLorentzVector;

/**
 * @namespace doocore::physics
 * @brief DooCore physics functionality namespace
 *
 * This namespace contains all DooCore functionality that does physics 
 * calculations (kinematics and such).
 */

/**
 * @namespace doocore::physics::kinematic
 * @brief DooCore kinematic functionality namespace
 *
 * This namespace contains all DooCore functionality that does kinematic
 * calculations.
 */

namespace doocore {
namespace physics {
namespace kinematic {
  
/**
 *  @brief Two body wrong mass hypothesis mass calculator
 *
 *  Based on two TLorentzVectors (the daughter particles) a mother particle
 *  mass is calculated under the assumption that the first daughter's mass 
 *  hypothesis is wrong and needs to be replaced by another mass.
 *
 *  @param daughter1 first daughter to use with different mass hypothesis
 *  @param daughter2 second daughter
 *  @param new_mass new mass hypothesis for daughter1
 *  @return the mother mass
 */
double TwoBodyWrongMassHypothesisMass(const TLorentzVector& daughter1,
                                      const TLorentzVector& daughter2,
                                      double new_mass);
  
} // namespace kinematic
} // namespace physics
} // namespace doofit

#endif // DOOCORE_PHYSICS_KINEMATIC_H
