// -*- C++ -*-
//
// Package:    METSigTuning/MakeNtuple
// Class:      MakeNtuple
// 
/**\class MakeNtuple MakeNtuple.cc METSigTuning/MakeNtuple/src/MakeNtuple.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Nathan Mirman
//         Created:  Wed, 26 Nov 2014 14:22:08 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "JetMETCorrections/Modules/interface/JetResolution.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 

#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "TMatrixD.h"

#include "Math/LorentzVector.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <stdlib.h>

#define NUMPUBINS 50

//
// class declaration
//

class MakeNtuple : public edm::EDAnalyzer {
   public:
      explicit MakeNtuple(const edm::ParameterSet&);
      ~MakeNtuple();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      std::vector<pat::Jet> cleanJets(double, double,
            std::vector<pat::Jet>&, std::vector<reco::Candidate::LorentzVector>&);
      edm::EDGetTokenT<edm::View<reco::Candidate> > inputToken_;
      edm::EDGetTokenT<edm::View<pat::Jet> > jetToken_;
      edm::EDGetTokenT<edm::View<pat::Jet> > jetPuppiToken_;
      std::vector< edm::EDGetTokenT<edm::View<reco::Candidate> > > lepTokens_;
      edm::EDGetTokenT<edm::View<pat::MET> > metToken_;
      edm::EDGetTokenT<edm::View<pat::MET> > metPuppiToken_;
      edm::EDGetTokenT<GenEventInfoProduct> genToken_;
      edm::EDGetTokenT<LHEEventProduct> lheToken_;
      edm::EDGetTokenT< std::vector<pat::Muon> > muonToken_;
      edm::EDGetTokenT<edm::View<reco::Vertex> > verticesToken_;
      Bool_t runOnMC_;
      //edm::InputTag addPileupInfo_;
      edm::EDGetTokenT<edm::View<PileupSummaryInfo> > pileupToken_;
      edm::EDGetTokenT<double> rhoToken_;
      std::string jetSFType_;
      std::string jetResPtType_;
      std::string jetResPhiType_;
      std::string jetPuppiSFType_;
      std::string jetPuppiResPtType_;
      std::string jetPuppiResPhiType_;
    
      edm::Service<TFileService> fs;

      edm::LumiReWeighting LumiWeights_;

      //std::string pfjetCorrectorL1_;
      //std::string pfjetCorrectorL123_;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------

      static const double PU2015_MCf[NUMPUBINS];
      static const double PU2015_Dataf[NUMPUBINS];

    TTree *results_tree;
    //    TFile *OutFile__file;
    //std::string OutputFileName_;

      Long64_t run, event, lumi;

      std::vector<double> lep_pt, lep_energy, lep_phi, lep_eta;
      std::vector<double> muon_pt, muon_energy, muon_phi, muon_eta;
      std::vector<double> jet_pt, jet_energy, jet_phi, jet_eta;
      std::vector<double> jet_sigmapt, jet_sigmaphi;
      std::vector<double> jet_corrL1, jet_corrL123;
      std::vector<bool> jet_passid;
      std::vector<double> jet_sf;
      double met_pt, met_energy, met_phi, met_eta, met_sumpt;
      std::vector<double> jetPuppi_pt, jetPuppi_energy, jetPuppi_phi, jetPuppi_eta;
      std::vector<double> jetPuppi_sigmapt, jetPuppi_sigmaphi;
      std::vector<double> jetPuppi_corrL1, jetPuppi_corrL123;
      std::vector<bool> jetPuppi_passid;
      std::vector<double> jetPuppi_sf;
      double metPuppi_pt, metPuppi_energy, metPuppi_phi, metPuppi_eta, metPuppi_sumpt;
      int nvertices;
      double weight_pu;
      double mcweight, mcweightSum;

      double jetThreshold;

      int events_total, events_pass;

      double T_nvertices;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MakeNtuple::MakeNtuple(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   inputToken_ = consumes<edm::View<reco::Candidate> >(iConfig.getParameter<edm::InputTag>("src"));
   jetToken_ = consumes<edm::View<pat::Jet> >(iConfig.getParameter<edm::InputTag>("jets"));
   jetPuppiToken_ = consumes<edm::View<pat::Jet> >(iConfig.getParameter<edm::InputTag>("jetsPuppi"));
   std::vector<edm::InputTag> srcLeptonsTags = iConfig.getParameter< std::vector<edm::InputTag> >("leptons");
   for(std::vector<edm::InputTag>::const_iterator it=srcLeptonsTags.begin();it!=srcLeptonsTags.end();it++) {
      lepTokens_.push_back( consumes<edm::View<reco::Candidate> >( *it ) );
   }
   metToken_ = consumes<edm::View<pat::MET> >(iConfig.getParameter<edm::InputTag>("met"));
   metPuppiToken_ = consumes<edm::View<pat::MET> >(iConfig.getParameter<edm::InputTag>("metPuppi"));
   muonToken_ = consumes< std::vector<pat::Muon> >(iConfig.getParameter<edm::InputTag>("muons"));
   verticesToken_ = consumes< edm::View<reco::Vertex> >(iConfig.getParameter<edm::InputTag>("vertices"));
   genToken_ = consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("generator"));
   lheToken_ = consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("lheprod"));
   runOnMC_ = iConfig.getUntrackedParameter<Bool_t>("runOnMC");
   //addPileupInfo_ = iConfig.getUntrackedParameter<edm::InputTag>("pileup");
   pileupToken_ = consumes<edm::View<PileupSummaryInfo> >(iConfig.getUntrackedParameter<edm::InputTag>("pileup"));
   rhoToken_ = consumes<double>(iConfig.getParameter<edm::InputTag>("rho"));

   jetSFType_ = iConfig.getParameter<std::string>("srcJetSF");
   jetResPtType_ = iConfig.getParameter<std::string>("srcJetResPt");
   jetResPhiType_ = iConfig.getParameter<std::string>("srcJetResPhi");

   jetPuppiSFType_ = iConfig.getParameter<std::string>("srcFileJetPuppiSF");
   jetPuppiResPtType_ = iConfig.getParameter<std::string>("srcFileJetPuppiResPt");
   jetPuppiResPhiType_ = iConfig.getParameter<std::string>("srcFileJetPuppiResPhi");

   //pfjetCorrectorL1_  = iConfig.getUntrackedParameter<std::string>("pfjetCorrectorL1");
   //pfjetCorrectorL123_ = iConfig.getUntrackedParameter<std::string>("pfjetCorrectorL123");

   jetThreshold = 15;

   mcweight = 1.0;
   mcweightSum = 1.0;

   //   OutputFileName_ = "ntuple.root";

}


MakeNtuple::~MakeNtuple()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

const double MakeNtuple::PU2015_MCf[NUMPUBINS] = {
   // pileup distribution for Fall2015 MC
   // obtained at https://twiki.cern.ch/twiki/bin/viewauth/CMS/PdmVPileUpDescription#Startup2015
   // from file SimGeneral/MixingModule/python/mix_2015_25ns_FallMC_matchData_PoissonOOTPU_cfi.py
   0.000108643,
   0.000388957,
   0.000332882,
   0.00038397,
   0.000549167,
   0.00105412,
   0.00459007,
   0.0210314,
   0.0573688,
   0.103986,
   0.142369,
   0.157729,
   0.147685,
   0.121027,
   0.08855,
   0.0582866,
   0.0348526,
   0.019457,
   0.0107907,
   0.00654313,
   0.00463195,
   0.00370927,
   0.0031137,
   0.00261141,
   0.00215499,
   0.00174491,
   0.00138268,
   0.00106731,
   0.000798828,
   0.00057785,
   0.00040336,
   0.00027161,
   0.000176535,
   0.00011092,
   6.75502e-05,
   4.00323e-05,
   2.32123e-05,
   1.32585e-05,
   7.51611e-06,
   4.25902e-06,
   2.42513e-06,
   1.39077e-06,
   8.02452e-07,
   4.64159e-07,
   2.67845e-07,
   1.5344e-07,
   8.68966e-08,
   4.84931e-08,
   2.6606e-08,
   1.433e-08
};

const double MakeNtuple::PU2015_Dataf[NUMPUBINS] = {
   // 'true' distribution for 2015 RunC+D dataset
   // obtained with pileupCalc.py (04.21.2016)
   122059,
   583201,
   828340,
   1.1958e+06,
   2.11956e+06,
   5.0101e+06,
   1.57464e+07,
   6.08081e+07,
   1.66592e+08,
   2.83249e+08,
   3.66702e+08,
   3.94001e+08,
   3.50137e+08,
   2.5144e+08,
   1.45141e+08,
   6.85505e+07,
   2.81446e+07,
   1.16883e+07,
   5.74759e+06,
   3.04099e+06,
   1.40423e+06,
   512164,
   146784,
   35302.9,
   8271.3,
   2235.61,
   721.38,
   258.85,
   97.2709,
   36.8716,
   13.7275,
   4.93171,
   1.69241,
   0.551894,
   0.170601,
   0.0499358,
   0.0138339,
   0.00362662,
   0.000899606,
   0.000211148,
   4.68928e-05,
   9.85392e-06,
   1.95929e-06,
   3.6862e-07,
   6.56248e-08,
   1.10534e-08,
   1.76248e-09,
   2.61497e-10,
   4.768e-11,
   0
};

// ------------ method called for each event  ------------
void
MakeNtuple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   // clear all vectors
   lep_pt.clear();
   lep_energy.clear();
   lep_phi.clear();
   lep_eta.clear();
   muon_pt.clear();
   muon_energy.clear();
   muon_phi.clear();
   muon_eta.clear();
   jet_pt.clear();
   jet_passid.clear();
   jet_energy.clear();
   jet_phi.clear();
   jet_eta.clear();
   jet_sigmapt.clear();
   jet_sigmaphi.clear();
   jet_sf.clear();
   jetPuppi_pt.clear();
   jetPuppi_passid.clear();
   jetPuppi_energy.clear();
   jetPuppi_phi.clear();
   jetPuppi_eta.clear();
   jetPuppi_sigmapt.clear();
   jetPuppi_sigmaphi.clear();
   jetPuppi_sf.clear();

   run = iEvent.id().run();
   event = iEvent.id().event();
   lumi = iEvent.id().luminosityBlock();

   Handle<View<reco::Candidate> > input;
   iEvent.getByToken(inputToken_, input);

   // offline primary vertices
   edm::Handle<edm::View<reco::Vertex> > vertices;
   iEvent.getByToken(verticesToken_, vertices);
   nvertices = int(vertices->size());

   // leptons
   std::vector<reco::CandidatePtr> footprint;
   std::vector<reco::CandidatePtr> footprintPuppi;
   std::vector<reco::Candidate::LorentzVector> leptons;
   for ( std::vector<EDGetTokenT<View<reco::Candidate> > >::const_iterator srcLeptons_i = lepTokens_.begin();
         srcLeptons_i != lepTokens_.end(); ++srcLeptons_i ) {
      Handle<reco::CandidateView> leptons_i;
      iEvent.getByToken(*srcLeptons_i, leptons_i);
      for ( reco::CandidateView::const_iterator lepton = leptons_i->begin();
            lepton != leptons_i->end(); ++lepton ) {
         // cut on lepton pt
         if( lepton->pt() > 10 ){
            leptons.push_back(lepton->p4());
            for( unsigned int n=0; n < lepton->numberOfSourceCandidatePtrs(); n++){
               if( lepton->sourceCandidatePtr(n).isNonnull() and lepton->sourceCandidatePtr(n).isAvailable() ){
                  footprint.push_back(lepton->sourceCandidatePtr(n));
                  footprintPuppi.push_back(lepton->sourceCandidatePtr(n));
               }
            }
         }
      }
   }

   // muons (for event selection)
   Handle< std::vector<pat::Muon> > muons;
   iEvent.getByToken(muonToken_, muons);
   double nmuons = 0;
   double charge = 1.0;
   for ( std::vector<pat::Muon>::const_iterator muon = muons->begin();
         muon != muons->end(); ++muon ) {

      bool muId = muon->isTightMuon((*(vertices->begin())));
      //bool muId = muon->isLooseMuon();

      double dr04chHad = muon->pfIsolationR04().sumChargedHadronPt;
      double dr04neutHad = muon->pfIsolationR04().sumNeutralHadronEt;
      double dr04photons = muon->pfIsolationR04().sumPhotonEt;

      bool muIso = (dr04chHad + dr04neutHad + dr04photons)/muon->pt() < 0.12;

      if( muon->pt() > 20 and fabs(muon->eta()) < 2.4 and muId and muIso ){
         muon_pt.push_back( muon->pt() );
         muon_energy.push_back( muon->energy() );
         muon_phi.push_back( muon->phi() );
         muon_eta.push_back( muon->eta() );
         nmuons++;
         charge *= muon->charge();
      }
   }

   double dimuon_mass = 0;
   if( nmuons == 2 ){
      TLorentzVector mu1, mu2;
      mu1.SetPtEtaPhiE( muon_pt[0], muon_eta[0], muon_phi[0], muon_energy[0] );
      mu2.SetPtEtaPhiE( muon_pt[1], muon_eta[1], muon_phi[1], muon_energy[1] );
      dimuon_mass = (mu1+mu2).M();
   }

   // jet energy corrections
   //const JetCorrector* corrL1  = JetCorrector::getJetCorrector (pfjetCorrectorL1_, iSetup);
   //const JetCorrector* corrL123 = JetCorrector::getJetCorrector (pfjetCorrectorL123_, iSetup);
       
   // jets
   Handle<View<pat::Jet>> inputJets;
   iEvent.getByToken( jetToken_, inputJets );
   std::vector<pat::Jet> jets;
   for(View<pat::Jet>::const_iterator jet = inputJets->begin(); jet != inputJets->end(); ++jet) {
      jets.push_back( *jet );
   }

   // disambiguate jets and leptons
   std::vector<pat::Jet> cleanjets = cleanJets(jetThreshold, 0.4, jets, leptons);

   // loop over jets to disambiguate candidates
   for(std::vector<pat::Jet>::const_iterator jet = cleanjets.begin(); jet != cleanjets.end(); ++jet) {
      for( unsigned int n=0; n < jet->numberOfSourceCandidatePtrs(); n++){
         if( jet->sourceCandidatePtr(n).isNonnull() and jet->sourceCandidatePtr(n).isAvailable() ){
            footprint.push_back(jet->sourceCandidatePtr(n));
         }
      }
   }

   //PUPPI jets
   Handle<View<pat::Jet>> inputJetsPuppi;
   iEvent.getByToken( jetPuppiToken_, inputJetsPuppi );
   std::vector<pat::Jet> jetsPuppi;
   for(View<pat::Jet>::const_iterator jet = inputJetsPuppi->begin(); jet != inputJetsPuppi->end(); ++jet) {
      jetsPuppi.push_back( *jet );
   }

   // disambiguate jets and leptons
   std::vector<pat::Jet> cleanjetsPuppi = cleanJets(jetThreshold, 0.4, jetsPuppi, leptons);

   // loop over jets to disambiguate candidates
   for(std::vector<pat::Jet>::const_iterator jet = cleanjetsPuppi.begin(); jet != cleanjetsPuppi.end(); ++jet) {
      for( unsigned int n=0; n < jet->numberOfSourceCandidatePtrs(); n++){
         if( jet->sourceCandidatePtr(n).isNonnull() and jet->sourceCandidatePtr(n).isAvailable() ){
            footprintPuppi.push_back(jet->sourceCandidatePtr(n));
         }
      }
   }

   // met
   edm::Handle<edm::View<pat::MET> > metHandle;
   iEvent.getByToken(metToken_, metHandle);
   const pat::MET& met = (*metHandle)[0];

   //PUPPI met
   edm::Handle<edm::View<pat::MET> > metPuppiHandle;
   iEvent.getByToken(metPuppiToken_, metPuppiHandle);
   const pat::MET& metPuppi = (*metPuppiHandle)[0];

   // put met into a 4-vector, implement type-1 corrections
   /*
   reco::Candidate::LorentzVector pmetvec = met.shiftedP4_74x(pat::MET::METUncertainty(12),pat::MET::Raw);
   TLorentzVector metvec;
   metvec.SetPxPyPzE(pmetvec.px(), pmetvec.py(), pmetvec.pz(), pmetvec.energy());
   for(std::vector<pat::Jet>::const_iterator jet = cleanjets.begin(); jet != cleanjets.end(); ++jet){
      TLorentzVector jettemp;
      jettemp.SetPxPyPzE(jet->px(), jet->py(), jet->pz(), jet->energy());
      double cL1 = corrL1->correction( *jet, iEvent, iSetup );
      double cL123 = corrL123->correction( *jet, iEvent, iSetup );
      metvec -= (cL123-cL1)*jettemp;
   }
   */

   met_pt = met.pt();
   met_energy = met.energy();
   met_phi = met.phi();
   met_eta = met.eta();

   metPuppi_pt = metPuppi.pt();
   metPuppi_energy = metPuppi.energy();
   metPuppi_phi = metPuppi.phi();
   metPuppi_eta = metPuppi.eta();

   /*
   std::cout << "MET (new) = " << met_pt << std::endl;

   edm::Handle<edm::View<reco::MET> > metHandle2;
   iEvent.getByLabel("slimmedMETs", metHandle2);
   const reco::MET& met2 = (*metHandle2)[0];

   double met2_pt = met2.pt();
   std::cout << "MET (old) = " << met2_pt << std::endl;

   edm::Handle<edm::View<reco::MET> > metHandle3;
   iEvent.getByLabel("pfMetRERUN", metHandle3);
   const reco::MET& met3 = (*metHandle3)[0];

   double met3_pt = met3.pt();
   std::cout << "MET (uncorr) = " << met3_pt << std::endl;

   std::cout << " ************************* " << std::endl;

   fflush(stdout);
   */

   // candidates
   std::vector<reco::Candidate::LorentzVector> candidates;
   for(View<reco::Candidate>::const_iterator cand = input->begin();
         cand != input->end(); ++cand) {
      unsigned int iter = cand - input->begin();
      if (std::find(footprint.begin(), footprint.end(),
               reco::CandidatePtr(input,iter)) != footprint.end()) {
         continue;
      }
      candidates.push_back( cand->p4() );
   }

   // candidates with Puppi
   std::vector<reco::Candidate::LorentzVector> candidatesPuppi;
   for(View<reco::Candidate>::const_iterator cand = input->begin();
         cand != input->end(); ++cand) {
      unsigned int iter = cand - input->begin();
      if (std::find(footprintPuppi.begin(), footprintPuppi.end(),
               reco::CandidatePtr(input,iter)) != footprintPuppi.end()) {
         continue;
      }
      candidatesPuppi.push_back( cand->p4() );
   }

   // resolutions for PUPPI
   //std::string path = "metsigtuning/MakeNtuple/data/";
   //std::string resEra_ = "Fall15_25nsV2";
   //std::string resAlg_ = "AK4PFPuppi";
   //std::string ptResFileName  = path + resEra_ + "_MC_PtResolution_" +resAlg_+".txt";
   //std::string phiResFileName = path + resEra_ + "_MC_PhiResolution_" +resAlg_+".txt";
   //std::string sfResFileName  = path + resEra_ + "_MC_SF_" +resAlg_+".txt";
   //std::string ptResFileName  = resEra_ + "_MC_PtResolution_" +resAlg_+".txt";
   //std::string phiResFileName = resEra_ + "_MC_PhiResolution_" +resAlg_+".txt";
   //std::string sfResFileName  = resEra_ + "_MC_SF_" +resAlg_+".txt";

   // old framework
   //   FileInPath fpt(ptResFileName);
   //FileInPath fphi(phiResFileName);
   //FileInPath fsf(sfResFileName);

   //   std::cout<<"%%%%%%%%%%%%%%%%%%"<<std::endl;
   //system("pwd");
   //system("ls CMSSW_7_6_5/src/");
   //std::cout<<"------------------"<<std::endl;
   //system("ls");
   //std::cout<<"%%%%%%%%%%%%%%%%%%"<<std::endl;

   FileInPath fpt(jetPuppiResPtType_);
   FileInPath fphi(jetPuppiResPhiType_);
   FileInPath fsf(jetPuppiSFType_);

   // accessJERs using text files
   JME::JetResolution resolutionPuppi_pt = JME::JetResolution(fpt.fullPath().c_str());
   JME::JetResolutionScaleFactor resolutionPuppi_sf = JME::JetResolutionScaleFactor(fsf.fullPath().c_str());
   JME::JetResolution resolutionPuppi_phi = JME::JetResolution(fphi.fullPath().c_str());

   //   JME::JetResolution resolutionPuppi_pt = JME::JetResolution(jetPuppiResPtType_.fullPath().c_str());
   //JME::JetResolutionScaleFactor resolutionPuppi_sf = JME::JetResolutionScaleFactor(jetPuppiSFType_.fullPath().c_str());
   //JME::JetResolution resolutionPuppi_phi = JME::JetResolution(jetPuppiResPhiType_.fullPath().c_str());

   // access JERs for AK4PFchs using DB
   JME::JetResolution resolution_pt = JME::JetResolution::get(iSetup, jetResPtType_);
   JME::JetResolution resolution_phi = JME::JetResolution::get(iSetup, jetResPhiType_);
   JME::JetResolutionScaleFactor resolution_sf = JME::JetResolutionScaleFactor::get(iSetup, jetSFType_);

   //JetResolution *phiRes_ = new JetResolution(fphi_old.fullPath().c_str(),false);

   //
   // begin ttree variables
   //

   if( runOnMC_ ){
      Handle<GenEventInfoProduct> gi;
      iEvent.getByToken(genToken_, gi);

      //Handle<LHEEventProduct> li;
      //iEvent.getByToken(lheToken_, li) ;

      mcweight = gi->weight();
      //mcweightSum = li->originalXWGTUP();
      //std::cout << mcweight << " " << mcweightSum << std::endl;
      //fflush(stdout);
   }

   weight_pu = 1.0;
   if( runOnMC_ ){
      View<PileupSummaryInfo>::const_iterator PVI;
      Handle<View<PileupSummaryInfo> > PupInfo;
      iEvent.getByToken(pileupToken_, PupInfo);
   //Handle<View<pat::Jet>> inputJets;
   //Event.getByToken( jetToken_, inputJets );
   //std::vector<pat::Jet> jets;
   //for(View<pat::Jet>::const_iterator jet = inputJets->begin(); jet != inputJets->end(); ++jet) {

      float Tnvtx = -1.0;
      for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {

         int BX = PVI->getBunchCrossing();

         if(BX == 0) { 
            Tnvtx = PVI->getTrueNumInteractions(); 
            continue;
         }

         weight_pu = LumiWeights_.weight( Tnvtx );
         T_nvertices = Tnvtx;
      }
   }

   // calculate met_sumpt
   // candidates already have clean jets removed
   met_sumpt = 0;
   for( std::vector<reco::Candidate::LorentzVector>::const_iterator cand = candidates.begin();
         cand != candidates.end(); ++cand){
      met_sumpt += cand->Pt();
   }


   metPuppi_sumpt = 0;
   for( std::vector<reco::Candidate::LorentzVector>::const_iterator cand = candidatesPuppi.begin();
         cand != candidatesPuppi.end(); ++cand){
      metPuppi_sumpt += cand->Pt();
   }


   /*
   // loop over leptons
   for ( std::vector<reco::Candidate::LorentzVector>::const_iterator lepton = leptons.begin();
         lepton != leptons.end(); ++lepton ) {
      lep_pt.push_back( lepton->Pt() );
      lep_energy.push_back( lepton->E() );
      lep_phi.push_back( lepton->Phi() );
      lep_eta.push_back( lepton->Eta() );
   }
   */
   edm::Handle<double> rho;
   iEvent.getByToken(rhoToken_, rho);

   // loop over jets
   for(std::vector<pat::Jet>::const_iterator jet = cleanjets.begin(); jet != cleanjets.end(); ++jet) {
      double jpt  = jet->pt();
      double jeta = jet->eta();

      // jet energy resolutions
      //double jeta_res = (fabs(jeta) < 9.9) ? jeta : 9.89; // JetResolutions defined for |eta|<9.9
      //TF1* fPtEta    = ptRes_ -> parameterEta("sigma",jeta_res);
      //TF1* fPhiEta   = phiRes_-> parameterEta("sigma",jeta_res);
      //double sigmapt = fPtEta->Eval(jpt);
      //double sigmaphi = fPhiEta->Eval(jpt);
      //delete fPtEta;
      //delete fPhiEta;

      // new framework
      JME::JetParameters parameters;
      parameters.setJetPt(jpt).setJetEta(jeta).setRho(*rho);
      double sigmapt = resolution_pt.getResolution(parameters);
      double sigmaphi = resolution_phi.getResolution(parameters);
      double sf = resolution_sf.getScaleFactor(parameters);

      // split into high-pt and low-pt sector
      if( jpt > jetThreshold ){
         // high-pt jets enter into the covariance matrix via JER

         jet_pt.push_back( jet->pt() );
         jet_energy.push_back( jet->energy() );
         jet_phi.push_back( jet->phi() );
         jet_eta.push_back( jet->eta() );
         jet_sigmapt.push_back( sigmapt );
         jet_sigmaphi.push_back( sigmaphi );
         jet_sf.push_back( sf );

         // jet id
         bool id_24 = true;
         bool id_30 = true;
         bool id_forw = true;
         if( fabs(jet->eta()) <= 3.0 ){
            id_30 = jet->neutralHadronEnergyFraction() < 0.99 and jet->neutralEmEnergyFraction() < 0.99
               and (jet->chargedMultiplicity() + jet->neutralMultiplicity()) > 1;
            if( fabs(jet->eta()) <= 2.4 ){
               id_24 = jet->chargedHadronEnergyFraction() > 0 and jet->chargedMultiplicity() > 0
                  and jet->chargedEmEnergyFraction() < 0.99;
            }
         } else {
            id_forw = jet->neutralEmEnergyFraction() < 0.9 and jet->neutralMultiplicity() > 10;
         }
         jet_passid.push_back( id_24 and id_30 and id_forw );

         //jet_corrL1.push_back( corrL1->correction(*jet, iEvent, iSetup) );
         //jet_corrL123.push_back( corrL123->correction(*jet, iEvent, iSetup) );

      }else{

         // add the (corrected) jet to the met_sumpt
         // (was subtracted previously)
         met_sumpt += jpt;

      }
   }

   // loop over Puppi jets
   for(std::vector<pat::Jet>::const_iterator jet = cleanjetsPuppi.begin(); jet != cleanjetsPuppi.end(); ++jet) {
      double jpt  = jet->pt();
      double jeta = jet->eta();

      // jet energy resolutions
      //double jeta_res = (fabs(jeta) < 9.9) ? jeta : 9.89; // JetResolutions defined for |eta|<9.9
      //TF1* fPtEta    = ptRes_ -> parameterEta("sigma",jeta_res);
      //TF1* fPhiEta   = phiRes_-> parameterEta("sigma",jeta_res);
      //double sigmapt = fPtEta->Eval(jpt);
      //double sigmaphi = fPhiEta->Eval(jpt);
      //delete fPtEta;
      //delete fPhiEta;

      // new framework
      JME::JetParameters parameters;
      parameters.setJetPt(jpt).setJetEta(jeta).setRho(*rho);
      double sigmapt = resolutionPuppi_pt.getResolution(parameters);
      double sigmaphi = resolutionPuppi_phi.getResolution(parameters);
      double sf = resolutionPuppi_sf.getScaleFactor(parameters);

      // split into high-pt and low-pt sector
      if( jpt > jetThreshold ){
         // high-pt jets enter into the covariance matrix via JER

         jetPuppi_pt.push_back( jet->pt() );
         jetPuppi_energy.push_back( jet->energy() );
         jetPuppi_phi.push_back( jet->phi() );
         jetPuppi_eta.push_back( jet->eta() );
         jetPuppi_sigmapt.push_back( sigmapt );
         jetPuppi_sigmaphi.push_back( sigmaphi );
         jetPuppi_sf.push_back( sf );

         // jet id
         bool id_24 = true;
         bool id_30 = true;
         bool id_forw = true;
         if( fabs(jet->eta()) <= 3.0 ){
            id_30 = jet->neutralHadronEnergyFraction() < 0.99 and jet->neutralEmEnergyFraction() < 0.99
               and (jet->chargedMultiplicity() + jet->neutralMultiplicity()) > 1;
            if( fabs(jet->eta()) <= 2.4 ){
               id_24 = jet->chargedHadronEnergyFraction() > 0 and jet->chargedMultiplicity() > 0
                  and jet->chargedEmEnergyFraction() < 0.99;
            }
         } else {
            id_forw = jet->neutralEmEnergyFraction() < 0.9 and jet->neutralMultiplicity() > 10;
         }
         jetPuppi_passid.push_back( id_24 and id_30 and id_forw );

         //jet_corrL1.push_back( corrL1->correction(*jet, iEvent, iSetup) );
         //jet_corrL123.push_back( corrL123->correction(*jet, iEvent, iSetup) );

      }else{

         // add the (corrected) jet to the met_sumpt
         // (was subtracted previously)
         metPuppi_sumpt += jpt;

      }
   }


   events_total++;
   bool pass_selection = (nmuons == 2) and (dimuon_mass > 60) and (dimuon_mass < 120);
   if( pass_selection ){
      results_tree -> Fill();
      events_pass++;
      //std::cout << cleanjets.size() << " / " << jets.size() << std::endl;
      //std::cout << "Muon charge prod = " << charge; 
      //if( charge > 0 ) std::cout << "****************************************************************";
      //std::cout << std::endl;
      //fflush(stdout);

   }

   //delete ptRes_;
   //delete phiRes_;

}

   std::vector<pat::Jet>
MakeNtuple::cleanJets(double ptThreshold, double dRmatch,
      std::vector<pat::Jet>& jets, std::vector<reco::Candidate::LorentzVector>& leptons)
{
   double dR2match = dRmatch*dRmatch;
   std::vector<pat::Jet> retVal;
   for ( std::vector<pat::Jet>::const_iterator jet = jets.begin();
         jet != jets.end(); ++jet ) {
      bool isOverlap = false;
      for ( std::vector<reco::Candidate::LorentzVector>::const_iterator lepton = leptons.begin();
            lepton != leptons.end(); ++lepton ) {
         TLorentzVector ljet, llep;
         ljet.SetPtEtaPhiE( jet->pt(), jet->eta(), jet->phi(), jet->energy() );
         llep.SetPtEtaPhiE( lepton->pt(), lepton->eta(), lepton->phi(), lepton->energy() );
         if ( pow(ljet.DeltaR( llep ),2) < dR2match ) isOverlap = true;
      }
      //if ( jet->pt() > ptThreshold && !isOverlap ){
      if ( !isOverlap ){
         retVal.push_back(*jet);
      }
   }

   return retVal;
}


// ------------ method called once each job just before starting event loop  ------------
void 
MakeNtuple::beginJob()
{

   events_total=0, events_pass=0;

   //   OutFile__file  = new TFile( OutputFileName_.c_str(), "RECREATE" );

   // pileup reweighting
   std::vector< float > PU2015_MC;
   std::vector< float > PU2015_Data;

   for( int i=0; i<NUMPUBINS; i++) {
      PU2015_MC.push_back( PU2015_MCf[i] );
      PU2015_Data.push_back( PU2015_Dataf[i] );
   }
   LumiWeights_ = edm::LumiReWeighting( PU2015_MC, PU2015_Data);

   results_tree = fs->make<TTree>("events", "events");
   results_tree -> Branch("run", &run, "run/I");
   results_tree -> Branch("lumi", &lumi, "lumi/I");
   results_tree -> Branch("event", &event, "event/I");
   results_tree -> Branch("mcweight", &mcweight, "mcweight/D");
   //results_tree -> Branch("mcweightSum", &mcweightSum, "mcweightSum/D");

   results_tree -> Branch("muon_pt", &muon_pt);
   results_tree -> Branch("muon_energy", &muon_energy);
   results_tree -> Branch("muon_phi", &muon_phi);
   results_tree -> Branch("muon_eta", &muon_eta);

   /*
   results_tree -> Branch("lep_pt", &lep_pt);
   results_tree -> Branch("lep_energy", &lep_energy);
   results_tree -> Branch("lep_phi", &lep_phi);
   results_tree -> Branch("lep_eta", &lep_eta);
   */

   results_tree -> Branch("jet_pt", &jet_pt);
   results_tree -> Branch("jet_energy", &jet_energy);
   results_tree -> Branch("jet_phi", &jet_phi);
   results_tree -> Branch("jet_eta", &jet_eta);
   results_tree -> Branch("jet_sigmapt", &jet_sigmapt);
   results_tree -> Branch("jet_sf", &jet_sf);
   results_tree -> Branch("jet_sigmaphi", &jet_sigmaphi);
   results_tree -> Branch("jet_passid", &jet_passid);
   results_tree -> Branch("jetPuppi_pt", &jetPuppi_pt);
   results_tree -> Branch("jetPuppi_energy", &jetPuppi_energy);
   results_tree -> Branch("jetPuppi_phi", &jetPuppi_phi);
   results_tree -> Branch("jetPuppi_eta", &jetPuppi_eta);
   results_tree -> Branch("jetPuppi_sigmapt", &jetPuppi_sigmapt);
   results_tree -> Branch("jetPuppi_sf", &jetPuppi_sf);
   results_tree -> Branch("jetPuppi_sigmaphi", &jetPuppi_sigmaphi);
   results_tree -> Branch("jetPuppi_passid", &jetPuppi_passid);
   //results_tree -> Branch("jet_corrL1", &jet_corrL1);
   //results_tree -> Branch("jet_corrL123", &jet_corrL123);

   results_tree -> Branch("met_pt", &met_pt);
   results_tree -> Branch("met_energy", &met_energy);
   results_tree -> Branch("met_phi", &met_phi);
   results_tree -> Branch("met_eta", &met_eta);
   results_tree -> Branch("met_sumpt", &met_sumpt);

   results_tree -> Branch("metPuppi_pt", &metPuppi_pt);
   results_tree -> Branch("metPuppi_energy", &metPuppi_energy);
   results_tree -> Branch("metPuppi_phi", &metPuppi_phi);
   results_tree -> Branch("metPuppi_eta", &metPuppi_eta);
   results_tree -> Branch("metPuppi_sumpt", &metPuppi_sumpt);

   results_tree -> Branch("nvertices", &nvertices);
   results_tree -> Branch("weight_pu", &weight_pu);
   results_tree -> Branch("nvert_true", &T_nvertices);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
MakeNtuple::endJob() 
{
    //    OutFile__file -> Write();
    //OutFile__file -> Close();
   std::cout << " *** NUMBER OF EVENTS PASSING SELECTION *** " << std::endl;
   std::cout << " ------> " << events_pass << " / " << events_total << " = " << double(events_pass)/events_total << std::endl;
}

// ------------ method called when starting to processes a run  ------------
/*
void 
MakeNtuple::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
MakeNtuple::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
MakeNtuple::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
MakeNtuple::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MakeNtuple::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MakeNtuple);
