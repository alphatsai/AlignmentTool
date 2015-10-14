using namespace std;
void fitResidual(TH1D* h1, string xName, Color_t color=2){
    TF1* gaus1_ = new TF1("gaus1_","gaus", -3., 3.);
    gaus1_->SetLineColor(color); 
    h1->Fit("gaus1_","WR"); cout<<endl;
    double Mean = gaus1_->GetParameter(1);
    double Width = gaus1_->GetParameter(2);
    gaus1_->SetRange(Mean-1.5*Width,Mean+1.5*Width);
    h1->Fit("gaus1_", "WR");
    h1->SetXTitle(xName.c_str());
    h1->SetYTitle("Yields");
}
void setStyle( TH1D* h1){
    h1->SetMarkerColor(1);
    h1->SetMarkerStyle(20);
    h1->SetMarkerSize(0.6);
}

void drawResidual( TFile* f1, TCanvas* c1, string savePath, int xrange=3, Color_t color=2 ){
    CMSstyle();
    gStyle->SetOptFit(101);
    gStyle->SetFitFormat("3.3g");
    //gStyle->SetStatX(0.4);      // Set x-position (fraction of pad size)
    //gStyle->SetStatY(0.9);      // Set y-position (fraction of pad size)
    //gStyle->SetStatW(0.18);      // Set width of stat-box (fraction of pad size)
    //gStyle->SetStatH(0.2);      // Set height of stat-box (fraction of pad size)
    //

    TH1D* pF = (TH1D*)f1->Get("ESAlignmentTool/ESpF_residualX");  
    TH1D* pR = (TH1D*)f1->Get("ESAlignmentTool/ESpR_residualY");  
    TH1D* mF = (TH1D*)f1->Get("ESAlignmentTool/ESmF_residualX");  
    TH1D* mR = (TH1D*)f1->Get("ESAlignmentTool/ESmR_residualY");  

    cout<<"=============== +Front, X-Residual ================="<<endl;	fitResidual(pF,"+Front, X-Residual(cm)",color);	cout<<endl;
    cout<<"=============== +Rear, X-Residual  ================="<<endl;	fitResidual(pR,"+Rear, Y-Residual(cm)",color);	cout<<endl;
    cout<<"=============== -Front, X-Residual ================="<<endl;	fitResidual(mF,"-Front, X-Residual(cm)",color);	cout<<endl;
    cout<<"=============== -Rear, X-Residual  ================="<<endl;	fitResidual(mR,"-Rear, Y-Residual(cm)",color);	cout<<endl;
    pF->UseCurrentStyle(); pF->GetXaxis()->SetRangeUser(-1*xrange,xrange); pF->GetYaxis()->SetRangeUser(0,pF->GetMaximum()+(pF->GetMaximum())/2);
    pR->UseCurrentStyle(); pR->GetXaxis()->SetRangeUser(-1*xrange,xrange); pR->GetYaxis()->SetRangeUser(0,pR->GetMaximum()+(pR->GetMaximum())/2);
    mF->UseCurrentStyle(); mF->GetXaxis()->SetRangeUser(-1*xrange,xrange); mF->GetYaxis()->SetRangeUser(0,mF->GetMaximum()+(mF->GetMaximum())/2);
    mR->UseCurrentStyle(); mR->GetXaxis()->SetRangeUser(-1*xrange,xrange); mR->GetYaxis()->SetRangeUser(0,mR->GetMaximum()+(mR->GetMaximum())/2);
    
    setStyle(pF); setStyle(pR); setStyle(mF); setStyle(mR);

    c1->Clear();
    c1->Divide(2,2);
    c1->cd(1);
    pF->Draw("pe");
    c1->cd(2);
    pR->Draw("pe");
    c1->cd(3);
    mF->Draw("pe");
    c1->cd(4);
    mR->Draw("pe");
    c1->SaveAs(savePath.c_str());

}

void draw4CorResidual( TFile* f1, string outputName, int plan, Color_t color=2 ){ //plain 1:pF, 2:pR, 3:mF, 4:mR
    string planName;
    string DirctionOfResi;
    switch (plan){
        case 1:
            planName = "ESpF_resiX";
            DirctionOfResi = "X-Residual";
            break;
        case 2:
            planName = "ESpR_resiY";
            DirctionOfResi = "Y-Residual";
            break;
        case 3:
            planName = "ESmF_resiX";
            DirctionOfResi = "X-Residual";
            break;
        case 4:
            planName = "ESmR_resiY";
            DirctionOfResi = "Y-Residual";
            break;
        default:
            cout<<"You should give 3 iterm a the right number as following";
            cout<<"1:pF, 2:pR, 3:mF, 4:mR"<<endl;
            return;
    }

    cout<<"#############################################################################################"<<endl;
    cout<<"######################### 4Corner Residual:"<<outputName<<", "<<planName<<" #########################"<<endl;
    cout<<"#############################################################################################"<<endl;
    CMSstyle();
    string top 	= "h_"+planName+"_Top";		string xNameT = "Top, "+DirctionOfResi+"(cm)";
    string bottom	= "h_"+planName+"_Bottom";	string xNameB = "Bottom, "+DirctionOfResi+"(cm)";
    string left	= "h_"+planName+"_Left";	string xNameL = "Left, "+DirctionOfResi+"(cm)";
    string right	= "h_"+planName+"_Right";	string xNameR = "Right, "+DirctionOfResi+"(cm)";
    TH1D* TOP 	= (TH1D*)f1->Get(top.c_str()); 
    TH1D* BOTTOM 	= (TH1D*)f1->Get(bottom.c_str()); 
    TH1D* LEFT 	= (TH1D*)f1->Get(left.c_str()); 
    TH1D* RIGHT 	= (TH1D*)f1->Get(right.c_str()); 
    string savePath = "png/Residual_4Corner_"+outputName+"_"+planName+".png";

    cout<<"=============== Top, "<<DirctionOfResi<<" ================="<<endl;	fitResidual(TOP, xNameT, color);	cout<<endl;
    cout<<"=============== Bottom, "<<DirctionOfResi<<" ================="<<endl;	fitResidual(BOTTOM, xNameB, color);	cout<<endl;
    cout<<"=============== Left, "<<DirctionOfResi<<" ================="<<endl;	fitResidual(LEFT, xNameL, color);	cout<<endl;
    cout<<"=============== Right, "<<DirctionOfResi<<" ================="<<endl;	fitResidual(RIGHT, xNameR, color);	cout<<endl;
    TOP->UseCurrentStyle();		TOP->GetXaxis()->SetRangeUser(-3,3);
    BOTTOM->UseCurrentStyle();	BOTTOM->GetXaxis()->SetRangeUser(-3,3);
    LEFT->UseCurrentStyle();	LEFT->GetXaxis()->SetRangeUser(-3,3);
    RIGHT->UseCurrentStyle();	RIGHT->GetXaxis()->SetRangeUser(-3,3);

    TCanvas* c1 = new TCanvas("c1", "", 850,700);
    c1->Divide(2,2);
    c1->cd(1);
    TOP->Draw("pe");
    c1->cd(2);
    BOTTOM->Draw("pe");
    c1->cd(3);
    LEFT->Draw("pe");
    c1->cd(4);
    RIGHT->Draw("pe");
    c1->SaveAs(savePath.c_str());


}

