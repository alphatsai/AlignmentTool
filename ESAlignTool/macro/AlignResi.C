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
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "CMSstyle.C"
#include "DrawFunResi.C"
using namespace std;
void AlignResi(){
	TCanvas* c1 = new TCanvas("c1","", 850, 700 ); 
	TFile* fResidual = new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_7_0_7_dev/src/AlignmentTool/ESAlignTool/test/OnLxplus/02Oct_PoinGun1M_GTv6/job_SinglePionGun_GEN_SIM_v1__tdoan-job_SinglePionGun_RECO_v1-a2645d794d4263c6295ef839881df757__USER.root");	
	//TFile* fResidual = new TFile("/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_7_0_7_dev/src/AlignmentTool/ESAlignTool/test/OnLxplus/02Oct_PoinGun1M_GTv6/job_SinglePionGun_GEN_SIM_v1__tdoan-job_SinglePionGun_RECO_v1-a2645d794d4263c6295ef839881df757__USER.root");	
	//TFile* FourCor_i11_ESpF = new TFile("/Users/Alpha/MyAna/ESAlignMent/result/2011A_RawRECO_SigEle_Nov/4Corner/iter11/ESpF_4Corner_iter11.root");	
	//TFile* FourCor_i11_ESpR = new TFile("/Users/Alpha/MyAna/ESAlignMent/result/2011A_RawRECO_SigEle_Nov/4Corner/iter11/ESpR_4Corner_iter11.root");	
	//TFile* FourCor_i11_ESmF = new TFile("/Users/Alpha/MyAna/ESAlignMent/result/2011A_RawRECO_SigEle_Nov/4Corner/iter11/ESmF_4Corner_iter11.root");	
	//TFile* FourCor_i11_ESmR = new TFile("/Users/Alpha/MyAna/ESAlignMent/result/2011A_RawRECO_SigEle_Nov/4Corner/iter11/ESmR_4Corner_iter11.root");	

	drawResidual(fResidual, c1, "png/MisAlignmentResidual_GTv6.png");
	//drawResidual(merge_i11,"png/AlignmentResidual.png");
	//draw4CorResidual(FourCor_i11_ESpF,"Align",1);
	//draw4CorResidual(FourCor_i11_ESpR,"Align",2);
	//draw4CorResidual(FourCor_i11_ESmF,"Align",3);
	//draw4CorResidual(FourCor_i11_ESmR,"Align",4);
}
	
