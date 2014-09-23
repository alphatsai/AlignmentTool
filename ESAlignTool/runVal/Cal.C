{
        double ESmF_O_R[3][3] = {
                {1, 0.000491103, -0.000500033},
                {-0.000489998, 0.999997, 0.0022},
                {0.000501112, -0.00219975, 0.999997}};
        double ESmR_O_R[3][3] = {
                {1, -0.000130381, 0.0002},
                {0.000130002, 0.999998, 0.00190003},
                {-0.000200247, -0.0019, 0.999998}};
        double ESpF_O_R[3][3] = {
                {1, 0.000759701, -0.00030002},
                {-0.000760002, 0.999999, -0.000999998},
                {0.00029926, 0.00100023, 0.999999}};
        double ESpR_O_R[3][3]={
                {1, 0.000520795, 0.000499935},
                {-0.000519997, 0.999999, -0.00160001},
                {-0.000500768, 0.00159975, 0.999999}
        };


        double ESmF_O_R_dA=TMath::ASin(ESmF_O_R[1][2]);
        double ESmF_O_R_dB=-(ESmF_O_R[0][2]/TMath::Cos(ESmF_O_R_dA));
        double ESmF_O_R_dG=-(ESmF_O_R[1][0]/TMath::Cos(ESmF_O_R_dA));
        cout<<"ESmF_O_R_dA "<<ESmF_O_R_dA<<endl;
        cout<<"ESmF_O_R_dB "<<ESmF_O_R_dB<<endl;
        cout<<"ESmF_O_R_dG "<<ESmF_O_R_dG<<endl;

        double ESmR_O_R_dA=TMath::ASin(ESmR_O_R[1][2]);
        double ESmR_O_R_dB=-(ESmR_O_R[0][2]/TMath::Cos(ESmR_O_R_dA));
        double ESmR_O_R_dG=-(ESmR_O_R[1][0]/TMath::Cos(ESmR_O_R_dA));
        cout<<"ESmR_O_R_dA "<<ESmR_O_R_dA<<endl;
        cout<<"ESmR_O_R_dB "<<ESmR_O_R_dB<<endl;
        cout<<"ESmR_O_R_dG "<<ESmR_O_R_dG<<endl;

        double ESpF_O_R_dA=TMath::ASin(ESpF_O_R[1][2]);
        double ESpF_O_R_dB=-(ESpF_O_R[0][2]/TMath::Cos(ESpF_O_R_dA));
        double ESpF_O_R_dG=-(ESpF_O_R[1][0]/TMath::Cos(ESpF_O_R_dA));
        cout<<"ESpF_O_R_dA "<<ESpF_O_R_dA<<endl;
        cout<<"ESpF_O_R_dB "<<ESpF_O_R_dB<<endl;
        cout<<"ESpF_O_R_dG "<<ESpF_O_R_dG<<endl;

        double ESpR_O_R_dA=TMath::ASin(ESpR_O_R[1][2]);
        double ESpR_O_R_dB=-(ESpR_O_R[0][2]/TMath::Cos(ESpR_O_R_dA));
        double ESpR_O_R_dG=-(ESpR_O_R[1][0]/TMath::Cos(ESpR_O_R_dA));
        cout<<"ESpR_O_R_dA "<<ESpR_O_R_dA<<endl;
        cout<<"ESpR_O_R_dB "<<ESpR_O_R_dB<<endl;
        cout<<"ESpR_O_R_dG "<<ESpR_O_R_dG<<endl;






}



