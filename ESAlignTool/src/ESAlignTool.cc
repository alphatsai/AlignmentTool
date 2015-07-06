// -*- C++ -*-
//
// Package:    ESAlignTool
// Class:      ESAlignTool
// 
/**\class ESAlignTool ESAlignTool.cc AlignmentTool/ESAlignTool/src/ESAlignTool.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/


// system include files
#include <memory>

// user include files
#include "AlignmentTool/ESAlignTool/interface/ESAlignTool.h"
#include "AlignmentTool/ESAlignTool/plugins/ESAlignSelections.C"
#include "AlignmentTool/ESAlignTool/plugins/ESAlignDeadZone.C"
#include "AlignmentTool/ESAlignTool/func/BadSensorLogic.C"
#include "AlignmentTool/ESAlignTool/func/AlignmentCalculation.C"

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
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
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
ESAlignTool::ESAlignTool(const edm::ParameterSet& iConfig)
{
	std::cout << "In ESAlignTool Constructor\n" ;
	initAllPara(iConfig);
}

ESAlignTool::~ESAlignTool()
{

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)
	std::cout << "In ESAlignTool destructor\n" ;
} 


void ESAlignTool::initAllPara(const edm::ParameterSet& iConfig)
{
	std::cout<<"Initializing All Parameters..."<<std::endl;

	ESpF_Printed=0; ESpR_Printed=0;
	ESmF_Printed=0; ESmR_Printed=0;

	for(int i=0;i<2;i++)
	{for(int j=0;j<2;j++)
		{
			ES_M11[i][j]=0.; ES_M12[i][j]=0.; ES_M13[i][j]=0.;
			ES_M14[i][j]=0.; ES_M15[i][j]=0.; ES_M16[i][j]=0.;
			ES_M22[i][j]=0.; ES_M23[i][j]=0.; ES_M24[i][j]=0.;
			ES_M25[i][j]=0.; ES_M26[i][j]=0.;
			ES_M33[i][j]=0.; ES_M34[i][j]=0.; ES_M35[i][j]=0.;
			ES_M36[i][j]=0.;
			ES_M44[i][j]=0.; ES_M45[i][j]=0.; ES_M46[i][j]=0.;
			ES_M55[i][j]=0.; ES_M56[i][j]=0.;
			ES_M66[i][j]=0.;

			ES_P1[i][j]=0.; ES_P2[i][j]=0.; ES_P3[i][j]=0.;
			ES_P4[i][j]=0.; ES_P5[i][j]=0.; ES_P6[i][j]=0.;
			ES_CHI2[i][j]=0.;
			ES_NTracks[i][j]=0;
			ES_dX[i][j]=0.; ES_dY[i][j]=0.; ES_dZ[i][j]=0.;
			ES_dXerr[i][j]=0.; ES_dYerr[i][j]=0.; ES_dZerr[i][j]=0.;
			ES_dAlpha[i][j]=0.; ES_dBeta[i][j]=0.; ES_dGamma[i][j]=0.;
			ES_dAlphaerr[i][j]=0.; ES_dBetaerr[i][j]=0.; ES_dGammaerr[i][j]=0.;

			ES_M31Err2[i][j]=0.; ES_M32Err2[i][j]=0.; ES_M33Err2[i][j]=0.;
			ES_P1Err2[i][j]=0.; ES_P2Err2[i][j]=0.; ES_P3Err2[i][j]=0.;

			ES_InvM11Err2[i][j]=0.; ES_InvM12Err2[i][j]=0.; ES_InvM13Err2[i][j]=0.;
			ES_InvM22Err2[i][j]=0.; ES_InvM23Err2[i][j]=0.; ES_InvM33Err2[i][j]=0.;

			ES_R11[i][j]=0.; ES_R12[i][j]=0.; ES_R13[i][j]=0.;
			ES_R21[i][j]=0.; ES_R22[i][j]=0.; ES_R23[i][j]=0.;
			ES_R31[i][j]=0.; ES_R32[i][j]=0.; ES_R33[i][j]=0.;

			ES_O_R11[i][j]=1.; ES_O_R12[i][j]=0.; ES_O_R13[i][j]=0.;
			ES_O_R21[i][j]=0.; ES_O_R22[i][j]=1.; ES_O_R23[i][j]=0.;
			ES_O_R31[i][j]=0.; ES_O_R32[i][j]=0.; ES_O_R33[i][j]=1.;

			ES_Alpha[i][j]=0.; ES_Beta[i][j]=0.; ES_Gamma[i][j]=0;
			ES_O_Alpha[i][j]=0.; ES_O_Beta[i][j]=0.; ES_O_Gamma[i][j]=0;
		}}

		e_xxlimit = iConfig.getParameter<double>("e_xxlimit");
		e_yylimit = iConfig.getParameter<double>("e_yylimit");
		e_yxlimit = iConfig.getParameter<double>("e_yxlimit");
		winlimit = iConfig.getParameter<double>("winlimit");

		b_DrawMagField = iConfig.getParameter<bool>("DrawMagField");
		b_PrintPosition = iConfig.getParameter<bool>("PrintPosition");
		b_ReSetRfromOutside = iConfig.getParameter<bool>("ReSetRfromOutside");
		b_InputRefitter = iConfig.getParameter<bool>("InputRefitter");

		Cal_ESorigin_from_Geometry = iConfig.getParameter<bool>("CalculateESorigin");
		Cal_ESaxes_from_Geometry = iConfig.getParameter<bool>("CalculateESaxes");
		b_Overwrite_RotationMatrix_fromGeometry = iConfig.getParameter<bool>("OverwriteRotationM");

		b_storeDetail = iConfig.getParameter<bool>("StoreDetail");
		b_PrintMatrix = iConfig.getParameter<bool>("PrintMatrix");
		Selected_idee = iConfig.getParameter<unsigned int>("Selected_idee");
		Selected_RUNmin = iConfig.getParameter<int>("Selected_RUNmin");
		Selected_RUNmax = iConfig.getParameter<int>("Selected_RUNmax");

		RecHitLabel_ = iConfig.getParameter<edm::InputTag>("RecHitLabel"); 
		TrackLabel_ = iConfig.getParameter<edm::InputTag>("TrackLabel"); 

		iterN = iConfig.getParameter<unsigned int>("IterN");
		if(iterN<0||iterN>11) std::cout<<"Error : Out of Range for iterN!!!!\n";

		DefaultESLocation_ = iConfig.getParameter<edm::ParameterSet>("DefaultESLocation");
		ESpF_defaultX = DefaultESLocation_.getParameter<double>("ESpF_X");
		ESpF_defaultY = DefaultESLocation_.getParameter<double>("ESpF_Y");
		ESpF_defaultZ = DefaultESLocation_.getParameter<double>("ESpF_Z");
		ESpF_defaultAlpha = DefaultESLocation_.getParameter<double>("ESpF_Alpha");
		ESpF_defaultBeta  = DefaultESLocation_.getParameter<double>("ESpF_Beta");
		ESpF_defaultGamma = DefaultESLocation_.getParameter<double>("ESpF_Gamma");
		ESmF_defaultX = DefaultESLocation_.getParameter<double>("ESmF_X");
		ESmF_defaultY = DefaultESLocation_.getParameter<double>("ESmF_Y");
		ESmF_defaultZ = DefaultESLocation_.getParameter<double>("ESmF_Z");
		ESmF_defaultAlpha = DefaultESLocation_.getParameter<double>("ESmF_Alpha");
		ESmF_defaultBeta  = DefaultESLocation_.getParameter<double>("ESmF_Beta");
		ESmF_defaultGamma = DefaultESLocation_.getParameter<double>("ESmF_Gamma");
		ESpR_defaultX = DefaultESLocation_.getParameter<double>("ESpR_X");
		ESpR_defaultY = DefaultESLocation_.getParameter<double>("ESpR_Y");
		ESpR_defaultZ = DefaultESLocation_.getParameter<double>("ESpR_Z");
		ESpR_defaultAlpha = DefaultESLocation_.getParameter<double>("ESpR_Alpha");
		ESpR_defaultBeta  = DefaultESLocation_.getParameter<double>("ESpR_Beta");
		ESpR_defaultGamma = DefaultESLocation_.getParameter<double>("ESpR_Gamma");
		ESmR_defaultX = DefaultESLocation_.getParameter<double>("ESmR_X");
		ESmR_defaultY = DefaultESLocation_.getParameter<double>("ESmR_Y");
		ESmR_defaultZ = DefaultESLocation_.getParameter<double>("ESmR_Z");
		ESmR_defaultAlpha = DefaultESLocation_.getParameter<double>("ESmR_Alpha");
		ESmR_defaultBeta  = DefaultESLocation_.getParameter<double>("ESmR_Beta");
		ESmR_defaultGamma = DefaultESLocation_.getParameter<double>("ESmR_Gamma");

		MatrixElements_ = iConfig.getParameter<edm::ParameterSet>("MatrixElements");
		char buf[20];
		for(int iterN_idx=1;iterN_idx<iterN;iterN_idx++)
		{
			sprintf(buf,"Iter%i_ESpFdX",iterN_idx);  iter_ESpFdX[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESpFdY",iterN_idx);  iter_ESpFdY[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESpFdZ",iterN_idx);  iter_ESpFdZ[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESpRdX",iterN_idx);  iter_ESpRdX[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESpRdY",iterN_idx);  iter_ESpRdY[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESpRdZ",iterN_idx);  iter_ESpRdZ[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmFdX",iterN_idx);  iter_ESmFdX[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmFdY",iterN_idx);  iter_ESmFdY[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmFdZ",iterN_idx);  iter_ESmFdZ[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmRdX",iterN_idx);  iter_ESmRdX[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmRdY",iterN_idx);  iter_ESmRdY[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmRdZ",iterN_idx);  iter_ESmRdZ[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);

			sprintf(buf,"Iter%i_ESpFdAlpha",iterN_idx);  iter_ESpFdAlpha[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESpFdBeta",iterN_idx);   iter_ESpFdBeta[iterN_idx-1]  = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESpFdGamma",iterN_idx);  iter_ESpFdGamma[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESpRdAlpha",iterN_idx);  iter_ESpRdAlpha[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESpRdBeta",iterN_idx);   iter_ESpRdBeta[iterN_idx-1]  = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESpRdGamma",iterN_idx);  iter_ESpRdGamma[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmFdAlpha",iterN_idx);  iter_ESmFdAlpha[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmFdBeta",iterN_idx);   iter_ESmFdBeta[iterN_idx-1]  = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmFdGamma",iterN_idx);  iter_ESmFdGamma[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmRdAlpha",iterN_idx);  iter_ESmRdAlpha[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmRdBeta",iterN_idx);   iter_ESmRdBeta[iterN_idx-1]  = MatrixElements_.getParameter<double>(buf);
			sprintf(buf,"Iter%i_ESmRdGamma",iterN_idx);  iter_ESmRdGamma[iterN_idx-1] = MatrixElements_.getParameter<double>(buf);
		}
		for(int iterN_idx=1;iterN_idx<iterN;iterN_idx++)
		{
			ES_dX[0][0] -= iter_ESmFdX[iterN_idx-1]; ES_dY[0][0] -= iter_ESmFdY[iterN_idx-1]; ES_dZ[0][0] -= iter_ESmFdZ[iterN_idx-1];
			ES_dX[0][1] -= iter_ESmRdX[iterN_idx-1]; ES_dY[0][1] -= iter_ESmRdY[iterN_idx-1]; ES_dZ[0][1] -= iter_ESmRdZ[iterN_idx-1];
			ES_dX[1][0] -= iter_ESpFdX[iterN_idx-1]; ES_dY[1][0] -= iter_ESpFdY[iterN_idx-1]; ES_dZ[1][0] -= iter_ESpFdZ[iterN_idx-1];
			ES_dX[1][1] -= iter_ESpRdX[iterN_idx-1]; ES_dY[1][1] -= iter_ESpRdY[iterN_idx-1]; ES_dZ[1][1] -= iter_ESpRdZ[iterN_idx-1];

			ES_dAlpha[0][0] -= iter_ESmFdAlpha[iterN_idx-1]; ES_dBeta[0][0] -= iter_ESmFdBeta[iterN_idx-1]; ES_dGamma[0][0] -= iter_ESmFdGamma[iterN_idx-1];
			ES_dAlpha[0][1] -= iter_ESmRdAlpha[iterN_idx-1]; ES_dBeta[0][1] -= iter_ESmRdBeta[iterN_idx-1]; ES_dGamma[0][1] -= iter_ESmRdGamma[iterN_idx-1];
			ES_dAlpha[1][0] -= iter_ESpFdAlpha[iterN_idx-1]; ES_dBeta[1][0] -= iter_ESpFdBeta[iterN_idx-1]; ES_dGamma[1][0] -= iter_ESpFdGamma[iterN_idx-1];
			ES_dAlpha[1][1] -= iter_ESpRdAlpha[iterN_idx-1]; ES_dBeta[1][1] -= iter_ESpRdBeta[iterN_idx-1]; ES_dGamma[1][1] -= iter_ESpRdGamma[iterN_idx-1];
		}

		// All default will be repalce by re-caculate axes or position, if you open the option.
		_evt_run = 0;
		woRotate=new RotationType(); //defualt
		ESpF_O=new PositionType( ESpF_defaultX, ESpF_defaultY, ESpF_defaultZ); //default
		ESpR_O=new PositionType( ESpR_defaultX, ESpR_defaultY, ESpR_defaultZ); //default
		ESmF_O=new PositionType( ESmF_defaultX, ESmF_defaultY, ESmF_defaultZ); //default 
		ESmR_O=new PositionType( ESmR_defaultX, ESmR_defaultY, ESmR_defaultZ); //default
		ESpF_Oap=new PositionType( ESpF_defaultX+ES_dX[1][0], ESpF_defaultY+ES_dY[1][0], ESpF_defaultZ+ES_dZ[1][0]); //default
		ESpR_Oap=new PositionType( ESpR_defaultX+ES_dX[1][1], ESpR_defaultY+ES_dY[1][1], ESpR_defaultZ+ES_dZ[1][1]); //default
		ESmF_Oap=new PositionType( ESmF_defaultX+ES_dX[0][0], ESmF_defaultY+ES_dY[0][0], ESmF_defaultZ+ES_dZ[0][0]); //default 
		ESmR_Oap=new PositionType( ESmR_defaultX+ES_dX[0][1], ESmR_defaultY+ES_dY[0][1], ESmR_defaultZ+ES_dZ[0][1]); //default
		UnAligned_Original( 1, 0, ESpF_O);  Aligned_Original( 1, 0, ESpF_Oap);
		UnAligned_Original( 1, 1, ESpR_O);  Aligned_Original( 1, 1, ESpR_Oap);
		UnAligned_Original( 0, 0, ESmF_O);  Aligned_Original( 0, 0, ESmF_Oap);
		UnAligned_Original( 0, 1, ESmR_O);  Aligned_Original( 0, 1, ESmR_Oap);

		if(b_ReSetRfromOutside) std::cout<<"Nothing to do"<<std::endl;

		ES_O_Alpha[1][0]=ESpF_defaultAlpha; ES_O_Beta[1][0]=ESpF_defaultBeta; ES_O_Gamma[1][0]=ESpF_defaultGamma; //default
		ES_O_Alpha[1][1]=ESpR_defaultAlpha; ES_O_Beta[1][1]=ESpR_defaultBeta; ES_O_Gamma[1][1]=ESpR_defaultGamma; //default
		ES_O_Alpha[0][0]=ESmF_defaultAlpha; ES_O_Beta[0][0]=ESmF_defaultBeta; ES_O_Gamma[0][0]=ESmF_defaultGamma; //default
		ES_O_Alpha[0][1]=ESmR_defaultAlpha; ES_O_Beta[0][1]=ESmR_defaultBeta; ES_O_Gamma[0][1]=ESmR_defaultGamma; //default
		for(int i=0;i<2;i++) //default
		{for(int j=0;j<2;j++)
			{
				ES_Alpha[i][j]=ES_dAlpha[i][j]+ES_O_Alpha[i][j]; 
				ES_Beta[i][j] =ES_dBeta[i][j] +ES_O_Beta[i][j]; 
				ES_Gamma[i][j]=ES_dGamma[i][j]+ES_O_Gamma[i][j];
				Aligned_RotationMatrices(i, j, ES_Alpha[i][j], ES_Beta[i][j], ES_Gamma[i][j]);
				UnAligned_RotationMatrices(i, j, ES_O_Alpha[i][j], ES_O_Beta[i][j], ES_O_Gamma[i][j]);
			}} //default

			ESpF_residualX=f->make<TH1D>("ESpF_residualX","ES+Front residualX",300,-15,15); 
			ESpF_residualY=f->make<TH1D>("ESpF_residualY","ES+Front residualY",300,-15,15);
			ESpR_residualX=f->make<TH1D>("ESpR_residualX","ES+Rear residualX",300,-15,15);
			ESpR_residualY=f->make<TH1D>("ESpR_residualY","ES+Rear residualY",300,-15,15);
			ESmF_residualX=f->make<TH1D>("ESmF_residualX","ES-Front residualX",300,-15,15);
			ESmF_residualY=f->make<TH1D>("ESmF_residualY","ES-Front residualY",300,-15,15);
			ESmR_residualX=f->make<TH1D>("ESmR_residualX","ES-Rear residualX",300,-15,15);
			ESmR_residualY=f->make<TH1D>("ESmR_residualY","ES-Rear residualY",300,-15,15);

}
void ESAlignTool::UnAligned_Original( int iz, int ip, PositionType* plan){
	ES_O_X[iz][ip]=plan->x(); 
	ES_O_Y[iz][ip]=plan->y(); 
	ES_O_Z[iz][ip]=plan->z();
}
void ESAlignTool::Aligned_Original( int iz, int ip, PositionType* plan){
	ES_Oap_X[iz][ip]=plan->x(); 
	ES_Oap_Y[iz][ip]=plan->y(); 
	ES_Oap_Z[iz][ip]=plan->z();
}
void ESAlignTool::UnAligned_RotationMatrices( int iz, int ip, double Alpha, double Beta, double Gamma){
	ES_O_R11[iz][ip] = cos(Beta)*cos(Gamma) - sin(Alpha)*sin(Beta)*sin(Gamma);
	ES_O_R12[iz][ip] = cos(Beta)*sin(Gamma) + sin(Alpha)*sin(Beta)*cos(Gamma);
	ES_O_R13[iz][ip] = -cos(Alpha)*sin(Beta);
	ES_O_R21[iz][ip] = -cos(Alpha)*sin(Gamma);
	ES_O_R22[iz][ip] = cos(Alpha)*cos(Gamma);
	ES_O_R23[iz][ip] = sin(Alpha);
	ES_O_R31[iz][ip] = sin(Beta)*cos(Gamma) + sin(Alpha)*cos(Beta)*sin(Gamma);
	ES_O_R32[iz][ip] = sin(Beta)*sin(Gamma) - sin(Alpha)*cos(Beta)*cos(Gamma);
	ES_O_R33[iz][ip] = cos(Alpha)*cos(Beta);
}
void ESAlignTool::Aligned_RotationMatrices( int iz, int ip, double Alpha, double Beta, double Gamma){
	ES_R11[iz][ip] = cos(Beta)*cos(Gamma) - sin(Alpha)*sin(Beta)*sin(Gamma);
	ES_R12[iz][ip] = cos(Beta)*sin(Gamma) + sin(Alpha)*sin(Beta)*cos(Gamma);
	ES_R13[iz][ip] = -cos(Alpha)*sin(Beta);
	ES_R21[iz][ip] = -cos(Alpha)*sin(Gamma);
	ES_R22[iz][ip] = cos(Alpha)*cos(Gamma);
	ES_R23[iz][ip] = sin(Alpha);
	ES_R31[iz][ip] = sin(Beta)*cos(Gamma) + sin(Alpha)*cos(Beta)*sin(Gamma);
	ES_R32[iz][ip] = sin(Beta)*sin(Gamma) - sin(Alpha)*cos(Beta)*cos(Gamma);
	ES_R33[iz][ip] = cos(Alpha)*cos(Beta);
}

//
// member functions
//

// ------------ method called to for each event  ------------
	void
ESAlignTool::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	//1.initialize for each events
	std::cout << "\nIn ESAlignTool.analyze()\n";
	std::cout << "Event : "<<_evt_run<<"\n";

	_runNum = iEvent.id().run();
	_evtNum = iEvent.id().event();
	init_perEvent();

	int Run = iEvent.id().run();
	int RunMin=Selected_RUNmin;
	int RunMax=Selected_RUNmax; //std::cout<<"RunMin="<<RunMin<<", RunMax="<<RunMax<<", Run="<<Run<<"\n";
	if(  Selected_RUNmin==0 || Selected_RUNmax==0
			|| ( RunMin!=0 && RunMax!=0 && Run >= RunMin && Run <= RunMax )
	  )   
	{
		std::cout<<"Pass EvtRun Selection.\n";

		ESHandle<CaloGeometry> caloGeometry;
		iSetup.get<CaloGeometryRecord>().get(caloGeometry);
		const CaloGeometry *caloGeom = caloGeometry.product();

		ESHandle<MagneticField> theMagField; 
		iSetup.get<IdealMagneticFieldRecord>().get(theMagField); 

		ESHandle<GlobalTrackingGeometry> theTrackingGeometry; 
		iSetup.get<GlobalTrackingGeometryRecord>().get( theTrackingGeometry ); 

		ESHandle<Propagator> shPropAlong;
		iSetup.get<TrackingComponentsRecord>().get("SteppingHelixPropagatorAlong", shPropAlong);

		//for MagneticField on ES
		if(b_DrawMagField)  LoadMagFieldonES(_evt_run,caloGeom,theMagField);
		//for Log some position
		if(b_PrintPosition) PrintPosition(_evt_run,caloGeom);


		//2.fill es hits
		edm::Handle<EcalRecHitCollection> PreshowerRecHits;
		iEvent.getByLabel(RecHitLabel_, PreshowerRecHits);
		fill_esRecHit(caloGeom,PreshowerRecHits);
		std::cout << " number of reco-hits " << PreshowerRecHits->size() << std::endl;


		//3.fill Track
		edm::Handle<reco::TrackCollection>   TrackCol;
		iEvent.getByLabel( TrackLabel_,      TrackCol );

		std::cout << " number of tracks " << TrackCol->size() << std::endl;

		fill_tracks(TrackCol);
		//3.1 Association for ESRecHits and tracks
		fill_esRecHitAssociation();


		//4.locate local ES origin
		set_ESorigin(_evt_run,caloGeom);
		set_ESaxes(_evt_run,caloGeom);
		if(b_Overwrite_RotationMatrix_fromGeometry) Overwrite_RotationMatrix(_evt_run);

		if( _evt_run == 1 ){ 
			std::cout<<std::endl;
			std::cout<<"UnAligned, P"<<std::endl;
			std::cout<<"-F X: "<<ES_O_X[0][0]<<", Y: "<<ES_O_Y[0][0]<<", Z: "<<ES_O_Z[0][0]<<std::endl;
			std::cout<<"-R X: "<<ES_O_X[0][1]<<", Y: "<<ES_O_Y[0][1]<<", Z: "<<ES_O_Z[0][1]<<std::endl;
			std::cout<<"+F X: "<<ES_O_X[1][0]<<", Y: "<<ES_O_Y[1][0]<<", Z: "<<ES_O_Z[1][0]<<std::endl;
			std::cout<<"+R X: "<<ES_O_X[1][1]<<", Y: "<<ES_O_Y[1][1]<<", Z: "<<ES_O_Z[1][1]<<std::endl;
			std::cout<<"-F Alpha: "<<ES_O_Alpha[0][0]<<", Beta: "<<ES_O_Beta[0][0]<<", Gamma: "<<ES_O_Gamma[0][0]<<std::endl;
			std::cout<<"-R Alpha: "<<ES_O_Alpha[0][1]<<", Beta: "<<ES_O_Beta[0][1]<<", Gamma: "<<ES_O_Gamma[0][1]<<std::endl;
			std::cout<<"+F Alpha: "<<ES_O_Alpha[1][0]<<", Beta: "<<ES_O_Beta[1][0]<<", Gamma: "<<ES_O_Gamma[1][0]<<std::endl;
			std::cout<<"+R Alpha: "<<ES_O_Alpha[1][1]<<", Beta: "<<ES_O_Beta[1][1]<<", Gamma: "<<ES_O_Gamma[1][1]<<std::endl;
			std::cout<<std::endl;
			std::cout<<"Aligned, P-dP"<<std::endl; 
			std::cout<<"-F X: "<<ES_Oap_X[0][0]<<", Y: "<<ES_Oap_Y[0][0]<<", Z: "<<ES_Oap_Z[0][0]<<std::endl;
			std::cout<<"-R X: "<<ES_Oap_X[0][1]<<", Y: "<<ES_Oap_Y[0][1]<<", Z: "<<ES_Oap_Z[0][1]<<std::endl;
			std::cout<<"+F X: "<<ES_Oap_X[1][0]<<", Y: "<<ES_Oap_Y[1][0]<<", Z: "<<ES_Oap_Z[1][0]<<std::endl;
			std::cout<<"+R X: "<<ES_Oap_X[1][1]<<", Y: "<<ES_Oap_Y[1][1]<<", Z: "<<ES_Oap_Z[1][1]<<std::endl;
			std::cout<<"-F Alpha: "<<ES_Alpha[0][0]<<", Beta: "<<ES_Beta[0][0]<<", Gamma: "<<ES_Gamma[0][0]<<std::endl;
			std::cout<<"-R Alpha: "<<ES_Alpha[0][1]<<", Beta: "<<ES_Beta[0][1]<<", Gamma: "<<ES_Gamma[0][1]<<std::endl;
			std::cout<<"+F Alpha: "<<ES_Alpha[1][0]<<", Beta: "<<ES_Beta[1][0]<<", Gamma: "<<ES_Gamma[1][0]<<std::endl;
			std::cout<<"+R Alpha: "<<ES_Alpha[1][1]<<", Beta: "<<ES_Beta[1][1]<<", Gamma: "<<ES_Gamma[1][1]<<std::endl;
		}


		//5.fill PredictionState on ES
		int iz=-1; int ip=1; int idee=0; //dee=1 -X/Y ; dee=2 +X/Y
		iz=1; ip=1;idee=0; fill_PredictionState(iz,ip,idee,TrackCol, theMagField, theTrackingGeometry, shPropAlong );
		iz=1; ip=2;idee=0; fill_PredictionState(iz,ip,idee,TrackCol, theMagField, theTrackingGeometry, shPropAlong );
		iz=-1;ip=1;idee=0; fill_PredictionState(iz,ip,idee,TrackCol, theMagField, theTrackingGeometry, shPropAlong );
		iz=-1;ip=2;idee=0; fill_PredictionState(iz,ip,idee,TrackCol, theMagField, theTrackingGeometry, shPropAlong );

		//6.Residual and Calculation
		iz=1; 
		fill_residual(iz);
		iz=-1;
		fill_residual(iz);

		t_ESAlign->Fill();

	}//end if seleted RUN
}


// ------------ method called once each job just before starting event loop  ------------
void ESAlignTool::init_perEvent()
{
	_evt_run++; Ntrack = Nesrh = 0;

	for ( int i=0; i<10000; i++)
	{
		_esRecHit_E[i]=-1; _esRecHit_X[i]=0; _esRecHit_Y[i]=0; _esRecHit_Z[i]=0;
		_esRecHit_siZ[i]=0;   _esRecHit_siP[i]=0; 
		_esRecHit_siX[i]=0;   _esRecHit_siY[i]=0;  _esRecHit_Strip[i]=0; 
		_esRecHit_Noisy[i]=0; 
		for( int j=0; j<2000; j++){
			_esRecHit_MatchedTrk_fromOuter[i][j]=0;
		}
	}//end init esRecHit

	for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			ES_CHI2[i][j]=0;
			ES_NTracks[i][j]=0;
			ES_M11[i][j]=0; ES_M12[i][j]=0; ES_M13[i][j]=0; ES_M14[i][j]=0; ES_M15[i][j]=0; ES_M16[i][j]=0;
			ES_M22[i][j]=0; ES_M23[i][j]=0; ES_M24[i][j]=0; ES_M25[i][j]=0; ES_M26[i][j]=0;
			ES_M33[i][j]=0; ES_M34[i][j]=0; ES_M35[i][j]=0; ES_M36[i][j]=0;
			ES_M44[i][j]=0; ES_M45[i][j]=0; ES_M46[i][j]=0;
			ES_M55[i][j]=0; ES_M56[i][j]=0;
			ES_M66[i][j]=0; ES_P1[i][j]=0; ES_P2[i][j]=0; ES_P3[i][j]=0; ES_P4[i][j]=0; ES_P5[i][j]=0; ES_P6[i][j]=0;
			ES_M31Err2[i][j]=0; ES_M32Err2[i][j]=0; ES_M33Err2[i][j]=0;
			ES_P1Err2[i][j]=0; ES_P2Err2[i][j]=0; ES_P3Err2[i][j]=0;
		}//end plan
	}//end z

	for ( int i=0; i<2000; i++ )
	{
		_TrackPt[i] = _TrackEta[i] = _TrackPhi[i] =0.; _TrackCharge[i] = 0;
		_TrackVx[i] = _TrackVy[i] = _TrackVz[i] =0.;
		_TrackNHit[i]=0; 
		_TrackNChi2[i]=0; 
		_Trackd0[i] = 0.; 
		_TrackPtError[i]=0.; _TrackQuality[i]=-1; 
		_TrackOuterZ[i]=0.; _TrackOuterEta[i]=0.; _TrackOuterPhi[i]=0.;
		_TrackInnerX[i]=0.; _TrackInnerY[i]=0.; _TrackInnerZ[i]=0.;

		for(int j=0;j<2;j++)
		{for(int k=0;k<2;k++)
			{
				PredictionState_iz[i][j][k]=0;
				PredictionState_ip[i][j][k]=0;
				PredictionState_valid[i][j][k]=0;
				PredictionState_X[i][j][k]=0.;
				PredictionState_Y[i][j][k]=0.;
				PredictionState_Z[i][j][k]=0.;
				PredictionState_Px[i][j][k]=0.;
				PredictionState_Py[i][j][k]=0.;
				PredictionState_Pz[i][j][k]=0.;
				PredictionState_Bx[i][j][k]=0.;
				PredictionState_By[i][j][k]=0.;
				PredictionState_Bz[i][j][k]=0.;
				PredictionState_Exx[i][j][k]=0.;
				PredictionState_Eyx[i][j][k]=0.;
				PredictionState_Eyy[i][j][k]=0.;
				PredictionState_E44[i][j][k]=0.;
				PredictionState_E55[i][j][k]=0.;
				PredictionState_E66[i][j][k]=0.;
				PredictionState_E45[i][j][k]=0.;
				PredictionState_E46[i][j][k]=0.;
				PredictionState_E56[i][j][k]=0.;
				PredictionState_E14[i][j][k]=0.;
				PredictionState_E15[i][j][k]=0.;
				PredictionState_E16[i][j][k]=0.;
				PredictionState_E24[i][j][k]=0.;
				PredictionState_E25[i][j][k]=0.;
				PredictionState_E26[i][j][k]=0.;
				PredictionState_delX[i][j][k]=0.;
				PredictionState_delY[i][j][k]=0.;
				PredictionState_MatchedRec[i][j][k]=-1;
				PredictionState_resiX[i][j][k]=0.;
				PredictionState_resiY[i][j][k]=0.;
			}}
	}//end init Track n PredictionState

}

void ESAlignTool::LoadMagFieldonES(Long64_t _evt_run, const CaloGeometry *caloGeom, edm::ESHandle<MagneticField> theMagField )
{
	if(_evt_run==1)
	{
		for(int ix=1;ix<=40;ix++)
		{for(int iy=1;iy<=40;iy++)
			{for(int is=1;is<=32;is++)
				{
					ESpF_B_x[ix-1][iy-1][is-1]=0.;
					ESpF_B_y[ix-1][iy-1][is-1]=0.;
					ESpF_B_z[ix-1][iy-1][is-1]=0.;
					ESpR_B_x[ix-1][iy-1][is-1]=0.;
					ESpR_B_y[ix-1][iy-1][is-1]=0.;
					ESpR_B_z[ix-1][iy-1][is-1]=0.;
					ESmF_B_x[ix-1][iy-1][is-1]=0.;
					ESmF_B_y[ix-1][iy-1][is-1]=0.;
					ESmF_B_z[ix-1][iy-1][is-1]=0.;
					ESmR_B_x[ix-1][iy-1][is-1]=0.;
					ESmR_B_y[ix-1][iy-1][is-1]=0.;
					ESmR_B_z[ix-1][iy-1][is-1]=0.;
				}}}
	}

	if(_evt_run==1)
	{
		for(int iz=-1;iz<2;iz=iz+2)
		{for(int ip=1;ip<=2;ip++)
			{for(int ix=1;ix<=40;ix++)
				{for(int iy=1;iy<=40;iy++)
					{for(int is=1;is<=32;is++)
						{
							if(ESDetId::validDetId(is,ix,iy,ip,iz))
							{
								ESDetId SaveMag(is,ix,iy,ip,iz);

								if(SaveMag.zside()==1&&SaveMag.plane()==1)
								{
									ESpF_B_x[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).x();
									ESpF_B_y[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).y();
									ESpF_B_z[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).z();
								}
								if(SaveMag.zside()==1&&SaveMag.plane()==2)
								{
									ESpR_B_x[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).x();
									ESpR_B_y[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).y();
									ESpR_B_z[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).z();
								}
								if(SaveMag.zside()==-1&&SaveMag.plane()==1)
								{
									ESmF_B_x[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).x();
									ESmF_B_y[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).y();
									ESmF_B_z[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).z();
								}
								if(SaveMag.zside()==-1&&SaveMag.plane()==2)
								{
									ESmR_B_x[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).x();
									ESmR_B_y[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).y();
									ESmR_B_z[SaveMag.six()-1][SaveMag.siy()-1][SaveMag.strip()-1]=theMagField->inTesla(caloGeom->getPosition(SaveMag)).z();
								}
							}}}}}}

							t_ESField->Fill();
	}
}

void ESAlignTool::PrintPosition(Long64_t _evt_run, const CaloGeometry *caloGeom)
{
	if(_evt_run==1)
	{
		for(int iz=-1;iz<2;iz=iz+2)
		{for(int ip=1;ip<=2;ip++)
			{for(int ix=1;ix<=40;ix++)
				{for(int iy=1;iy<=40;iy++)
					{for(int is=1;is<=32;is++)
						{
							if(ESDetId::validDetId(is,ix,iy,ip,iz))
							{
								ESDetId SaveMag(is,ix,iy,ip,iz);
								//For logout position //START
								if(  (  SaveMag.zside()==1&&SaveMag.plane()==1&&SaveMag.six()==5
											&&SaveMag.siy()==20&&SaveMag.strip()==1 
								     )
										||(  SaveMag.zside()==1&&SaveMag.plane()==1&&SaveMag.six()==35
											&&SaveMag.siy()==20&&SaveMag.strip()==1
										  )
										||(  SaveMag.zside()==1&&SaveMag.plane()==1&&SaveMag.six()==5
											&&SaveMag.siy()==20&&SaveMag.strip()==32
										  )
										||(  SaveMag.zside()==1&&SaveMag.plane()==1&&SaveMag.six()==5
											&&SaveMag.siy()==21&&SaveMag.strip()==1
										  )
										||(  SaveMag.zside()==1&&SaveMag.plane()==1&&SaveMag.six()==5
											&&SaveMag.siy()==21&&SaveMag.strip()==32
										  )
										||(  SaveMag.zside()==-1&&SaveMag.plane()==1&&SaveMag.six()==5
											&&SaveMag.siy()==20&&SaveMag.strip()==1
										  )
										||(  SaveMag.zside()==-1&&SaveMag.plane()==1&&SaveMag.six()==5
											&&SaveMag.siy()==20&&SaveMag.strip()==32
										  )
										||(  SaveMag.zside()==-1&&SaveMag.plane()==1&&SaveMag.six()==5
												&&SaveMag.siy()==21&&SaveMag.strip()==1
										  )
										||(  SaveMag.zside()==-1&&SaveMag.plane()==1&&SaveMag.six()==5
												&&SaveMag.siy()==21&&SaveMag.strip()==32
										  )
										||(  SaveMag.zside()==1&&SaveMag.plane()==2&&SaveMag.six()==20
												&&SaveMag.siy()==5&&SaveMag.strip()==1
										  )
										||(  SaveMag.zside()==1&&SaveMag.plane()==2&&SaveMag.six()==20
												&&SaveMag.siy()==5&&SaveMag.strip()==32
										  )
										||(  SaveMag.zside()==1&&SaveMag.plane()==2&&SaveMag.six()==21
												&&SaveMag.siy()==5&&SaveMag.strip()==1
										  )
										||(  SaveMag.zside()==1&&SaveMag.plane()==2&&SaveMag.six()==21
												&&SaveMag.siy()==5&&SaveMag.strip()==32
										  )
										||(  SaveMag.zside()==-1&&SaveMag.plane()==2&&SaveMag.six()==20
												&&SaveMag.siy()==5&&SaveMag.strip()==1
										  )
										||(  SaveMag.zside()==-1&&SaveMag.plane()==2&&SaveMag.six()==20
												&&SaveMag.siy()==5&&SaveMag.strip()==32
										  )
										||(  SaveMag.zside()==-1&&SaveMag.plane()==2&&SaveMag.six()==21
												&&SaveMag.siy()==5&&SaveMag.strip()==1
										  )
										||(  SaveMag.zside()==-1&&SaveMag.plane()==2&&SaveMag.six()==21
												&&SaveMag.siy()==5&&SaveMag.strip()==32
										  )
										)
										{
											std::cout<<"iz="<<SaveMag.zside()<<",ip="<<SaveMag.plane();
											std::cout<<",ix="<<SaveMag.six()<<",iy="<<SaveMag.siy();
											std::cout<<",is="<<SaveMag.strip()<<"\n";
											std::cout<<"(X,Y,Z)=("<<caloGeom->getPosition(SaveMag).x();
											std::cout<<","<<caloGeom->getPosition(SaveMag).y();
											std::cout<<","<<caloGeom->getPosition(SaveMag).z();
											std::cout<<")\n";
										}//For log out position //END
							}}}}}}}
}

void ESAlignTool::fill_esRecHit(const CaloGeometry *caloGeom, edm::Handle<EcalRecHitCollection> PreshowerRecHits)
{
	const ESRecHitCollection *ESRH = PreshowerRecHits.product();
	EcalRecHitCollection::const_iterator esrh_it;

	for ( esrh_it = ESRH->begin(); esrh_it != ESRH->end(); esrh_it++)
	{
		Double_t esrh_x = caloGeom->getPosition(esrh_it->id()).x();
		Double_t esrh_y = caloGeom->getPosition(esrh_it->id()).y();
		Double_t esrh_z = caloGeom->getPosition(esrh_it->id()).z();
		Double_t esrh_eta = caloGeom->getPosition(esrh_it->id()).eta();
		Double_t esrh_phi = caloGeom->getPosition(esrh_it->id()).phi();
		ESDetId esdetid = ESDetId(esrh_it->id());
		if(Nesrh>=10000)
		{
			edm::LogWarning("fill_esRecHit")<<"Too many ES RecHits.\n";
			continue;
		}
		_esRecHit_E[Nesrh] = esrh_it->energy(); 
		_esRecHit_X[Nesrh] = esrh_x; 
		_esRecHit_Y[Nesrh] = esrh_y; 
		_esRecHit_Z[Nesrh] = esrh_z; 
		_esRecHit_Eta[Nesrh] = esrh_eta; 
		_esRecHit_Phi[Nesrh] = esrh_phi; 
		_esRecHit_siZ[Nesrh] = esdetid.zside(); 
		_esRecHit_siP[Nesrh] = esdetid.plane(); 
		_esRecHit_siX[Nesrh] = esdetid.six(); 
		_esRecHit_siY[Nesrh] = esdetid.siy(); 
		_esRecHit_Strip[Nesrh] = esdetid.strip(); 
		Nesrh++;
	}

	for(int iesrh=0;iesrh<Nesrh;iesrh++)
	{for(int jesrh=iesrh+1;jesrh<Nesrh;jesrh++)
		{
			if(
					(_esRecHit_siZ[iesrh]==_esRecHit_siZ[jesrh])
					&&(_esRecHit_siP[iesrh]==_esRecHit_siP[jesrh])
					&&(_esRecHit_siX[iesrh]==_esRecHit_siX[jesrh])
					&&(_esRecHit_siY[iesrh]==_esRecHit_siY[jesrh]) 
			  )
			{ _esRecHit_Noisy[iesrh]=1; _esRecHit_Noisy[jesrh]=1; }
		}}//end find noisy sensor / ambiguous for matching

}

void ESAlignTool::fill_tracks(edm::Handle<reco::TrackCollection> TrackCol)
{
	for(reco::TrackCollection::const_iterator itTrack = TrackCol->begin();
			itTrack != TrackCol->end(); ++itTrack)
	{    
		if(Ntrack>=2000)
		{
			edm::LogWarning("fill_tracks")<<"Too many selected tracks.\n";
			continue;
		}
		if ( itTrack->charge()!=0 )
		{
			if( pass_TrackSelection(itTrack) )
			{
				_TrackPt[Ntrack] = itTrack->pt(); 
				_TrackEta[Ntrack] = itTrack->eta(); 
				_TrackPhi[Ntrack] = itTrack->phi(); 
				_TrackVx[Ntrack] = itTrack->vx(); 
				_TrackVy[Ntrack] = itTrack->vy(); 
				_TrackVz[Ntrack] = itTrack->vz(); 
				_TrackCharge[Ntrack] = itTrack->charge(); 
				_Trackd0[Ntrack]=itTrack->d0(); 
				_TrackNHit[Ntrack]=itTrack->found(); 
				_TrackNChi2[Ntrack]=itTrack->normalizedChi2(); 
				_TrackPtError[Ntrack]=itTrack->ptError();
				_TrackQuality[Ntrack]=itTrack->qualityMask();
				_TrackOuterZ[Ntrack]=itTrack->outerZ();
				_TrackOuterEta[Ntrack]=itTrack->outerEta();
				_TrackOuterPhi[Ntrack]=itTrack->outerPhi();

				if(itTrack->innerOk())
				{
					_TrackInnerX[Ntrack] = itTrack->innerPosition().X();
					_TrackInnerY[Ntrack] = itTrack->innerPosition().Y();
					_TrackInnerZ[Ntrack] = itTrack->innerPosition().Z();
				}
				Ntrack++;
			}//end pass_TrackSelection
		}//charge!=0
	}//TrackCollection
}

void ESAlignTool::fill_esRecHitAssociation()
{
	for(int itrk=0;itrk<Ntrack;itrk++)
	{
		for(int i=0;i<Nesrh;i++)
		{
			double r = deltaR(_TrackOuterEta[itrk],_TrackOuterPhi[itrk],_esRecHit_Eta[i],_esRecHit_Phi[i]);
			//if(r<0.5) _esRecHit_MatchedTrk_fromOuter[i]=itrk;
			if(r<0.5) _esRecHit_MatchedTrk_fromOuter[i][itrk]=1;
		}
	}
}

void ESAlignTool::set_ESorigin(int _evt_run, const CaloGeometry *caloGeom)
{
	int iz=-1; int ip=1; int ix=0; int iy=0; int is=0;
	Double_t X=0.; Double_t Y=0.; Double_t Z=0.;
	ESDetId esid;
	if( Cal_ESorigin_from_Geometry && _evt_run==1)
	{
		std::cout<<"\nSetting ES local origins... \n";
		ip=1; iz=-1; //ES-Front
		ix=2;iy=15;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=2;iy=15;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=2;iy=26;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=2;iy=26;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=39;iy=15;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=39;iy=15;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=39;iy=26;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=39;iy=26;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=16;iy=1;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=16;iy=1;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=16;iy=40;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=16;iy=40;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=25;iy=1;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=25;iy=1;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=25;iy=40;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=25;iy=40;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		X/=16.;Y/=16.;Z/=16.;
		delete ESmF_O;  delete ESmF_Oap;
		ESmF_O=new PositionType(X,Y,Z);
		ESmF_Oap=new PositionType(X+ES_dX[0][0],Y+ES_dY[0][0],Z+ES_dZ[0][0]);
		std::cout<<"ESmF_O("<<ESmF_O->x()<<","<<ESmF_O->y()<<","<<ESmF_O->z()<<")\n";

		X=0.0; Y=0.0; Z=0.0;
		ip=1; iz=1; //ES+Front
		ix=2;iy=15;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=2;iy=15;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=2;iy=26;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=2;iy=26;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=39;iy=15;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=39;iy=15;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=39;iy=26;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=39;iy=26;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=16;iy=1;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=16;iy=1;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=16;iy=40;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=16;iy=40;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=25;iy=1;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=25;iy=1;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=25;iy=40;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=25;iy=40;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		X/=16.;Y/=16.;Z/=16.;
		delete ESpF_O;  delete ESpF_Oap;
		ESpF_O=new PositionType(X,Y,Z);
		ESpF_Oap=new PositionType(X+ES_dX[1][0],Y+ES_dY[1][0],Z+ES_dZ[1][0]);
		std::cout<<"ESpF_O("<<ESpF_O->x()<<","<<ESpF_O->y()<<","<<ESpF_O->z()<<")\n";

		X=0.0; Y=0.0; Z=0.0;
		ip=2; iz=-1; //ES-Rear
		ix=1;iy=25;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=1;iy=25;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=1;iy=16;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=1;iy=16;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=40;iy=25;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=40;iy=25;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=40;iy=16;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=40;iy=16;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=15;iy=2;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=15;iy=2;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=26;iy=2;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=26;iy=2;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=15;iy=39;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=15;iy=39;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=26;iy=39;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=26;iy=39;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		X/=16.;Y/=16.;Z/=16.;
		delete ESmR_O;  delete ESmR_Oap;
		ESmR_O=new PositionType(X,Y,Z);
		ESmR_Oap=new PositionType(X+ES_dX[0][1],Y+ES_dY[0][1],Z+ES_dZ[0][1]);
		std::cout<<"ESmR_O("<<ESmR_O->x()<<","<<ESmR_O->y()<<","<<ESmR_O->z()<<")\n";

		X=0.0; Y=0.0; Z=0.0;
		ip=2; iz=1; //ES+Rear
		ix=1;iy=25;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=1;iy=25;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=1;iy=16;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=1;iy=16;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=40;iy=25;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=40;iy=25;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=40;iy=16;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=40;iy=16;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=15;iy=2;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=15;iy=2;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=26;iy=2;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=26;iy=2;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=15;iy=39;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=15;iy=39;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=26;iy=39;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		ix=26;iy=39;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X, Y, Z);
		X/=16.;Y/=16.;Z/=16.;
		delete ESpR_O;  delete ESpR_Oap;
		ESpR_O=new PositionType(X,Y,Z);
		ESpR_Oap=new PositionType(X+ES_dX[1][1],Y+ES_dY[1][1],Z+ES_dZ[1][1]);
		std::cout<<"ESpR_O("<<ESpR_O->x()<<","<<ESpR_O->y()<<","<<ESpR_O->z()<<")\n";
		std::cout<<"\n\n";

		UnAligned_Original( 1, 0, ESpF_O);  Aligned_Original( 1, 0, ESpF_Oap);
		UnAligned_Original( 1, 1, ESpR_O);  Aligned_Original( 1, 1, ESpR_Oap);
		UnAligned_Original( 0, 0, ESmF_O);  Aligned_Original( 0, 0, ESmF_Oap);
		UnAligned_Original( 0, 1, ESmR_O);  Aligned_Original( 0, 1, ESmR_Oap);
	}
}

void ESAlignTool::set_ESaxes(int _evt_run, const CaloGeometry *caloGeom)
{
	int iz=-1; int ip=1; int ix=0; int iy=0; int is=0;
	Double_t X=0.; Double_t Y=0.; Double_t Z=0.;
	Double_t X1=0.; Double_t Y1=0.; Double_t Z1=0.;
	Double_t X2=0.; Double_t Y2=0.; Double_t Z2=0.;
	ESDetId esid;
	if( Cal_ESaxes_from_Geometry && _evt_run==1)
	{
		std::cout<<"\nSetting ES local axes... \n";
		ip=1; iz=-1; //ES-Front
		X1=0.;  Y1=0.;  Z1=0.;  X2=0.;  Y2=0.;  Z2=0.;
		ix=2;iy=15;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=2;iy=15;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=2;iy=26;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=2;iy=26;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=39;iy=15;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=39;iy=15;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=39;iy=26;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=39;iy=26;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		X=(X2-X1)/8.;  Y=(Y2-Y1)/8.;  Z=(Z2-Z1)/8.;   normalize(X,Y,Z);
		ES_O_R11[0][0]=X;  ES_O_R12[0][0]=Y;  ES_O_R13[0][0]=Z;

		X1=0.;  Y1=0.;  Z1=0.;  X2=0.;  Y2=0.;  Z2=0.;
		ix=16;iy=1;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=16;iy=1;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=25;iy=1;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=25;iy=1;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=16;iy=40;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=16;iy=40;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=25;iy=40;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=25;iy=40;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		X=(X2-X1)/8.;  Y=(Y2-Y1)/8.;  Z=(Z2-Z1)/8.;   normalize(X,Y,Z);
		ES_O_R21[0][0]=X;  ES_O_R22[0][0]=Y;  ES_O_R23[0][0]=Z;

		X=ES_O_R12[0][0]*ES_O_R23[0][0]-ES_O_R13[0][0]*ES_O_R22[0][0];
		Y=ES_O_R13[0][0]*ES_O_R21[0][0]-ES_O_R11[0][0]*ES_O_R23[0][0];
		Z=ES_O_R11[0][0]*ES_O_R22[0][0]-ES_O_R12[0][0]*ES_O_R21[0][0];
		normalize(X,Y,Z);
		ES_O_R31[0][0]=X;  ES_O_R32[0][0]=Y;  ES_O_R33[0][0]=Z;

		std::cout<<"ESmF_O_R:\n";
		std::cout<<ES_O_R11[0][0]<<", "<<ES_O_R12[0][0]<<", "<<ES_O_R13[0][0]<<")\n";
		std::cout<<ES_O_R21[0][0]<<", "<<ES_O_R22[0][0]<<", "<<ES_O_R23[0][0]<<")\n";
		std::cout<<ES_O_R31[0][0]<<", "<<ES_O_R32[0][0]<<", "<<ES_O_R33[0][0]<<")\n";

		ES_O_Alpha[0][0]= asin(ES_O_R23[0][0]); 
		ES_O_Beta[0][0] = -1*ES_O_R13[0][0]/cos(ES_O_Alpha[0][0]);	
		ES_O_Gamma[0][0]= -1*ES_O_R21[0][0]/cos(ES_O_Alpha[0][0]);	
		std::cout<<"dAlpha: "<<ES_O_Alpha[0][0]<<", dBeta: "<<ES_O_Beta[0][0]<<", dGamma: "<<ES_O_Gamma[0][0]<<std::endl;

		ip=2; iz=-1; //ES-Rear
		X1=0.;  Y1=0.;  Z1=0.;  X2=0.;  Y2=0.;  Z2=0.;
		ix=1;iy=16;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=1;iy=16;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=1;iy=25;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=1;iy=25;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=40;iy=16;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=40;iy=16;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=40;iy=25;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=40;iy=25;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		X=(X2-X1)/8.;  Y=(Y2-Y1)/8.;  Z=(Z2-Z1)/8.;   normalize(X,Y,Z);
		ES_O_R11[0][1]=X;  ES_O_R12[0][1]=Y;  ES_O_R13[0][1]=Z;

		X1=0.;  Y1=0.;  Z1=0.;  X2=0.;  Y2=0.;  Z2=0.;
		ix=15;iy=2;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=15;iy=2;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=26;iy=2;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=26;iy=2;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=15;iy=39;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=15;iy=39;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=26;iy=39;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=26;iy=39;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		X=(X2-X1)/8.;  Y=(Y2-Y1)/8.;  Z=(Z2-Z1)/8.;   normalize(X,Y,Z);
		ES_O_R21[0][1]=X;  ES_O_R22[0][1]=Y;  ES_O_R23[0][1]=Z;

		X=ES_O_R12[0][1]*ES_O_R23[0][1]-ES_O_R13[0][1]*ES_O_R22[0][1];
		Y=ES_O_R13[0][1]*ES_O_R21[0][1]-ES_O_R11[0][1]*ES_O_R23[0][1];
		Z=ES_O_R11[0][1]*ES_O_R22[0][1]-ES_O_R12[0][1]*ES_O_R21[0][1];
		normalize(X,Y,Z);
		ES_O_R31[0][1]=X;  ES_O_R32[0][1]=Y;  ES_O_R33[0][1]=Z;

		std::cout<<"ESmR_O_R:\n";
		std::cout<<ES_O_R11[0][1]<<", "<<ES_O_R12[0][1]<<", "<<ES_O_R13[0][1]<<")\n";
		std::cout<<ES_O_R21[0][1]<<", "<<ES_O_R22[0][1]<<", "<<ES_O_R23[0][1]<<")\n";
		std::cout<<ES_O_R31[0][1]<<", "<<ES_O_R32[0][1]<<", "<<ES_O_R33[0][1]<<")\n";

		ES_O_Alpha[0][1]= asin(ES_O_R23[0][1]); 
		ES_O_Beta[0][1] = -1*ES_O_R13[0][1]/cos(ES_O_Alpha[0][1]);	
		ES_O_Gamma[0][1]= -1*ES_O_R21[0][1]/cos(ES_O_Alpha[0][1]);	
		std::cout<<"dAlpha: "<<ES_O_Alpha[0][1]<<", dBeta: "<<ES_O_Beta[0][1]<<", dGamma: "<<ES_O_Gamma[0][1]<<std::endl;

		ip=1; iz=1; //ES+Front
		X1=0.;  Y1=0.;  Z1=0.;  X2=0.;  Y2=0.;  Z2=0.;
		ix=2;iy=15;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=2;iy=15;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=2;iy=26;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=2;iy=26;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=39;iy=15;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=39;iy=15;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=39;iy=26;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=39;iy=26;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		X=(X2-X1)/8.;  Y=(Y2-Y1)/8.;  Z=(Z2-Z1)/8.;   normalize(X,Y,Z);
		ES_O_R11[1][0]=X;  ES_O_R12[1][0]=Y;  ES_O_R13[1][0]=Z;

		X1=0.;  Y1=0.;  Z1=0.;  X2=0.;  Y2=0.;  Z2=0.;
		ix=16;iy=1;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=16;iy=1;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=25;iy=1;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=25;iy=1;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=16;iy=40;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=16;iy=40;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=25;iy=40;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=25;iy=40;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		X=(X2-X1)/8.;  Y=(Y2-Y1)/8.;  Z=(Z2-Z1)/8.;   normalize(X,Y,Z);
		ES_O_R21[1][0]=X;  ES_O_R22[1][0]=Y;  ES_O_R23[1][0]=Z;

		X=ES_O_R12[1][0]*ES_O_R23[1][0]-ES_O_R13[1][0]*ES_O_R22[1][0];
		Y=ES_O_R13[1][0]*ES_O_R21[1][0]-ES_O_R11[1][0]*ES_O_R23[1][0];
		Z=ES_O_R11[1][0]*ES_O_R22[1][0]-ES_O_R12[1][0]*ES_O_R21[1][0];
		normalize(X,Y,Z);
		ES_O_R31[1][0]=X;  ES_O_R32[1][0]=Y;  ES_O_R33[1][0]=Z;

		std::cout<<"ESpF_O_R:\n";
		std::cout<<ES_O_R11[1][0]<<", "<<ES_O_R12[1][0]<<", "<<ES_O_R13[1][0]<<")\n";
		std::cout<<ES_O_R21[1][0]<<", "<<ES_O_R22[1][0]<<", "<<ES_O_R23[1][0]<<")\n";
		std::cout<<ES_O_R31[1][0]<<", "<<ES_O_R32[1][0]<<", "<<ES_O_R33[1][0]<<")\n";

		ES_O_Alpha[1][0]= asin(ES_O_R23[1][0]); 
		ES_O_Beta[1][0] = -1*ES_O_R13[1][0]/cos(ES_O_Alpha[1][0]);	
		ES_O_Gamma[1][0]= -1*ES_O_R21[1][0]/cos(ES_O_Alpha[1][0]);	
		std::cout<<"dAlpha: "<<ES_O_Alpha[1][0]<<", dBeta: "<<ES_O_Beta[1][0]<<", dGamma: "<<ES_O_Gamma[1][0]<<std::endl;

		ip=2; iz=1; //ES+Rear
		X1=0.;  Y1=0.;  Z1=0.;  X2=0.;  Y2=0.;  Z2=0.;
		ix=1;iy=16;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=1;iy=16;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=1;iy=25;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=1;iy=25;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=40;iy=16;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=40;iy=16;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=40;iy=25;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=40;iy=25;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		X=(X2-X1)/8.;  Y=(Y2-Y1)/8.;  Z=(Z2-Z1)/8.;   normalize(X,Y,Z);
		ES_O_R11[1][1]=X;  ES_O_R12[1][1]=Y;  ES_O_R13[1][1]=Z;

		X1=0.;  Y1=0.;  Z1=0.;  X2=0.;  Y2=0.;  Z2=0.;
		ix=15;iy=2;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=15;iy=2;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=26;iy=2;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=26;iy=2;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X1, Y1, Z1);
		ix=15;iy=39;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=15;iy=39;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=26;iy=39;is=1;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		ix=26;iy=39;is=32;  esid=ESDetId(is,ix,iy,ip,iz);
		Sum_ESPosition(caloGeom, esid, X2, Y2, Z2);
		X=(X2-X1)/8.;  Y=(Y2-Y1)/8.;  Z=(Z2-Z1)/8.;   normalize(X,Y,Z);
		ES_O_R21[1][1]=X;  ES_O_R22[1][1]=Y;  ES_O_R23[1][1]=Z;

		X=ES_O_R12[1][1]*ES_O_R23[1][1]-ES_O_R13[1][1]*ES_O_R22[1][1];
		Y=ES_O_R13[1][1]*ES_O_R21[1][1]-ES_O_R11[1][1]*ES_O_R23[1][1];
		Z=ES_O_R11[1][1]*ES_O_R22[1][1]-ES_O_R12[1][1]*ES_O_R21[1][1];
		normalize(X,Y,Z);
		ES_O_R31[1][1]=X;  ES_O_R32[1][1]=Y;  ES_O_R33[1][1]=Z;

		std::cout<<"ESpR_O_R:\n";
		std::cout<<ES_O_R11[1][1]<<", "<<ES_O_R12[1][1]<<", "<<ES_O_R13[1][1]<<")\n";
		std::cout<<ES_O_R21[1][1]<<", "<<ES_O_R22[1][1]<<", "<<ES_O_R23[1][1]<<")\n";
		std::cout<<ES_O_R31[1][1]<<", "<<ES_O_R32[1][1]<<", "<<ES_O_R33[1][1]<<")\n";

		ES_O_Alpha[1][1]= asin(ES_O_R23[1][1]); 
		ES_O_Beta[1][1] = -1*ES_O_R13[1][1]/cos(ES_O_Alpha[1][1]);	
		ES_O_Gamma[1][1]= -1*ES_O_R21[1][1]/cos(ES_O_Alpha[1][1]);	
		std::cout<<"dAlpha: "<<ES_O_Alpha[1][1]<<", dBeta: "<<ES_O_Beta[1][1]<<", dGamma: "<<ES_O_Gamma[1][1]<<std::endl;

		//Set the roation after alignment 	
		ES_Alpha[1][0]=ES_dAlpha[1][0]+ES_O_Alpha[1][0]; ES_Beta[1][0]=ES_dBeta[1][0]+ES_O_Beta[1][0], ES_Gamma[1][0]=ES_dGamma[1][0]+ES_O_Gamma[1][0];
		ES_Alpha[1][1]=ES_dAlpha[1][1]+ES_O_Alpha[1][1]; ES_Beta[1][1]=ES_dBeta[1][1]+ES_O_Beta[1][1], ES_Gamma[1][1]=ES_dGamma[1][1]+ES_O_Gamma[1][1];
		ES_Alpha[0][0]=ES_dAlpha[0][0]+ES_O_Alpha[0][0]; ES_Beta[0][0]=ES_dBeta[0][0]+ES_O_Beta[0][0], ES_Gamma[0][0]=ES_dGamma[0][0]+ES_O_Gamma[0][0];
		ES_Alpha[0][1]=ES_dAlpha[0][1]+ES_O_Alpha[0][1]; ES_Beta[0][1]=ES_dBeta[0][1]+ES_O_Beta[0][1], ES_Gamma[0][1]=ES_dGamma[0][1]+ES_O_Gamma[0][1];
		Aligned_RotationMatrices(1, 0, ES_Alpha[1][0], ES_Beta[1][0], ES_Gamma[1][0]);
		Aligned_RotationMatrices(1, 1, ES_Alpha[1][1], ES_Beta[1][1], ES_Gamma[1][1]);
		Aligned_RotationMatrices(0, 0, ES_Alpha[0][0], ES_Beta[0][0], ES_Gamma[0][0]);
		Aligned_RotationMatrices(0, 1, ES_Alpha[0][1], ES_Beta[0][1], ES_Gamma[0][1]);
	}
}

void ESAlignTool::Sum_ESPosition(const CaloGeometry *caloGeom, ESDetId esid, Double_t &X, Double_t &Y, Double_t &Z)
{
	X += ( caloGeom->getPosition(esid).x() );
	Y += ( caloGeom->getPosition(esid).y() );
	Z += ( caloGeom->getPosition(esid).z() );
}

void ESAlignTool::normalize(Double_t &X, Double_t &Y, Double_t &Z)
{
	Double_t buf=X*X+Y*Y+Z*Z;
	buf=TMath::Sqrt(buf);
	X/=buf; Y/=buf; Z/=buf;
}

void ESAlignTool::Overwrite_RotationMatrix(int _evt_run)
{
	if( _evt_run==1 )
	{
		for(int i=0;i<2;i++)
		{
			for(int j=0;j<2;j++)
			{
				ES_R11[i][j]=ES_O_R11[i][j]; ES_R12[i][j]=ES_O_R12[i][j]; ES_R13[i][j]=ES_O_R13[i][j];
				ES_R21[i][j]=ES_O_R21[i][j]; ES_R22[i][j]=ES_O_R22[i][j]; ES_R23[i][j]=ES_O_R23[i][j];
				ES_R31[i][j]=ES_O_R31[i][j]; ES_R32[i][j]=ES_O_R32[i][j]; ES_R33[i][j]=ES_O_R33[i][j];
			}}
	}
}

void ESAlignTool::fill_PredictionState(int iz, int ip, int idee, edm::Handle<reco::TrackCollection> TrackCol, edm::ESHandle<MagneticField> theMagField, edm::ESHandle<GlobalTrackingGeometry> theTrackingGeometry, edm::ESHandle<Propagator> shPropAlong )
{
	PositionType *ES_Oap;
	RotationType *ES_Rotation;

	Double_t R11,R12,R13,R21,R22,R23,R31,R32,R33;
	Double_t cxx,cyy,czz,cyx,czx,czy;

	if( (iz==1||iz==-1) && (ip==1||ip==2) )
	{
		int a,b;
		iz==-1 ? a=0 : a=1;
		b=ip-1;
		ES_Oap=new PositionType(ES_Oap_X[a][b],ES_Oap_Y[a][b],ES_Oap_Z[a][b]);
		ES_Rotation=new RotationType(ES_R11[a][b],ES_R12[a][b],ES_R13[a][b], ES_R21[a][b],ES_R22[a][b],ES_R23[a][b],ES_R31[a][b],ES_R32[a][b],ES_R33[a][b]);

		R11=ES_R11[a][b]; R12=ES_R12[a][b]; R13=ES_R13[a][b];
		R21=ES_R21[a][b]; R22=ES_R22[a][b]; R23=ES_R23[a][b];
		R31=ES_R31[a][b]; R32=ES_R32[a][b]; R33=ES_R33[a][b];

		if(b_PrintMatrix)
		{
			if(ESpF_Printed==0)
			{
				if(iz==1&&ip==1)
				{
					std::cout << "Printing Rotation Matrix :\n";
					std::cout << "ES+F :\n";
					std::cout << R11 << ", " << R12 << ", " << R13 << ",\n";
					std::cout << R21 << ", " << R22 << ", " << R23 << ",\n";
					std::cout << R31 << ", " << R32 << ", " << R33 << ",\n";
					ESpF_Printed=1;
				}
			}
			if(ESpR_Printed==0)
			{
				if(iz==1&&ip==2)
				{
					std::cout << "Printing Rotation Matrix :\n";
					std::cout << "ES+R :\n";
					std::cout << R11 << ", " << R12 << ", " << R13 << ",\n";
					std::cout << R21 << ", " << R22 << ", " << R23 << ",\n";
					std::cout << R31 << ", " << R32 << ", " << R33 << ",\n";
					ESpR_Printed=1;
				}
			}
			if(ESmF_Printed==0)
			{
				if(iz==-1&&ip==1)
				{
					std::cout << "Printing Rotation Matrix :\n";
					std::cout << "ES-F :\n";
					std::cout << R11 << ", " << R12 << ", " << R13 << ",\n";
					std::cout << R21 << ", " << R22 << ", " << R23 << ",\n";
					std::cout << R31 << ", " << R32 << ", " << R33 << ",\n";
					ESmF_Printed=1;
				}
			}
			if(ESmR_Printed==0)
			{
				if(iz==-1&&ip==2)
				{
					std::cout << "Printing Rotation Matrix :\n";
					std::cout << "ES-R :\n";
					std::cout << R11 << ", " << R12 << ", " << R13 << ",\n";
					std::cout << R21 << ", " << R22 << ", " << R23 << ",\n";
					std::cout << R31 << ", " << R32 << ", " << R33 << ",\n";
					ESmR_Printed=1;
				}
			}
		}

		Plane::PlanePointer Plane_ES = Plane::build(*ES_Oap,*ES_Rotation);

		int iTrk=0;
		for(reco::TrackCollection::const_iterator itTrack = TrackCol->begin();
				itTrack != TrackCol->end(); ++itTrack)
		{    
			if(iTrk>=2000)
			{
				edm::LogWarning("fill_tracks")<<"Too many selected tracks.\n";
				continue;
			}
			if ( itTrack->charge()!=0 )
			{
				if( pass_TrackSelection(itTrack) )
				{

					reco::TransientTrack TTrack(*itTrack,theMagField.product(),theTrackingGeometry);
					TrajectoryStateOnSurface ipTSOS = TTrack.outermostMeasurementState();

					if(ipTSOS.isValid())
					{
						TrajectoryStateOnSurface ES_prediction = shPropAlong->propagate(*(ipTSOS.freeState()),*Plane_ES);
						if( (ES_prediction.isValid())&&(ES_prediction.hasError()) )
						{
							PredictionState_iz[iTrk][a][b]=iz;
							PredictionState_ip[iTrk][a][b]=ip;
							PredictionState_valid[iTrk][a][b]=1;
							PredictionState_X[iTrk][a][b]=ES_prediction.globalPosition().x();
							PredictionState_Y[iTrk][a][b]=ES_prediction.globalPosition().y();
							PredictionState_Z[iTrk][a][b]=ES_prediction.globalPosition().z();
							PredictionState_Px[iTrk][a][b]=ES_prediction.globalMomentum().x();
							PredictionState_Py[iTrk][a][b]=ES_prediction.globalMomentum().y();
							PredictionState_Pz[iTrk][a][b]=ES_prediction.globalMomentum().z();
							PredictionState_Bx[iTrk][a][b]=theMagField->inTesla(ES_prediction.globalPosition()).x();
							PredictionState_By[iTrk][a][b]=theMagField->inTesla(ES_prediction.globalPosition()).y();
							PredictionState_Bz[iTrk][a][b]=theMagField->inTesla(ES_prediction.globalPosition()).z();

							cxx=ES_prediction.cartesianError().position().cxx();
							cyy=ES_prediction.cartesianError().position().cyy();
							czz=ES_prediction.cartesianError().position().czz();
							cyx=ES_prediction.cartesianError().position().cyx();
							czx=ES_prediction.cartesianError().position().czx();
							czy=ES_prediction.cartesianError().position().czy();

							PredictionState_Exx[iTrk][a][b]= R11*R11*cxx+R12*R12*cyy+R13*R13*czz+2.*R11*R12*cyx+2.*R11*R13*czx+2.*R12*R13*czy;
							PredictionState_Eyy[iTrk][a][b]= R21*R21*cxx+R22*R22*cyy+R23*R23*czz+2.*R21*R22*cyx+2.*R21*R23*czx+2.*R22*R23*czy;
							PredictionState_Eyx[iTrk][a][b]= R11*R21*cxx+R12*R22*cyy+R13*R23*czz+(R11*R22+R21*R12)*cyx+(R11*R23+R21*R13)*czx+(R12*R23+R22*R13)*czy;

							PredictionState_E44[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(3,3));
							PredictionState_E55[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(4,4));
							PredictionState_E66[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(5,5));
							PredictionState_E45[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(3,4));
							PredictionState_E46[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(3,5));
							PredictionState_E56[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(4,5));
							PredictionState_E14[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(0,3));
							PredictionState_E15[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(0,4));
							PredictionState_E16[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(0,5));
							PredictionState_E24[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(1,3));
							PredictionState_E25[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(1,4));
							PredictionState_E26[iTrk][a][b]=((ES_prediction.cartesianError().matrix())(1,5));
							PredictionState_delX[iTrk][a][b]=PredictionState_Px[iTrk][a][b]/PredictionState_Pz[iTrk][a][b];
							PredictionState_delY[iTrk][a][b]=PredictionState_Py[iTrk][a][b]/PredictionState_Pz[iTrk][a][b];
						}//end if ES_prediction Valid and hasError
					}//end if innerTSOS.isValid()
					iTrk++;
				}//pass trk selections
			}//trk charge !=0
		}//loop trks
	}//if one of 4 planes
}

void ESAlignTool::fill_residual(int iz)
{
	if(iz==1||iz==-1)
	{
		int a; iz==-1 ? a=0 : a=1;
		for(int iTrk=0;iTrk<Ntrack;iTrk++)// Predicted hits from global to local
		{
			if(PredictionState_valid[iTrk][a][0]!=1) continue; //ES+/- F
			if(PredictionState_valid[iTrk][a][1]!=1) continue; //ES+/- R
			Double_t eF_xx= (PredictionState_Exx[iTrk][a][0]);
			Double_t eF_yy= (PredictionState_Eyy[iTrk][a][0]);
			Double_t eF_yx= (PredictionState_Eyx[iTrk][a][0]);
			//e_yx=0.;
			if( eF_xx*eF_yy-eF_yx*eF_yx <= 0 ) continue;
			if( eF_xx > e_xxlimit ) continue;
			if( eF_yy > e_yylimit ) continue;
			if( fabs(eF_yx) > e_yxlimit ) continue;
			eF_xx+=pow(0.055029,2.); eF_yy+=pow(1.760918,2.);
			Double_t eR_yy= (PredictionState_Eyy[iTrk][a][1]);
			Double_t eR_xx= (PredictionState_Exx[iTrk][a][1]);
			Double_t eR_yx= (PredictionState_Eyx[iTrk][a][1]);
			//e_yx=0.;
			if( eR_xx*eR_yy-eR_yx*eR_yx <= 0 ) continue;
			if( eR_xx > e_xxlimit ) continue;
			if( eR_yy > e_yylimit ) continue;
			if( fabs(eR_yx) > e_yxlimit ) continue;
			eR_yy+=pow(0.055029,2.); eR_xx+=pow(1.760918,2.);
			//Prediction Point on Local coordinate
			Double_t X0F,Y0F;
			X0F
				= (PredictionState_X[iTrk][a][0]-ES_Oap_X[a][0])*ES_R11[a][0]
				+(PredictionState_Y[iTrk][a][0]-ES_Oap_Y[a][0])*ES_R12[a][0]
				+(PredictionState_Z[iTrk][a][0]-ES_Oap_Z[a][0])*ES_R13[a][0];
			Y0F
				= (PredictionState_X[iTrk][a][0]-ES_Oap_X[a][0])*ES_R21[a][0]
				+(PredictionState_Y[iTrk][a][0]-ES_Oap_Y[a][0])*ES_R22[a][0]
				+(PredictionState_Z[iTrk][a][0]-ES_Oap_Z[a][0])*ES_R23[a][0];
			if(check_DeadZone(iz,1,X0F,Y0F)==0) continue; 
			if(check_Radius(X0F,Y0F)==0) continue;
			Double_t X0R,Y0R;
			X0R
				= (PredictionState_X[iTrk][a][1]-ES_Oap_X[a][1])*ES_R11[a][1]
				+(PredictionState_Y[iTrk][a][1]-ES_Oap_Y[a][1])*ES_R12[a][1]
				+(PredictionState_Z[iTrk][a][1]-ES_Oap_Z[a][1])*ES_R13[a][1];
			Y0R
				= (PredictionState_X[iTrk][a][1]-ES_Oap_X[a][1])*ES_R21[a][1]
				+(PredictionState_Y[iTrk][a][1]-ES_Oap_Y[a][1])*ES_R22[a][1]
				+(PredictionState_Z[iTrk][a][1]-ES_Oap_Z[a][1])*ES_R23[a][1];
			if(check_DeadZone(iz,2,X0R,Y0R)==0) continue;
			if(check_Radius(X0R,Y0R)==0) continue;

			Double_t disF=(winlimit*winlimit); int indF=-1;
			Double_t resiXF,resiYF,resiXR,resiYR;
			resiXF=30.; resiYF=30.; resiXR=30.; resiYR=30.;
			for(int irec=0;irec<Nesrh;irec++)// recoHits from global to local, for first plane
			{
				if(_esRecHit_siZ[irec]!=iz||_esRecHit_siP[irec]!=1) continue;
				if(_esRecHit_X[irec]==0.&&_esRecHit_Y[irec]==0.) continue;
				if(_esRecHit_MatchedTrk_fromOuter[irec][iTrk]==0) continue;
				//if(_esRecHit_MatchedTrk_fromOuter[irec]!=iTrk) continue;
				Double_t X,Y;
				X= (_esRecHit_X[irec]-ES_O_X[a][0])*ES_O_R11[a][0]
					+(_esRecHit_Y[irec]-ES_O_Y[a][0])*ES_O_R12[a][0]
					+(_esRecHit_Z[irec]-ES_O_Z[a][0])*ES_O_R13[a][0];
				Y= (_esRecHit_X[irec]-ES_O_X[a][0])*ES_O_R21[a][0]
					+(_esRecHit_Y[irec]-ES_O_Y[a][0])*ES_O_R22[a][0]
					+(_esRecHit_Z[irec]-ES_O_Z[a][0])*ES_O_R23[a][0];
				if( fabs(X-X0F)>winlimit ) continue;
				if( fabs(Y-Y0F)>winlimit ) continue;
				Double_t buf = pow(X-X0F,2.)+pow(Y-Y0F,2.);
				if(buf<disF) //Find the cloest predicted hit one in local coordinate
				{ indF=irec; disF=buf; resiXF=X0F-X; resiYF=Y0F-Y; }
			}//end for-loop ESrechit

			Double_t disR=(winlimit*winlimit); int indR=-1;
			for(int irec=0;irec<Nesrh;irec++)// recoHits from global to local, for second plane
			{	
				if(_esRecHit_siZ[irec]!=iz||_esRecHit_siP[irec]!=2) continue;
				if(_esRecHit_X[irec]==0.&&_esRecHit_Y[irec]==0.) continue;
				if(_esRecHit_MatchedTrk_fromOuter[irec][iTrk]==0) continue;
				//if(_esRecHit_MatchedTrk_fromOuter[irec]!=iTrk) continue;
				Double_t X,Y;
				X= (_esRecHit_X[irec]-ES_O_X[a][1])*ES_O_R11[a][1]
					+(_esRecHit_Y[irec]-ES_O_Y[a][1])*ES_O_R12[a][1]
					+(_esRecHit_Z[irec]-ES_O_Z[a][1])*ES_O_R13[a][1];
				Y= (_esRecHit_X[irec]-ES_O_X[a][1])*ES_O_R21[a][1]
					+(_esRecHit_Y[irec]-ES_O_Y[a][1])*ES_O_R22[a][1]
					+(_esRecHit_Z[irec]-ES_O_Z[a][1])*ES_O_R23[a][1];
				if( fabs(X-X0R)>winlimit) continue;
				if( fabs(Y-Y0R)>winlimit) continue;
				Double_t buf = pow(X-X0R,2.)+pow(Y-Y0R,2.);
				if(buf<disR)//Find the cloest predicted hit one in local coordinate
				{ indR=irec; disR=buf; resiXR=X0R-X; resiYR=Y0R-Y; }
			}//end for-loop ESrechita

			if(indF>-1&&_esRecHit_Noisy[indF]==0&&indR>-1&&_esRecHit_Noisy[indR]==0
					&& BadSensor(_esRecHit_siZ[indF],_esRecHit_siP[indF],_esRecHit_siX[indF],_esRecHit_siY[indF])==0
					&& BadSensor(_esRecHit_siZ[indR],_esRecHit_siP[indR],_esRecHit_siX[indR],_esRecHit_siY[indR])==0
			  )
			{
				PredictionState_MatchedRec[iTrk][a][0]=indF;
				PredictionState_resiX[iTrk][a][0]=resiXF;
				PredictionState_resiY[iTrk][a][0]=resiYF;
				if(a==1)
				{
					ESpF_residualX->Fill(PredictionState_resiX[iTrk][a][0]);
					ESpF_residualY->Fill(PredictionState_resiY[iTrk][a][0]);
				}
				if(a==0)
				{
					ESmF_residualX->Fill(PredictionState_resiX[iTrk][a][0]);
					ESmF_residualY->Fill(PredictionState_resiY[iTrk][a][0]);
				}
				Double_t XDF=PredictionState_resiX[iTrk][a][0];
				Double_t YDF=PredictionState_resiY[iTrk][a][0];

				if(  Selected_idee==0
						|| (Selected_idee==1&&_esRecHit_X[indF]-ES_O_X[a][0]>6.1 )
						|| (Selected_idee==2&&_esRecHit_X[indF]-ES_O_X[a][0]<-6.1 )
				  )
				{
					Cal_MatrixM_doRotation(iTrk,iz,1,eF_xx,eF_yx,eF_yy);
					Cal_VectorP_doRotation(iTrk,iz,1,eF_xx,eF_yx,eF_yy);
					Cal_CHI2(iz,1,eF_xx,eF_yx,eF_yy,XDF,YDF);
					ES_NTracks[a][0] += 1;
				}

				PredictionState_MatchedRec[iTrk][a][1]=indR;
				PredictionState_resiX[iTrk][a][1]=resiXR;
				PredictionState_resiY[iTrk][a][1]=resiYR;

				if(a==1)
				{
					ESpR_residualX->Fill(PredictionState_resiX[iTrk][a][1]);
					ESpR_residualY->Fill(PredictionState_resiY[iTrk][a][1]);
				}
				if(a==0)
				{
					ESmR_residualX->Fill(PredictionState_resiX[iTrk][a][1]);
					ESmR_residualY->Fill(PredictionState_resiY[iTrk][a][1]);
				}
				Double_t XDR=PredictionState_resiX[iTrk][a][1];
				Double_t YDR=PredictionState_resiY[iTrk][a][1];

				if(  Selected_idee==0
						|| (Selected_idee==1&&_esRecHit_Y[indR]-ES_O_Y[a][0]>6.1 )
						|| (Selected_idee==2&&_esRecHit_Y[indR]-ES_O_Y[a][0]<-6.1 )
				  )
				{
					Cal_MatrixM_doRotation(iTrk,iz,2,eR_xx,eR_yx,eR_yy);
					Cal_VectorP_doRotation(iTrk,iz,2,eR_xx,eR_yx,eR_yy);
					Cal_CHI2(iz,2,eR_xx,eR_yx,eR_yy,XDR,YDR);
					ES_NTracks[a][1] += 1;
				}

			}//end if good matching
		}//end for-loop Trk

	}//iz==1 or -1
}

int ESAlignTool::check_Radius(Double_t X, Double_t Y)
{
	int res=0;
	if(  (X*X+Y*Y)>60.*60. && (X*X+Y*Y)<110.*110.) res=1;
	return res;
}


	void 
ESAlignTool::beginJob()
{
	std::cout << "In ESAlignTool.beginJob\n";

	t_ESAlign = f->make<TTree>("tree","");

	if( b_storeDetail )
	{
		t_ESAlign->Branch("runNumber", &_runNum, "runNumber/L");
		t_ESAlign->Branch("evtNumber", &_evtNum, "evtNumber/L");

		t_ESAlign->Branch("Nesrh",    &Nesrh,       "Nesrh/I"); 
		t_ESAlign->Branch("_esRecHit_E",   &_esRecHit_E[0],   "_esRecHit_E[Nesrh]/D"); 
		t_ESAlign->Branch("_esRecHit_X",  &_esRecHit_X[0],  "_esRecHit_X[Nesrh]/D");
		t_ESAlign->Branch("_esRecHit_Y", &_esRecHit_Y[0], "_esRecHit_Y[Nesrh]/D");
		t_ESAlign->Branch("_esRecHit_Z", &_esRecHit_Z[0], "_esRecHit_Z[Nesrh]/D");
		t_ESAlign->Branch("_esRecHit_Eta",  &_esRecHit_Eta[0],  "_esRecHit_Eta[Nesrh]/D");
		t_ESAlign->Branch("_esRecHit_Phi",  &_esRecHit_Phi[0],  "_esRecHit_Phi[Nesrh]/D");
		t_ESAlign->Branch("_esRecHit_siZ", &_esRecHit_siZ[0], "_esRecHit_siZ[Nesrh]/I");
		t_ESAlign->Branch("_esRecHit_siP", &_esRecHit_siP[0], "_esRecHit_siP[Nesrh]/I");
		t_ESAlign->Branch("_esRecHit_siX", &_esRecHit_siX[0], "_esRecHit_siX[Nesrh]/IL");
		t_ESAlign->Branch("_esRecHit_siY", &_esRecHit_siY[0], "_esRecHit_siY[Nesrh]/IL");
		t_ESAlign->Branch("_esRecHit_Strip", &_esRecHit_Strip[0], "_esRecHit_Strip[Nesrh]/I");
		t_ESAlign->Branch("_esRecHit_Noisy",_esRecHit_Noisy,"_esRecHit_Noisy[Nesrh]/S");
		t_ESAlign->Branch("_esRecHit_MatchedTrk_fromOuter",_esRecHit_MatchedTrk_fromOuter,"_esRecHit_MatchedTrk_fromOuter[Nesrh][Ntrack]/I");
		t_ESAlign->Branch("Ntrack"          , &Ntrack          , "Ntrack/I");
		t_ESAlign->Branch("TrackPt"         , &_TrackPt[0]      , "TrackPt[Ntrack]/D");
		t_ESAlign->Branch("TrackEta"        , &_TrackEta[0]     , "TrackEta[Ntrack]/D");
		t_ESAlign->Branch("TrackPhi"        , &_TrackPhi[0]     , "TrackPhi[Ntrack]/D");
		t_ESAlign->Branch("TrackVx"         , &_TrackVx[0]      , "TrackVx[Ntrack]/D");
		t_ESAlign->Branch("TrackVy"         , &_TrackVy[0]      , "TrackVy[Ntrack]/D");
		t_ESAlign->Branch("TrackVz"         , &_TrackVz[0]      , "TrackVz[Ntrack]/D");
		t_ESAlign->Branch("Trackd0"     , &_Trackd0[0]  , "Trackd0[Ntrack]/D");
		t_ESAlign->Branch("TrackInnerX"     , &_TrackInnerX[0]  , "TrackInnerX[Ntrack]/D");
		t_ESAlign->Branch("TrackInnerY"     , &_TrackInnerY[0]  , "TrackInnerY[Ntrack]/D");
		t_ESAlign->Branch("TrackInnerZ"     , &_TrackInnerZ[0]  , "TrackInnerZ[Ntrack]/D");
		t_ESAlign->Branch("TrackOuterZ"     , &_TrackOuterZ[0]  , "TrackOuterZ[Ntrack]/D");
		t_ESAlign->Branch("TrackOuterEta"     , &_TrackOuterEta[0]  , "TrackOuterEta[Ntrack]/D");
		t_ESAlign->Branch("TrackOuterPhi"     , &_TrackOuterPhi[0]  , "TrackOuterPhi[Ntrack]/D");
		t_ESAlign->Branch("TrackCharge"     , &_TrackCharge[0]  , "TrackCharge[Ntrack]/S");
		t_ESAlign->Branch("TrackNHit"     , &_TrackNHit[0]  , "TrackNHit[Ntrack]/I");
		t_ESAlign->Branch("TrackNChi2"     , &_TrackNChi2[0]  , "TrackNChi2[Ntrack]/D");
		t_ESAlign->Branch("TrackPtError",_TrackPtError,"TrackPtError[Ntrack]/D");
		t_ESAlign->Branch("TrackQuality",_TrackQuality,"TrackQuality[Ntrack]/I");

		t_ESAlign->Branch("PredictionState_iz",PredictionState_iz,"PredictionState_iz[Ntrack][2][2]/I");
		t_ESAlign->Branch("PredictionState_ip",PredictionState_ip,"PredictionState_ip[Ntrack][2][2]/I");
		t_ESAlign->Branch("PredictionState_valid",PredictionState_valid,"PredictionState_valid[Ntrack][2][2]/S");
		t_ESAlign->Branch("PredictionState_X",PredictionState_X,"PredictionState_X[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_Y",PredictionState_Y,"PredictionState_Y[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_Z",PredictionState_Z,"PredictionState_Z[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_Px",PredictionState_Px,"PredictionState_Px[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_Py",PredictionState_Py,"PredictionState_Py[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_Pz",PredictionState_Pz,"PredictionState_Pz[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_Bx",PredictionState_Bx,"PredictionState_Bx[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_By",PredictionState_By,"PredictionState_By[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_Bz",PredictionState_Bz,"PredictionState_Bz[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_Exx",PredictionState_Exx,"PredictionState_Exx[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_Eyx",PredictionState_Eyx,"PredictionState_Eyx[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_Eyy",PredictionState_Eyy,"PredictionState_Eyy[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_delX",PredictionState_delX,"PredictionState_delX[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_delY",PredictionState_delY,"PredictionState_delY[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_MatchedRec",PredictionState_MatchedRec,"PredictionState_MatchedRec[Ntrack][2][2]/I");
		t_ESAlign->Branch("PredictionState_resiX",PredictionState_resiX,"PredictionState_resiX[Ntrack][2][2]/D");
		t_ESAlign->Branch("PredictionState_resiY",PredictionState_resiY,"PredictionState_resiY[Ntrack][2][2]/D");
	}

	t_ESAlign->Branch("ES_CHI2",ES_CHI2,"ES_CHI2[2][2]/D");
	t_ESAlign->Branch("ES_NTracks",ES_NTracks,"ES_NTracks[2][2]/L");
	t_ESAlign->Branch("ES_M11",ES_M11,"ES_M11[2][2]/D");
	t_ESAlign->Branch("ES_M12",ES_M12,"ES_M12[2][2]/D");
	t_ESAlign->Branch("ES_M13",ES_M13,"ES_M13[2][2]/D");
	t_ESAlign->Branch("ES_M14",ES_M14,"ES_M14[2][2]/D");
	t_ESAlign->Branch("ES_M15",ES_M15,"ES_M15[2][2]/D");
	t_ESAlign->Branch("ES_M16",ES_M16,"ES_M16[2][2]/D");
	t_ESAlign->Branch("ES_M22",ES_M22,"ES_M22[2][2]/D");
	t_ESAlign->Branch("ES_M23",ES_M23,"ES_M23[2][2]/D");
	t_ESAlign->Branch("ES_M24",ES_M24,"ES_M24[2][2]/D");
	t_ESAlign->Branch("ES_M25",ES_M25,"ES_M25[2][2]/D");
	t_ESAlign->Branch("ES_M26",ES_M26,"ES_M26[2][2]/D");
	t_ESAlign->Branch("ES_M33",ES_M33,"ES_M33[2][2]/D");
	t_ESAlign->Branch("ES_M34",ES_M34,"ES_M34[2][2]/D");
	t_ESAlign->Branch("ES_M35",ES_M35,"ES_M35[2][2]/D");
	t_ESAlign->Branch("ES_M36",ES_M36,"ES_M36[2][2]/D");
	t_ESAlign->Branch("ES_M44",ES_M44,"ES_M44[2][2]/D");
	t_ESAlign->Branch("ES_M45",ES_M45,"ES_M45[2][2]/D");
	t_ESAlign->Branch("ES_M46",ES_M46,"ES_M46[2][2]/D");
	t_ESAlign->Branch("ES_M55",ES_M55,"ES_M55[2][2]/D");
	t_ESAlign->Branch("ES_M56",ES_M56,"ES_M56[2][2]/D");
	t_ESAlign->Branch("ES_M66",ES_M66,"ES_M66[2][2]/D");
	t_ESAlign->Branch("ES_P1",ES_P1,"ES_P1[2][2]/D");
	t_ESAlign->Branch("ES_P2",ES_P2,"ES_P2[2][2]/D");
	t_ESAlign->Branch("ES_P3",ES_P3,"ES_P3[2][2]/D");
	t_ESAlign->Branch("ES_P4",ES_P4,"ES_P4[2][2]/D");
	t_ESAlign->Branch("ES_P5",ES_P5,"ES_P5[2][2]/D");
	t_ESAlign->Branch("ES_P6",ES_P6,"ES_P6[2][2]/D");
	t_ESAlign->Branch("ES_M31Err2",ES_M31Err2,"ES_M31Err2[2][2]/D");
	t_ESAlign->Branch("ES_M32Err2",ES_M32Err2,"ES_M32Err2[2][2]/D");
	t_ESAlign->Branch("ES_M33Err2",ES_M33Err2,"ES_M33Err2[2][2]/D");
	t_ESAlign->Branch("ES_P1Err2",ES_P1Err2,"ES_P1Err2[2][2]/D");
	t_ESAlign->Branch("ES_P2Err2",ES_P2Err2,"ES_P2Err2[2][2]/D");
	t_ESAlign->Branch("ES_P3Err2",ES_P3Err2,"ES_P3Err2[2][2]/D");

	if(b_DrawMagField)
	{
		t_ESField = new TTree("ESField","tree",0);
		t_ESField->Branch("ESpF_B_x",ESpF_B_x,"ESpF_B_x[40][40][32]/D");
		t_ESField->Branch("ESpF_B_y",ESpF_B_y,"ESpF_B_y[40][40][32]/D");
		t_ESField->Branch("ESpF_B_z",ESpF_B_z,"ESpF_B_z[40][40][32]/D");
		t_ESField->Branch("ESpR_B_x",ESpR_B_x,"ESpR_B_x[40][40][32]/D");
		t_ESField->Branch("ESpR_B_y",ESpR_B_y,"ESpR_B_y[40][40][32]/D");
		t_ESField->Branch("ESpR_B_z",ESpR_B_z,"ESpR_B_z[40][40][32]/D");
		t_ESField->Branch("ESmF_B_x",ESmF_B_x,"ESmF_B_x[40][40][32]/D");
		t_ESField->Branch("ESmF_B_y",ESmF_B_y,"ESmF_B_y[40][40][32]/D");
		t_ESField->Branch("ESmF_B_z",ESmF_B_z,"ESmF_B_z[40][40][32]/D");
		t_ESField->Branch("ESmR_B_x",ESmR_B_x,"ESmR_B_x[40][40][32]/D");
		t_ESField->Branch("ESmR_B_y",ESmR_B_y,"ESmR_B_y[40][40][32]/D");
		t_ESField->Branch("ESmR_B_z",ESmR_B_z,"ESmR_B_z[40][40][32]/D");

	}

}

// ------------ method called once each job just after ending the event loop  ------------
void 
ESAlignTool::endJob() {
	std::cout<<std::endl << "In ESAlignTool.endJob\n";

	if(b_DrawMagField)  t_ESField->Write();

	ESpF_residualX->Write();  ESpF_residualY->Write();
	ESpR_residualX->Write();  ESpR_residualY->Write();
	ESmF_residualX->Write();  ESmF_residualY->Write();
	ESmR_residualX->Write();  ESmR_residualY->Write();


	std::cout << std::endl;
	std::cout << " --------------------------------------------- " << std::endl;
	std::cout << " number of events processed  " << _evt_run << std::endl; 
	std::cout << " Last Event number # " << _evtNum << std::endl; 
	std::cout << " --------------------------------------------- " << std::endl;


	delete woRotate;
	delete ESpF_O;  delete ESpR_O;  delete ESmF_O;  delete ESmR_O;
	delete ESpF_Oap;  delete ESpR_Oap;  delete ESmF_Oap;  delete ESmR_Oap;

	delete ESpF_residualX;  delete ESpF_residualY;
	delete ESpR_residualX;  delete ESpR_residualY;
	delete ESmF_residualX;  delete ESmF_residualY;
	delete ESmR_residualX;  delete ESmR_residualY;
}

//define this as a plug-in
DEFINE_FWK_MODULE(ESAlignTool);
