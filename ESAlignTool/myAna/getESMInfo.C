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
	f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_8_0_7_patch3/src/AlignmentTool/ESAlignTool/test/OnLxplus/24May_Run2016B_iter1/HLTPhysics.root");
	//f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_8_0_7_patch3/src/AlignmentTool/ESAlignTool/test/OnLxplus/24May_Run2016B_iter2/HLTPhysics.root");
	//f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_8_0_7_patch3/src/AlignmentTool/ESAlignTool/test/OnLxplus/24May_Run2016B_iter3/HLTPhysics.root");
	//f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_8_0_7_patch3/src/AlignmentTool/ESAlignTool/test/OnLxplus/24May_Run2016B_iter4/HLTPhysics.root");
	//f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_8_0_7_patch3/src/AlignmentTool/ESAlignTool/test/OnLxplus/24May_Run2016B_iter5/HLTPhysics.root");
	//f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_8_0_7_patch3/src/AlignmentTool/ESAlignTool/test/OnLxplus/24May_Run2016B_iter6/HLTPhysics.root");
	//f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_8_0_7_patch3/src/AlignmentTool/ESAlignTool/test/OnLxplus/24May_Run2016B_iter7/HLTPhysics.root");
	//f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_8_0_7_patch3/src/AlignmentTool/ESAlignTool/test/OnLxplus/24May_Run2016B_iter8/HLTPhysics.root");
	//f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_8_0_7_patch3/src/AlignmentTool/ESAlignTool/test/OnLxplus/24May_Run2016B_iter9/HLTPhysics.root");
	//f=new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_8_0_7_patch3/src/AlignmentTool/ESAlignTool/test/OnLxplus/24May_Run2016B_iter10/HLTPhysics.root");
	t=(TTree*)f->Get("ESAlignmentTool/tree");
  	CalIter_wRotation caculate;
	caculate.registerESMatrix(t);
	caculate.iterN=iterN;
	caculate.run("BonRun2016B_Iter1");
}
