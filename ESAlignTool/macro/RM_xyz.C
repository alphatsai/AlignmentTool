#include<cmath>
#include<vector>
using namespace std;
void printRM( std::vector<std::vector<float> > RM33 )
{
    printf(" | %5.2f  %5.2f  %5.2f |\n", RM33[0][0], RM33[0][1], RM33[0][2]);
    printf(" | %5.2f  %5.2f  %5.2f |\n", RM33[1][0], RM33[1][1], RM33[1][2]);
    printf(" | %5.2f  %5.2f  %5.2f |\n", RM33[2][0], RM33[2][1], RM33[2][2]);
}
float RM_xyz( float angle[3], std::vector<std::vector<float> > &RM33, bool printInfo=true ){
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
        printf("Input angle(a,b,g) = (%5.2f,%5.2f,%5.2f)\n", Alpha, Beta, Gamma);
        printf("Rotation matrix RM =\n");
        printRM(RM33);
        // Debug
        //printf(" | %5.2f  %5.2f  %5.2f |\n", R11, R12, R13);
        //printf(" | %5.2f  %5.2f  %5.2f |\n", R21, R22, R23);
        //printf(" | %5.2f  %5.2f  %5.2f |\n", R31, R32, R33);
        printf("Determinant det(RM) = %5.2f\n", det);
    }

    return det;
}
