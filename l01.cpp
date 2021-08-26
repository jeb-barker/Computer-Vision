#include<iostream>
#include<fstream>
#include<stdlib.h>

using namespace std;
int * trianglepts(){
    double tpts[3];
    for(int x=0; x<3; x++){
        tpts[x] = (rand() % 10000)/10000;
    }
    int tps[3];
    for(int x=0; x<3; x++){
        tps[x] = (int)(tpts[x]);
    }
    return tps;
}
int main()
{
    const int size = 800;
    ofstream drawing;
    drawing.open("drawing.ppm");
    drawing << "P3 800 800 255";
    for(int i=0; i<size; i++){
        drawing << "\n";
        for(int j=0; j<size; j++){
            drawing << "0 0 0 ";
        }
    }
    drawing.close();
    int tps[] = trianglepts();
    return 0;
}

