#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include <time.h>

#define SIZE 800

using namespace std;
class Triangle{
    private:
        
    public:
        Triangle(){}
        vector<int>* trianglepts(){
            vector<double> dblpoints;
            vector<int>* points;
            points = new vector<int>;
            dblpoints.push_back(( (double) rand() ) / (RAND_MAX) );
            dblpoints.push_back(( (double) rand() ) / (RAND_MAX) );
            //cout << rand() << " --  " << rand() << endl;
            // cout << dblpoints[0] << "\n^^^^dblpts\n";
            int i = int(dblpoints[0] * SIZE);
            // cout << "i=" << i << endl;
            (*points).push_back(int(dblpoints[0] * double(SIZE)));
            (*points).push_back(int(dblpoints[1] * double(SIZE)));
            // (*points).push_back(5);
            // (*points).push_back(78);
            // cout << (*points)[0] << "\n^^^^pts\n";
            return points;
        }

        ~Triangle(){}
};
void bresenham_pos(int(*grid)[SIZE], int x1, int y1, int x2, int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;
    int j = y1;
    int m = dy - dx;

    for(int i = x1; i < x2; i++){
        grid[i][j] = 10;
        if(m >= 0){
            j += 1;
            m -= dx;
        }
        m += dy;
    }
};
void bresenham_neg(int(*grid)[SIZE], int x1, int y1, int x2, int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;
    int j = y1;
    int m = dy - dx;

    for(int i = x1; i < x2; i++){
        grid[i][j] = 10;
        if(m <= 0){
            j -= 1;
            m -= dx;
        }
        m += dy;
    }
};

void printVector(int index, vector<int> & vec){
    cout << vec[0] << " " << vec[1] << " " << index << endl; 
};

int main()
{
    const int size = SIZE;
    srand(time(NULL));
    ofstream drawing;
    Triangle t;
    // vector<int>* a;
    // vector<int>* b;
    // vector<int>* c;
    // vector<int>* d;
    vector<int>*points[4]; 
    for(int x = 0; x < 3; x++){
        points[x] = t.trianglepts();
    }
    points[3] = points[0];

    printVector(0, *points[0]);
    printVector(1, *points[1]);
    printVector(2, *points[2]);
    printVector(3, *points[3]);
    // cout << d << endl;
    // cout << a << endl;
    

    //points contains 3 points (vector<int> with x, y) that represent points of a triangle
    int(*grid)[SIZE] = new int[SIZE][SIZE];
    // bresenham_pos(grid, 0, 0, 800, 800);
    // bresenham_neg(grid, 0, 800, 800, 0);

    for(int t = 0; t < 3; t++){
        cout << (*points[t+1])[0] - (*points[t])[0] << endl;
        if(((*points[t+1])[0] - (*points[t])[0]) < 0){
            if(((*points[t+1])[1] - (*points[t])[1]) < 0){
                bresenham_neg(grid, (*points[t+1])[0], (*points[t+1])[1], (*points[t])[0], (*points[t])[1]);
            }
            else{
                bresenham_pos(grid, (*points[t+1])[0], (*points[t+1])[1], (*points[t])[0], (*points[t])[1]);
            } 
        }
        else{
            if(((*points[t+1])[1] - (*points[t])[1]) < 0){
                bresenham_neg(grid, (*points[t])[0], (*points[t])[1], (*points[t+1])[0], (*points[t+1])[1]);
            }
            else{
                bresenham_pos(grid, (*points[t])[0], (*points[t])[1], (*points[t+1])[0], (*points[t+1])[1]);
            }
        }
    }
    cout << "abt to open file";
    drawing.open("drawing.ppm");
    cout << "opened";
    drawing << "P3 800 800 255";
    for(int i=0; i< size; i++){
        drawing << "\n";
        for(int j=0; j < size; j++){
            // cout << "abt to check draw";
            if(grid[i][j] == 10){
                drawing << "0 0 0 ";
                // cout << "drawing pixel" << endl;
            }
            else{
                drawing << "255 255 255 ";
            }
        }
    }
    drawing.close();
    delete grid;
    for(int x = 0; x < 3; x++){
        delete points[x];
    }
    return 0;
}

