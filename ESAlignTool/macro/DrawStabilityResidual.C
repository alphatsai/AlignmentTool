#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include "TF1.h"
#include "TFile.h"
#include "TLine.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TGraph.h"
#include "CMSstyle.C"
#include "PlotFunc.C"
using namespace std;
string int2str( int i )
{
    std::string s;
    stringstream ss(s);
    ss << i;
    return ss.str();	
}
void setTGraphStyle( TGraph* gr, string name, int iters){
    gr->GetXaxis()->SetRangeUser( 1, iters);
    gr->GetYaxis()->SetRangeUser(-0.3, 0.3);
    gr->GetYaxis()->SetTitle("#Delta Residual");
    gr->GetXaxis()->SetTitle((name+", iteration").c_str());
    gr->SetMarkerSize(1.5);
    gr->SetMarkerColor(4);
}
void DrawStabilityResidual()
{
    CMSstyle();
    const int iters=7;
    const int iters2=iters-1;
    double iter2[iters2];
    for( int i=0; i<iters2; i++){
        iter2[i]=i+1;
    }
    double mean[4][iters]={	
        {-1.97733e-01,-7.54692e-03,-4.43703e-04,-3.07239e-04,-1.03924e-04, -2.68090e-04, -4.51351e-04}, //+Front
        { 5.18299e-02, 7.52129e-05,-1.24620e-03,-1.20525e-03,-1.04070e-03, -1.03025e-03, -8.31440e-04}, //+Rear
        {-2.31406e-01,-7.58292e-03, 6.74692e-04, 1.00713e-03, 1.63951e-03,  1.56777e-03,  1.32733e-03}, //-Front
        { 2.78056e-01, 9.32818e-03, 1.46624e-03, 3.93765e-04, 1.73617e-05, -3.20296e-04, -5.99112e-05 }};//-Rear
    double delta[4][iters2];
    for( int i=0; i<4; i++){
        for( int j=1; j<iters; j++){
            int p = j-1;
            delta[i][p] = mean[i][j]-mean[i][p];
        }
    }
    TGraph* g_pF = new  TGraph( iters-1, iter2, delta[0]); g_pF->UseCurrentStyle(); setTGraphStyle(g_pF, "+Front", iters2); 
    TGraph* g_pR = new  TGraph( iters-1, iter2, delta[1]); g_pR->UseCurrentStyle(); setTGraphStyle(g_pR, "+Rear", iters2);
    TGraph* g_mF = new  TGraph( iters-1, iter2, delta[2]); g_mF->UseCurrentStyle(); setTGraphStyle(g_mF, "-Front", iters2);
    TGraph* g_mR = new  TGraph( iters-1, iter2, delta[3]); g_mR->UseCurrentStyle(); setTGraphStyle(g_mR, "-Rear", iters2);

    TLine* line = new TLine(1,0,iters2,0);
    line->SetLineColor(2);
    line->SetLineWidth(1);

    TCanvas* c1 = new TCanvas("c1","", 850, 700 ); 
    c1->Clear();
    c1->Divide(2,2);
    c1->cd(1);
    g_pF->Draw("ap*l");
    line->Draw("same");
    c1->cd(2);
    g_pR->Draw("ap*l");
    line->Draw("same");
    c1->cd(3);
    g_mF->Draw("ap*l");
    line->Draw("same");
    c1->cd(4);
    g_mR->Draw("ap*l");
    line->Draw("same");
    c1->SaveAs("plot/stability.pdf");

}
