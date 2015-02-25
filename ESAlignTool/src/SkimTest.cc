// -*- C++ -*-
//
// Package:    SkimTest
// Class:      SkimTest
// 
/**\class SkimTest SkimTest.cc AlignmentTool/SkimTest/src/SkimTest.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Kai-Yi Kao,27 2-020,+41227674870,
//         Created:  Wed Jun 16 09:36:53 CEST 2010
// $Id: SkimTest.cc,v 1.5 2011/05/26 12:27:33 chiyi Exp $
//
//


// system include files
#include <memory>

// user include files
#include "AlignmentTool/ESAlignTool/interface/SkimTest.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"
//ROOT includes
#include "TMath.h"
#include <Math/VectorUtil.h>
#include <TLorentzVector.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>

//
// constructors and destructor
//
SkimTest::SkimTest(const edm::ParameterSet& iConfig)
{
 std::cout << "In SkimTest Constructor\n" ;
 initAllPara(iConfig);
}

SkimTest::~SkimTest()
{
 std::cout << "In SkimTest destructor\n" ;
} 


void SkimTest::initAllPara(const edm::ParameterSet& iConfig)
{
 std::cout<<"Initializing All Parameters..."<<std::endl;
  _evt_run=_good_tracks=0;	
 RecHitLabel_ = iConfig.getParameter<edm::InputTag>("RecHitLabel"); 
 TrackLabel_ = iConfig.getParameter<edm::InputTag>("TrackLabel"); 
}

void 
SkimTest::beginJob()
{}

bool SkimTest::isEndCapTrack( reco::Track track )
{
	if( fabs(track.eta())<3 && fabs(track.eta())>1.5 ) return true;
	else return false;
}
bool SkimTest::isGoodTrack( reco::Track track )
{ 
 	if( track.pt()>1.5 && 
	    fabs(track.outerZ())>260&&fabs(track.outerZ())<280 &&
	    fabs(track.eta())<2.3&&fabs(track.eta())>1.7 &&
	    track.found()>=10 && 
	    ((track.qualityMask())%8) >= 4 //highPurity Bit
	) return true;
	else return false;

}
// ------------ method called to for each event  ------------
void
SkimTest::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

 //1.initialize for each events
 std::cout << "\nIn SkimTest.analyze()\n";
 std::cout << "Event : "<<_evt_run<<"\n";
 
 _runNum = iEvent.id().run();
 _evtNum = iEvent.id().event();

  std::cout<<"Pass EvtRun Selection.\n";
 
  edm::Handle<EcalRecHitCollection> PreshowerRecHits;
  iEvent.getByLabel(RecHitLabel_, PreshowerRecHits);
  std::cout << " number of ES reco-hits " << PreshowerRecHits->size() << std::endl;
 
 
  edm::Handle<reco::TrackCollection>   TrackCol;
  iEvent.getByLabel( TrackLabel_,      TrackCol );
  std::cout << " number of tracks " << TrackCol->size() << std::endl;

  int NtrackEndCap=0;
  int NtrackGood=0;
  int Ntrack = TrackCol->size();
  if( TrackCol.isValid() ){ 
	  for( reco::TrackCollection::const_iterator itTrack = TrackCol->begin(); itTrack != TrackCol->end(); ++itTrack){
		  if( isEndCapTrack(*itTrack) ) NtrackEndCap++;
		  if( isGoodTrack(*itTrack) ) NtrackGood++;
		  std::cout<<" Track "<<Ntrack<<std::endl;
		  std::cout<<"   pT "<<itTrack->pt()<<std::endl;
		  std::cout<<"   outerZ "<<itTrack->outerZ()<<std::endl;
		  std::cout<<"   eta "<<fabs(itTrack->eta())<<std::endl;
		  std::cout<<"   quality "<<(itTrack->qualityMask())%8<<std::endl;
		  std::cout<<"   Num all hits "<<itTrack->recHitsSize()<<std::endl;
		  std::cout<<"   Num validated hits "<<itTrack->found()<<std::endl;
		  int nValidHit=0;
		  //for( unsigned int iHit = 0; iHit < itTrack->recHitsSize(); iHit++) {
		  for( unsigned int iHit = 0; iHit < itTrack->found(); iHit++) {
			  TrackingRecHit *hit = itTrack->recHit(iHit)->clone();
			  if( hit->isValid()) nValidHit++;
			  //std::cout<<"      Hits "<<iHit<<" isValid "<<hit->isValid()<<std::endl;
			  delete hit;
		  }
		  std::cout<<"   Num validated hits, by checking each hit "<<nValidHit<<std::endl;
		  Ntrack++;
	  }
	  std::cout<<"    Num tacks in endcap "<<NtrackEndCap<<"/"<<Ntrack<<std::endl;
	  std::cout<<"    Num tacks is good in endcap "<<NtrackGood<<"/"<<NtrackEndCap<<std::endl;
  }
  _good_tracks+=NtrackGood; 
  _evt_run++;	
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SkimTest::endJob() {
  std::cout<<std::endl << "In SkimTest.endJob\n";

  std::cout << std::endl;
  std::cout << " --------------------------------------------- " << std::endl;
  std::cout << " number of events processed  " << _evt_run << std::endl; 
  std::cout << " number of good track in endcap  " << _good_tracks << std::endl; 
  std::cout << " Last Event number # " << _evtNum << std::endl; 
  std::cout << " --------------------------------------------- " << std::endl;


}

//define this as a plug-in
DEFINE_FWK_MODULE(SkimTest);
