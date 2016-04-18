#include<cmath>
#include<vector>
#include "TMath.h"
using namespace std;

void printRM( std::vector<std::vector<double> > RM33 )
{
    printf(" | %11.8f  %11.8f  %11.8f |\n", RM33[0][0], RM33[0][1], RM33[0][2]);
    printf(" | %11.8f  %11.8f  %11.8f |\n", RM33[1][0], RM33[1][1], RM33[1][2]);
    printf(" | %11.8f  %11.8f  %11.8f |\n", RM33[2][0], RM33[2][1], RM33[2][2]);
}

// Caculate rotation matrix in ES
double RM_xyz( double Alpha, double Beta, double Gamma, std::vector<std::vector<double> > &RM33, bool printInfo=true )
{
    // Init
    std::vector<double> row1;
    std::vector<double> row2;
    std::vector<double> row3;

    //Caculation
    double R11 = cos(Beta)*cos(Gamma)-sin(Alpha)*sin(Beta)*sin(Gamma);
    double R12 = cos(Beta)*sin(Gamma) + sin(Alpha)*sin(Beta)*cos(Gamma);
    double R13 = -cos(Alpha)*sin(Beta);
    double R21 = -cos(Alpha)*sin(Gamma);
    double R22 = cos(Alpha)*cos(Gamma);
    double R23 = sin(Alpha);
    double R31 = sin(Beta)*cos(Gamma)+sin(Alpha)*cos(Beta)*sin(Gamma);
    double R32 = sin(Beta)*sin(Gamma)-sin(Alpha)*cos(Beta)*cos(Gamma);
    double R33 = cos(Alpha)*cos(Beta);
    double det = ( R11*R22*R33 + R12*R23*R31 + R13*R21*R32 ) - ( R11*R23*R32 + R12*R21*R33 + R13*R22*R31 );
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
double RM_xyz( double angle[3], std::vector<std::vector<double> > &RM33, bool printInfo=true )
{
    return RM_xyz( angle[0], angle[1], angle[2], RM33, printInfo );
}

// Caculate rotation matrix in Euler angles
double RM_euler( double Phi, double Theta, double Psi, std::vector<std::vector<double> > &RM33, bool printInfo=true )
{
    // Init
    std::vector<double> row1;
    std::vector<double> row2;
    std::vector<double> row3;

    //Caculation
    double R11 =    cos(Psi)*cos(Phi)-cos(Theta)*sin(Psi)*sin(Phi);
    double R12 =    cos(Psi)*sin(Phi)+cos(Theta)*sin(Psi)*cos(Phi);
    double R13 =    sin(Psi)*sin(Theta);
    double R21 = -1*sin(Psi)*cos(Phi)-cos(Theta)*cos(Psi)*sin(Phi);
    double R22 = -1*sin(Psi)*sin(Phi)+cos(Theta)*cos(Psi)*cos(Phi);
    double R23 =    cos(Psi)*sin(Theta); 
    double R31 =    sin(Theta)*sin(Phi); 
    double R32 = -1*sin(Theta)*cos(Phi); 
    double R33 =    cos(Theta); 
    double det = ( R11*R22*R33 + R12*R23*R31 + R13*R21*R32 ) - ( R11*R23*R32 + R12*R21*R33 + R13*R22*R31 );
    row1.push_back(R11); row1.push_back(R12); row1.push_back(R13);
    row2.push_back(R21); row2.push_back(R22); row2.push_back(R23);
    row3.push_back(R31); row3.push_back(R32); row3.push_back(R33);
    RM33.push_back(row1);
    RM33.push_back(row2);
    RM33.push_back(row3);

    // Print out detail
    if( printInfo )
    {
        printf("Input angle(phi, theta, psi) = (%11.8f,%11.8f,%11.8f)\n", Phi, Theta, Psi );
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
double RM_euler( double angle[3], std::vector<std::vector<double> > &RM33, bool printInfo=true )
{
    return RM_euler( angle[0], angle[1], angle[2], RM33, printInfo );
}

// Two solutions if fabs(RM33[e3][e3]) != 1., angle = spi, theta and phi
int getEulerAngle( std::vector<std::vector<double> > RM33, double angle1[3], double angle2[3], bool printInfo=true )
{
    int solCase=0; // -1: R31=-1, 1: R31=1, 0: R31!=1or-1 
    int e1=0;
    int e2=1;
    int e3=2;
    double theta1=0, theta2=0;
    double phi1=0, phi2=0;
    double psi1=0, psi2=0;

    if( fabs(RM33[e3][e3]) != 1. ) // R33 != 1, -1
    {
        theta1 = acos(RM33[e3][e3]);
        theta2 = -1*theta1;
        //theta2 = 2*TMath::Pi()-theta1;
        psi1 = atan2( RM33[e1][e3]/sin(theta1), RM33[e2][e3]/sin(theta1) );
        psi2 = atan2( RM33[e1][e3]/sin(theta2), RM33[e2][e3]/sin(theta2) );
        phi1 = TMath::Pi()-atan2( RM33[e3][e1]/sin(theta1), RM33[e3][e2]/sin(theta1) );
        phi2 = TMath::Pi()-atan2( RM33[e3][e1]/sin(theta2), RM33[e3][e2]/sin(theta2) );
        //phi1 = -atan2( RM33[e3][e1]/sin(theta1), RM33[e3][e2]/sin(theta1) );
        //phi2 = -atan2( RM33[e3][e1]/sin(theta2), RM33[e3][e2]/sin(theta2) );
    }
    else
    {
        if( RM33[e3][e3] == -1 )
        {   
            solCase=-1;
            theta1 = theta2 = TMath::Pi();
            psi1 = psi2 = phi1 + atan2( -1*RM33[e1][e2], -1*RM33[e2][e2] );
        }
        else
        {
            solCase=1;
            theta1 = theta2 = 0;
            psi1 = psi2 = -1*phi1 + atan2( RM33[e1][e2], RM33[e2][e2] );
        }    
    }
    angle1[0]=phi1;   angle2[0]=phi2;
    angle1[1]=theta1; angle2[1]=theta2;
    angle1[2]=psi1;   angle2[2]=psi2;
   
    // Print out detail 
    if( printInfo )
    {
        printf("Input matrix\n");
        printRM(RM33);
        printf("Output results with case %d ( 1/-1: one solution, 0: two solutions )\n", solCase );
        printf(" Sol 1. ( phi, theta, psi ) = ( %11.8f,%11.8f,%11.8f )\n", angle1[0], angle1[1], angle1[2]);
        printf(" Sol 2. ( phi, theta, psi ) = ( %11.8f,%11.8f,%11.8f )\n", angle2[0], angle2[1], angle2[2]);
    }

    return solCase;
}

//// Caculate rotation matrix in Euler angles
// Follow https://github.com/alphatsai/AlignmentTool/blob/master/ESAlignTool/macro/euler.pdf
//double RM_euler( double Psi, double Theta, double Phi, std::vector<std::vector<double> > &RM33, bool printInfo=true )
//{
//    // Init
//    std::vector<double> row1;
//    std::vector<double> row2;
//    std::vector<double> row3;
//
//    //Caculation
//    double R11 = cos(Theta)*cos(Phi);
//    double R12 = sin(Psi)*sin(Theta)*cos(Phi)-cos(Psi)*sin(Phi);
//    double R13 = cos(Psi)*sin(Theta)*cos(Phi)+sin(Psi)*sin(Phi);
//    double R21 = cos(Theta)*sin(Phi);
//    double R22 = sin(Psi)*sin(Theta)*sin(Phi)+cos(Psi)*cos(Phi);
//    double R23 = cos(Psi)*sin(Theta)*sin(Phi)-sin(Psi)*cos(Phi);
//    double R31 = -sin(Theta);
//    double R32 = sin(Psi)*cos(Theta);
//    double R33 = cos(Psi)*cos(Theta);
//    double det = ( R11*R22*R33 + R12*R23*R31 + R13*R21*R32 ) - ( R11*R23*R32 + R12*R21*R33 + R13*R22*R31 );
//    row1.push_back(R11); row1.push_back(R12); row1.push_back(R13);
//    row2.push_back(R21); row2.push_back(R22); row2.push_back(R23);
//    row3.push_back(R31); row3.push_back(R32); row3.push_back(R33);
//    RM33.push_back(row1);
//    RM33.push_back(row2);
//    RM33.push_back(row3);
//
//    // Print out detail
//    if( printInfo )
//    {
//        printf("Input angle(psi, theta, phi) = (%11.8f,%11.8f,%11.8f)\n", Psi, Theta, Phi );
//        printf("Rotation matrix RM =\n");
//        printRM(RM33);
//        // Debug
//        //printf(" | %11.8f  %11.8f  %11.8f |\n", R11, R12, R13);
//        //printf(" | %11.8f  %11.8f  %11.8f |\n", R21, R22, R23);
//        //printf(" | %11.8f  %11.8f  %11.8f |\n", R31, R32, R33);
//        printf("Determinant det(RM) = %11.8f\n", det);
//    }
//
//    return det;
//}
//double RM_euler( double angle[3], std::vector<std::vector<double> > &RM33, bool printInfo=true )
//{
//    return RM_euler( angle[0], angle[1], angle[2], RM33, printInfo );
//}
//
//// Two solutions if fabs(RM33[e3][e1]) != 1., angle = spi, theta and phi
//// Follow https://github.com/alphatsai/AlignmentTool/blob/master/ESAlignTool/macro/euler.pdf
//int getEulerAngle( std::vector<std::vector<double> > RM33, double angle1[3], double angle2[3], bool printInfo=true )
//{
//    int solCase=0; // -1: R31=-1, 1: R31=1, 0: R31!=1or-1 
//    int e1=0;
//    int e2=1;
//    int e3=2;
//    double theta1=0, theta2=0;
//    double phi1=0, phi2=0;
//    double psi1=0, psi2=0;
//
//    if( fabs(RM33[e3][e1]) != 1. ) // R31 != 1, -1
//    {
//        theta1 = -asin(RM33[e3][e1]);
//        theta2 = TMath::Pi()-theta1;
//        psi1 = atan2( RM33[e3][e2]/cos(theta1), RM33[e3][e3]/cos(theta1) );
//        psi2 = atan2( RM33[e3][e2]/cos(theta2), RM33[e3][e3]/cos(theta2) );
//        phi1 = atan2( RM33[e2][e1]/cos(theta1), RM33[e1][e1]/cos(theta1) );
//        phi2 = atan2( RM33[e2][e1]/cos(theta2), RM33[e1][e1]/cos(theta2) );
//    }
//    else
//    {
//        if( RM33[e3][e1] == -1 )
//        {   
//            solCase=-1;
//            theta1 = theta2 = TMath::Pi()/2;
//            psi1 = psi2 = phi1 + atan2( RM33[e1][e2], RM33[e1][e3] );
//        }
//        else
//        {
//            solCase=1;
//            theta1 = theta2 = -1*TMath::Pi()/2;
//            psi1 = psi2 = -1*phi1 + atan2( -1*RM33[e1][e2], -1*RM33[e1][e3] );
//        }    
//    }
//    angle1[0]=psi1;   angle2[0]=psi2;
//    angle1[1]=theta1; angle2[1]=theta2;
//    angle1[2]=phi1;   angle2[2]=phi2;
//   
//    // Print out detail 
//    if( printInfo )
//    {
//        printf("Input matrix\n");
//        printRM(RM33);
//        printf("Output results with case %d ( 1/-1: one solution, 0: two solutions )\n", solCase );
//        printf(" Sol 1. ( spi, theta, phi ) = ( %11.8f,%11.8f,%11.8f )\n", angle1[0], angle1[1], angle1[2]);
//        printf(" Sol 2. ( spi, theta, phi ) = ( %11.8f,%11.8f,%11.8f )\n", angle2[0], angle2[1], angle2[2]);
//    }
//
//    return solCase;
//}
