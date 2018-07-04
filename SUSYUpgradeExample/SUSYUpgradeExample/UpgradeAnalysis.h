#ifndef SUSYUpgradeExample_UpgradeAnalysis_H
#define SUSYUpgradeExample_UpgradeAnalysis_H

#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <EventLoopAlgs/AlgSelect.h>


#include <TLorentzVector.h>
#include <TH1F.h>
#include <TH2F.h>
#include <cmath>
#include <iostream>
#include "vector"
#include <map>
#include <TString.h>
#include <TTree.h>
#include "TRandom3.h"
#include "UpgradePerformanceFunctions/UpgradePerformanceFunctions.h"
#include "SUSYUpgradeExample/Particle.h"


using namespace std;

double M_T(TLorentzVector vec1, TLorentzVector vec2);

float getMT2(Particle p1, Particle p2, TLorentzVector met, double invisiblemass_1=0., double invisiblemass_2=0.);

void RemoveBad(vector<Particle> &vec1);

struct compare_pt {
  bool operator()(const Particle &left, const Particle &right) {
    return left.Pt() > right.Pt();
  }
};


class UpgradeAnalysis : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;
  UpgradePerformanceFunctions::UpgradeLayout layout = UpgradePerformanceFunctions::gold;
  float averageMuValue = 0.;


  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:

  //std::string outputName;
  //TTree *tree; //!
  // TH1 *myHist; //!

  const double GeV = 0.001;
  int m_eventCounter; //!

  int m_Nee; //!
  int m_Nmm; //!
  int m_Nem; //!

  TH1F* h_NEvents; //!
  //TH1F* h_CrossSection; //!
  //TH1F* h_mcChannel; //!
  //
  
  int m_eventNumber; //!
  //
  int m_elec_n; //!
  int m_muon_n; //!
  //
  vector<double> *m_elec_eta =0; //!
  vector<double> *m_elec_phi =0; //!
  vector<double> *m_elec_pt =0; //!
  vector<double> *m_elec_E =0; //!
  vector<double> *m_elec_charge =0; //!
  //
  vector<double> *m_muon_eta =0; //!
  vector<double> *m_muon_phi =0; //!
  vector<double> *m_muon_pt =0; //!
  vector<double> *m_muon_E =0; //!
  vector<double> *m_muon_charge =0; //!
  //
  vector<double> *m_jet_eta =0; //!
  vector<double> *m_jet_phi =0; //!
  vector<double> *m_jet_pt =0; //!
  vector<double> *m_jet_E =0; //!
  vector<double> *m_jet_id =0; //!
  //
  double m_MET_x; //!
  double m_MET_y; //!
  double m_MET_phi; //!
  //
  double m_gen_weight; //!
  double m_trigEff_e; //!
  double m_trigEff_m; //!
  double m_trigEff_ee; //!
  double m_trigEff_mm; //!
  double m_trigEff_em; //!
  //
  std::vector<TString> m_cuts;
  //
  TRandom3 m_random3; //!
  UpgradePerformanceFunctions *m_upgrade; //!
  //

  virtual EL::StatusCode  getTruthPhotons(vector<Particle> &GenPho);
  virtual EL::StatusCode  getTruthElectrons(vector<Particle> &GenEleMuo);
  virtual EL::StatusCode  getTruthMuons(vector<Particle> &GenEleMuo);
  virtual EL::StatusCode  getTruthHadronicTaus(vector<Particle> &GenHadTau);
  virtual EL::StatusCode  getTruthJets(vector<Particle> &GenJet, vector<Particle> &GenBJet);
  virtual EL::StatusCode  getTruthMET(TLorentzVector &m_GenMETTLV, double &m_GenMETSumet);
  //
  void SmearPhotons(vector<Particle> GenPho, vector<Particle> &SmearedPho);
  void SmearElectrons(vector<Particle> GenEleMuo, vector<Particle> &SmearedEleMuo);
  void SmearMuons(vector<Particle> GenEleMuo, vector<Particle> &SmearedEleMuo);
  void SmearHadTaus(vector<Particle> GenHadTau, vector<Particle> &SmearedHadTau);
  void SmearJets(vector<Particle> GenJet, vector<Particle> &SmearedJet);
  void SmearMET(TLorentzVector m_GenMETTLV, double m_GenMETSumet, TLorentzVector &m_SmearedMETTLV);
  //
  void ApplyPhotonFakes(vector<Particle> SmearedJet, vector<Particle> &SmearedPho);
  void ApplyElectronFakes(vector<Particle> SmearedJet, vector<Particle> &SmearedEleMuo);  
  void ApplyTauFakes(vector<Particle> SmearedJet, vector<Particle> &SmearedHadTau);
  void ApplyBtagging(vector<Particle> &SmearedJet);
  //
  void ApplyPtEtaThresholds(vector<Particle> &SmearedPho, vector<Particle> &SmearedEleMuo, vector<Particle> &SmearedHadTau, vector<Particle> &SmearedJet, vector<Particle> &SmearedBJet);
  void OverlapRemoval(vector<Particle> &SmearedPho, vector<Particle> &SmearedEleMuo, vector<Particle> &SmearedHadTau, vector<Particle> &SmearedJet, vector<Particle> &SmearedBJet);
  void ApplyIsolation(vector<Particle> &SmearedPho, vector<Particle> &SmearedEleMuo);
  void RemoveLowMassPairs(vector<Particle> &SmearedEleMuo);
  //
  void ApplyTriggerEfficiency(vector<Particle> &SmearedEleMuo, double &m_trig_Eff_e, double &m_trig_Eff_m, double &m_trig_Eff_ee, double &m_trig_Eff_mm, double &m_trig_Eff_em);
  //
  void calculateEventVariables(vector<Particle> &SmearedPho, vector<Particle> &SmearedEleMuo, vector<Particle> &SmearedHadTau, vector<Particle> &SmearedJet, vector<Particle> &SmearedBJet, TLorentzVector &m_SmearedMETTLV, double &m_met, bool &m_gotSFOS, double &m_mll, double &m_mT, double &m_ht, double &m_lt, double &m_meff, double &m_mT2);
  //
  void WriteEvent(vector<Particle> SmearedPho, vector<Particle> SmearedEleMuo, vector<Particle> SmearedHadTau, vector<Particle> SmearedJet, vector<Particle> SmearedBJet, TLorentzVector m_SmearedMETTLV, double m_evNumber, double m_genWeight, double m_trig_Eff_e, double m_trig_Eff_m, double m_trig_Eff_ee, double m_trig_Eff_mm, double m_trig_Eff_em);
  //
  void FillHistos(TString cutname);
  //

  // this is a standard constructor
  UpgradeAnalysis ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(UpgradeAnalysis, 1);
};

#endif
