#include "RM_xyz.C"
#include <iostream>
void testRM()
{
    //double a[3]={0.0014,0.000799989,0.000600002};
    //double a[3]={0,0,0};
    double a[3]={0.0014, 0.0005, 0};
    //double a[3]={0.0001 , 0 , 0};
    double a1[3];
    double a2[3];
    vector<std::vector<double> > RM, RM1, RM2;
    RM_xyz(a,RM);
    getEulerAngle(RM,a1,a2);
    RM_euler(a1,RM1);
    RM_euler(a2,RM2);

    // Only try rotation maxtrix of CMSSW
    //double a[3]={-0.0001 , -0 , -0};
    //double a[3]={-0 , -0, -0.0001};
    //vector<std::vector<double> > RM;
    //RM_euler(a,RM);
}
