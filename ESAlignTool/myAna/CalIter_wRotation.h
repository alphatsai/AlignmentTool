#include <iostream>
#include <fstream>

#include "TROOT.h"


class CalIter_wRotation
{
 public:
   CalIter_wRotation();
   void run( std::string outputName="file");
   void registerESMatrix(TTree* tree);
   Int_t iterN;
 private:
   Double_t detSym(int,int);
   Double_t GetIterX(int,int);
   Double_t GetIterY(int,int);
   Double_t GetIterZ(int,int);
   Double_t GetIterAlpha(int,int);
   Double_t GetIterBeta(int,int);
   Double_t GetIterGamma(int,int);
   Int_t GetIteration(Int_t,Int_t,Double_t &,Double_t &,Double_t &,Double_t &,Double_t &,Double_t &);
   Double_t detM33(Double_t [][3]);
   void InvM33(Double_t [][3],Double_t [][3]);
   void DotM33(Double_t [][3],Double_t [][3],Double_t [][3]);
   void AddM33(Double_t [][3],Double_t [][3],Double_t [][3],Double_t);
   void setzeroM33(Double_t [3][3]);

   Double_t ES_M11[2][2],ES_M12[2][2],ES_M13[2][2],ES_M14[2][2],ES_M15[2][2],ES_M16[2][2],ES_M22[2][2],ES_M23[2][2],ES_M24[2][2],ES_M25[2][2],ES_M26[2][2],ES_M33[2][2],ES_M34[2][2],ES_M35[2][2],ES_M36[2][2],ES_M44[2][2],ES_M45[2][2],ES_M46[2][2],ES_M55[2][2],ES_M56[2][2],ES_M66[2][2];
   Double_t ES_bM11[2][2],ES_bM12[2][2],ES_bM13[2][2],ES_bM14[2][2],ES_bM15[2][2],ES_bM16[2][2],ES_bM22[2][2],ES_bM23[2][2],ES_bM24[2][2],ES_bM25[2][2],ES_bM26[2][2],ES_bM33[2][2],ES_bM34[2][2],ES_bM35[2][2],ES_bM36[2][2],ES_bM44[2][2],ES_bM45[2][2],ES_bM46[2][2],ES_bM55[2][2],ES_bM56[2][2],ES_bM66[2][2];
   Double_t ES_P1[2][2],ES_P2[2][2],ES_P3[2][2],ES_P4[2][2],ES_P5[2][2],ES_P6[2][2];
   Double_t ES_bP1[2][2],ES_bP2[2][2],ES_bP3[2][2],ES_bP4[2][2],ES_bP5[2][2],ES_bP6[2][2];

   Double_t InvM66[2][2][6][6];
   Double_t ES_CHI2[2][2];
   Double_t ES_bCHI2[2][2];
   Long64_t ES_NTracks[2][2];
   Long64_t ES_bNTracks[2][2];
   Double_t ES_dX[2][2],ES_dY[2][2],ES_dZ[2][2];
   Double_t ES_dAlpha[2][2],ES_dBeta[2][2],ES_dGamma[2][2];
};
void CalIter_wRotation::registerESMatrix(TTree* tree){
	tree->SetBranchAddress("ES_M11", &ES_bM11[0][0]);	
	tree->SetBranchAddress("ES_M12", &ES_bM12[0][0]);
	tree->SetBranchAddress("ES_M13", &ES_bM13[0][0]);	
	tree->SetBranchAddress("ES_M14", &ES_bM14[0][0]);	
	tree->SetBranchAddress("ES_M15", &ES_bM15[0][0]);	
	tree->SetBranchAddress("ES_M16", &ES_bM16[0][0]);	
	tree->SetBranchAddress("ES_M22", &ES_bM22[0][0]);	
	tree->SetBranchAddress("ES_M23", &ES_bM23[0][0]);	
	tree->SetBranchAddress("ES_M24", &ES_bM24[0][0]);	
	tree->SetBranchAddress("ES_M25", &ES_bM25[0][0]);	
	tree->SetBranchAddress("ES_M26", &ES_bM26[0][0]);	
	tree->SetBranchAddress("ES_M33", &ES_bM33[0][0]);	
	tree->SetBranchAddress("ES_M34", &ES_bM34[0][0]);	
	tree->SetBranchAddress("ES_M35", &ES_bM35[0][0]);	
	tree->SetBranchAddress("ES_M36", &ES_bM36[0][0]);	
	tree->SetBranchAddress("ES_M44", &ES_bM44[0][0]);	
	tree->SetBranchAddress("ES_M45", &ES_bM45[0][0]);	
	tree->SetBranchAddress("ES_M46", &ES_bM46[0][0]);	
	tree->SetBranchAddress("ES_M55", &ES_bM55[0][0]);	
	tree->SetBranchAddress("ES_M56", &ES_bM56[0][0]);	
	tree->SetBranchAddress("ES_M66", &ES_bM66[0][0]);
	tree->SetBranchAddress("ES_P1", &ES_bP1[0][0]);	
	tree->SetBranchAddress("ES_P2", &ES_bP2[0][0]);
	tree->SetBranchAddress("ES_P3", &ES_bP3[0][0]);	
	tree->SetBranchAddress("ES_P4", &ES_bP4[0][0]);	
	tree->SetBranchAddress("ES_P5", &ES_bP5[0][0]);	
	tree->SetBranchAddress("ES_P6", &ES_bP6[0][0]);	
	tree->SetBranchAddress("ES_CHI2", &ES_bCHI2[0][0]);	
	tree->SetBranchAddress("ES_NTracks", &ES_bNTracks[0][0]);	
	for( int entry=0; entry<tree->GetEntries(); entry++){
		tree->GetEntry(entry);
		for(int j=0; j<2; j++){
			for( int k=0; k<2; k++){
				ES_M11[j][k]+=ES_bM11[j][k];
				ES_M12[j][k]+=ES_bM12[j][k];
				ES_M13[j][k]+=ES_bM13[j][k];
				ES_M14[j][k]+=ES_bM14[j][k];
				ES_M15[j][k]+=ES_bM15[j][k];
				ES_M16[j][k]+=ES_bM16[j][k];
				ES_M22[j][k]+=ES_bM22[j][k];
				ES_M23[j][k]+=ES_bM23[j][k];
				ES_M24[j][k]+=ES_bM24[j][k];
				ES_M25[j][k]+=ES_bM25[j][k];
				ES_M26[j][k]+=ES_bM26[j][k];
				ES_M33[j][k]+=ES_bM33[j][k];
				ES_M34[j][k]+=ES_bM34[j][k];
				ES_M35[j][k]+=ES_bM35[j][k];
				ES_M36[j][k]+=ES_bM36[j][k];
				ES_M44[j][k]+=ES_bM44[j][k];
				ES_M45[j][k]+=ES_bM45[j][k];
				ES_M46[j][k]+=ES_bM46[j][k];
				ES_M55[j][k]+=ES_bM55[j][k];
				ES_M56[j][k]+=ES_bM56[j][k];
				ES_M66[j][k]+=ES_bM66[j][k];
				ES_P1[j][k]+=ES_bP1[j][k];
				ES_P2[j][k]+=ES_bP2[j][k];
				ES_P3[j][k]+=ES_bP3[j][k];
				ES_P4[j][k]+=ES_bP4[j][k];
				ES_P5[j][k]+=ES_bP5[j][k];
				ES_P6[j][k]+=ES_bP6[j][k];
				ES_CHI2[j][k]+=ES_bCHI2[j][k];
				ES_NTracks[j][k]+=ES_bNTracks[j][k];
			}
		}
	}
}

CalIter_wRotation::CalIter_wRotation()
{
   iterN=0;
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
     ES_dX[i][j]=0.; ES_dY[i][j]=0.; ES_dZ[i][j]=0.;
     ES_dAlpha[i][j]=0.; ES_dBeta[i][j]=0.; ES_dGamma[i][j]=0.;
     ES_NTracks[i][j]=0;
   }}
}

void CalIter_wRotation::run( std::string outputName)
{
  cout<<"Alignment on ES+Front :\n";
  if(GetIteration(1,1,ES_dX[1][0],ES_dY[1][0],ES_dZ[1][0],ES_dAlpha[1][0],ES_dBeta[1][0],ES_dGamma[1][0])==1)
  {
   cout<<"dX="<<ES_dX[1][0]<<", dY="<<ES_dY[1][0]<<", dZ="<<ES_dZ[1][0]<<"\n";
   cout<<"dAlpha="<<ES_dAlpha[1][0]<<", dBeta="<<ES_dBeta[1][0]<<", dGamma="<<ES_dGamma[1][0]<<"\n";
   cout<<"normalized CHI2 before iteration="<<ES_CHI2[1][0]/ES_NTracks[1][0]<<" ";
   cout<<"# of Tracks="<<ES_NTracks[1][0]<<"\n";
  }
  cout<<"Alignment on ES+Rear :\n";
  if(GetIteration(1,2,ES_dX[1][1],ES_dY[1][1],ES_dZ[1][1],ES_dAlpha[1][1],ES_dBeta[1][1],ES_dGamma[1][1])==1)
  {
   cout<<"dX="<<ES_dX[1][1]<<", dY="<<ES_dY[1][1]<<", dZ="<<ES_dZ[1][1]<<"\n";
   cout<<"dAlpha="<<ES_dAlpha[1][1]<<", dBeta="<<ES_dBeta[1][1]<<", dGamma="<<ES_dGamma[1][1]<<"\n";
   cout<<"normalized CHI2 before iteration="<<ES_CHI2[1][1]/ES_NTracks[1][1]<<" ";
   cout<<"# of Tracks="<<ES_NTracks[1][1]<<"\n";
  }
  cout<<"Alignment on ES-Front :\n";
  if(GetIteration(-1,1,ES_dX[0][0],ES_dY[0][0],ES_dZ[0][0],ES_dAlpha[0][0],ES_dBeta[0][0],ES_dGamma[0][0])==1)
  {
   cout<<"dX="<<ES_dX[0][0]<<", dY="<<ES_dY[0][0]<<", dZ="<<ES_dZ[0][0]<<"\n";
   cout<<"dAlpha="<<ES_dAlpha[0][0]<<", dBeta="<<ES_dBeta[0][0]<<", dGamma="<<ES_dGamma[0][0]<<"\n";
   cout<<"normalized CHI2 before iteration="<<ES_CHI2[0][0]/ES_NTracks[0][0]<<" ";
   cout<<"# of Tracks="<<ES_NTracks[0][0]<<"\n";
  }
  cout<<"Alignment on ES-Rear :\n";
  if(GetIteration(-1,2,ES_dX[0][1],ES_dY[0][1],ES_dZ[0][1],ES_dAlpha[0][1],ES_dBeta[0][1],ES_dGamma[0][1])==1)
  {
   cout<<"dX="<<ES_dX[0][1]<<", dY="<<ES_dY[0][1]<<", dZ="<<ES_dZ[0][1]<<"\n";
   cout<<"dAlpha="<<ES_dAlpha[0][1]<<", dBeta="<<ES_dBeta[0][1]<<", dGamma="<<ES_dGamma[0][1]<<"\n";
   cout<<"normalized CHI2 before iteration="<<ES_CHI2[0][1]/ES_NTracks[0][1]<<" ";
   cout<<"# of Tracks="<<ES_NTracks[0][1]<<"\n";
  }

  ofstream file;
  file.open((outputName+".txt").c_str(),ios::out|ios::app);
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpFdX = cms.double("<<ES_dX[1][0]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpFdY = cms.double("<<ES_dY[1][0]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpFdZ = cms.double("<<ES_dZ[1][0]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpFdAlpha = cms.double("<<ES_dAlpha[1][0]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpFdBeta = cms.double("<<ES_dBeta[1][0]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpFdGamma = cms.double("<<ES_dGamma[1][0]<<")\n";

  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpRdX = cms.double("<<ES_dX[1][1]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpRdY = cms.double("<<ES_dY[1][1]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpRdZ = cms.double("<<ES_dZ[1][1]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpRdAlpha = cms.double("<<ES_dAlpha[1][1]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpRdBeta = cms.double("<<ES_dBeta[1][1]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESpRdGamma = cms.double("<<ES_dGamma[1][1]<<")\n";

  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmFdX = cms.double("<<ES_dX[0][0]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmFdY = cms.double("<<ES_dY[0][0]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmFdZ = cms.double("<<ES_dZ[0][0]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmFdAlpha = cms.double("<<ES_dAlpha[0][0]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmFdBeta = cms.double("<<ES_dBeta[0][0]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmFdGamma = cms.double("<<ES_dGamma[0][0]<<")\n";

  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmRdX = cms.double("<<ES_dX[0][1]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmRdY = cms.double("<<ES_dY[0][1]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmRdZ = cms.double("<<ES_dZ[0][1]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmRdAlpha = cms.double("<<ES_dAlpha[0][1]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmRdBeta = cms.double("<<ES_dBeta[0][1]<<")\n";
  file<<"MatrixElementsTmp.Iter"<<iterN<<"_ESmRdGamma = cms.double("<<ES_dGamma[0][1]<<")\n";

  file<<"\n";
  file.close();

  ofstream file2;
  file2.open((outputName+"2.txt").c_str(),ios::out|ios::app);
  file2<<"idx="<<iterN-1<<";\n";
  file2<<"ESpFdX-="<<ES_dX[1][0]<<"; ESpFdY-="<<ES_dY[1][0]<<"; ESpFdZ-="<<ES_dZ[1][0]<<";\n";
  file2<<"ESpFdAlpha-="<<ES_dAlpha[1][0]<<"; ESpFdBeta-="<<ES_dBeta[1][0]<<"; ESpFdGamma-="<<ES_dGamma[1][0]<<";\n";
  file2<<"ESpFdXArray[idx]=ESpFdX; ESpFdYArray[idx]=ESpFdY; ESpFdZArray[idx]=ESpFdZ;\n";
  file2<<"ESpFdAlphaArray[idx]=ESpFdAlpha; ESpFdBetaArray[idx]=ESpFdBeta; ESpFdGammaArray[idx]=ESpFdGamma;\n";
  file2<<"ESpFchisqArray[idx]="<<ES_CHI2[1][0]/ES_NTracks[1][0]<<"; //";
  file2<<ES_NTracks[1][0]<<"\n";

  file2<<"ESpRdX-="<<ES_dX[1][1]<<"; ESpRdY-="<<ES_dY[1][1]<<"; ESpRdZ-="<<ES_dZ[1][1]<<";\n";
  file2<<"ESpRdAlpha-="<<ES_dAlpha[1][1]<<"; ESpRdBeta-="<<ES_dBeta[1][1]<<"; ESpRdGamma-="<<ES_dGamma[1][1]<<";\n";
  file2<<"ESpRdXArray[idx]=ESpRdX; ESpRdYArray[idx]=ESpRdY; ESpRdZArray[idx]=ESpRdZ;\n";
  file2<<"ESpRdAlphaArray[idx]=ESpRdAlpha; ESpRdBetaArray[idx]=ESpRdBeta; ESpRdGammaArray[idx]=ESpRdGamma;\n";
  file2<<"ESpRchisqArray[idx]="<<ES_CHI2[1][1]/ES_NTracks[1][1]<<"; //";
  file2<<ES_NTracks[1][1]<<"\n";

  file2<<"ESmFdX-="<<ES_dX[0][0]<<"; ESmFdY-="<<ES_dY[0][0]<<"; ESmFdZ-="<<ES_dZ[0][0]<<";\n";
  file2<<"ESmFdAlpha-="<<ES_dAlpha[0][0]<<"; ESmFdBeta-="<<ES_dBeta[0][0]<<"; ESmFdGamma-="<<ES_dGamma[0][0]<<";\n";
  file2<<"ESmFdXArray[idx]=ESmFdX; ESmFdYArray[idx]=ESmFdY; ESmFdZArray[idx]=ESmFdZ;\n";
  file2<<"ESmFdAlphaArray[idx]=ESmFdAlpha; ESmFdBetaArray[idx]=ESmFdBeta; ESmFdGammaArray[idx]=ESmFdGamma;\n";
  file2<<"ESmFchisqArray[idx]="<<ES_CHI2[0][0]/ES_NTracks[0][0]<<"; //";
  file2<<ES_NTracks[0][0]<<"\n";

  file2<<"ESmRdX-="<<ES_dX[0][1]<<"; ESmRdY-="<<ES_dY[0][1]<<"; ESmRdZ-="<<ES_dZ[0][1]<<";\n";
  file2<<"ESmRdAlpha-="<<ES_dAlpha[0][1]<<"; ESmRdBeta-="<<ES_dBeta[0][1]<<"; ESmRdGamma-="<<ES_dGamma[0][1]<<";\n";
  file2<<"ESmRdXArray[idx]=ESmRdX; ESmRdYArray[idx]=ESmRdY; ESmRdZArray[idx]=ESmRdZ;\n";
  file2<<"ESmRdAlphaArray[idx]=ESmRdAlpha; ESmRdBetaArray[idx]=ESmRdBeta; ESmRdGammaArray[idx]=ESmRdGamma;\n";
  file2<<"ESmRchisqArray[idx]="<<ES_CHI2[0][1]/ES_NTracks[0][1]<<"; //";
  file2<<ES_NTracks[0][1]<<"\n";

  file2<<"\n";
  file2.close();

}

Int_t CalIter_wRotation::GetIteration(Int_t iz,Int_t ip,Double_t &IterX,Double_t &IterY,Double_t &IterZ,Double_t &IterAlpha,Double_t &IterBeta,Double_t &IterGamma)
{
 Int_t res=0;int a,b;
 if( (iz==1||iz==-1) && (ip==1||ip==2) )
 {
  iz==-1 ? a=0 : a=1; b=ip-1;
  if( detSym(iz,ip)!=0 )
  {
   IterX=GetIterX(iz,ip);
   IterY=GetIterY(iz,ip);
   IterZ=GetIterZ(iz,ip);
   IterAlpha=GetIterAlpha(iz,ip);
   IterBeta=GetIterBeta(iz,ip);
   IterGamma=GetIterGamma(iz,ip);
   res=1;
 }}//4planes det!=0

 return res;
}

Double_t CalIter_wRotation::detSym(int iz,int ip)
{
 Double_t res=0.; int a,b;
 if( (iz==1||iz==-1) && (ip==1||ip==2) )
 {
  iz==-1 ? a=0 : a=1; b=ip-1;
  Double_t M11=ES_M11[a][b];
  Double_t M12=ES_M12[a][b];
  Double_t M13=ES_M13[a][b];
  Double_t M14=ES_M14[a][b];
  Double_t M15=ES_M15[a][b];
  Double_t M16=ES_M16[a][b];
  Double_t M22=ES_M22[a][b];
  Double_t M23=ES_M23[a][b];
  Double_t M24=ES_M24[a][b];
  Double_t M25=ES_M25[a][b];
  Double_t M26=ES_M26[a][b];
  Double_t M33=ES_M33[a][b];
  Double_t M34=ES_M34[a][b];
  Double_t M35=ES_M35[a][b];
  Double_t M36=ES_M36[a][b];
  Double_t M44=ES_M44[a][b];
  Double_t M45=ES_M45[a][b];
  Double_t M46=ES_M46[a][b];
  Double_t M55=ES_M55[a][b];
  Double_t M56=ES_M56[a][b];
  Double_t M66=ES_M66[a][b];

  Double_t detA=0.; Double_t dettempK=0.;
  Double_t A[3][3];
  A[0][0]=M11; A[0][1]=M12; A[0][2]=M13;
  A[1][0]=M12; A[1][1]=M22; A[1][2]=M23;
  A[2][0]=M13; A[2][1]=M23; A[2][2]=M33;
  Double_t B[3][3];
  B[0][0]=M14; B[0][1]=M15; B[0][2]=M16;
  B[1][0]=M24; B[1][1]=M25; B[1][2]=M26;
  B[2][0]=M34; B[2][1]=M35; B[2][2]=M36;
  Double_t C[3][3];
  C[0][0]=M14; C[0][1]=M24; C[0][2]=M34;
  C[1][0]=M15; C[1][1]=M25; C[1][2]=M35;
  C[2][0]=M16; C[2][1]=M26; C[2][2]=M36;
  Double_t D[3][3];
  D[0][0]=M44; D[0][1]=M45; D[0][2]=M46;
  D[1][0]=M45; D[1][1]=M55; D[1][2]=M56;
  D[2][0]=M46; D[2][1]=M56; D[2][2]=M66;

  detA=detM33(A);
  if(detA==0)   return detA;
  
 
  Double_t InvA[3][3],tmpCInvA[3][3],tmpCInvAB[3][3];
  Double_t InvMA[3][3],InvMB[3][3],InvMC[3][3],InvMD[3][3];
  Double_t tempK[3][3]; //K = D-C.InvA.B
  Double_t temp1[3][3],temp2[3][3];
  Double_t zero[3][3];
  setzeroM33(zero);

  InvM33(InvA,A);
  DotM33(tmpCInvA,C,InvA);
  DotM33(tmpCInvAB,tmpCInvA,B);
  AddM33(tempK,D,tmpCInvAB,-1.);
  
  dettempK=detM33(tempK); res=dettempK;
  if(dettempK==0) return dettempK;

  InvM33(InvMD,tempK); //result InvMD

  AddM33(temp1,zero,InvMD,-1.);
  DotM33(temp2,temp1,C);
  DotM33(InvMC,temp2,InvA); //result InvMC

  AddM33(temp1,zero,InvA,-1.);
  DotM33(temp2,temp1,B);
  DotM33(InvMB,temp2,InvMD); //result InvMB

  DotM33(temp1,InvMB,C);
  DotM33(temp2,temp1,InvA);
  AddM33(InvMA,InvA,temp2,-1.); //result InvMA

  for(int i=0;i<3;i++)
  {
   for(int j=0;j<3;j++)
   {
    InvM66[a][b][i][j] = InvMA[i][j];
    InvM66[a][b][i][j+3] = InvMB[i][j];
    InvM66[a][b][i+3][j] = InvMC[i][j];
    InvM66[a][b][i+3][j+3] = InvMD[i][j];
  }}
 }
 return res;
}
Double_t CalIter_wRotation::GetIterX(int iz,int ip)
{
 Double_t res=0.; int a,b;

 if( (iz==1||iz==-1) && (ip==1||ip==2) )
 {
  iz==-1 ? a=0 : a=1; b=ip-1;
  
  res=  InvM66[a][b][0][0]*ES_P1[a][b] + InvM66[a][b][0][1]*ES_P2[a][b]
      + InvM66[a][b][0][2]*ES_P3[a][b] + InvM66[a][b][0][3]*ES_P4[a][b]
      + InvM66[a][b][0][4]*ES_P5[a][b] + InvM66[a][b][0][5]*ES_P6[a][b] ;
 }
 return res;
}
Double_t CalIter_wRotation::GetIterY(int iz,int ip)
{
 Double_t res=0.; int a,b;

 if( (iz==1||iz==-1) && (ip==1||ip==2) )
 {
  iz==-1 ? a=0 : a=1; b=ip-1;

  res=  InvM66[a][b][1][0]*ES_P1[a][b] + InvM66[a][b][1][1]*ES_P2[a][b]
      + InvM66[a][b][1][2]*ES_P3[a][b] + InvM66[a][b][1][3]*ES_P4[a][b]
      + InvM66[a][b][1][4]*ES_P5[a][b] + InvM66[a][b][1][5]*ES_P6[a][b] ;
 }
 return res;
}
Double_t CalIter_wRotation::GetIterZ(int iz,int ip)
{
 Double_t res=0.; int a,b;

 if( (iz==1||iz==-1) && (ip==1||ip==2) )
 {
  iz==-1 ? a=0 : a=1; b=ip-1;

  res=  InvM66[a][b][2][0]*ES_P1[a][b] + InvM66[a][b][2][1]*ES_P2[a][b]
      + InvM66[a][b][2][2]*ES_P3[a][b] + InvM66[a][b][2][3]*ES_P4[a][b]
      + InvM66[a][b][2][4]*ES_P5[a][b] + InvM66[a][b][2][5]*ES_P6[a][b] ;
 }
 return res;
}
Double_t CalIter_wRotation::GetIterAlpha(int iz,int ip)
{ 
 Double_t res=0.; int a,b;
  
 if( (iz==1||iz==-1) && (ip==1||ip==2) )
 {  
  iz==-1 ? a=0 : a=1; b=ip-1;
  
  res=  InvM66[a][b][3][0]*ES_P1[a][b] + InvM66[a][b][3][1]*ES_P2[a][b]
      + InvM66[a][b][3][2]*ES_P3[a][b] + InvM66[a][b][3][3]*ES_P4[a][b]
      + InvM66[a][b][3][4]*ES_P5[a][b] + InvM66[a][b][3][5]*ES_P6[a][b] ;
 }
 return res;
} 
Double_t CalIter_wRotation::GetIterBeta(int iz,int ip)
{
 Double_t res=0.; int a,b;
  
 if( (iz==1||iz==-1) && (ip==1||ip==2) )
 {
  iz==-1 ? a=0 : a=1; b=ip-1;
  
  res=  InvM66[a][b][4][0]*ES_P1[a][b] + InvM66[a][b][4][1]*ES_P2[a][b]
      + InvM66[a][b][4][2]*ES_P3[a][b] + InvM66[a][b][4][3]*ES_P4[a][b]
      + InvM66[a][b][4][4]*ES_P5[a][b] + InvM66[a][b][4][5]*ES_P6[a][b] ;
 }
 return res;
} 
Double_t CalIter_wRotation::GetIterGamma(int iz,int ip)
{ 
 Double_t res=0.; int a,b;
  
 if( (iz==1||iz==-1) && (ip==1||ip==2) )
 {
  iz==-1 ? a=0 : a=1; b=ip-1;
   
  res=  InvM66[a][b][5][0]*ES_P1[a][b] + InvM66[a][b][5][1]*ES_P2[a][b]
      + InvM66[a][b][5][2]*ES_P3[a][b] + InvM66[a][b][5][3]*ES_P4[a][b]
      + InvM66[a][b][5][4]*ES_P5[a][b] + InvM66[a][b][5][5]*ES_P6[a][b] ;
 }
 return res;
}

Double_t CalIter_wRotation::detM33(Double_t M33[3][3])
{
 Double_t res=0.;
 res = -(M33[0][2]*M33[1][1]*M33[2][0]) + M33[0][1]*M33[1][2]*M33[2][0] + M33[0][2]*M33[1][0]*M33[2][1] - M33[0][0]*M33[1][2]*M33[2][1] - M33[0][1]*M33[1][0]*M33[2][2] + M33[0][0]*M33[1][1]*M33[2][2];
 return res;
}

void CalIter_wRotation::InvM33(Double_t InvM33[3][3],Double_t M33[3][3])
{
 Double_t buf=0.;
 Double_t det=detM33(M33);
 if(det!=0.)
 {
  InvM33[0][0]= -(M33[1][2]*M33[2][1]) + M33[1][1]*M33[2][2];
  InvM33[0][1]= M33[0][2]*M33[2][1] - M33[0][1]*M33[2][2];
  InvM33[0][2]= -(M33[0][2]*M33[1][1]) + M33[0][1]*M33[1][2];
 
  InvM33[1][0]= M33[1][2]*M33[2][0] - M33[1][0]*M33[2][2];
  InvM33[1][1]= -(M33[0][2]*M33[2][0]) + M33[0][0]*M33[2][2];
  InvM33[1][2]= M33[0][2]*M33[1][0] - M33[0][0]*M33[1][2];
 
  InvM33[2][0]= -(M33[1][1]*M33[2][0]) + M33[1][0]*M33[2][1];
  InvM33[2][1]= M33[0][1]*M33[2][0] - M33[0][0]*M33[2][1];
  InvM33[2][2]= -(M33[0][1]*M33[1][0]) + M33[0][0]*M33[1][1];

  for(int i=0;i<3;i++)
  {
   for(int j=0;j<3;j++)
   {
    InvM33[i][j] /= det;
  }}
 }
}

void CalIter_wRotation::DotM33(Double_t tmpC[3][3],Double_t tmpA[3][3],Double_t tmpB[3][3])
{
 tmpC[0][0] = tmpA[0][0]*tmpB[0][0] + tmpA[0][1]*tmpB[1][0] + tmpA[0][2]*tmpB[2][0];
 tmpC[0][1] = tmpA[0][0]*tmpB[0][1] + tmpA[0][1]*tmpB[1][1] + tmpA[0][2]*tmpB[2][1];
 tmpC[0][2] = tmpA[0][0]*tmpB[0][2] + tmpA[0][1]*tmpB[1][2] + tmpA[0][2]*tmpB[2][2];
 
 tmpC[1][0] = tmpA[1][0]*tmpB[0][0] + tmpA[1][1]*tmpB[1][0] + tmpA[1][2]*tmpB[2][0];
 tmpC[1][1] = tmpA[1][0]*tmpB[0][1] + tmpA[1][1]*tmpB[1][1] + tmpA[1][2]*tmpB[2][1];
 tmpC[1][2] = tmpA[1][0]*tmpB[0][2] + tmpA[1][1]*tmpB[1][2] + tmpA[1][2]*tmpB[2][2];
 
 tmpC[2][0] = tmpA[2][0]*tmpB[0][0] + tmpA[2][1]*tmpB[1][0] + tmpA[2][2]*tmpB[2][0];
 tmpC[2][1] = tmpA[2][0]*tmpB[0][1] + tmpA[2][1]*tmpB[1][1] + tmpA[2][2]*tmpB[2][1];
 tmpC[2][2] = tmpA[2][0]*tmpB[0][2] + tmpA[2][1]*tmpB[1][2] + tmpA[2][2]*tmpB[2][2];
}

void CalIter_wRotation::AddM33(Double_t tmpC[3][3],Double_t tmpA[3][3],Double_t tmpB[3][3],Double_t fac)
{  
 for(int i=0;i<3;i++)
 {for(int j=0;j<3;j++)
  {
   tmpC[i][j] = tmpA[i][j] + fac * tmpB[i][j];
 }}
}

void CalIter_wRotation::setzeroM33(Double_t tmpC[3][3])
{
 for(int i=0;i<3;i++)
 {for(int j=0;j<3;j++)
  {
   tmpC[i][j] = 0.;
 }}
}

