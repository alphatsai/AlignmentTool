#include<cmath>
#include<vector>
#include "TMath.h"
using namespace std;
void printRM( std::vector<std::vector<float> > RM33 )
{
    printf(" | %11.8f  %11.8f  %11.8f |\n", RM33[0][0], RM33[0][1], RM33[0][2]);
    printf(" | %11.8f  %11.8f  %11.8f |\n", RM33[1][0], RM33[1][1], RM33[1][2]);
    printf(" | %11.8f  %11.8f  %11.8f |\n", RM33[2][0], RM33[2][1], RM33[2][2]);
}
float RM_xyz( float angle[3], std::vector<std::vector<float> > &RM33, bool printInfo=true )
{
    // Init
    float Alpha=angle[0];
    float Beta=angle[1];
    float Gamma=angle[2];
    std::vector<float> row1;
    std::vector<float> row2;
    std::vector<float> row3;

    //Caculation
    float R11 = cos(Beta)*cos(Gamma)-sin(Alpha)*sin(Beta)*sin(Gamma);
    float R12 = cos(Beta)*sin(Gamma) + sin(Alpha)*sin(Beta)*cos(Gamma);
    float R13 = -cos(Alpha)*sin(Beta);
    float R21 = -cos(Alpha)*sin(Gamma);
    float R22 = cos(Alpha)*cos(Gamma);
    float R23 = sin(Alpha);
    float R31 = sin(Beta)*cos(Gamma)+sin(Alpha)*cos(Beta)*sin(Gamma);
    float R32 = sin(Beta)*sin(Gamma)-sin(Alpha)*cos(Beta)*cos(Gamma);
    float R33 = cos(Alpha)*cos(Beta);
    float det = ( R11*R22*R33 + R12*R23*R31 + R13*R21*R32 ) - ( R11*R23*R32 + R12*R21*R33 + R13*R22*R31 );
    row1.push_back(R11); row1.push_back(R12); row1.push_back(R13);
    row2.push_back(R21); row2.push_back(R22); row2.push_back(R23);
    row3.push_back(R31); row3.push_back(R32); row3.push_back(R33);
    RM33.push_back(row1);
    RM33.push_back(row2);
    RM33.push_back(row3);

    // Print out detail
    if( printInfo )
    {
        printf("Input angle(a,b,g) = (%11.8f,%11.8f,%11.8f)\n", Alpha, Beta, Gamma);
        printf("Rotation matrix RM =\n");
        printRM(RM33);
        // Debug
        //printf(" | %11.8f  %11.8f  %11.8f |\n", R11, R12, R13);
        //printf(" | %11.8f  %11.8f  %11.8f |\n", R21, R22, R23);
        //printf(" | %11.8f  %11.8f  %11.8f |\n", R31, R32, R33);
        printf("Determinant det(RM) = %11.8f\n", det);
    }

    return det;
}

// Two solutions if fabs(RM33[e3][e1]) != 1., angle = spi, theta and phi
int getEulerAngle( std::vector<std::vector<float> > RM33, float angle1[3], float angle2[3], bool printInfo=true )
{
    int solCase=0; // -1: R31=-1, 1: R31=1, 0: R31!=1or-1 
    int e1=0;
    int e2=1;
    int e3=2;
    float theta1=0, theta2=0;
    float phi1=0, phi2=0;
    float psi1=0, psi2=0;

    if( fabs(RM33[e3][e1]) != 1. ) // R31 != 1, -1
    {
        theta1 = -asin(RM33[e3][e1]);
        theta2 = TMath::Pi()-theta1;
        psi1 = atan2( RM33[e3][e2]/cos(theta1), RM33[e3][e3]/cos(theta1) );
        psi2 = atan2( RM33[e3][e2]/cos(theta2), RM33[e3][e3]/cos(theta2) );
        phi1 = atan2( RM33[e2][e1]/cos(theta1), RM33[e1][e1]/cos(theta1) );
        phi2 = atan2( RM33[e2][e1]/cos(theta2), RM33[e1][e1]/cos(theta2) );
    }
    else
    {
        if( RM33[e3][e1] == -1 )
        {   
            solCase=-1;
            theta1 = theta2 = TMath::Pi()/2;
            psi1 = psi2 = phi1 + atan2( RM33[e1][e2], RM33[e1][e3] );
        }
        else
        {
            solCase=1;
            theta1 = theta2 = -1*TMath::Pi()/2;
            psi1 = psi2 = -1*phi1 + atan2( -1*RM33[e1][e2], -1*RM33[e1][e3] );
        }    
    }
    angle1[0]=psi1;   angle2[0]=psi2;
    angle1[1]=theta1; angle2[1]=theta2;
    angle1[2]=phi1;   angle2[2]=phi2;
   
    // Print out detail 
    if( printInfo )
    {
        printf("Input matrix\n");
        printRM(RM33);
        printf("Output results with case %d ( 1/-1: one solution, 0: two solutions )\n", solCase );
        printf(" Sol 1. ( spi, theta, phi ) = ( %11.8f,%11.8f,%11.8f )\n", angle1[0], angle1[1], angle1[2]);
        printf(" Sol 2. ( spi, theta, phi ) = ( %11.8f,%11.8f,%11.8f )\n", angle2[0], angle2[1], angle2[2]);
    }

    return solCase;
}
