//ESmF_O_R:
//1, 0.000491103, -0.000500033)
//-0.00049, 0.999997, 0.0022)
//0.000501112, -0.00219975, 0.999997)
//DAlpha: 0.0022, dBeta: 0.000500035, dGamma: 0.000490001
//ESmR_O_R:
//1, -0.000130381, 0.0002)
//0.000129998, 0.999998, 0.00190003)
//-0.000200247, -0.0019, 0.999998)
//DAlpha: 0.00190003, dBeta: -0.0002, dGamma: -0.000129999
//ESpF_O_R:
//1, 0.000759701, -0.00030002)
//-0.000759996, 0.999999, -0.000999998)
//0.00029926, 0.00100023, 0.999999)
//DAlpha: -0.000999999, dBeta: 0.00030002, dGamma: 0.000759997
//ESpR_O_R:
//1, 0.000520797, 0.000499935)
//-0.000520001, 0.999999, -0.00160001)
//-0.000500768, 0.00159975, 0.999999)

void DrawPredictedHits( TFile* f  )
{
	TTree* t = (TTree*)f->Get("ESAlignmentTool/tree");
	TH2D* mf = new TH2D("h_mf","-Front;x[cm];y[cm]",250,-125,125,250,-125,125);
	TH2D* mr = new TH2D("h_mr","-Rear;x[cm];y[cm]",250,-125,125,250,-125,125);
	TH2D* pf = new TH2D("h_pf","+Front;x[cm];y[cm]",250,-125,125,250,-125,125);
	TH2D* pr = new TH2D("h_pr","+Rear;x[cm];y[cm]",250,-125,125,250,-125,125);

	const int nHits = 2000;
	const int nZ = 2; // +/-, 0=-, 1=+
	const int nP = 2; // R/F, 0=F, 1=R

	double ES_O_X[2][2],ES_O_Y[2][2],ES_O_Z[2][2];
	double ES_O_R11[2][2],ES_O_R12[2][2],ES_O_R13[2][2];
 	double ES_O_R21[2][2],ES_O_R22[2][2],ES_O_R23[2][2];
 	double ES_O_R31[2][2],ES_O_R32[2][2],ES_O_R33[2][2];

	ES_O_X[0][0]=-0.0502356;ES_O_Y[0][0]=-0.778966; ES_O_Z[0][0]=-304.646;
	ES_O_X[0][1]=-0.0599058;ES_O_Y[0][1]=-0.749106; ES_O_Z[0][1]=-309.286;
	ES_O_X[1][0]=0.52014; 	ES_O_Y[1][0]=-0.96953; 	ES_O_Z[1][0]=304.466;
	ES_O_X[1][1]=0.539764; 	ES_O_Y[1][1]=-0.959248; ES_O_Z[1][1]=309.116;

	ES_O_R11[0][0]=1.;  		ES_O_R12[0][0]=0.000491103;  	ES_O_R13[0][0]=-0.000500033;
	ES_O_R21[0][0]=-0.00049;  	ES_O_R22[0][0]=0.999997;  	ES_O_R23[0][0]=0.0022;
	ES_O_R31[0][0]=0.000501112;  	ES_O_R32[0][0]=-0.00219975;  	ES_O_R33[0][0]=0.999997;

	ES_O_R11[0][1]=1.;  		ES_O_R12[0][1]=-0.000130381;  	ES_O_R13[0][1]=0.0002;
	ES_O_R21[0][1]=0.000129998;  	ES_O_R22[0][1]=0.999998;  	ES_O_R23[0][1]=0.00190003;
	ES_O_R31[0][1]=-0.000200247;  	ES_O_R32[0][1]=-0.0019;  	ES_O_R33[0][1]=0.999998;

	ES_O_R11[1][0]=1.; 	 	ES_O_R12[1][0]=0.000759701;  	ES_O_R13[1][0]=-0.00030002;
	ES_O_R21[1][0]=-0.000759996;  	ES_O_R22[1][0]=0.999999;  	ES_O_R23[1][0]=-0.000999998;
	ES_O_R31[1][0]=0.00029926;  	ES_O_R32[1][0]=0.00100023;  	ES_O_R33[1][0]=0.999999;

	ES_O_R11[1][1]=1.;		ES_O_R12[1][1]=0.000520797;  	ES_O_R13[1][1]=0.000499935;
	ES_O_R21[1][1]=-0.000520001;  	ES_O_R22[1][1]=0.999999;	ES_O_R23[1][1]=-0.00160001;
	ES_O_R31[1][1]=-0.000500768;  	ES_O_R32[1][1]=0.00159975;  	ES_O_R33[1][1]=0.999999;

	int Ntrack;
	double PredictionState_X[nHits][nZ][nP];
	double PredictionState_Y[nHits][nZ][nP];
	double PredictionState_Z[nHits][nZ][nP];
	t->SetBranchAddress("Ntrack", &Ntrack);
	t->SetBranchAddress("PredictionState_X", &PredictionState_X[0][0][0]);	
	t->SetBranchAddress("PredictionState_Y", &PredictionState_Y[0][0][0]);	
	t->SetBranchAddress("PredictionState_Z", &PredictionState_Z[0][0][0]);

	cout<<"Total events: "<<t->GetEntries()<<endl;
	for( int entry=0; entry<t->GetEntries(); entry++ )	
	{
		if( entry%100 == 0 ) cout<<".";
		t->GetEntry(entry);
		for( int itrk=0; itrk<Ntrack; itrk++){
			double X[nZ][nP];
			double Y[nZ][nP];
			for( int z=0; z<2; z++){
				for( int p=0; p<2; p++)
				{
					X[z][p]= (PredictionState_X[itrk][z][p]-ES_O_X[z][p])*ES_O_R11[z][p]
					        +(PredictionState_Y[itrk][z][p]-ES_O_Y[z][p])*ES_O_R12[z][p]
					        +(PredictionState_Z[itrk][z][p]-ES_O_Z[z][p])*ES_O_R13[z][p];
					Y[z][p]= (PredictionState_X[itrk][z][p]-ES_O_X[z][p])*ES_O_R21[z][p]
					        +(PredictionState_Y[itrk][z][p]-ES_O_Y[z][p])*ES_O_R22[z][p]
					        +(PredictionState_Z[itrk][z][p]-ES_O_Z[z][p])*ES_O_R23[z][p];
				}
			}
			mf->Fill(X[0][0],Y[0][0]);
			mr->Fill(X[0][1],Y[0][1]);
			pf->Fill(X[1][0],Y[1][0]);
			pr->Fill(X[1][1],Y[1][0]);
		}
	}
	cout<<endl<<"End"<<endl;	
	TCanvas* c1 = new TCanvas("4 plots", "", 1000, 800);
	c1->Divide(2,2);
	c1->cd(1);
	mf->Draw("COLZ");
	c1->cd(2);
	mr->Draw("COLZ");
	c1->cd(3);
	pf->Draw("COLZ");
	c1->cd(4);
	pr->Draw("COLZ");
}

//void DrawRecHits( TFile* f  )
//{
//	TTree* t = (TTree*)f->Get("ESAlignmentTool/tree");
//	TH2D* mf = new TH2D("h_mf","-Front;x[cm];y[cm]",250,-125,125,250,-125,125);
//	TH2D* mr = new TH2D("h_mr","-Rear;x[cm];y[cm]",250,-125,125,250,-125,125);
//	TH2D* pf = new TH2D("h_pf","+Front;x[cm];y[cm]",250,-125,125,250,-125,125);
//	TH2D* pr = new TH2D("h_pr","+Rear;x[cm];y[cm]",250,-125,125,250,-125,125);
//
//	const int nesrHits = 10000;
//	const int nZ = 2; // +/-, 0=-, 1=+
//	const int nP = 2; // R/F, 0=F, 1=R
//
//	double ES_O_X[2][2],ES_O_Y[2][2],ES_O_Z[2][2];
//	double ES_O_R11[2][2],ES_O_R12[2][2],ES_O_R13[2][2];
// 	double ES_O_R21[2][2],ES_O_R22[2][2],ES_O_R23[2][2];
// 	double ES_O_R31[2][2],ES_O_R32[2][2],ES_O_R33[2][2];
//
//	ES_O_X[0][0]=-0.0502356;ES_O_Y[0][0]=-0.778966; ES_O_Z[0][0]=-304.646;
//	ES_O_X[0][1]=-0.0599058;ES_O_Y[0][1]=-0.749106; ES_O_Z[0][1]=-309.286;
//	ES_O_X[1][0]=0.52014; 	ES_O_Y[1][0]=-0.96953; 	ES_O_Z[1][0]=304.466;
//	ES_O_X[1][1]=0.539764; 	ES_O_Y[1][1]=-0.959248; ES_O_Z[1][1]=309.116;
//
//	ES_O_R11[0][0]=1.;  		ES_O_R12[0][0]=0.000491103;  	ES_O_R13[0][0]=-0.000500033;
//	ES_O_R21[0][0]=-0.00049;  	ES_O_R22[0][0]=0.999997;  	ES_O_R23[0][0]=0.0022;
//	ES_O_R31[0][0]=0.000501112;  	ES_O_R32[0][0]=-0.00219975;  	ES_O_R33[0][0]=0.999997;
//
//	ES_O_R11[0][1]=1.;  		ES_O_R12[0][1]=-0.000130381;  	ES_O_R13[0][1]=0.0002;
//	ES_O_R21[0][1]=0.000129998;  	ES_O_R22[0][1]=0.999998;  	ES_O_R23[0][1]=0.00190003;
//	ES_O_R31[0][1]=-0.000200247;  	ES_O_R32[0][1]=-0.0019;  	ES_O_R33[0][1]=0.999998;
//
//	ES_O_R11[1][0]=1.; 	 	ES_O_R12[1][0]=0.000759701;  	ES_O_R13[1][0]=-0.00030002;
//	ES_O_R21[1][0]=-0.000759996;  	ES_O_R22[1][0]=0.999999;  	ES_O_R23[1][0]=-0.000999998;
//	ES_O_R31[1][0]=0.00029926;  	ES_O_R32[1][0]=0.00100023;  	ES_O_R33[1][0]=0.999999;
//
//	ES_O_R11[1][1]=1.;		ES_O_R12[1][1]=0.000520797;  	ES_O_R13[1][1]=0.000499935;
//	ES_O_R21[1][1]=-0.000520001;  	ES_O_R22[1][1]=0.999999;	ES_O_R23[1][1]=-0.00160001;
//	ES_O_R31[1][1]=-0.000500768;  	ES_O_R32[1][1]=0.00159975;  	ES_O_R33[1][1]=0.999999;
//
//	int Ntrack, NESRehits;
//	double _esRecHit_X[nesrHits];
//	double _esRecHit_Y[nesrHits];
//	double _esRecHit_Z[nesrHits];
//	int _esRecHit_siZ[nesrHits];
//	int _esRecHit_siP[nesrHits];
//	int _esRecHit_Noisy[nesrHits];
//	t->SetBranchAddress("Nesrh", &NESRehits);
//	t->SetBranchAddress("_esRecHit_X", &_esRecHit_X[0]);	
//	t->SetBranchAddress("_esRecHit_Y", &_esRecHit_Y[0]);	
//	t->SetBranchAddress("_esRecHit_Z", &_esRecHit_Z[0]);
//	t->SetBranchAddress("_esRecHit_siZ", &_esRecHit_siZ[0]);	
//	t->SetBranchAddress("_esRecHit_siP", &_esRecHit_siP[0]);
//	t->SetBranchAddress("_esRecHit_Noisy", &_esRecHit_siP[0]);
//
//	cout<<"Total events: "<<t->GetEntries()<<endl;
//	for( int entry=0; entry<t->GetEntries(); entry++ )	
//	{
//		if( entry%100 == 0 ) cout<<".";
//		t->GetEntry(entry);
//		for( int irh=0; irh<NESRehits; irh++){
//			double X[nZ][nP];
//			double Y[nZ][nP];
//			if( _esRecHit_Noisy[irh] == 1 ) continue;
//			for( int z=0; z<2; z++){
//				int zi;
//				z == 0 ? zi=-1:zi=1;
//				if( _esRecHit_siP[irh] != zi ) continue;
//				for( int p=0; p<2; p++)
//				{
//					X[z][p]= (PredictionState_X[itrk][z][p]-ES_O_X[z][p])*ES_O_R11[z][p]
//					        +(PredictionState_Y[itrk][z][p]-ES_O_Y[z][p])*ES_O_R12[z][p]
//					        +(PredictionState_Z[itrk][z][p]-ES_O_Z[z][p])*ES_O_R13[z][p];
//					Y[z][p]= (PredictionState_X[itrk][z][p]-ES_O_X[z][p])*ES_O_R21[z][p]
//					        +(PredictionState_Y[itrk][z][p]-ES_O_Y[z][p])*ES_O_R22[z][p]
//					        +(PredictionState_Z[itrk][z][p]-ES_O_Z[z][p])*ES_O_R23[z][p];
//				}
//			}
//			mf->Fill(X[0][0],Y[0][0]);
//			mr->Fill(X[0][1],Y[0][1]);
//			pf->Fill(X[1][0],Y[1][0]);
//			pr->Fill(X[1][1],Y[1][0]);
//		}
//	}
//	cout<<endl<<"End"<<endl;	
//	TCanvas* c1 = new TCanvas("4 plots", "", 1000, 800);
//	c1->Divide(2,2);
//	c1->cd(1);
//	mf->Draw("COLZ");
//	c1->cd(2);
//	mr->Draw("COLZ");
//	c1->cd(3);
//	pf->Draw("COLZ");
//	c1->cd(4);
//	pr->Draw("COLZ");
//
//}
