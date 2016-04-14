#include "RM_xyz.C"
#include <iostream>
void testRM()
{
    //float a[3]={0.0014,0.000799989,0.000600002};
    float a[3]={0,0,0};
    float a1[3];
    float a2[3];
    vector<std::vector<float> > RM;
    RM_xyz(a,RM);
    getEulerAngle(RM,a1,a2);
}
