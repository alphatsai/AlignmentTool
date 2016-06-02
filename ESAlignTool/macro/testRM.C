#include "RM_xyz.C"
#include <iostream>
void testRM()
{
    ////// Conversion of ES and CLHEP  ------------------------
    // *1 Input (alpha, beta, gamma) from ES alignment tool representations.
    //double a[3]={0,0,0};
    //double a[3]={0.0014, 0.0005, 0};       // -rear
    double a[3]={ -0.0017, 0.0004, 0.0006};  // +rear

    // *2 Create and get rotation matrix.
    vector<std::vector<double> > RM;
    RM_ES(a,RM);

    // *3 Input rotation matrix get two solutions of (phi, theta, psi) for CLHEP.
    double a1[3];
    double a2[3];
    getAnglesCLHEP(RM,a1,a2,true,1000);

    // *4 Validate the two solutions can provide the same roation matrix.
    vector<std::vector<double> > RM1, RM2;
    RM_CLHEP(a1,RM1);
    RM_CLHEP(a2,RM2);

    //////// Only try rotation maxtrix of CLHEP  ------------------
    double a1[3]={ 0.00053855, 0, 0.00053855};
    double a2[3]={ 0.0003591,  0,  0.0003591};
    double a3[3]={-0.00013951, 0, -0.00013951};
    double a4[3]={-0.00049269, 0, -0.00049269};
    vector<std::vector<double> > RM1, RM2, RM3, RM4;
    RM_CLHEP(a1,RM1);
    RM_CLHEP(a2,RM2);
    RM_CLHEP(a3,RM3);
    RM_CLHEP(a4,RM4);
}
