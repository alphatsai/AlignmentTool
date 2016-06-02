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
#include "PlotFunc.C"
using namespace std;
void DrawResidual()
{
    TCanvas* c1 = new TCanvas("c1","", 850, 700 ); 

    TFile* fResidual0   = new TFile("results/24May_Run2016B/HLTPhysics_iter0.root");
    TFile* fResidual1   = new TFile("results/24May_Run2016B/HLTPhysics_iter1.root");
    TFile* fResidual2   = new TFile("results/24May_Run2016B/HLTPhysics_iter2.root");
    TFile* fResidual3   = new TFile("results/24May_Run2016B/HLTPhysics_iter3.root");
    TFile* fResidual4   = new TFile("results/24May_Run2016B/HLTPhysics_iter4.root");
    TFile* fResidual5   = new TFile("results/24May_Run2016B/HLTPhysics_iter5.root");
    TFile* fResidual6   = new TFile("results/24May_Run2016B/HLTPhysics_iter6.root");
    TFile* fResidual7   = new TFile("results/24May_Run2016B/HLTPhysics_iter7.root");
    TFile* fResidual8   = new TFile("results/24May_Run2016B/HLTPhysics_iter8.root");
    TFile* fResidual9   = new TFile("results/24May_Run2016B/HLTPhysics_iter9.root");
    TFile* fResidual10  = new TFile("results/24May_Run2016B/HLTPhysics_iter10.root");
    TFile* fResidualVal = new TFile("results/24May_Run2016B/HLTPhysics_ValDB.root");

    drawResidual(fResidual0,   c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter0.pdf",  1);
    drawResidual(fResidual1,   c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter1.pdf",  1);
    drawResidual(fResidual2,   c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter2.pdf",  1);
    drawResidual(fResidual3,   c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter3.pdf",  1);
    drawResidual(fResidual4,   c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter4.pdf",  1);
    drawResidual(fResidual5,   c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter5.pdf",  1);
    drawResidual(fResidual6,   c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter6.pdf",  1);
    drawResidual(fResidual7,   c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter7.pdf",  1);
    drawResidual(fResidual8,   c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter8.pdf",  1);
    drawResidual(fResidual9,   c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter9.pdf",  1);
    drawResidual(fResidual10,  c1, "results/24May_Run2016B/pdf/AlignmentResidual_iter10.pdf", 1);
    drawResidual(fResidualVal, c1, "results/24May_Run2016B/pdf/AlignmentResidual_valDB.pdf",  1);

    //TFile* FourCor_i11_ESpF = new TFile("/Users/Alpha/MyAna/ESAlignMent/result/2011A_RawRECO_SigEle_Nov/4Corner/iter11/ESpF_4Corner_iter11.root");	
    //TFile* FourCor_i11_ESpR = new TFile("/Users/Alpha/MyAna/ESAlignMent/result/2011A_RawRECO_SigEle_Nov/4Corner/iter11/ESpR_4Corner_iter11.root");	
    //TFile* FourCor_i11_ESmF = new TFile("/Users/Alpha/MyAna/ESAlignMent/result/2011A_RawRECO_SigEle_Nov/4Corner/iter11/ESmF_4Corner_iter11.root");	
    //TFile* FourCor_i11_ESmR = new TFile("/Users/Alpha/MyAna/ESAlignMent/result/2011A_RawRECO_SigEle_Nov/4Corner/iter11/ESmR_4Corner_iter11.root");	
    //draw4CorResidual(FourCor_i11_ESpF,"Align",1);
    //draw4CorResidual(FourCor_i11_ESpR,"Align",2);
    //draw4CorResidual(FourCor_i11_ESmF,"Align",3);
    //draw4CorResidual(FourCor_i11_ESmR,"Align",4);
}

