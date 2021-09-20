//Jeb Barker 9/14/21
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include <time.h>
#include <math.h>

#define SIZE 800

using namespace std;
class Line{
    private:
        Point *a, *b;
        double dx;
        double dy;
        double m;
    public:
        Line(){}
        Line(double x1, double y1, double x2, double y2){
            a = new Point(x1, y1);
            b = new Point(x2, y2);
            dx = x2 - x1;
            dy = y2 - y1;
            m = dy / dx;
        }
        double distance(){
            return sqrt((pow((dx), 2.0)+pow((dy), 2.0)));
        }
        double getdx(){
            return dx;
        }
        double getdy(){
            return dy;
        }
        Point* getA(){
            return a; 
        }
        Point* getB(){
            return b;
        }
    //!must delete pointer after done! Finds intersection point between this Line and other Line.
    //Uses vector algebra to find intersection
    Point* intersection(Line other){
        // This Line represented as a1x + b1y = c1
        double a1 = dy;
        double b1 = -dx;
        double c1 = a1*((*a).getx()) + b1*((*a).gety());

        // other Line represented as a2x + b2y = c2
        double a2 = other.getdy();
        double b2 = -1 * other.getdx();
        double c2 = a2*((*other.getA()).getx())+ b2*((*other.getA()).gety());
        double determinant = a1*b2 - a2*b1;
        (b2*c1 - b1*c2)/determinant;
        (a1*c2 - a2*c1)/determinant;
        Point* ret = new Point((b2*c1 - b1*c2)/determinant, (a1*c2 - a2*c1)/determinant);
        return ret;
    }
    ~Line(){
        delete a, b;
    }
};
class Point{
    private:
        double x, y;
    public:
        Point(){
            x = 0.0;
            y = 0.0;
        }
        Point(double x1, double y1){
            x = x1;
            y = y1;
        }
        double getx(){
            return x;
        }
        double gety(){
            return y;
        }
        ~Point(){}
};
class Canvas{
    private:
        vector<Point>* polygonPoints;
        int(*grid)[SIZE];
        Point points;
    public:
        Canvas(){
            polygonPoints = new vector<Point>();
            grid = new int[SIZE][SIZE];
        }

        double distance(Point a, Point b){
            Line a = Line(a.getx(), a.gety(), b.getx(), b.gety());
        }

        //Helper Method --> The Bresenham algorithm that works in all cases.
        void bresenham_pos(int x1, int y1, int x2, int y2){
            int dx = x2 - x1;
            int dy = y2 - y1; 
            int inc = 1;
            if(dy < 0){
                inc = -1;
                dy = y1 - y2;
                if(dy > dx){
                    int yt, xt;
                    xt = x1;
                    yt = y1;
                    x1 = x2;
                    y1 = y2;
                    x2 = xt;
                    y2 = yt;
                }
            }
            if(dx > dy){
                int j = y1;
                int m = dy - dx;
                for(int i = x1; i < x2; i++){
                    if(i >= 0 && i < SIZE && j >= 0 && j < SIZE){grid[i][j] = 10;}
                    if(m >= 0){
                        j += inc;
                        m -= dx;
                    }
                    m += dy;
                }
            }
            else{
                dy = y2 - y1;
                dx = x2 - x1;
                if(dx < 0){
                    dx = x1 - x2;
                }
                int i = x1;
                int m = dx - dy;
                for(int j = y1; j < y2; j++){
                    if(i >= 0 && i < SIZE && j >= 0 && j < SIZE){grid[i][j] = 10;}
                    if(m >= 0){
                        i += inc;
                        m -= dy;
                    }
                    m += dx;
                }
            }
        };

        void addToPoints(Point a){
            (*polygonPoints).push_back(a);
        }

        Point generatept(){
            Point points;
            points = Point(( (double) rand() ) / (RAND_MAX), ( (double) rand() ) / (RAND_MAX));
            return points;
        }
        //generate n (where n is a positive integer) Points, returns the class member variable polygonPoints which is a vector<Point*>
        vector<Point>* generatePoints(int n){
            for(int x = 0; x < n; x++){

            }
        }

        void draw_grid(){
            ofstream drawing;
            drawing.open("drawing.ppm");
            drawing << "P3 "<<SIZE<< " "<<SIZE<< " 255";
            for(int i=0; i< SIZE; i++){
                drawing << "\n";
                for(int j=0; j < SIZE; j++){
                    // cout << "abt to check draw";
                    if(grid[j][i] == 10){
                        drawing << "0 0 0 ";
                        // cout << "drawing pixel" << endl;
                    }
                    else{
                        drawing << "255 255 255 ";
                    }
                }
            }
            drawing.close();
        }

        void printVector(int index, vector<int> & vec){
            cout << vec[0] << " " << vec[1] << " " << index << endl; 
        };

        ~Canvas(){
            delete polygonPoints;
            delete [] grid;
        }
};