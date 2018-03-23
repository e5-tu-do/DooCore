#ifndef DOOCORE_PHYSICS_KINEMATIC_H
#define DOOCORE_PHYSICS_KINEMATIC_H

// from STL

// from ROOT
#include "TLorentzVector.h"
#include "TMath.h"

// from RooFit

// forward decalarations

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
 *  @brief Two body wrong mass hypothesis mother calculator
 *
 *  Based on two TLorentzVectors (the daughter particles) a mother particle
 *  is calculated under the assumption that the first daughter's mass hypothesis 
 *  is wrong and needs to be replaced by another mass.
 *
 *  Time per call (on my MacBook, -O0): 1.1466e-06 s
 *
 *  @param daughter1 first daughter to use with different mass hypothesis
 *  @param daughter2 second daughter
 *  @param new_mass new mass hypothesis for daughter1
 *  @return the mother
 */
TLorentzVector MotherTwoBodyWrongMassHypothesis(const TLorentzVector& daughter1,
                                                const TLorentzVector& daughter2,
                                                double new_mass) {
  TLorentzVector daughter1_new;
  daughter1_new.SetVectM(daughter1.Vect(), new_mass);
  
  return daughter1_new + daughter2;
}

/**
 *  @brief Three body wrong mass hypothesis mother calculator
 *
 *  Based on three TLorentzVectors (the daughter particles) a mother particle
 *  is calculated under the assumption that the first daughter's mass hypothesis 
 *  is wrong and needs to be replaced by another mass.
 *
 *  Time per call (on my MacBook, -O0): to be done
 *
 *  @param daughter1 first daughter to use with different mass hypothesis
 *  @param daughter2 second daughter
 *  @param daughter3 third daughter
 *  @param new_mass new mass hypothesis for daughter1
 *  @return the mother
 */
TLorentzVector MotherThreeBodyWrongMassHypothesis(const TLorentzVector& daughter1,
                                                  const TLorentzVector& daughter2,
                                                  const TLorentzVector& daughter3,
                                                  double new_mass) {
  TLorentzVector daughter1_new;
  daughter1_new.SetVectM(daughter1.Vect(), new_mass);
  
  return daughter1_new + daughter2 + daughter3;
}

/**
 *  @brief Four body wrong mass hypothesis mother calculator
 *
 *  Based on four TLorentzVectors (the daughter particles) a mother particle
 *  is calculated under the assumption that the first daughter's mass hypothesis 
 *  is wrong and needs to be replaced by another mass.
 *
 *  Time per call (on my MacBook, -O0): to be done
 *
 *  @param daughter1 first daughter to use with different mass hypothesis
 *  @param daughter2 second daughter
 *  @param daughter3 third daughter
 *  @param daughter4 fourth daughter
 *  @param new_mass new mass hypothesis for daughter1
 *  @return the mother
 */
TLorentzVector MotherFourBodyWrongMassHypothesis(const TLorentzVector& daughter1,
                                                  const TLorentzVector& daughter2,
                                                  const TLorentzVector& daughter3,
                                                  const TLorentzVector& daughter4,
                                                  double new_mass) {
  TLorentzVector daughter1_new;
  daughter1_new.SetVectM(daughter1.Vect(), new_mass);
  
  return daughter1_new + daughter2 + daughter3 + daughter4;
}
  
/**
 *  @brief Two body decay mother calculator
 *
 *  Based on two daughter particle properties a mother particle is calculated 
 *  under the assumption of daughter's masses.
 *
 *  Time per call (on my MacBook, -O0): 2.853e-07 s
 *
 *  @param d1_px first daughter px
 *  @param d1_py first daughter py
 *  @param d1_pz first daughter pz
 *  @param d1_m first daughter mass
 *  @param d2_px second daughter px
 *  @param d2_py second daughter py
 *  @param d2_pz second daughter pz
 *  @param d2_m second daughter mass
 *  @return the mother
 */
TLorentzVector MotherTwoBodyDecay(double d1_px, double d1_py,
                                  double d1_pz, double d1_m,
                                  double d2_px, double d2_py,
                                  double d2_pz, double d2_m) {
  // calculation by hand more efficient
  //  TLorentzVector daughter1;
  //  daughter1.SetXYZM(d1_px, d1_py, d1_pz, d1_m);
  //  TLorentzVector daughter2;
  //  daughter2.SetXYZM(d2_px, d2_py, d2_pz, d2_m);
  //
  //  return daughter1 + daughter2;
  
  double d1_E = TMath::Sqrt(d1_m*d1_m + d1_px*d1_px + d1_py*d1_py + d1_pz*d1_pz);
  double d2_E = TMath::Sqrt(d2_m*d2_m + d2_px*d2_px + d2_py*d2_py + d2_pz*d2_pz);
  
  double m_px = (d1_px+d2_px);
  double m_py = (d1_py+d2_py);
  double m_pz = (d1_pz+d2_pz);
  double m_E  = (d1_E+d2_E);
  
  return TLorentzVector(m_px, m_py, m_pz, m_E);
}

/**
 *  @brief Three body decay mother calculator
 *
 *  Based on three daughter particle properties a mother particle is calculated
 *  under the assumption of daughter's masses.
 *
 *  Time per call (on my MacBook, -O0): to be done
 *
 *  @param d1_px first daughter px
 *  @param d1_py first daughter py
 *  @param d1_pz first daughter pz
 *  @param d1_m first daughter mass
 *  @param d2_px second daughter px
 *  @param d2_py second daughter py
 *  @param d2_pz second daughter pz
 *  @param d2_m second daughter mass
 *  @param d3_px third daughter px
 *  @param d3_py third daughter py
 *  @param d3_pz third daughter pz
 *  @param d3_m third daughter mass
 *  @return the mother
 */
TLorentzVector MotherThreeBodyDecay(double d1_px, double d1_py,
                                    double d1_pz, double d1_m,
                                    double d2_px, double d2_py,
                                    double d2_pz, double d2_m,
                                    double d3_px, double d3_py,
                                    double d3_pz, double d3_m) {
  // calculation by hand more efficient
  //  TLorentzVector daughter1;
  //  daughter1.SetXYZM(d1_px, d1_py, d1_pz, d1_m);
  //  TLorentzVector daughter2;
  //  daughter2.SetXYZM(d2_px, d2_py, d2_pz, d2_m);
  //
  //  return daughter1 + daughter2;
  
  double d1_E = TMath::Sqrt(d1_m*d1_m + d1_px*d1_px + d1_py*d1_py + d1_pz*d1_pz);
  double d2_E = TMath::Sqrt(d2_m*d2_m + d2_px*d2_px + d2_py*d2_py + d2_pz*d2_pz);
  double d3_E = TMath::Sqrt(d3_m*d3_m + d3_px*d3_px + d3_py*d3_py + d3_pz*d3_pz);
  
  double m_px = (d1_px+d2_px+d3_px);
  double m_py = (d1_py+d2_py+d3_py);
  double m_pz = (d1_pz+d2_pz+d3_pz);
  double m_E  = (d1_E+d2_E+d3_E);
  
  return TLorentzVector(m_px, m_py, m_pz, m_E);
}

/**
 *  @brief Four body decay mother calculator
 *
 *  Based on four daughter particle properties a mother particle is calculated
 *  under the assumption of daughter's masses.
 *
 *  Time per call (on my MacBook, -O0): to be done
 *
 *  @param d1_px first daughter px
 *  @param d1_py first daughter py
 *  @param d1_pz first daughter pz
 *  @param d1_m first daughter mass
 *  @param d2_px second daughter px
 *  @param d2_py second daughter py
 *  @param d2_pz second daughter pz
 *  @param d2_m second daughter mass
 *  @param d3_px third daughter px
 *  @param d3_py third daughter py
 *  @param d3_pz third daughter pz
 *  @param d3_m third daughter mass
 *  @param d4_px fourth daughter px
 *  @param d4_py fourth daughter py
 *  @param d4_pz fourth daughter pz
 *  @param d4_m fourth daughter mass
 *  @return the mother
 */
TLorentzVector MotherFourBodyDecay(double d1_px, double d1_py,
                                    double d1_pz, double d1_m,
                                    double d2_px, double d2_py,
                                    double d2_pz, double d2_m,
                                    double d3_px, double d3_py,
                                    double d3_pz, double d3_m,
                                    double d4_px, double d4_py,
                                    double d4_pz, double d4_m) {
  // calculation by hand more efficient
  //  TLorentzVector daughter1;
  //  daughter1.SetXYZM(d1_px, d1_py, d1_pz, d1_m);
  //  TLorentzVector daughter2;
  //  daughter2.SetXYZM(d2_px, d2_py, d2_pz, d2_m);
  //
  //  return daughter1 + daughter2;
  
  double d1_E = TMath::Sqrt(d1_m*d1_m + d1_px*d1_px + d1_py*d1_py + d1_pz*d1_pz);
  double d2_E = TMath::Sqrt(d2_m*d2_m + d2_px*d2_px + d2_py*d2_py + d2_pz*d2_pz);
  double d3_E = TMath::Sqrt(d3_m*d3_m + d3_px*d3_px + d3_py*d3_py + d3_pz*d3_pz);
  double d4_E = TMath::Sqrt(d4_m*d4_m + d4_px*d4_px + d4_py*d4_py + d4_pz*d4_pz);
  
  double m_px = (d1_px+d2_px+d3_px+d4_px);
  double m_py = (d1_py+d2_py+d3_py+d4_py);
  double m_pz = (d1_pz+d2_pz+d3_pz+d4_pz);
  double m_E  = (d1_E+d2_E+d3_E+d4_E);
  
  return TLorentzVector(m_px, m_py, m_pz, m_E);
}
  
/**
 *  @brief Two body decay mother mass calculator
 *
 *  Based on two daughter particle properties a mother mass is calculated 
 *  under the assumption of daughter's masses. This function is more efficient 
 *  than the other functions calculating the mother's TLorentzVector.
 *
 *  Time per call (on my MacBook, -O0): 4.27e-08 s
 *
 *  @param d1_px first daughter px
 *  @param d1_py first daughter py
 *  @param d1_pz first daughter pz
 *  @param d1_m first daughter mass
 *  @param d2_px second daughter px
 *  @param d2_py second daughter py
 *  @param d2_pz second daughter pz
 *  @param d2_m second daughter mass
 *  @return the mother mass
 */
double MotherTwoBodyDecayMass(double d1_px, double d1_py,
                                  double d1_pz, double d1_m,
                                  double d2_px, double d2_py,
                                  double d2_pz, double d2_m) {
  double m_px = (d1_px+d2_px);
  double m_py = (d1_py+d2_py);
  double m_pz = (d1_pz+d2_pz);
  double m_E  = (TMath::Sqrt(d1_m*d1_m + d1_px*d1_px + d1_py*d1_py + d1_pz*d1_pz)
                +TMath::Sqrt(d2_m*d2_m + d2_px*d2_px + d2_py*d2_py + d2_pz*d2_pz));
    
  return TMath::Sqrt(m_E*m_E - (m_px*m_px + m_py*m_py + m_pz*m_pz));
}

/**
 *  @brief Three body decay mother mass calculator
 *
 *  Based on three daughter particle properties a mother mass is calculated
 *  under the assumption of daughter's masses. This function is more efficient 
 *  than the other functions calculating the mother's TLorentzVector.
 *
 *  Time per call (on my MacBook, -O0): to be done
 *
 *  @param d1_px first daughter px
 *  @param d1_py first daughter py
 *  @param d1_pz first daughter pz
 *  @param d1_m first daughter mass
 *  @param d2_px second daughter px
 *  @param d2_py second daughter py
 *  @param d2_pz second daughter pz
 *  @param d2_m second daughter mass
 *  @param d3_px third daughter px
 *  @param d3_py third daughter py
 *  @param d3_pz third daughter pz
 *  @param d3_m third daughter mass
 *  @return the mother mass
 */
double MotherThreeBodyDecayMass(double d1_px, double d1_py,
                                double d1_pz, double d1_m,
                                double d2_px, double d2_py,
                                double d2_pz, double d2_m,
                                double d3_px, double d3_py,
                                double d3_pz, double d3_m) {
  double m_px = (d1_px+d2_px+d3_px);
  double m_py = (d1_py+d2_py+d3_py);
  double m_pz = (d1_pz+d2_pz+d3_pz);
  double m_E  = (TMath::Sqrt(d1_m*d1_m + d1_px*d1_px + d1_py*d1_py + d1_pz*d1_pz)
                +TMath::Sqrt(d2_m*d2_m + d2_px*d2_px + d2_py*d2_py + d2_pz*d2_pz)
                +TMath::Sqrt(d3_m*d3_m + d3_px*d3_px + d3_py*d3_py + d3_pz*d3_pz));
    
  return TMath::Sqrt(m_E*m_E - (m_px*m_px + m_py*m_py + m_pz*m_pz));
}
  
/**
 *  @brief Four body decay mother mass calculator
 *
 *  Based on four daughter particle properties a mother mass is calculated
 *  under the assumption of daughter's masses. This function is more efficient 
 *  than the other functions calculating the mother's TLorentzVector.
 *
 *  Time per call (on my MacBook, -O0): to be done
 *
 *  @param d1_px first daughter px
 *  @param d1_py first daughter py
 *  @param d1_pz first daughter pz
 *  @param d1_m first daughter mass
 *  @param d2_px second daughter px
 *  @param d2_py second daughter py
 *  @param d2_pz second daughter pz
 *  @param d2_m second daughter mass
 *  @param d3_px third daughter px
 *  @param d3_py third daughter py
 *  @param d3_pz third daughter pz
 *  @param d3_m third daughter mass
 *  @param d4_px fourth daughter px
 *  @param d4_py fourth daughter py
 *  @param d4_pz fourth daughter pz
 *  @param d4_m fourth daughter mass
 *  @return the mother mass
 */
double MotherFourBodyDecayMass(double d1_px, double d1_py,
                               double d1_pz, double d1_m,
                               double d2_px, double d2_py,
                               double d2_pz, double d2_m,
                               double d3_px, double d3_py,
                               double d3_pz, double d3_m,
                               double d4_px, double d4_py,
                               double d4_pz, double d4_m) {
  double m_px = (d1_px+d2_px+d3_px+d4_px);
  double m_py = (d1_py+d2_py+d3_py+d4_py);
  double m_pz = (d1_pz+d2_pz+d3_pz+d4_pz);
  double m_E  = (TMath::Sqrt(d1_m*d1_m + d1_px*d1_px + d1_py*d1_py + d1_pz*d1_pz)
                +TMath::Sqrt(d2_m*d2_m + d2_px*d2_px + d2_py*d2_py + d2_pz*d2_pz)
                +TMath::Sqrt(d3_m*d3_m + d3_px*d3_px + d3_py*d3_py + d3_pz*d3_pz)
                +TMath::Sqrt(d4_m*d4_m + d4_px*d4_px + d4_py*d4_py + d4_pz*d4_pz));
    
  return TMath::Sqrt(m_E*m_E - (m_px*m_px + m_py*m_py + m_pz*m_pz));
}

double TwoBodyDecayAngle(double m_px, double m_py, double m_pz, double m_m,
                         double d_px, double d_py, double d_pz, double d_m,
                         double gd_px, double gd_py, double gd_pz, double gd_m) {
  double m_E = TMath::Sqrt(m_px*m_px + m_py*m_py + m_pz*m_pz + m_m*m_m);
  double d_E = TMath::Sqrt(d_px*d_px + d_py*d_py + d_pz*d_pz + d_m*d_m);
  double gd_E = TMath::Sqrt(gd_px*gd_px + gd_py*gd_py + gd_pz*gd_pz + gd_m*gd_m);

  double m_times_d = m_E*d_E - m_px*d_px - m_py*d_py - m_pz*d_pz;
  double m_times_gd = m_E*gd_E - m_px*gd_px - m_py*gd_py - m_pz*gd_pz;
  double d_times_gd = d_E*gd_E - d_px*gd_px - d_py*gd_py - d_pz*gd_pz;

  return (m_times_gd*d_m*d_m - m_times_d*d_times_gd)/TMath::Sqrt((m_times_d*m_times_d - d_m*d_m*m_m*m_m)*(d_times_gd*d_times_gd - d_m*d_m*gd_m*gd_m));
}

double ThreeBodyDecayAngle(double   m_px, double   m_py, double   m_pz, double   m_m,
                           double   d_px, double   d_py, double   d_pz, double   d_m,
                           double gd1_px, double gd1_py, double gd1_pz, double gd1_m,
                           double gd2_px, double gd2_py, double gd2_pz, double gd2_m) {
  double m_E = TMath::Sqrt(m_px*m_px + m_py*m_py + m_pz*m_pz + m_m*m_m);
  double d_E = TMath::Sqrt(d_px*d_px + d_py*d_py + d_pz*d_pz + d_m*d_m);
  double gd1_E = TMath::Sqrt(gd1_px*gd1_px + gd1_py*gd1_py + gd1_pz*gd1_pz + gd1_m*gd1_m);
  double gd2_E = TMath::Sqrt(gd2_px*gd2_px + gd2_py*gd2_py + gd2_pz*gd2_pz + gd2_m*gd2_m);

  double m_times_d = m_E*d_E - m_px*d_px - m_py*d_py - m_pz*d_pz;
  double l_px = d_py*(gd1_pz*gd2_E - gd1_E*gd2_pz) + d_pz*(gd1_E*gd2_py - gd1_py*gd2_E) + d_E*(gd1_py*gd2_pz - gd1_pz*gd2_py);
  double l_py = d_px*(gd1_E*gd2_pz - gd1_pz*gd2_E) + d_pz*(gd1_px*gd2_E - gd1_E*gd2_px) + d_E*(gd1_pz*gd2_px - gd1_px*gd2_pz);
  double l_pz = d_px*(gd1_py*gd2_E - gd1_E*gd2_py) + d_py*(gd1_E*gd2_px - gd1_px*gd2_E) + d_E*(gd1_px*gd2_py - gd1_py*gd2_px);
  double l_E = -(d_px*(gd1_pz*gd2_py - gd1_py*gd2_pz) + d_py*(gd1_px*gd2_pz - gd1_pz*gd2_px) + d_pz*(gd1_py*gd2_px - gd1_px*gd2_py));
  double m_times_l = m_E*l_E - m_px*l_px - m_py*l_py - m_pz*l_pz;
  double l_m2 = l_E*l_E - l_px*l_px - l_py*l_py - l_pz*l_pz;

  return d_m*m_times_l/TMath::Sqrt(-(m_times_d*m_times_d - m_m*m_m*d_m*d_m)*l_m2);
}

double AzimuthalAngleInDecayPlane(double   m_px, double   m_py, double   m_pz, double   m_m,
                                  double   d_px, double   d_py, double   d_pz, double   d_m,
                                  double gd1_px, double gd1_py, double gd1_pz, double gd1_m,
                                  double gd2_px, double gd2_py, double gd2_pz, double gd2_m) {
  double m_E = TMath::Sqrt(m_px*m_px + m_py*m_py + m_pz*m_pz + m_m*m_m);
  double d_E = TMath::Sqrt(d_px*d_px + d_py*d_py + d_pz*d_pz + d_m*d_m);
  double gd1_E = TMath::Sqrt(gd1_px*gd1_px + gd1_py*gd1_py + gd1_pz*gd1_pz + gd1_m*gd1_m);
  double gd2_E = TMath::Sqrt(gd2_px*gd2_px + gd2_py*gd2_py + gd2_pz*gd2_pz + gd2_m*gd2_m);

  double l_px = d_py*(gd1_pz*gd2_E - gd1_E*gd2_pz) + d_pz*(gd1_E*gd2_py - gd1_py*gd2_E) + d_E*(gd1_py*gd2_pz - gd1_pz*gd2_py);
  double l_py = d_px*(gd1_E*gd2_pz - gd1_pz*gd2_E) + d_pz*(gd1_px*gd2_E - gd1_E*gd2_px) + d_E*(gd1_pz*gd2_px - gd1_px*gd2_pz);
  double l_pz = d_px*(gd1_py*gd2_E - gd1_E*gd2_py) + d_py*(gd1_E*gd2_px - gd1_px*gd2_E) + d_E*(gd1_px*gd2_py - gd1_py*gd2_px);
  double l_E = -(d_px*(gd1_pz*gd2_py - gd1_py*gd2_pz) + d_py*(gd1_px*gd2_pz - gd1_pz*gd2_px) + d_pz*(gd1_py*gd2_px - gd1_px*gd2_py));
  double l_m2 = l_E*l_E - l_px*l_px - l_py*l_py - l_pz*l_pz;
  double m_times_d = m_E*d_E - m_px*d_px - m_py*d_py - m_pz*d_pz;
  double m_times_l = m_E*l_E - m_px*l_px - m_py*l_py - m_pz*l_pz;
  double m_times_gd1 = m_E*gd1_E - m_px*gd1_px - m_py*gd1_py - m_pz*gd1_pz;
  double l_times_gd1 = l_E*gd1_E - l_px*gd1_px - l_py*gd1_py - l_pz*gd1_pz;
  double gd1_times_d = gd1_E*d_E - gd1_px*d_px - gd1_py*d_py - gd1_pz*d_pz;
  double m_abs = TMath::Sqrt(pow(m_times_d/d_m,2)-m_m*m_m);
  double gd1_abs = TMath::Sqrt(pow(gd1_times_d/d_m,2) - gd1_m*gd1_m);
  double cos_phi = -m_times_l/(m_abs*TMath::Sqrt(-l_m2));
  double m_parallel_abs = m_abs*TMath::Sqrt(1 - cos_phi*cos_phi);
  double m_parallel_times_gd1 =  m_times_d *gd1_times_d/(d_m*d_m) + m_times_l*l_times_gd1/l_m2 - m_times_gd1;

  return TMath::ACos(-m_parallel_times_gd1/(m_parallel_abs*gd1_abs));
}

TLorentzVector Lvector(double   d_px, double   d_py, double   d_pz, double   d_m,
                       double gd1_px, double gd1_py, double gd1_pz, double gd1_m,
                       double gd2_px, double gd2_py, double gd2_pz, double gd2_m) {
  TLorentzVector  lvector;

  double d_E = TMath::Sqrt(d_px*d_px + d_py*d_py + d_pz*d_pz + d_m*d_m);
  double gd1_E = TMath::Sqrt(gd1_px*gd1_px + gd1_py*gd1_py + gd1_pz*gd1_pz + gd1_m*gd1_m);
  double gd2_E = TMath::Sqrt(gd2_px*gd2_px + gd2_py*gd2_py + gd2_pz*gd2_pz + gd2_m*gd2_m);

  double l_px = d_py*(gd1_pz*gd2_E - gd1_E*gd2_pz) + d_pz*(gd1_E*gd2_py - gd1_py*gd2_E) + d_E*(gd1_py*gd2_pz - gd1_pz*gd2_py);
  double l_py = d_px*(gd1_E*gd2_pz - gd1_pz*gd2_E) + d_pz*(gd1_px*gd2_E - gd1_E*gd2_px) + d_E*(gd1_pz*gd2_px - gd1_px*gd2_pz);
  double l_pz = d_px*(gd1_py*gd2_E - gd1_E*gd2_py) + d_py*(gd1_E*gd2_px - gd1_px*gd2_E) + d_E*(gd1_px*gd2_py - gd1_py*gd2_px);
  double l_E = -(d_px*(gd1_pz*gd2_py - gd1_py*gd2_pz) + d_py*(gd1_px*gd2_pz - gd1_pz*gd2_px) + d_pz*(gd1_py*gd2_px - gd1_px*gd2_py));
  lvector.SetPxPyPzE(l_px,l_py,l_pz,l_E);

  return lvector;
}

TLorentzVector Lvector_boosted(double   d_px, double   d_py, double   d_pz, double   d_m,
                               double gd1_px, double gd1_py, double gd1_pz, double gd1_m,
                               double gd2_px, double gd2_py, double gd2_pz, double gd2_m) {
  TLorentzVector daughter_boosted, granddaughter1_boosted, granddaughter2_boosted, l_boosted;
  daughter_boosted.SetXYZM(d_px,d_py,d_pz,d_m);
  granddaughter1_boosted.SetXYZM(gd1_px,gd1_py,gd1_pz,gd1_m);
  granddaughter2_boosted.SetXYZM(gd2_px,gd2_py,gd2_pz,gd2_m);

  TVector3 boost_vector = daughter_boosted.BoostVector();

  daughter_boosted.Boost(-boost_vector);
  granddaughter1_boosted.Boost(-boost_vector);
  granddaughter2_boosted.Boost(-boost_vector);

  double d_E_boosted = daughter_boosted.E();
  double d_px_boosted = daughter_boosted.Px();
  double d_py_boosted = daughter_boosted.Py();
  double d_pz_boosted = daughter_boosted.Pz();

  double gd1_E_boosted = granddaughter1_boosted.E();
  double gd1_px_boosted = granddaughter1_boosted.Px();
  double gd1_py_boosted = granddaughter1_boosted.Py();
  double gd1_pz_boosted = granddaughter1_boosted.Pz();

  double gd2_E_boosted = granddaughter2_boosted.E();
  double gd2_px_boosted = granddaughter2_boosted.Px();
  double gd2_py_boosted = granddaughter2_boosted.Py();
  double gd2_pz_boosted = granddaughter2_boosted.Pz();

  double l_px_boosted = d_py_boosted*(gd1_pz_boosted*gd2_E_boosted - gd1_E_boosted*gd2_pz_boosted) + d_pz_boosted*(gd1_E_boosted*gd2_py_boosted - gd1_py_boosted*gd2_E_boosted) + d_E_boosted*(gd1_py_boosted*gd2_pz_boosted - gd1_pz_boosted*gd2_py_boosted);
  double l_py_boosted = d_px_boosted*(gd1_E_boosted*gd2_pz_boosted - gd1_pz_boosted*gd2_E_boosted) + d_pz_boosted*(gd1_px_boosted*gd2_E_boosted - gd1_E_boosted*gd2_px_boosted) + d_E_boosted*(gd1_pz_boosted*gd2_px_boosted - gd1_px_boosted*gd2_pz_boosted);
  double l_pz_boosted = d_px_boosted*(gd1_py_boosted*gd2_E_boosted - gd1_E_boosted*gd2_py_boosted) + d_py_boosted*(gd1_E_boosted*gd2_px_boosted - gd1_px_boosted*gd2_E_boosted) + d_E_boosted*(gd1_px_boosted*gd2_py_boosted - gd1_py_boosted*gd2_px_boosted);
  double l_E_boosted = -(d_px_boosted*(gd1_pz_boosted*gd2_py_boosted - gd1_py_boosted*gd2_pz_boosted) + d_py_boosted*(gd1_px_boosted*gd2_pz_boosted - gd1_pz_boosted*gd2_px_boosted) + d_pz_boosted*(gd1_py_boosted*gd2_px_boosted - gd1_px_boosted*gd2_py_boosted));
  l_boosted.SetPxPyPzE(l_px_boosted,l_py_boosted,l_pz_boosted,l_E_boosted);

  return l_boosted;
}

TLorentzVector Daughtervector_boosted(double d_px, double d_py, double d_pz, double d_m) {
  TLorentzVector daughter_boosted;
  daughter_boosted.SetXYZM(d_px,d_py,d_pz,d_m);
  TVector3 boost_vector = daughter_boosted.BoostVector();
  daughter_boosted.Boost(-boost_vector);

  return daughter_boosted;
}

double FourMomentumProduct(double u_x, double u_y, double u_z, double u_E,
                           double v_x, double v_y, double v_z, double v_E) {
  return u_E*v_E - u_x*v_x - u_y*v_y - u_z*v_z;
}

double OpeningAngleInRestFrame(double  m_px, double  m_py, double  m_pz, double  m_m,
                               double d1_px, double d1_py, double d1_pz, double d1_m,
                               double d2_px, double d2_py, double d2_pz, double d2_m) {
  double m_E = TMath::Sqrt(m_px*m_px + m_py*m_py + m_pz*m_pz + m_m*m_m);
  double d1_E = TMath::Sqrt(d1_px*d1_px + d1_py*d1_py + d1_pz*d1_pz + d1_m*d1_m);
  double d2_E = TMath::Sqrt(d2_px*d2_px + d2_py*d2_py + d2_pz*d2_pz + d2_m*d2_m);

  double m_times_d1 = FourMomentumProduct(m_px, m_py, m_pz, m_E, d1_px, d1_py, d1_pz, d1_E);
  double m_times_d2 = FourMomentumProduct(m_px, m_py, m_pz, m_E, d2_px, d2_py, d2_pz, d2_E);
  double d1_times_d2 = FourMomentumProduct(d1_px, d1_py, d1_pz, d1_E, d2_px, d2_py, d2_pz, d2_E);

  double d1_abs = TMath::Sqrt(pow(m_times_d1/m_m,2) - d1_m*d1_m);
  double d2_abs = TMath::Sqrt(pow(m_times_d2/m_m,2) - d2_m*d2_m);

  return TMath::ACos((m_times_d1*m_times_d2/(m_m*m_m) - d1_times_d2)/(d1_abs*d2_abs));
}

/**
 *  @brief Pointing angle between two vertices calculator
 *
 *  With this function the angle between the momentum vector of a
 *  particle and the connection vector between its production and
 *  its decay vertex is calculated.
 *
 *  @param x_pv x coordinate of production vertex
 *  @param y_pv y coordinate of production vertex
 *  @param z_pv z coordinate of production vertex
 *  @param x_sv x coordinate of decay vertex
 *  @param y_sv y coordinate of decay vertex
 *  @param z_sv z coordinate of decay vertex
 *  @param px particle's px
 *  @param py particle's py
 *  @param pz particle's pz
 *  @return the absolute value of the cosine of the angle
 */
double PointingAngleBetweenVertices(double x_pv, double y_pv, double z_pv,
                                    double x_sv, double y_sv, double z_sv,
                                    double px, double py, double pz) {
  double x_diff = x_pv - x_sv;
  double y_diff = y_pv - y_sv;
  double z_diff = z_pv - z_sv;

  double diff_abs = TMath::Sqrt(pow(x_diff,2) + pow(y_diff,2) + pow(z_diff,2));

  double p = TMath::Sqrt(pow(px,2) + pow(py,2) + pow(pz,2));

  double numerator = x_diff * px + y_diff * py + z_diff * pz;
  double denominator = diff_abs * p;

  return abs(numerator) / denominator;
}

} // namespace kinematic
} // namespace physics
} // namespace doofit

#endif // DOOCORE_PHYSICS_KINEMATIC_H
