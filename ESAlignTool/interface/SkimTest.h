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
  Long64_t _evt_run; 
  Long64_t _runNum, _evtNum;

  Int_t Ntrack; 

  edm::Service<TFileService> f;

  TH1D *ESpF_residualX;   TH1D *ESpF_residualY;

private:
  /*double detSym(double,double,double,double,double,double);
  double GetIterX(double,double,double,double,double,double,double,double,double);
  double GetIterY(double,double,double,double,double,double,double,double,double);
  double GetIterZ(double,double,double,double,double,double,double,double,double);

  int GetIteration(int,int,double &,double &,double &);
  void setInvMError(int,int,double,double,double,double,double,double,double,double,double);
  int GetIterationError(int,int,double &,double &,double &);*/

  edm::InputTag RecHitLabel_;
  edm::InputTag TrackLabel_;
 
};

#endif

