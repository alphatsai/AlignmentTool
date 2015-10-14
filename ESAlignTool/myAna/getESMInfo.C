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
	f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_7_4_12_patch4/src/AlignmentTool/ESAlignTool/test/OnLxplus/14Oct_BoffRunC_Iter1_nofulldata/SingleElectron_Run2015C-PromptReco-v1.root");
	t=(TTree*)f->Get("ESAlignmentTool/tree");
  	CalIter_wRotation caculate;
	caculate.registerESMatrix(t);
	caculate.iterN=iterN;
	caculate.run("BoffRunC_Iter1");
}
