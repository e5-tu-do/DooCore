#include "doocore/physics/kinematic/kinematic.h"

// from ROOT
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TStopwatch.h"

// from project
#include "doocore/io/MsgStream.h"
#include "doocore/physics/kinematic/kinematic.h"

using namespace doocore::physics::kinematic;
using namespace doocore::io;

int main() {
  double d1_px = -7.90399999999999977e+02;
  double d1_py = 3.91569999999999993e+02;
  double d1_pz = 8.83469000000000051e+03;
  double d1_E = 8.87971207048951874e+03;
  double d2_px = -1.95069999999999993e+02;
  double d2_py = 4.48259999999999991e+02;
  double d2_pz = 5.27313000000000011e+03;
  double d2_E = 5.29758144469693252e+03;
  double m_pi = 1.39570175170937631e+02;
  double m_K = 493.68;
  
  TLorentzVector d1(d1_px, d1_py, d1_pz, d1_E);
  TLorentzVector d2(d2_px, d2_py, d2_pz, d2_E);
  
  sdebug << "daughter 1 mass: " << d1.M() << endmsg;
  sdebug << "daughter 2 mass: " << d2.M() << endmsg;
  
  double mother_mass = MotherTwoBodyWrongMassHypothesis(d1,d2,m_pi).M();
  double wrong_mother_mass = MotherTwoBodyWrongMassHypothesis(d1,d2,m_K).M();
  double wrong_mother_mass2 = MotherTwoBodyDecay(d1_px, d1_py, d1_pz, m_K, d2_px, d2_py, d2_pz, m_pi).M();
  double wrong_mother_mass3 = MotherTwoBodyDecayMass(d1_px, d1_py, d1_pz, m_K, d2_px, d2_py, d2_pz, m_pi);
  
  sdebug << "correct mother mass: " << mother_mass << endmsg;
  sdebug << "wrong mother mass:   " << wrong_mother_mass << endmsg;
  sdebug << "wrong mother mass 2: " << wrong_mother_mass2 << endmsg;
  sdebug << "wrong mother mass 3: " << wrong_mother_mass3 << endmsg;

  TRandom3 rand;
  int num_steps=10000000;
  TStopwatch sw;
  sw.Start();
  TLorentzVector d1_n(d1_px, d1_py, d1_pz, d1_E);
  TLorentzVector d2_n(d2_px, d2_py, d2_pz, d2_E);
  for (int i=0; i<num_steps; ++i) {
    MotherTwoBodyWrongMassHypothesis(d1_n,d2_n,m_K).M();
  }
  sinfo << "time per call: " << sw.CpuTime()/num_steps << " s." << endmsg;
  
  sw.Reset();
  sw.Start();
  for (int i=0; i<num_steps; ++i) {
    MotherTwoBodyDecay(d1_px, d1_py, d1_pz, m_K, d2_px, d2_py, d2_pz, m_pi).M();
  }
  sinfo << "time per call: " << sw.CpuTime()/num_steps << " s." << endmsg;
  
  sw.Reset();
  sw.Start();
  for (int i=0; i<num_steps; ++i) {
    MotherTwoBodyDecayMass(d1_px, d1_py, d1_pz, m_K, d2_px, d2_py, d2_pz, m_pi);
  }
  sinfo << "time per call: " << sw.CpuTime()/num_steps << " s." << endmsg;
}