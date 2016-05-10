#ifndef AlignmentTool_SkimTest_h
#define AlignmentTool_SkimTest_h



// -*- C++ -*-
//// -*- C++ -*-
//
// Package:    SkimTest
// Class:      SkimTest
//
#include <memory>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"


#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"
//#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
//#include "DataFormats/GeometrySurface/interface/Surface.h"
//#include "DataFormats/GeometrySurface/interface/SimpleDiskBounds.h"

// ROOT include files
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>

//
// class declaration
//
using namespace edm;
using namespace reco;   
using namespace std; 

class SkimTest : public edm::EDAnalyzer  // public edm::EDAnalyzer
{
public:
  explicit SkimTest(const edm::ParameterSet&);
  virtual ~SkimTest();
    // beginJob
  virtual void beginJob() ;
  // produce is where the ntuples are made
  virtual void analyze(const edm::Event &, const edm::EventSetup & );
  // endJob
  virtual void endJob() ;


protected:
  void initAllPara(const edm::ParameterSet & );
  // The main sub-object which does the real work
  // Verbosity
  Long64_t _evt_run, _good_tracks; 
  Long64_t _runNum, _evtNum;

  Int_t Ntrack; 

  edm::Service<TFileService> f;

  TH1D *ESpF_residualX;   TH1D *ESpF_residualY;

private:
  bool isEndCapTrack(reco::Track track);
  bool isGoodTrack(reco::Track track);
  //edm::InputTag RecHitLabel_;
  //edm::InputTag TrackLabel_;
  edm::EDGetTokenT<ESRecHitCollection> RecHitLabel_;
  edm::EDGetTokenT<TrackCollection> TrackLabel_;

};

#endif

