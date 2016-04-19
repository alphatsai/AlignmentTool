#include "RM_xyz.C"
#include <iostream>
void testRM()
{
    //// Conversion of ES and CLHEP  ------------------------
    // *1 Input (alpha, beta, gamma) from ES alignment tool representations.
    //double a[3]={0,0,0};
    double a[3]={0.0014, 0.0005, 0};

    // *2 Create and get rotation matrix.
    vector<std::vector<double> > RM;
    RM_ES(a,RM);

    // *3 Input rotation matrix get two solutions of (phi, theta, psi) for CLHEP.
    double a1[3];
    double a2[3];
    getAnglesCLHEP(RM,a1,a2);

    // *4 Valiate the two solutions can provide the same roation matrix.
    vector<std::vector<double> > RM1, RM2;
    RM_CLHEP(a1,RM1);
    RM_CLHEP(a2,RM2);

    ////// Only try rotation maxtrix of CLHEP  ------------------
    //double a3[3]={ -0 , -0.0001 , 0};
    //vector<std::vector<double> > RM3;
    //RM_CLHEP(a3,RM3);
}
