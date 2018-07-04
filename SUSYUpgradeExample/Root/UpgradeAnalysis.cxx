#include <SUSYUpgradeExample/UpgradeAnalysis.h>
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include <AsgTools/MessageCheck.h>
#include "xAODEventInfo/EventInfo.h"
#include <SUSYUpgradeExample/MT2_ROOT.h>
#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODCore/AuxContainerBase.h"

// this is needed to distribute the algorithm to the workers
ClassImp(UpgradeAnalysis)



UpgradeAnalysis :: UpgradeAnalysis ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  layout = UpgradePerformanceFunctions::gold; 
  averageMuValue = 0.;
}



EL::StatusCode UpgradeAnalysis :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  // let's initialize the algorithm to use the xAODRootAccess package
  //cout << " < setupJob " << endl;

  job.useXAOD ();

  ANA_CHECK_SET_TYPE (EL::StatusCode); // set type of return code you are expecting (add to top of each function once)

  ANA_CHECK(xAOD::Init());

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode UpgradeAnalysis :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  //cout << " < histInitialize " << endl;


  h_NEvents = new TH1F("h_NEvents", "", 1, -0.5, 0.5);
  wk()->addOutput (h_NEvents);
  h_CrossSection = new TH1F("h_CrossSection", "", 1, -0.5, 0.5);
  wk()->addOutput (h_CrossSection);
  h_mcChannel = new TH1F("h_mcChannel", "", 1, -0.5, 0.5);
  wk()->addOutput (h_mcChannel);

  /*
  m_cuts.push_back("NoCuts");
  m_cuts.push_back("2L");
  m_cuts.push_back("2L_MET100");
  m_cuts.push_back("3L");
  m_cuts.push_back("3L_Meff100");

  for( unsigned int i = 0; i < m_cuts.size(); i++) {

    h_NEvents[m_cuts[i]] = new TH1F("h_NEvents_"+ m_cuts[i], "", 1, -0.5, 0.5);  
    wk()->addOutput (h_NEvents[m_cuts[i]]);

    h_MET[m_cuts[i]] = new TH1F("h_MET_"+ m_cuts[i], "", 50, 0, 1000); 
    h_MET[m_cuts[i]]->SetXTitle("#it{E}_{T}^{miss} [GeV]");      h_MET[m_cuts[i]]->SetYTitle("Events / 20 GeV");
    wk()->addOutput (h_MET[m_cuts[i]]);

    h_MSFOS[m_cuts[i]] = new TH1F("h_MSFOS_"+ m_cuts[i], "", 100, 0, 1000); 
    h_MSFOS[m_cuts[i]]->SetXTitle("m_{SFOS} [GeV]");      h_MSFOS[m_cuts[i]]->SetYTitle("Events / 10 GeV");
    wk()->addOutput (h_MSFOS[m_cuts[i]]);

    h_Meff[m_cuts[i]] = new TH1F("h_Meff_"+ m_cuts[i], "", 75, 0, 1500); 
    h_Meff[m_cuts[i]]->SetXTitle("m_{eff} [GeV]");      h_Meff[m_cuts[i]]->SetYTitle("Events / 20 GeV");
    wk()->addOutput (h_Meff[m_cuts[i]]);

    h_MT[m_cuts[i]] = new TH1F("h_MT_"+ m_cuts[i], "", 50, 0, 1000); 
    h_MT[m_cuts[i]]->SetXTitle("m_{T} [GeV]");      h_MT[m_cuts[i]]->SetYTitle("Events / 20 GeV");
    wk()->addOutput (h_MT[m_cuts[i]]);

    h_MT2[m_cuts[i]] = new TH1F("h_MT2_"+ m_cuts[i], "", 50, 0, 1000); 
    h_MT2[m_cuts[i]]->SetXTitle("m_{T2} [GeV]");      h_MT2[m_cuts[i]]->SetYTitle("Events / 20 GeV");
    wk()->addOutput (h_MT2[m_cuts[i]]);

    //

    h_NPho[m_cuts[i]] = new TH1F("h_NPho_"+ m_cuts[i], "", 6, -0.5, 5.5); 
    h_NPho[m_cuts[i]]->SetXTitle("Photon multiplicity");      h_NPho[m_cuts[i]]->SetYTitle("Events");
    wk()->addOutput (h_NPho[m_cuts[i]]);

    h_NElec[m_cuts[i]] = new TH1F("h_NElec_"+ m_cuts[i], "", 6, -0.5, 5.5); 
    h_NElec[m_cuts[i]]->SetXTitle("Electron multiplicity");      h_NElec[m_cuts[i]]->SetYTitle("Events");
    wk()->addOutput (h_NElec[m_cuts[i]]);

    h_NMuon[m_cuts[i]] = new TH1F("h_NMuon_"+ m_cuts[i], "", 6, -0.5, 5.5); 
    h_NMuon[m_cuts[i]]->SetXTitle("Muon multiplicity");      h_NMuon[m_cuts[i]]->SetYTitle("Events");
    wk()->addOutput (h_NMuon[m_cuts[i]]);

    h_NTau[m_cuts[i]] = new TH1F("h_NTau_"+ m_cuts[i], "", 6, -0.5, 5.5); 
    h_NTau[m_cuts[i]]->SetXTitle("Tau multiplicity");      h_NTau[m_cuts[i]]->SetYTitle("Events");
    wk()->addOutput (h_NTau[m_cuts[i]]);

    h_NJet[m_cuts[i]] = new TH1F("h_NJet_"+ m_cuts[i], "", 20, -0.5, 19.5); 
    h_NJet[m_cuts[i]]->SetXTitle("Jet multiplicity");      h_NJet[m_cuts[i]]->SetYTitle("Events");
    wk()->addOutput (h_NJet[m_cuts[i]]);

    h_PtPhos[m_cuts[i]] = new TH1F("h_PtPhos_"+ m_cuts[i], "", 50, 0, 1000); 
    h_PtPhos[m_cuts[i]]->SetXTitle("Photon p_{T} [GeV]");      h_PtPhos[m_cuts[i]]->SetYTitle("Events / 20 GeV");
    wk()->addOutput (h_PtPhos[m_cuts[i]]);

    h_PtElecs[m_cuts[i]] = new TH1F("h_PtElecs_"+ m_cuts[i], "", 50, 0, 1000); 
    h_PtElecs[m_cuts[i]]->SetXTitle("Electron p_{T} [GeV]");      h_PtElecs[m_cuts[i]]->SetYTitle("Events / 20 GeV");
    wk()->addOutput (h_PtElecs[m_cuts[i]]);

    h_PtMuons[m_cuts[i]] = new TH1F("h_PtMuons_"+ m_cuts[i], "", 50, 0, 1000); 
    h_PtMuons[m_cuts[i]]->SetXTitle("Muon p_{T} [GeV]");      h_PtMuons[m_cuts[i]]->SetYTitle("Events / 20 GeV");
    wk()->addOutput (h_PtMuons[m_cuts[i]]);

    h_PtTaus[m_cuts[i]] = new TH1F("h_PtTaus_"+ m_cuts[i], "", 50, 0, 1000); 
    h_PtTaus[m_cuts[i]]->SetXTitle("Tau p_{T} [GeV]");      h_PtTaus[m_cuts[i]]->SetYTitle("Events / 20 GeV");
    wk()->addOutput (h_PtTaus[m_cuts[i]]);

    h_PtJets[m_cuts[i]] = new TH1F("h_PtJets_"+ m_cuts[i], "", 50, 0, 1000); 
    h_PtJets[m_cuts[i]]->SetXTitle("Jet p_{T} [GeV]");      h_PtJets[m_cuts[i]]->SetYTitle("Events / 20 GeV");
    wk()->addOutput (h_PtJets[m_cuts[i]]);

  }
  */
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode UpgradeAnalysis :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode UpgradeAnalysis :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode UpgradeAnalysis :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.
  //cout << " < initialize " << endl;
  
  xAOD::TEvent* event = wk()->xaodEvent();
  ANA_CHECK_SET_TYPE (EL::StatusCode);

  // as a check, let's see the number of events in our xAOD
  Info("initialize()", "Number of events = %lli", event->getEntries() ); // print long long int
  m_eventCounter = 0;
  m_Nee = 0;
  m_Nem = 0;
  m_Nmm = 0;

  averageMuValue = 200;  // change this!
  // initialise UpgradePerformanceFunctions
  m_random3.SetSeed(1); // for local smearing
  Info("initialize()", "Setting up UpgradePerformanceFunctions with mu=%f", averageMuValue); 

  // Initializations specific to UpgradePerformanceFunctions
  m_upgrade = new UpgradePerformanceFunctions();
  //m_upgrade->setLayout(UpgradePerformanceFunctions::gold); // change this!
  m_upgrade->setLayout(UpgradePerformanceFunctions::Step1p6); // change this!
  m_upgrade->setAvgMu(averageMuValue);    
  m_upgrade->setUseHGTD0(false);         
  m_upgrade->setUseHGTD1(false);                 
  cout << "Layout is " << m_upgrade->getLayout() << ", and mu value is " << m_upgrade->getAvgMu() << endl;
  //
  m_upgrade->setPhotonWorkingPoint(UpgradePerformanceFunctions::tightPhoton);
  m_upgrade->setPhotonRandomSeed(1);
  m_upgrade->setElectronWorkingPoint(UpgradePerformanceFunctions::tightElectron);
  m_upgrade->setElectronRandomSeed(1);
  m_upgrade->setMuonWorkingPoint(UpgradePerformanceFunctions::tightMuon);
  m_upgrade->setTauRandomSeed(1);
  m_upgrade->setJetRandomSeed(1);
  //m_upgrade->setFlavourTaggingCalibrationFilename("UpgradePerformanceFunctions/flavor_tags_v1.1.root"); // mv1 tag, gold only, mu=140,200
  m_upgrade->setUseHGTD0(false);
  m_upgrade->setFlavourTaggingCalibrationFilename("UpgradePerformanceFunctions/flavor_tags_v2.0.root"); // mv2c10 tag, mu=200 only
  m_upgrade->setMETRandomSeed(1);
  m_upgrade->loadMETHistograms("UpgradePerformanceFunctions/sumetPU_mu200_ttbar_gold.root");

  m_upgrade->setJetRandomSeed(1);
  m_upgrade->setPileupUseTrackConf(true); 
  m_upgrade->setPileupRandomSeed(1);
  m_upgrade->setPileupEfficiencyScheme(UpgradePerformanceFunctions::PileupEff::PU);
  m_upgrade->setPileupEff(0.02);
  m_upgrade->setPileupJetPtThresholdMeV(30000.);
  m_upgrade->setPileupTemplatesPath("/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/UpgradePerformanceFunctions/");
  //
  

  EL::NTupleSvc *ntuple = EL::getNTupleSvc (wk(), "output");
  ntuple->tree()->Branch("eventNumber", &m_eventNumber, "eventNumber/I");
  //
  ntuple->tree()->Branch("elec_eta", &m_elec_eta);
  ntuple->tree()->Branch("elec_phi", &m_elec_phi);
  ntuple->tree()->Branch("elec_pt", &m_elec_pt);
  ntuple->tree()->Branch("elec_E", &m_elec_E);
  ntuple->tree()->Branch("elec_charge", &m_elec_charge);
  ntuple->tree()->Branch("elec_n", &m_elec_n, "elec_n/I");
  //
  ntuple->tree()->Branch("muon_eta", &m_muon_eta);
  ntuple->tree()->Branch("muon_phi", &m_muon_phi);
  ntuple->tree()->Branch("muon_pt", &m_muon_pt);
  ntuple->tree()->Branch("muon_E", &m_muon_E);
  ntuple->tree()->Branch("muon_charge", &m_muon_charge);
  ntuple->tree()->Branch("muon_n", &m_muon_n, "muon_n/I");
  //
  ntuple->tree()->Branch("jet_eta", &m_jet_eta);
  ntuple->tree()->Branch("jet_phi", &m_jet_phi);
  ntuple->tree()->Branch("jet_pt", &m_jet_pt);
  ntuple->tree()->Branch("jet_E", &m_jet_E);
  ntuple->tree()->Branch("jet_id", &m_jet_id);
  //
  ntuple->tree()->Branch("MET_x", &m_MET_x, "MET_x/D");
  ntuple->tree()->Branch("MET_y", &m_MET_y, "MET_y/D");
  ntuple->tree()->Branch("MET_phi", &m_MET_phi, "MET_phi/D");
  //
  ntuple->tree()->Branch("gen_weight", &m_gen_weight, "gen_weight/D");
  ntuple->tree()->Branch("trigEff_e", &m_trigEff_e, "trigEff_e/D");
  ntuple->tree()->Branch("trigEff_m", &m_trigEff_m, "trigEff_m/D");
  ntuple->tree()->Branch("trigEff_ee", &m_trigEff_ee, "trigEff_ee/D");
  ntuple->tree()->Branch("trigEff_mm", &m_trigEff_mm, "trigEff_mm/D");
  ntuple->tree()->Branch("trigEff_em", &m_trigEff_em, "trigEff_em/D");
  //


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode UpgradeAnalysis :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.
  //cout << " < execute " << endl;
    
  xAOD::TEvent* event = wk()->xaodEvent();
  ANA_CHECK_SET_TYPE (EL::StatusCode);

  if( (m_eventCounter %1000) ==0 ) Info("execute()", "Event number = %i", m_eventCounter );
  m_eventCounter++;

  //----------------------------
  // Event information
  //---------------------------
  //cout << " < getting eventInfo" << endl;
  const xAOD::EventInfo* eventInfo = 0;
  ANA_CHECK(event->retrieve( eventInfo, "EventInfo"));  

  /* -------------- event objects ---------------- */
  
  vector<Particle> GenPho;  
  vector<Particle> GenEleMuo;  
  vector<Particle> GenHadTau; 
  vector<Particle> GenJet; 
  vector<Particle> GenBJet; 
  TLorentzVector   m_GenMETTLV; 
  double           m_GenMETSumet=-1.; 
  vector<Particle> SmearedPho;  
  vector<Particle> SmearedEleMuo;  
  vector<Particle> SmearedHadTau; 
  vector<Particle> SmearedJet; 
  vector<Particle> SmearedBJet; 
  TLorentzVector   m_SmearedMETTLV; 
  // event variables
  double              m_genWeight=0.; 
  int                 m_mcChannel=0; 
  int                 m_evNumber=0; 
  double              m_xs=0.; 
  double              m_trig_Eff_e=0.; 
  double              m_trig_Eff_m=0.; 
  double              m_trig_Eff_ee=0.; 
  double              m_trig_Eff_mm=0.; 
  double              m_trig_Eff_em=0.; 
  bool                m_gotSFOS=false; 
  double              m_mll=0.; 
  double              m_met=0.; 
  double              m_mT=0.; 
  double              m_ht=0.; 
  double              m_lt=0.; 
  double              m_meff=0.; 
  double              m_mT2=0.; 
  
  // generator weight
  m_genWeight = 1.;
  if (eventInfo->mcEventWeights().size() > 0) m_genWeight = (*(eventInfo->mcEventWeights().begin()));
  // channel
  m_mcChannel = eventInfo->runNumber();
  // cross-section in pb
  if( m_mcChannel == 410000 ) m_xs = 824.51 * 0.54383; //   ttbar notallhad
  if( m_mcChannel == 410009 ) m_xs = 824.51 * 0.10539; //   ttbar dilep
  if( m_mcChannel == 407099 ) m_xs = 824.51 * 0.10462; //   ttbar, at least 1L filter, MET>100 
  if( m_mcChannel == 147806 ) m_xs = 2057.; // Zee
  if( m_mcChannel == 147807 ) m_xs = 2058.; // Zmumu
  if( m_mcChannel == 147808 ) m_xs = 2057.5; // Ztautau
  if( m_mcChannel == 361100 ) m_xs = 12171.; // W+ ev
  if( m_mcChannel == 361101 ) m_xs = 12171.; // W+ muv
  if( m_mcChannel == 361102 ) m_xs = 12171.; // W+ tauv
  if( m_mcChannel == 361103 ) m_xs = 8979.9; // W- ev
  if( m_mcChannel == 361104 ) m_xs = 8979.9; // W- muv
  if( m_mcChannel == 361105 ) m_xs = 8979.9; // W- tauv
  if( m_mcChannel == 363716 ) m_xs = 1.0428; // VV llll
  if( m_mcChannel == 363718 ) m_xs = 4.3512; // VV lllv
  if( m_mcChannel == 363720 ) m_xs = 13.825; // VV llvv   llqq??
  if( m_mcChannel == 410066 ) m_xs = 0.20502; // ttW Np0
  if( m_mcChannel == 410067 ) m_xs = 0.17151; // ttW Np1
  if( m_mcChannel == 410068 ) m_xs = 0.17014; // ttW Np2
  if( m_mcChannel == 410081 ) m_xs = 9.9458E-03; // ttWW
  if( m_mcChannel == 410013 ) m_xs = 40.279; // Wt
  
  // C1C1 WW - centrally produced. xs taken from AMI -- can we find NLL?
  if( m_mcChannel == 202515 ) m_xs = 11.6119 * (0.109*3) * 0.32821; //   100,0
  if( m_mcChannel == 202519 ) m_xs = 5.09052 * (0.109*3) * 0.34586; //   125,0
  if( m_mcChannel == 202524 ) m_xs = 2.61231 * (0.109*3) * 0.32629; //   150,0
  if( m_mcChannel == 202530 ) m_xs = 0.902569 * (0.109*3) * 0.38818; //   200,0
  if( m_mcChannel == 202539 ) m_xs = 0.387534 * (0.109*3) * 0.40672; //   250,0
  if( m_mcChannel == 202550 ) m_xs = 0.190159 * (0.109*3) * 0.42545; //   300,0
  if( m_mcChannel == 202556 ) m_xs = 0.0586311 * (0.109*3) * 0.45028; //   400,0
  if( m_mcChannel == 202564 ) m_xs = 0.0221265 * (0.109*3) * 0.47271; //   500,0

  // C1N2 slep - privately produced. xs taken from MG -- can we find NLL? Yes but only up to 1.5 TeV
  if( m_mcChannel == 900000 ) m_xs =  1.27E-03 * 0.42; //   1000,0
  if( m_mcChannel == 900001 ) m_xs =  1.27E-03 * 0.42; //   1000,250
  if( m_mcChannel == 900002 ) m_xs =  1.27E-03 * 0.42; //   1000,500
  if( m_mcChannel == 900003 ) m_xs =  1.27E-03 * 0.42; //   1000,750
  
  if( m_mcChannel == 900004 ) m_xs =  0.321E-03 * 0.42; //   1250,0
  if( m_mcChannel == 900005 ) m_xs =  0.321E-03 * 0.43; //   1250,250
  if( m_mcChannel == 900006 ) m_xs =  0.321E-03 * 0.42; //   1250,500
  if( m_mcChannel == 900007 ) m_xs =  0.321E-03 * 0.42; //   1250,750
  if( m_mcChannel == 900008 ) m_xs =  0.321E-03 * 0.41; //   1250,1000

  if( m_mcChannel == 900009 ) m_xs =  9.08E-05 * 0.42; //   1500,0
  if( m_mcChannel == 900010 ) m_xs =  9.08E-05 * 0.42; //   1500,500
  if( m_mcChannel == 900011 ) m_xs =  9.08E-05 * 0.42; //   1500,1000

  if( m_mcChannel == 900012 ) m_xs =  2.75E-05 * 0.43; //   1750,0
  if( m_mcChannel == 900013 ) m_xs =  2.75E-05 * 0.42; //   1750,500
  if( m_mcChannel == 900014 ) m_xs =  2.75E-05 * 0.42; //   1750,1000
  if( m_mcChannel == 900015 ) m_xs =  2.75E-05 * 0.42; //   1750,1500
  
  if( m_mcChannel == 900016 ) m_xs =  8.70E-06 * 0.42; //   2000,0
  if( m_mcChannel == 900017 ) m_xs =  8.70E-06 * 0.42; //   2000,500
  if( m_mcChannel == 900018 ) m_xs =  8.70E-06 * 0.43; //   2000,1000
  if( m_mcChannel == 900019 ) m_xs =  8.70E-06 * 0.42; //   2000,1500
    
  // event number
  m_evNumber = eventInfo->eventNumber();
  // Do we want something to check the SUSY production process?

  /*
  cout << "Testing vector problem" << endl;
  vector<Particle> Bob;
  Bob.clear();
  for( int i=0; i<10; i++ ){
    Particle bob; bob.SetPtEtaPhiM(1.,2.,3.,4.);
    Bob.push_back(bob);
    cout << "Bob pushing back " << i << endl;
  }
  cout << "Bob size " << Bob.size() << endl;
  cout << "sorting... " << endl;
  sort(Bob.begin(), Bob.end(), compare_pt());
  cout << "done sorting " << endl;
  */

  
  /* ===============================================================
     ===== Get Gen objects from DAOD_Truth containers ==============
     =============================================================== */  
  //cout << " < clear vectors " << endl;
  GenPho.clear();
  //cout << "here?" << endl;
  GenEleMuo.clear();
  GenHadTau.clear();
  GenJet.clear();
  GenBJet.clear();
  //cout << " < clear vectors -- done " << endl;
  
  // Photons
  //getTruthPhotons(GenPho); 

  /* Electrons and Muons */
  //cout << " < GenEleMuo: size " << GenEleMuo.size() << endl;
  getTruthElectrons(GenEleMuo);
  //cout << " < GenEleMuo: size " << GenEleMuo.size() << endl;
  getTruthMuons(GenEleMuo);
  //cout << " < GenEleMuo: size " << GenEleMuo.size() << endl;

  /* Hadronically decaying Taus (visible part only) */
  //getTruthHadronicTaus(GenHadTau);

  /* Jets */
  getTruthJets(GenJet, GenBJet);
  
  /* MET */
  getTruthMET(m_GenMETTLV, m_GenMETSumet);

  //sort(GenPho.begin(), GenPho.end(), compare_pt());   //Sort objects in vector by Pt
  //cout << " < sort GenEleMuo " << GenEleMuo.size() << endl;
  sort(GenEleMuo.begin(), GenEleMuo.end(), compare_pt()); 
  //sort(GenHadTau.begin(), GenHadTau.end(), compare_pt());
  //cout << " < sort GenJet " << GenJet.size() << endl;
  sort(GenJet.begin(), GenJet.end(), compare_pt());
  //cout << " < sort GenBJet " << GenBJet.size() << endl;
  sort(GenBJet.begin(), GenBJet.end(), compare_pt());

  //cout << " < done sorting " << endl;
  /*
  cout << "  Before Smearing  ";
  for( unsigned int i=0; i<GenEleMuo.size(); i++){
    cout<< GenEleMuo[i].pdgid << " (" << GenEleMuo[i].Pt()*GeV << ") \t";
  }
  cout << endl;
  */
  /* ===============================================================
     ===== Smear objects with UpgradePerformanceFunctions ==========
     =============================================================== */
  //SmearedPho.clear();
  SmearedEleMuo.clear();
  //SmearedHadTau.clear();
  SmearedJet.clear();
  SmearedBJet.clear();
  //
  //SmearPhotons(GenPho, SmearedPho);
  SmearElectrons(GenEleMuo, SmearedEleMuo);
  SmearMuons(GenEleMuo, SmearedEleMuo);
  //SmearHadTaus(GenHadTau, SmearedHadTau);
  SmearJets(GenJet, SmearedJet); 
  SmearMET(m_GenMETTLV, m_GenMETSumet, m_SmearedMETTLV);
  
  //
  //ApplyPhotonFakes(SmearedJet, SmearedPho);
  ApplyElectronFakes(SmearedJet, SmearedEleMuo);
  //ApplyTauFakes(SmearedJet, SmearedHadTau);
  ApplyBtagging(SmearedJet);
  //
  sort(SmearedPho.begin(), SmearedPho.end(), compare_pt());   //Sort objects in vector by Pt
  sort(SmearedEleMuo.begin(), SmearedEleMuo.end(), compare_pt());   
  //sort(SmearedHadTau.begin(), SmearedHadTau.end(), compare_pt());   
  sort(SmearedJet.begin(), SmearedJet.end(), compare_pt());   
  sort(SmearedBJet.begin(), SmearedBJet.end(), compare_pt());   

  /* ===============================================================
     ===== Apply Pt and Eta thresholds =============================
     =============================================================== */
  /*cout << "  Before Pt/Eta  ";
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    cout<< SmearedEleMuo[i].pdgid << " (" << SmearedEleMuo[i].Pt()*GeV << ", " << SmearedEleMuo[i].Eta()<< ") \t";
  }
  cout << endl;*/
  ApplyPtEtaThresholds(SmearedPho, SmearedEleMuo, SmearedHadTau, SmearedJet, SmearedBJet);
  /* ===============================================================
     ===== Perform basic overlap removal ===========================
     =============================================================== */
  /*cout << "  Before OR  ";
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    cout<< SmearedEleMuo[i].pdgid << " (" << SmearedEleMuo[i].Pt()*GeV << ") \t";
  }
  cout << endl;*/
  OverlapRemoval(SmearedPho, SmearedEleMuo, SmearedHadTau, SmearedJet, SmearedBJet);
  /* ===============================================================
     ===== Signal leptons: isolation, remove low mass pairs ========
     =============================================================== */
  /*cout << "  Before Isolation  ";
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    cout<< SmearedEleMuo[i].pdgid << " (" << SmearedEleMuo[i].Pt()*GeV << ") \t";
  }
  cout << endl;*/
  ApplyIsolation(SmearedPho, SmearedEleMuo);
  /*cout << "  Before LowMass pairs  ";
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    cout<< SmearedEleMuo[i].pdgid << " (" << SmearedEleMuo[i].Pt()*GeV << ") \t";
  }
  cout << endl;*/
  RemoveLowMassPairs(SmearedEleMuo);

  /* ===============================================================
     ===== Trigger effiency.    Analysis dependent, so function ====
     ===== sets m_trigEff to 1 if not edited =======================
     =============================================================== */
  ApplyTriggerEfficiency(SmearedEleMuo, m_trig_Eff_e, m_trig_Eff_m, m_trig_Eff_ee, m_trig_Eff_mm, m_trig_Eff_em);

  /* ===============================================================
     ===== Event variables =========================================
     =============================================================== */
  calculateEventVariables(SmearedPho, SmearedEleMuo, SmearedHadTau, SmearedJet, SmearedBJet, m_SmearedMETTLV, m_met, m_gotSFOS, m_mll, m_mT, m_ht, m_lt, m_meff, m_mT2);

  /*cout << "  Final leptons  ";
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    cout<< SmearedEleMuo[i].pdgid << " (" << SmearedEleMuo[i].Pt()*GeV << ") \t";
  }
  cout << endl;*/

  /* ===============================================================
     ===== Fill histograms for each selection stage ================
     =============================================================== */
  /*
  FillHistos("NoCuts");
  //
  if( SmearedEleMuo.size()>=2 ) FillHistos("2L");
  if( SmearedEleMuo.size()>=2 && m_met*GeV>100 ) FillHistos("2L_MET100");
  //
  if( SmearedEleMuo.size()>=3 ) FillHistos("3L");
  if( SmearedEleMuo.size()>=3 && m_meff*GeV>100 ) FillHistos("3L_Meff100");
  */

  /* ===============================================================
     ===== Fill event info histograms and branches =================
     =============================================================== */

  h_NEvents->Fill(0.0, m_genWeight);
  if( m_eventCounter ==1 ){
    h_mcChannel->Fill(0.0, m_mcChannel);
    h_CrossSection->Fill(0.0, m_xs);
  }
  bool write_event = true;
  if( (m_mcChannel == 410009 || m_mcChannel == 410000) && m_GenMETTLV.Et()/1000.>100 ) write_event=false; // don't write out the high MET events for the ttbar dilepton or notallhad samples.

  if( write_event && SmearedEleMuo.size()>=2 ) WriteEvent(SmearedPho, SmearedEleMuo, SmearedHadTau, SmearedJet, SmearedBJet, m_SmearedMETTLV, m_evNumber, m_genWeight, m_trig_Eff_e, m_trig_Eff_m, m_trig_Eff_ee, m_trig_Eff_mm, m_trig_Eff_em);



  if( SmearedEleMuo.size()>=2 ){
    int nele=0; int nmuo=0; 
    for(int i=0; i<SmearedEleMuo.size(); i++){
      if(fabs(SmearedEleMuo[i].pdgid)==11 ) nele++;
      if(fabs(SmearedEleMuo[i].pdgid)==13 ) nmuo++;
    }
    if( nele>=2 ) m_Nee++;
    if( nele==1 ) m_Nem++;
    if( nele==0 ) m_Nmm++;
  }



  
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode UpgradeAnalysis :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  //cout << " < postExecute " << endl;
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode UpgradeAnalysis :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.
  //cout << " < finalize " << endl;
  
  xAOD::TEvent* event = wk()->xaodEvent();
  ANA_CHECK_SET_TYPE (EL::StatusCode);

  cout << " Analysed " << m_eventCounter << " events" << endl;
  cout << "    ee " << m_Nee << endl;
  cout << "    mm " << m_Nmm << endl;
  cout << "    em " << m_Nem << endl;

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode UpgradeAnalysis :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.
  //cout << " < histFinalize " << endl;
    
  return EL::StatusCode::SUCCESS;
}


void UpgradeAnalysis::FillHistos(TString cutname)
{
  //cout << " < FillHistos " << endl;
  /*
  if (h_NEvents.find(cutname) == h_NEvents.end()) {
    cout << "Error, cut " << cutname << " not found" << endl;
  }

  // number of events
  h_NEvents[cutname]->Fill( 0.0, m_trigEff);
  
  // event variables
  h_MET[cutname]->Fill( m_met*GeV , m_trigEff);
  h_MSFOS[cutname]->Fill( m_mll*GeV , m_trigEff);
  h_Meff[cutname]->Fill( m_meff*GeV , m_trigEff);
  h_MT[cutname]->Fill( m_mT*GeV , m_trigEff);
  h_MT2[cutname]->Fill( m_mT2*GeV , m_trigEff);

  // photon, lepton, tau and jet multiplicities
  h_NPho[cutname]->Fill( SmearedPho.size() , m_trigEff);
  int nele=0; int nmuo=0;
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    if( fabs(SmearedEleMuo->at(i).pdgid)==11 ) nele++;
    if( fabs(SmearedEleMuo->at(i).pdgid)==13 ) nmuo++;
  }
  h_NElec[cutname]->Fill( nele , m_trigEff);
  h_NMuon[cutname]->Fill( nmuo , m_trigEff);
  h_NTau[cutname]->Fill( SmearedHadTau.size() , m_trigEff);
  h_NJet[cutname]->Fill( SmearedJet.size() , m_trigEff);
  // photon, lepton, tau and jet pTs
  for( unsigned int i=0; i<SmearedPho.size(); i++) h_PtPhos[cutname]->Fill( SmearedPho->at(i).Pt()*GeV , m_trigEff);
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    if( fabs(SmearedEleMuo->at(i).pdgid)==11 ) h_PtElecs[cutname]->Fill( SmearedEleMuo->at(i).Pt()*GeV , m_trigEff);
    if( fabs(SmearedEleMuo->at(i).pdgid)==13 ) h_PtMuons[cutname]->Fill( SmearedEleMuo->at(i).Pt()*GeV , m_trigEff);
  }
  for( unsigned int i=0; i<SmearedHadTau.size(); i++) h_PtTaus[cutname]->Fill( SmearedHadTau->at(i).Pt()*GeV , m_trigEff);
  for( unsigned int i=0; i<SmearedJet.size(); i++) h_PtJets[cutname]->Fill( SmearedJet->at(i).Pt()*GeV , m_trigEff);
  */  
  return;
}




void UpgradeAnalysis::WriteEvent(vector<Particle> SmearedPho, vector<Particle> SmearedEleMuo, vector<Particle> SmearedHadTau, vector<Particle> SmearedJet, vector<Particle> SmearedBJet, TLorentzVector m_SmearedMETTLV, double m_evNumber, double m_genWeight, double m_trig_Eff_e, double m_trig_Eff_m, double m_trig_Eff_ee, double m_trig_Eff_mm, double m_trig_Eff_em )
{

  
  //cout << " < WriteEvent " << endl;
  m_eventNumber = m_evNumber;
  // elec: pt, eta, phi, E, charge
  int nele=0;
  m_elec_pt->clear();
  m_elec_eta->clear();
  m_elec_phi->clear();
  m_elec_E->clear();
  m_elec_charge->clear();
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    if( fabs(SmearedEleMuo[i].pdgid)!=11 ) continue;
    m_elec_pt->push_back(SmearedEleMuo[i].Pt());
    m_elec_eta->push_back(SmearedEleMuo[i].Eta());
    m_elec_phi->push_back(SmearedEleMuo[i].Phi());
    m_elec_E->push_back(SmearedEleMuo[i].E());
    if( SmearedEleMuo[i].pdgid>0 ) m_elec_charge->push_back(-1.);
    else m_elec_charge->push_back(1.);
    nele++;
  }
  m_elec_n = nele;
  // muon: pt, eta, phi, E, charge
  int nmuo=0;
  m_muon_pt->clear();
  m_muon_eta->clear();
  m_muon_phi->clear();
  m_muon_E->clear();
  m_muon_charge->clear();
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    if( fabs(SmearedEleMuo[i].pdgid)!=13 ) continue;
    m_muon_pt->push_back(SmearedEleMuo[i].Pt());
    m_muon_eta->push_back(SmearedEleMuo[i].Eta());
    m_muon_phi->push_back(SmearedEleMuo[i].Phi());
    m_muon_E->push_back(SmearedEleMuo[i].E());
    if( SmearedEleMuo[i].pdgid>0 ) m_muon_charge->push_back(-1.);
    else m_muon_charge->push_back(1.);
    nmuo++;
  }
  m_muon_n = nmuo;
  // jet: pt, eta, phi, E, weight
  m_jet_pt->clear();
  m_jet_eta->clear();
  m_jet_phi->clear();
  m_jet_E->clear();
  m_jet_id->clear();
  for( unsigned int i=0; i<SmearedJet.size(); i++){
    m_jet_pt->push_back(SmearedJet[i].Pt());
    m_jet_eta->push_back(SmearedJet[i].Eta());
    m_jet_phi->push_back(SmearedJet[i].Phi());
    m_jet_E->push_back(SmearedJet[i].E());
    m_jet_id->push_back(SmearedJet[i].pdgid);
  }
  // met: x, y, phi
  m_MET_x = m_SmearedMETTLV.Px();
  m_MET_y = m_SmearedMETTLV.Py();
  m_MET_phi = m_SmearedMETTLV.Phi();
  // weights: gen, triggers
  m_gen_weight = m_genWeight;
  m_trigEff_e = m_trig_Eff_e;
  m_trigEff_m = m_trig_Eff_m;
  m_trigEff_ee = m_trig_Eff_ee;
  m_trigEff_mm = m_trig_Eff_mm;
  m_trigEff_em = m_trig_Eff_em;

  
  EL::NTupleSvc *ntuple = EL::getNTupleSvc (wk(), "output");
  ntuple->tree()->Fill();
  return;
}

 
EL::StatusCode UpgradeAnalysis::getTruthPhotons(vector<Particle> &GenPho){
  //cout << " < getTruthPhotons " << endl;
  xAOD::TEvent* event = wk()->xaodEvent();
  const xAOD::TruthParticleContainer* xTruthPhotons = NULL;
  ANA_CHECK( event->retrieve( xTruthPhotons, "TruthPhotons"));
  xAOD::TruthParticleContainer::const_iterator itr;
  for( itr = xTruthPhotons->begin(); itr != xTruthPhotons->end(); itr++){
    if( (*itr)->status()!=1 ) continue;
    if( (*itr)->pt()*GeV < 10 ) continue;
    if( fabs((*itr)->eta()) > 2.5 ) continue;
    Particle thisPart;
    thisPart.SetPxPyPzE( (*itr)->px(), (*itr)->py(), (*itr)->pz(), (*itr)->e());
    thisPart.pdgid = (*itr)->pdgId();
    thisPart.nprong = 0;
    thisPart.etcone20 = (*itr)->auxdata<float>("etcone20");
    thisPart.ptcone30 = (*itr)->auxdata<float>("ptcone30");
    thisPart.Good = true;
    GenPho.push_back(thisPart);
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode UpgradeAnalysis::getTruthElectrons(vector<Particle> &GenEleMuo){
  //cout << " < getTruthElectrons: size " << GenEleMuo.size() << endl;
  xAOD::TEvent* event = wk()->xaodEvent();
  const xAOD::TruthParticleContainer* xTruthElectrons = NULL;
  ANA_CHECK( event->retrieve( xTruthElectrons, "TruthElectrons"));
  xAOD::TruthParticleContainer::const_iterator itr;
  for( itr = xTruthElectrons->begin(); itr != xTruthElectrons->end(); itr++){
    if((*itr)->status()!=1 ) continue;
    if( (*itr)->pt()*GeV < 5 ) continue;
    if( fabs((*itr)->eta()) > 2.7 ) continue;
    Particle thisPart;
    thisPart.SetPxPyPzE( (*itr)->px(), (*itr)->py(), (*itr)->pz(), (*itr)->e());
    thisPart.pdgid = (*itr)->pdgId();
    thisPart.nprong = 0;
    thisPart.etcone20 = (*itr)->auxdata<float>("etcone20");
    thisPart.ptcone30 = (*itr)->auxdata<float>("ptcone30");
    thisPart.Good = true;
    GenEleMuo.push_back(thisPart);
    //cout << " <  < GetTruthElectrons " << GenEleMuo.size() << "\t" << thisPart.Pt()*GeV << "\t" << thisPart.Eta() << "\t" << thisPart.pdgid << endl; 
  }
  //cout << " < end of GetTruthElectrons: size " << GenEleMuo.size() << endl;
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode UpgradeAnalysis::getTruthMuons(vector<Particle> &GenEleMuo){
  //cout << " < getTruthMuons:  size " << GenEleMuo.size() << endl;
  xAOD::TEvent* event = wk()->xaodEvent();
  const xAOD::TruthParticleContainer* xTruthMuons = NULL;
  ANA_CHECK( event->retrieve( xTruthMuons, "TruthMuons"));
  xAOD::TruthParticleContainer::const_iterator itr;
  for( itr = xTruthMuons->begin(); itr != xTruthMuons->end(); itr++){
    if((*itr)->status()!=1 ) continue;
    if( (*itr)->pt()*GeV < 5 ) continue;
    if( fabs((*itr)->eta()) > 2.7 ) continue;
    Particle thisPart;
    thisPart.SetPxPyPzE( (*itr)->px(), (*itr)->py(), (*itr)->pz(), (*itr)->e());
    thisPart.pdgid = (*itr)->pdgId();
    thisPart.nprong = 0;
    thisPart.etcone20 = (*itr)->auxdata<float>("etcone20");
    thisPart.ptcone30 = (*itr)->auxdata<float>("ptcone30");
    thisPart.Good = true;
    GenEleMuo.push_back(thisPart);
    //cout << " <  < GetTruthMuons " << GenEleMuo.size() << "\t" << thisPart.Pt()*GeV << "\t" << thisPart.Eta() << "\t" << thisPart.pdgid << endl; 
  }
  //cout << " < end of GetTruthMuons: size " << GenEleMuo.size() << endl;
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode UpgradeAnalysis::getTruthHadronicTaus(vector<Particle> &GenHadTau){
  //cout << " < getTruthTaus " << endl;
  xAOD::TEvent* event = wk()->xaodEvent();
  const xAOD::TruthParticleContainer* xTruthTaus = NULL;
  ANA_CHECK( event->retrieve( xTruthTaus, "TruthTaus"));
  xAOD::TruthParticleContainer::const_iterator itr;
  for( itr = xTruthTaus->begin(); itr != xTruthTaus->end(); itr++){
    if( (*itr)->auxdata<char>("IsHadronicTau") != 1 ) continue;
    int n_prong = (*itr)->auxdata<unsigned long>("numChargedPion");
    if( n_prong!=1 && n_prong!=3 ) continue;
    if( (*itr)->pt()*GeV < 15 ) continue;
    if( fabs((*itr)->eta()) > 2.7 ) continue;
    Particle thisPart;
    thisPart.SetPtEtaPhiM( (*itr)->auxdata<double>("pt_vis"), (*itr)->auxdata<double>("eta_vis"), (*itr)->auxdata<double>("phi_vis"), (*itr)->auxdata<double>("m_vis"));
    thisPart.pdgid = (*itr)->pdgId();
    thisPart.nprong = n_prong;
    thisPart.etcone20 = 0.;
    thisPart.ptcone30 = 0.;
    thisPart.Good = true;
    GenHadTau.push_back(thisPart);
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode UpgradeAnalysis::getTruthJets(vector<Particle> &GenJet, vector<Particle> &GenBJet){
  //cout << " < getTruthJets " << endl;
  xAOD::TEvent* event = wk()->xaodEvent();
  const xAOD::JetContainer* xTruthJets = 0;
  ANA_CHECK(event->retrieve( xTruthJets, "AntiKt4TruthJets" ));
  xAOD::JetContainer::const_iterator jet_itr = xTruthJets->begin();
  xAOD::JetContainer::const_iterator jet_end = xTruthJets->end();
  for( ; jet_itr != jet_end; ++jet_itr ) {
    if( (*jet_itr)->pt()*GeV < 10 ) continue;
    if( fabs((*jet_itr)->eta()) > 2.7 ) continue;
    // is bjet?
    bool isbjet=false;
    if(  fabs((*jet_itr)->auxdata<int>("ConeTruthLabelID"))==5 ) isbjet=true;

    Particle thisPart;
    thisPart.SetPtEtaPhiM( (*jet_itr)->pt(), (*jet_itr)->eta(), (*jet_itr)->phi(), (*jet_itr)->m());
    thisPart.pdgid = fabs((*jet_itr)->auxdata<int>("ConeTruthLabelID"));
    thisPart.nprong = 0;
    thisPart.etcone20 = 0.;
    thisPart.ptcone30 = 0.;
    thisPart.Good = true;
    GenJet.push_back(thisPart);
    
    if( isbjet ) GenBJet.push_back(thisPart);
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode UpgradeAnalysis::getTruthMET(TLorentzVector &m_GenMETTLV, double &m_GenMETSumet){
  //cout << " < getTruthMET " << endl;
  xAOD::TEvent* event = wk()->xaodEvent();
  const xAOD::MissingETContainer* xTruthMET = NULL;
  ANA_CHECK( event->retrieve( xTruthMET, "MET_Truth"));
  m_GenMETTLV.SetXYZM( (*xTruthMET)["NonInt"]->mpx(), (*xTruthMET)["NonInt"]->mpy(), 0,0); 
  m_GenMETSumet = (*xTruthMET)["NonInt"]->sumet();
  return EL::StatusCode::SUCCESS;
}

void UpgradeAnalysis::SmearPhotons(vector<Particle> GenPho, vector<Particle> &SmearedPho){
  //cout << " < SmearPhotons " << endl;
  SmearedPho.clear();

  for( unsigned int i=0; i<GenPho.size(); i++) {
    //cout << "photon " << GenPho[i].Pt()*GeV << "\t" << GenPho[i].Eta() << endl;
    // smear energy
    double fac = m_upgrade->getPhotonSmearedEnergy(GenPho[i].E(),GenPho[i].Eta())/GenPho[i].E();
    Particle newPhoton = GenPho[i];
    newPhoton.SetPtEtaPhiM(GenPho[i].Pt()*fac,GenPho[i].Eta(),GenPho[i].Phi(),GenPho[i].M());

    // Apply efficiency
    float eff = m_upgrade->getPhotonEfficiency(newPhoton.Et() );
    m_random3.SetSeed( (int)(1e+5*fabs( newPhoton.Phi() ) ) );	
    double x = m_random3.Rndm();
    // if passes eff, keep electron
    if (x < eff) { 
      SmearedPho.push_back(newPhoton);
    }
  }
  return;
}

void UpgradeAnalysis::SmearElectrons(vector<Particle> GenEleMuo, vector<Particle> &SmearedEleMuo){
  //cout << " < SmearElectrons " << endl;
  SmearedEleMuo.clear();

  for( unsigned int i=0; i<GenEleMuo.size(); i++) {
    if( fabs(GenEleMuo[i].pdgid)!=11 ) continue;
    //cout << "electron " << GenEleMuo[i].Pt()*GeV << "\t" << GenEleMuo[i].Eta() << endl;
    // smear energy
    double fac = m_upgrade->getElectronSmearedEnergy(GenEleMuo[i].E(),GenEleMuo[i].Eta())/GenEleMuo[i].E();
    Particle newElectron = GenEleMuo[i];
    newElectron.SetPtEtaPhiM(GenEleMuo[i].Pt()*fac,GenEleMuo[i].Eta(),GenEleMuo[i].Phi(),GenEleMuo[i].M());

    // Apply efficiency
    float eff = m_upgrade->getElectronEfficiency(newElectron.Et(), newElectron.Eta() );
    m_random3.SetSeed( (int)(1e+5*fabs( newElectron.Phi() ) ) );	
    double x = m_random3.Rndm();
    // if passes eff, keep electron
    if (x < eff) { 
      SmearedEleMuo.push_back(newElectron);
    }
  }
  return;
}

void UpgradeAnalysis::SmearMuons(vector<Particle> GenEleMuo, vector<Particle> &SmearedEleMuo){
  //cout << " < SmearMuons " << endl;
  for( unsigned int i=0; i<GenEleMuo.size(); i++ ){
    if( fabs(GenEleMuo[i].pdgid)!=13 ) continue;
    //cout << "muon " << GenEleMuo[i].Pt()*GeV << "\t" << GenEleMuo[i].Eta() << endl;
    // smear q/pT to allow for muon charge flip
    double charge =-1;
    if( GenEleMuo[i].pdgid < 0 ) charge=+1; 
    float qoverpt = charge / GenEleMuo[i].Pt();
    float muonQOverPtResolution = m_upgrade->getMuonQOverPtResolution( GenEleMuo[i].Pt(),  GenEleMuo[i].Eta());
    m_random3.SetSeed( (int)(1e+5*fabs( GenEleMuo[i].Phi() ) ) );	
    qoverpt += m_random3.Gaus(0., muonQOverPtResolution);
    float muonSmearedPt = fabs(1./qoverpt);
    Particle newMuon = GenEleMuo[i];
    newMuon.SetPtEtaPhiM( muonSmearedPt, GenEleMuo[i].Eta(), GenEleMuo[i].Phi(), GenEleMuo[i].M() );
    newMuon.pdgid = 13;
    if( qoverpt > 0 ) newMuon.pdgid=-13;
    // Apply efficiency
    float eff = m_upgrade->getMuonEfficiency(newMuon.Pt(), newMuon.Eta() );
    m_random3.SetSeed( (int)(1e+5*fabs( newMuon.Phi() ) ) );	
    double x = m_random3.Rndm();
    // if passes eff, keep muon
    if (x < eff) { 
      SmearedEleMuo.push_back(newMuon);
    }
  }
  return;
}

void UpgradeAnalysis::SmearHadTaus(vector<Particle> GenHadTau, vector<Particle> &SmearedHadTau){
  //cout << " < SmearTaus " << endl;
  SmearedHadTau.clear();
  // smear and eff
  for( unsigned int i=0; i<GenHadTau.size(); i++) {
    //cout << "tau " << GenHadTau[i].Pt()*GeV << "\t" << GenHadTau[i].Eta() << endl;
    // smear energy
    double fac = m_upgrade->getTauSmearedEnergy(GenHadTau[i].E(),GenHadTau[i].Eta(), GenHadTau[i].nprong)/GenHadTau[i].E();
    Particle newTau = GenHadTau[i];
    newTau.SetPtEtaPhiM(GenHadTau[i].Pt()*fac,GenHadTau[i].Eta(),GenHadTau[i].Phi(),GenHadTau[i].M());

    if( newTau.Pt()*GeV<20 || fabs(newTau.Eta())>2.5 ) continue;
    // Apply efficiency
    float eff = m_upgrade->getTauEfficiency(newTau.Et(), newTau.Eta(), GenHadTau[i].nprong, 1 ); // last arg =0,1,2 for loose, medium, tight
    m_random3.SetSeed( (int)(1e+5*fabs( newTau.Phi() ) ) );	
    double x = m_random3.Rndm();
    // if passes eff, keep electron
    if (x < eff) { 
      SmearedHadTau.push_back(newTau);
    }
  }  
  return;
}

void UpgradeAnalysis::SmearJets(vector<Particle> GenJet, vector<Particle> &SmearedJet){
  //cout << " < SmearJets " << endl;
  SmearedJet.clear();

  // Add in pileup jets
  std::vector<TLorentzVector> pujets;
  pujets = m_upgrade->getPileupJets();
  for( unsigned int i = 0; i < pujets.size(); i++) {
    Particle pujet;
    pujet.SetPtEtaPhiM(pujets[i].Pt(), pujets[i].Eta(), pujets[i].Phi(), pujets[i].M());
    if (pujet.Pt() < m_upgrade->getPileupJetPtThresholdMeV() || fabs(pujet.Eta()) > 3) continue;
    float trackEff = m_upgrade->getTrackJetConfirmEff(pujet.Pt(), pujet.Eta(), "PU");
    m_random3.SetSeed( (int)(1e+5*fabs( pujet.Phi() ) ) );	
    float puProb = m_random3.Uniform(1.0);
    if (puProb < trackEff){
      pujet.Good=true;
      pujet.pdgid=-1; //identify PU jets with -1
      SmearedJet.push_back(pujet);
    }
  }

  // smear truth jets
  for( unsigned int i=0; i<GenJet.size(); i++) {
    //cout << "jet " << GenJet[i].Pt()*GeV << "\t" << GenJet[i].Eta() << endl;
    // smear energy
    double smeared_pt = m_upgrade->getJetSmearedEnergy(GenJet[i].Pt(),GenJet[i].Eta(), true);
    Particle newJet = GenJet[i];
    newJet.SetPtEtaPhiM(smeared_pt,GenJet[i].Eta(),GenJet[i].Phi(),GenJet[i].M());

    // Apply efficiency
    float trackEff = m_upgrade->getTrackJetConfirmEff(newJet.Pt(), newJet.Eta(), "HS");
    m_random3.SetSeed( (int)(1e+5*fabs( newJet.Phi() ) ) );	
    float hsProb = m_random3.Uniform(1.0);
    if (hsProb < trackEff){
      SmearedJet.push_back(newJet);
    }
  }

  return;
}

void UpgradeAnalysis::ApplyPhotonFakes(vector<Particle> SmearedJet, vector<Particle> &SmearedPho){
  //cout << " < ApplyPhotonFakes " << endl;
  // Apply photon fakes
  for( unsigned int i=0; i<SmearedJet.size(); i++ ){
    float fakeeff = m_upgrade->getPhotonFakeRate(SmearedJet[i].Et());
    m_random3.SetSeed( (int)(1e+5*fabs( SmearedJet[i].Phi() ) ) );	
    float fakeProb = m_random3.Rndm();
    if( fakeProb < fakeeff ){ // add jet to SmearedPho as an photon
      double fac = m_upgrade->getPhotonFakeRescaledET(SmearedJet[i].Et()) / SmearedJet[i].Et();
      if (fac < 0.) fac = 0.; // protection
      Particle fakePho; 
      fakePho.SetPtEtaPhiM(SmearedJet[i].Pt()*fac, SmearedJet[i].Eta(), SmearedJet[i].Phi(), SmearedJet[i].M() );
      fakePho.pdgid=22;
      fakePho.Good = true;
      SmearedPho.push_back(fakePho);
    }
  }
  return;
}
void UpgradeAnalysis::ApplyElectronFakes(vector<Particle> SmearedJet, vector<Particle> &SmearedEleMuo){
    //cout << " < ApplyElectronFakes " << endl;
  // Apply electron fakes
  for( unsigned int i=0; i<SmearedJet.size(); i++ ){
    float fakeeff = m_upgrade->getElectronFakeRate(SmearedJet[i].Et(), SmearedJet[i].Eta());
    m_random3.SetSeed( (int)(1e+5*fabs( SmearedJet[i].Phi() ) ) );	
    float fakeProb = m_random3.Rndm();
    double fakeCharge = m_random3.Rndm();
    if( fakeProb < fakeeff ){ // add jet to SmearedEleMuo as an electron
      double fac = m_upgrade->getElectronFakeRescaledEnergy(SmearedJet[i].E(), SmearedJet[i].Eta()) / SmearedJet[i].E();
      if (fac < 0.) fac = 0.; // protection
      Particle fakeEle; 
      fakeEle.SetPtEtaPhiM(SmearedJet[i].Pt()*fac, SmearedJet[i].Eta(), SmearedJet[i].Phi(), SmearedJet[i].M() );
      fakeEle.pdgid=-11;
      if( fakeCharge>0.5 ) fakeEle.pdgid=+11;
      fakeEle.Good = true;
      SmearedEleMuo.push_back(fakeEle);
    }
  }
  return;
}
void UpgradeAnalysis::ApplyTauFakes(vector<Particle> SmearedJet, vector<Particle> &SmearedHadTau){
  //cout << " < ApplyTauFakes " << endl;
  // Apply tau fake rates
  for( unsigned int i=0; i<SmearedJet.size(); i++ ){
    if( SmearedJet[i].Pt()*GeV < 20 || fabs(SmearedJet[i].Eta())>2.5 ) continue; // pT and eta thresholds for FakeRate
    // decide on nprong for the jet 
    m_random3.SetSeed( (int)(1e+5*fabs( SmearedJet[i].Phi() ) ) );
    double x = m_random3.Rndm();
    int fake_nprong = 3;
    if( x < 0.77 ) fake_nprong=1; 
    float fakeeff = m_upgrade->getTauFakeRate(SmearedJet[i].Et(), SmearedJet[i].Eta(), fake_nprong, 1); // last arg =0,1,2 for loose, medium, tight
    m_random3.SetSeed( (int)(1e+5*fabs( SmearedJet[i].Phi() ) ) );
    float fakeProb = m_random3.Rndm();
    double fakeCharge = m_random3.Rndm();
    if( fakeProb < fakeeff ){ // add jet to SmearedHadTau
      double fac = 1.;// no function right now to rescale the energy
      if (fac < 0.) fac = 0.; // protection
      Particle fakeTau; 
      fakeTau.SetPtEtaPhiM(SmearedJet[i].Pt()*fac, SmearedJet[i].Eta(), SmearedJet[i].Phi(), SmearedJet[i].M() );
      fakeTau.pdgid=-15;
      if( fakeCharge>0.5 ) fakeTau.pdgid=+15;
      fakeTau.nprong = fake_nprong;
      fakeTau.Good = true;
      SmearedHadTau.push_back(fakeTau);
    }
  }
  return;
}

void UpgradeAnalysis::ApplyBtagging(vector<Particle> &SmearedJet){
    //cout << " < ApplyBtagging " << endl;
  // emulate the b-tagging efficiency
  for( unsigned int i=0; i<SmearedJet.size(); i++ ){
    double btag_eff;
    if( SmearedJet[i].pdgid == 5 ) btag_eff = m_upgrade->getFlavourTagEfficiency(SmearedJet[i].Pt(), SmearedJet[i].Eta(), 'B', "mv2c10", 70, m_upgrade->getPileupTrackConfSetting());
    else if( SmearedJet[i].pdgid == 4 ) btag_eff = m_upgrade->getFlavourTagEfficiency(SmearedJet[i].Pt(), SmearedJet[i].Eta(), 'C', "mv2c10", 70, m_upgrade->getPileupTrackConfSetting());
    else if( SmearedJet[i].pdgid == -1 ) btag_eff = m_upgrade->getFlavourTagEfficiency(SmearedJet[i].Pt(), SmearedJet[i].Eta(), 'P', "mv2c10", 70, m_upgrade->getPileupTrackConfSetting());
    else btag_eff = m_upgrade->getFlavourTagEfficiency(SmearedJet[i].Pt(), SmearedJet[i].Eta(), 'L', "mv2c10", 70, m_upgrade->getPileupTrackConfSetting());
    m_random3.SetSeed( (int)(1e+5*fabs( SmearedJet[i].Phi() ) ) );	
    double x = m_random3.Rndm();
    if (x < btag_eff) SmearedJet[i].pdgid=5;
    else SmearedJet[i].pdgid=0;
  }
  return;
}

void UpgradeAnalysis::SmearMET(TLorentzVector m_GenMETTLV, double m_GenMETSumet, TLorentzVector &m_SmearedMETTLV){
  //cout << " < SmearMET " << endl;
  UpgradePerformanceFunctions::MET smearMET = m_upgrade->getMETSmeared( m_GenMETSumet, m_GenMETTLV.Px(), m_GenMETTLV.Py());
  m_SmearedMETTLV.SetPxPyPzE(smearMET.first,smearMET.second,0.,TMath::Sqrt(smearMET.first*smearMET.first + smearMET.second*smearMET.second)); // 

  return;
}

void UpgradeAnalysis::ApplyPtEtaThresholds(vector<Particle> &SmearedPho, vector<Particle> &SmearedEleMuo, vector<Particle> &SmearedHadTau, vector<Particle> &SmearedJet, vector<Particle> &SmearedBJet){
  //cout << " < ApplyPtEtaThresholds " << endl;
  // photons pT>20 and eta < 2.47
  /*for( unsigned int i=0; i<SmearedPho.size(); i++ ){
    if( SmearedPho[i].Pt()*GeV < 20 || fabs(SmearedPho[i].Eta())>2.47 ) SmearedPho[i].Good=false;
    }*/
  // electrons and muons pT>15 and eta < 2.47/2.7
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++ ){
    if( fabs(SmearedEleMuo[i].pdgid) ==11 && (SmearedEleMuo[i].Pt()*GeV < 15 || fabs(SmearedEleMuo[i].Eta())>2.47) ) SmearedEleMuo[i].Good=false;
    if( fabs(SmearedEleMuo[i].pdgid) ==13 && (SmearedEleMuo[i].Pt()*GeV < 15 || fabs(SmearedEleMuo[i].Eta())>2.7) ) SmearedEleMuo[i].Good=false;
  }
  // hadronic taus pT>20 and eta < 2.47
  for( unsigned int i=0; i<SmearedHadTau.size(); i++ ){
    if( SmearedHadTau[i].Pt()*GeV < 20 || fabs(SmearedHadTau[i].Eta())>2.47 ) SmearedHadTau[i].Good=false;
  }
  // jets pT>20 and eta < 2.5
  for( unsigned int i=0; i<SmearedJet.size(); i++ ){
    if( SmearedJet[i].Pt()*GeV < 20 || fabs(SmearedJet[i].Eta())>2.5 ) SmearedJet[i].Good=false;
  }
  for( unsigned int i=0; i<SmearedBJet.size(); i++ ){
    if( SmearedBJet[i].Pt()*GeV < 20 || fabs(SmearedBJet[i].Eta())>2.5 ) SmearedBJet[i].Good=false;
  }
  RemoveBad(SmearedPho);
  RemoveBad(SmearedEleMuo);
  RemoveBad(SmearedHadTau);
  RemoveBad(SmearedJet);
  RemoveBad(SmearedBJet);
  return;
}

void UpgradeAnalysis::OverlapRemoval(vector<Particle> &SmearedPho, vector<Particle> &SmearedEleMuo, vector<Particle> &SmearedHadTau, vector<Particle> &SmearedJet, vector<Particle> &SmearedBJet){
  //cout << " < OverlapRemoval " << endl;
  // apply OR
  // 1. dR(tau, loose ele/muo)<0.2, discard tau
  /*for( unsigned int i=0; i<SmearedEleMuo.size(); i++ ){
    for( unsigned int j=0; j<SmearedHadTau.size(); j++ ){
      if( SmearedEleMuo[i].DeltaR(SmearedHadTau[j])<0.2) SmearedHadTau[j].Good=false;
    }
  }
  RemoveBad(SmearedHadTau);*/
  // 1. dR(photon, loose ele/muo)<0.4, discard photon
  /*for( unsigned int i=0; i<SmearedEleMuo.size(); i++ ){
    for( unsigned int j=0; j<SmearedPho.size(); j++ ){
      if( SmearedEleMuo[i].DeltaR(SmearedPho[j])<0.4) SmearedPho[j].Good=false;
    }
  }
  RemoveBad(SmearedPho);*/
  // 3. dR(ele, non b-tagged jet)<0.2, discard non b-tagged jet
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++ ){
    if( fabs(SmearedEleMuo[i].pdgid) != 11 ) continue;
    for( unsigned int j=0; j<SmearedJet.size(); j++ ){
      if( SmearedJet[i].pdgid==5 ) continue;
      if( SmearedEleMuo[i].DeltaR(SmearedJet[j])<0.2) SmearedJet[j].Good=false;
    }
  }
  RemoveBad(SmearedJet);
  // 4. dR(ele,jet)<0.4, discard ele
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++ ){
    if( fabs(SmearedEleMuo[i].pdgid) != 11 ) continue;
    for( unsigned int j=0; j<SmearedJet.size(); j++ ){
      if( SmearedEleMuo[i].DeltaR(SmearedJet[j])<0.4) SmearedEleMuo[i].Good=false;
    }
  }
  RemoveBad(SmearedEleMuo);
  // 5. dR(muon, non b-tagged jet)<0.2, discard non-btagged jet if mu pT/jet pT ratio >0.5
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++ ){
    if( fabs(SmearedEleMuo[i].pdgid) != 13 ) continue;
    for( unsigned int j=0; j<SmearedJet.size(); j++ ){
      if( SmearedJet[i].pdgid==5 ) continue;
      if( SmearedEleMuo[i].DeltaR(SmearedJet[j])>0.2) continue;
      if( SmearedEleMuo[i].Pt()/SmearedJet[j].Pt()>0.5 ) SmearedJet[j].Good=false;
    }
  }
  RemoveBad(SmearedJet);
  // 6. dR(muon, jet)<0.4, discard muon
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++ ){
    if( fabs(SmearedEleMuo[i].pdgid) != 13 ) continue;
    for( unsigned int j=0; j<SmearedJet.size(); j++ ){
      if( SmearedEleMuo[i].DeltaR(SmearedJet[j])<0.4) SmearedEleMuo[i].Good=false;
    }
  }
  RemoveBad(SmearedEleMuo);
  // 7. dR(tau, jet)<0.2, discard jet
  /*for( unsigned int i=0; i<SmearedHadTau.size(); i++ ){
    for( unsigned int j=0; j<SmearedJet.size(); j++ ){
      if( SmearedHadTau[i].DeltaR(SmearedJet->at(j))<0.2) SmearedJet[j].Good=false;
    }
  }
  RemoveBad(SmearedJet);*/
  // 8. dR(pho, jet)<0.2, discard jet
  /*for( unsigned int i=0; i<SmearedPho.size(); i++ ){
    for( unsigned int j=0; j<SmearedJet.size(); j++ ){
      if( SmearedPho[i].DeltaR(SmearedJet->at(j))<0.4) SmearedJet[j].Good=false;
    }
  }
  RemoveBad(SmearedJet);*/
  return;
}

void UpgradeAnalysis::ApplyIsolation(vector<Particle> &SmearedPho, vector<Particle> &SmearedEleMuo){
  //cout << " < ApplyIsolation " << endl;
  // Basic isolation of etcone20 and ptcone30 to be < 15% of the lepton pT
  // probably needs review for each analysis

  for( unsigned int i = 0; i<SmearedEleMuo.size(); i++){
    if( SmearedEleMuo[i].etcone20/SmearedEleMuo[i].Pt() > 0.15 ) SmearedEleMuo[i].Good=false;
    if( SmearedEleMuo[i].ptcone30/SmearedEleMuo[i].Pt() > 0.15 ) SmearedEleMuo[i].Good=false;
  }
  RemoveBad(SmearedEleMuo);
  for( unsigned int i = 0; i<SmearedPho.size(); i++){
    if( SmearedPho[i].etcone20/SmearedPho[i].Pt() > 0.15 ) SmearedPho[i].Good=false;
    if( SmearedPho[i].ptcone30/SmearedPho[i].Pt() > 0.15 ) SmearedPho[i].Good=false;
  }
  RemoveBad(SmearedPho);
  return;
}

void UpgradeAnalysis::RemoveLowMassPairs(vector<Particle> &SmearedEleMuo){
  //cout << " < RemoveLowMassPairs " << endl;
  // discard SFOS leptons with mll<12 GeV
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++ ){
    for( unsigned int j=0; j<SmearedEleMuo.size(); j++ ){
      if( i==j ) continue;
      if( SmearedEleMuo[i].pdgid*SmearedEleMuo[j].pdgid!=-121 && SmearedEleMuo[i].pdgid*SmearedEleMuo[j].pdgid!=-169 ) continue;
      TLorentzVector LL; LL = SmearedEleMuo[i]+SmearedEleMuo[j];
      if( LL.M()*GeV>12 ) continue;
      SmearedEleMuo[i].Good=false; SmearedEleMuo[j].Good=false;
    }
  }
  RemoveBad(SmearedEleMuo);
  return;
}

void UpgradeAnalysis::ApplyTriggerEfficiency(vector<Particle> &SmearedEleMuo, double &m_trig_Eff_e, double &m_trig_Eff_m, double &m_trig_Eff_ee, double &m_trig_Eff_mm, double &m_trig_Eff_em){
  //cout << " < ApplyTriggerEfficiency " << endl;
  //set to 100% as default
  m_trig_Eff_e = 1.;
  m_trig_Eff_m = 1.;
  m_trig_Eff_ee = 1.;
  m_trig_Eff_mm = 1.;
  m_trig_Eff_em = 1.;
  
  /* Single electron trigger */
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    if( fabs(SmearedEleMuo[i].pdgid)!=11 ) continue;
    m_trig_Eff_e =  m_upgrade->getSingleElectronTriggerEfficiency(SmearedEleMuo[i].Pt(), SmearedEleMuo[i].Eta());
    break;
  }

  /* Double electron trigger */
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    if( fabs(SmearedEleMuo[i].pdgid)!=11 ) continue;
    for( unsigned int j=0; j<SmearedEleMuo.size(); j++){
      if( i==j ) continue;
      if( fabs(SmearedEleMuo[j].pdgid)!=11 ) continue;
      m_trig_Eff_ee =  m_upgrade->getDiElectronTriggerEfficiency(SmearedEleMuo[i].Pt(), SmearedEleMuo[j].Pt(), SmearedEleMuo[i].Eta(), SmearedEleMuo[j].Eta());
      break;
    }
  }

  /* Single muon trigger */
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    if( fabs(SmearedEleMuo[i].pdgid)!=13 ) continue;
    m_trig_Eff_m =  m_upgrade->getSingleMuonTriggerEfficiency(SmearedEleMuo[i].Pt(), SmearedEleMuo[i].Eta());
    break;
  }
  
  /* Double muon trigger */
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    if( fabs(SmearedEleMuo[i].pdgid)!=13 ) continue;
    for( unsigned int j=0; j<SmearedEleMuo.size(); j++){
      if( i==j ) continue;
      if( fabs(SmearedEleMuo[j].pdgid)!=13 ) continue;
      m_trig_Eff_mm =  m_upgrade->getDiMuonTriggerEfficiency(SmearedEleMuo[i].Pt(), SmearedEleMuo[j].Pt(), SmearedEleMuo[i].Eta(), SmearedEleMuo[j].Eta());
      break;
    }
  }
  

  /* Electron+Muon trigger */
  for( unsigned int i=0; i<SmearedEleMuo.size(); i++){
    if( fabs(SmearedEleMuo[i].pdgid)!=11 ) continue;
    for( unsigned int j=0; j<SmearedEleMuo.size(); j++){
      if( fabs(SmearedEleMuo[j].pdgid)!=13 ) continue;
      m_trig_Eff_em =  m_upgrade->getElectronMuonTriggerEfficiency(SmearedEleMuo[i].Pt(), SmearedEleMuo[j].Pt(), SmearedEleMuo[i].Eta(), SmearedEleMuo[j].Eta());
      break;
    }
  }

  /* Single tau trigger */
  /*
  for( unsigned int i=0; i<SmearedHadTau.size(); i++){
    m_trigEff =  m_upgrade->getSingleTauTriggerEfficiency(SmearedHadTau[i].Pt(), SmearedHadTau[i].Eta(), SmearedHadTau[i].nprong);
    break;
  }
  */
  /* Double tau trigger */
  /*
  for( unsigned int i=0; i<SmearedHadTau.size(); i++){
    for( unsigned int j=0; j<SmearedHadTau.size(); j++){
      if( i==j ) continue;
      m_trigEff =  m_upgrade->getDiTauTriggerEfficiency(SmearedHadTau[i].Pt(), SmearedHadTau[j].Pt(), SmearedHadTau[i].Eta(), SmearedHadTau[j].Eta(), SmearedHadTau[i].nprong, SmearedHadTau[j].nprong);
      break;
    }
  }
  */

  return;
}



void UpgradeAnalysis::calculateEventVariables(vector<Particle> &SmearedPho, vector<Particle> &SmearedEleMuo, vector<Particle> &SmearedHadTau, vector<Particle> &SmearedJet, vector<Particle> &SmearedBJet, TLorentzVector &m_SmearedMETTLV, double &m_met, bool &m_gotSFOS, double &m_mll, double &m_mT, double &m_ht, double &m_lt, double &m_meff, double &m_mT2){
  //cout << " < calculateEventVariables " << endl;
  // met
  m_met = m_SmearedMETTLV.Et();

  // find the mass of the SFOS lepton pair closest to Z mass
  double mlldZ = 999999; m_mll=9999999999; m_gotSFOS=false; 
  for( unsigned int i = 0; i < SmearedEleMuo.size(); i++) {
    for( unsigned int j = 0; j < SmearedEleMuo.size(); j++) {
      if( i==j ) continue;
      TLorentzVector LL; LL = SmearedEleMuo[i]+SmearedEleMuo[j];
      double Mll = LL.M();
      if(  SmearedEleMuo[i].pdgid*SmearedEleMuo[j].pdgid !=-121 && SmearedEleMuo[i].pdgid*SmearedEleMuo[j].pdgid !=-169 ) continue;
      m_gotSFOS=true;
      if( fabs(Mll - 91200) < mlldZ ){ mlldZ = fabs(Mll - 91200); m_mll=Mll;}
    }
  }

  // transverse mass using leading lepton 
  m_mT = -9999999999;
  if( SmearedEleMuo.size()>0 ) m_mT = M_T(SmearedEleMuo[0], m_SmearedMETTLV);

  // sum of lepton pT
  m_lt=0.;
  for( unsigned int i = 0; i < SmearedEleMuo.size(); i++) m_lt += SmearedEleMuo[i].Pt();
  // sum of tau pT
  double sumtauPt=0.;
  for( unsigned int i = 0; i < SmearedHadTau.size(); i++) sumtauPt += SmearedHadTau[i].Pt();

  // sum jet (40 GeV) pT
  m_ht = 0.;
  for( unsigned int i = 0; i < SmearedJet.size(); i++){
    if( SmearedJet[i].Pt()*GeV<40 ) continue;
    m_ht += SmearedJet[i].Pt();
  }

  // effective mass
  m_meff = m_met + m_lt + sumtauPt + m_ht;

  // stransverse mass using the two leading leptons
  if( SmearedEleMuo.size() >=2 ) m_mT2=getMT2(SmearedEleMuo[0], SmearedEleMuo[1], m_SmearedMETTLV );

  return;
}


/* =========================================================================================================================== */
float getMT2(Particle p1, Particle p2, TLorentzVector met, double invisiblemass_1, double invisiblemass_2)
{
  TLorentzVector pA = p1;
  TLorentzVector pB = p2;
  ComputeMT2 mycalc = ComputeMT2(pA,pB,met, invisiblemass_1, invisiblemass_2);
  
  return mycalc.Compute();  
}


// Transverse mass
double M_T(TLorentzVector vec1, TLorentzVector vec2) {
  float mt = (vec1.Mt() + vec2.Mt())*(vec1.Mt() + vec2.Mt()) - (vec1+vec2).Perp2();
  mt = (mt >= 0.) ? sqrt(mt) : sqrt(-mt);
  return mt;
}



void RemoveBad(vector<Particle> &vec1) {
  vector<Particle> Survivors;
  for(vector<Particle>::iterator it1 = vec1.begin(); it1 != vec1.end(); ++it1) {
    if( it1->Good == false) continue;
    Survivors.push_back( *it1 );
  }
  vec1=Survivors;
  return;
}

