#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include <time.h>

#define SIZE 100

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
int main()
{
    const int size = SIZE;
    srand(time(NULL));
    ofstream drawing;
    Triangle t;
    vector<int>* a;
    vector<int>* b;
    vector<int>* c;
    vector<int>*(points)[4] = {a, b, c}; 
    for(int x = 0; x < 3; x++){
        points[x] = t.trianglepts();
    }
    points[3] = a;
    //points contains 3 points (vector<int> with x, y) that represent points of a triangle
    int(*grid)[SIZE] = new int[SIZE][SIZE];
    // bresenham_pos(grid, 0, 0, 800, 800);
    // bresenham_neg(grid, 0, 800, 800, 0);
    for(int t = 0; t < 3; t++){
        vector<int>* one = points[t];
        vector<int>* two = points[t+1];
        if(((*two)[0] - (*one)[0]) < 0){
            if(((*two)[1] - (*one)[1]) < 0){
                bresenham_neg(grid, (*two)[0], (*two)[1], (*one)[0], (*one)[1]);
            }
            else{
                bresenham_pos(grid, (*two)[0], (*two)[1], (*one)[0], (*one)[1]);
            } 
        }
        else{
            if(((*two)[1] - (*one)[1]) < 0){
                bresenham_neg(grid, (*one)[0], (*one)[1], (*two)[0], (*two)[1]);
            }
            else{
                bresenham_pos(grid, (*one)[0], (*one)[1], (*two)[0], (*two)[1]);
            }
        }
        delete one, two;
    }
    drawing.open("drawing.ppm");
    drawing << "P3 100 100 255";
    for(int i=0; i< size; i++){
        drawing << "\n";
        for(int j=0; j< size; j++){
            if(grid[i][j] == 10){
                drawing << "0 0 0 ";
            }
            else{
                drawing << "255 255 255 ";
            }
        }
    }
    drawing.close();
    delete points;
    delete grid;
    delete a, b, c;
    return 0;
}

