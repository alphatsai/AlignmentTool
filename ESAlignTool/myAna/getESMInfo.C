#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "CalIter_wRotation.h"
using namespace std;
const int iterN=1;
void getESMInfo(){
	TFile* f;	
	TTree* t;
	f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_7_0_7_dev/src/AlignmentTool/ESAlignTool/test/OnLxplus/09Oct_PionGun100K_GTv6_Iter1/job_SinglePionGun_GEN_SIM_v1__tdoan-job_SinglePionGun_RECO_v1-a2645d794d4263c6295ef839881df757__USER.root");
	t=(TTree*)f->Get("ESAlignmentTool/tree");
  	CalIter_wRotation caculate;
	caculate.registerESMatrix(t);
	caculate.iterN=iterN;
	caculate.run();
}
