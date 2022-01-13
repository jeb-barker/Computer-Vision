//Jeb Barker
//Slightly Overdue - Apologies
#include<iostream>
#include<iomanip>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<string>
#include <time.h>
#include <math.h>
#include<list>

#define SIZE 100

using namespace std;
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
        double dist(Point other){
            return sqrt((pow((other.getx() - x), 2.0)+pow((other.gety() - y), 2.0)));
        }
        ~Point(){}
};
class Line{
    private:
        Point a, b;
        double dx;
        double dy;
        double m;
    public:
        //undefined is meant to be a representation of whether or not the line is vertical.
        bool undefined;
        Line(){}
        Line(double x1, double y1, double x2, double y2){
            a = Point(x1, y1);
            b = Point(x2, y2);
            dx = x2 - x1;
            if(dx == 0){
                undefined = true;
            }
            else{
                m = dy / dx;
            }
            dy = y2 - y1;
        }
        //Pythagorean distance formula (dx is the same thing as x2 - x1 and dy is the same as y2 - y1).
        double distance(){
            return sqrt((pow((dx), 2.0)+pow((dy), 2.0)));
        }
        double getdx(){
            return dx;
        }
        double getdy(){
            return dy;
        }
        Point getA(){
            return a;
        }
        Point getB(){
            return b;
        }
        vector<Line> perpendicular(Point base){
            vector<Line> ret;
            ret.push_back(Line(base.getx(), base.gety(), base.getx() + (-1 * dy), base.gety() + (dx)));
            ret.push_back(Line(base.getx(), base.gety(), base.getx() - (-1 * dy), base.gety() - (dx)));
            return ret;
        }
        //Finds intersection point between this Line and other Line.
        //Uses vector algebra to find intersection
        Point intersection(Line other){
            // This Line represented as a1x + b1y = c1
            double a1 = dy;
            double b1 = -dx;
            double c1 = a1*(a.getx()) + b1*(a.gety());

            // other Line represented as a2x + b2y = c2
            double a2 = other.getdy();
            double b2 = -1 * other.getdx();
            double c2 = a2*(other.getA().getx())+ b2*(other.getA().gety());
            double determinant = a1*b2 - a2*b1;
            if(determinant == 0){
                //return a Point (5.0, 5.0) if the line is colinear. Maybe consider using NaN?
                //There is a point to be made for either a large number like 5.0 or Nan.
                return Point(5.0, 5.0);
            }
            Point ret = Point((b2*c1 - b1*c2)/determinant, (a1*c2 - a2*c1)/determinant);
            return ret;
        }
        ~Line(){
        }
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
            Line d = Line(a.getx(), a.gety(), b.getx(), b.gety());
            return d.distance();
        }

        void drawSquare(Line l){
            int ax, ay, bx, by;
            ax = (int)((l.getA().getx() + (l.getdx()*2))*SIZE);
            ay = (int)((l.getA().gety() + (l.getdy()*2))*SIZE);
            bx = (int)((l.getB().getx() - (l.getdx()*2))*SIZE);
            by = (int)((l.getB().gety() - (l.getdy()*2))*SIZE);
            bresenham_pos(ax, ay, bx, by);
        }

        void drawCircle(int x1, int y1, int r, int code){
            int x = 0;
            int y = r;
            int ty = 3 - (2*r);
            //if(!(x1+x > SIZE || x1-x < 0 || x1 +y > SIZE || x1-y < 0 || y1+x > SIZE || y1-x < 0 || y1 +y > SIZE || y1-y < 0)){
            if(x1+x < SIZE && y1+y < SIZE && x1+x >= 0 && y1+y >= 0){grid[x1 + x][y1 + y] = code;}
            if(x1+x < SIZE && y1-y < SIZE && x1+x >= 0 && y1-y >= 0){grid[x1 + x][y1 - y] = code;}
            if(x1-x < SIZE && y1+y < SIZE && x1-x >= 0 && y1+y >= 0){grid[x1 - x][y1 + y] = code;}
            if(x1-x < SIZE && y1-y < SIZE && x1-x >= 0 && y1-y >= 0){grid[x1 - x][y1 - y] = code;}
            if(x1+y < SIZE && y1+x < SIZE && x1+y >= 0 && y1+x >= 0){grid[x1 + y][y1 + x] = code;}
            if(x1-y < SIZE && y1+x < SIZE && x1-y >= 0 && y1+x >= 0){grid[x1 - y][y1 + x] = code;}
            if(x1+y < SIZE && y1-x < SIZE && x1+y >= 0 && y1-x >= 0){grid[x1 + y][y1 - x] = code;}
            if(x1-y < SIZE && y1-x < SIZE && x1-y >= 0 && y1-x >= 0){grid[x1 - y][y1 - x] = code;}
            //}

            while(y >= x){
                //increment clockwise
                x++;
                if(ty>0){
                    y-=1;
                    ty = ty+(4 * (x - y)) + 10;
                }
                else{
                    ty = ty + (4 * x) + 6;
                }
                if(x1+x < SIZE && y1+y < SIZE && x1+x >= 0 && y1+y >= 0){grid[x1 + x][y1 + y] = code;}
                if(x1+x < SIZE && y1-y < SIZE && x1+x >= 0 && y1-y >= 0){grid[x1 + x][y1 - y] = code;}
                if(x1-x < SIZE && y1+y < SIZE && x1-x >= 0 && y1+y >= 0){grid[x1 - x][y1 + y] = code;}
                if(x1-x < SIZE && y1-y < SIZE && x1-x >= 0 && y1-y >= 0){grid[x1 - x][y1 - y] = code;}
                if(x1+y < SIZE && y1+x < SIZE && x1+y >= 0 && y1+x >= 0){grid[x1 + y][y1 + x] = code;}
                if(x1-y < SIZE && y1+x < SIZE && x1-y >= 0 && y1+x >= 0){grid[x1 - y][y1 + x] = code;}
                if(x1+y < SIZE && y1-x < SIZE && x1+y >= 0 && y1-x >= 0){grid[x1 + y][y1 - x] = code;}
                if(x1-y < SIZE && y1-x < SIZE && x1-y >= 0 && y1-x >= 0){grid[x1 - y][y1 - x] = code;}

            }

        }
        //Helper Method --> The Bresenham algorithm that works in all cases.
        void bresenham_pos(int x1, int y1, int x2, int y2){
            if(x2 - x1 < 0){
                int yt, xt;
                xt = x1;
                yt = y1;
                x1 = x2;
                y1 = y2;
                x2 = xt;
                y2 = yt;
            }
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
        //returns a Point with two random doubles as x,y values.
        Point generatept(){
            Point points;
            points = Point(( (double) rand() ) / (RAND_MAX), ( (double) rand() ) / (RAND_MAX));
            return points;
        }
        //generate n (where n is a positive integer) Points, and then add them to polygonPoints. returns the class member variable polygonPoints which is a vector<Point*>
        vector<Point>* generatePoints(int n){
            vector<Point>* r = new vector<Point>();
            for(int x = 0; x < n; x++){
                (*r).push_back(generatept());
            }
            return r;
        }

        void draw_grid(){
            ofstream drawing;
            drawing.open("output.ppm");
            drawing << "P3 "<< SIZE << " "<< SIZE << " 1";
            for(int i=0; i< SIZE; i++){
                drawing << "\n";
                for(int j=0; j < SIZE; j++){
                    // std::cout<< "abt to check draw";
                    if(grid[j][i] == 10){
                        drawing << "0 0 0 ";
                        // std::cout<< "drawing pixel" << endl;
                    }
                    else if(grid[j][i] == 15){
                        drawing << "1 0 0 ";
                    }
                    else{
                        drawing << "1 1 1 ";
                    }
                }
            }
            drawing.close();
        }

        void printVector(int index, vector<int> & vec){
            std::cout<< vec[0] << " " << vec[1] << " " << index << endl;
        };

        ~Canvas(){
            delete polygonPoints;
            delete [] grid;
        }
};

class RGB{
    private:
        int r, g, b;
    public:
        RGB(){
            r = 0;
            g = 0;
            b = 0;
        }
        RGB(int rr, int gg, int bb){
            r= rr;
            g = gg;
            b = bb;
        }
        double getr(){
            return r;
        }
        double getg(){
            return g;
        }
        double getb(){
            return b;
        }
        ~RGB(){}
};

void draw_grid(vector<vector<int>> &grid, int width, int height, int max, string fname){
    ofstream drawing;
    drawing.open(fname);
    drawing << "P3 "<< width << " "<< height << " " << max;
    for(int i=0; i < width; i++){
        drawing << "\n";
        for(int j=0; j < height; j++){
            drawing << grid[i][j] << " " << grid[i][j] << " " << grid[i][j] << " ";
        }
    }
    drawing.close();
};

vector<string> split(const string& str, const string& delim) //using this for ppm parsing ease.
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

int main(){
    cout << "hereeee";
    fstream txt;
    cout << "here" << endl;
   int height, width;
    vector<vector<int>> input = vector<vector<int>>();
    vector<vector<int>> gradient = vector<vector<int>>();
    vector<vector<int>> outputVector = vector<vector<int>>();
    
    txt.open("puppy.ppm", ios::in);
    string line;
    getline(txt, line);
   //line is the P3 W H line right now... do smth accordingly.
    vector<string> splitter = split(line, " ");
    width = std::stod(splitter[1]);
    height = std::stod(splitter[2]);
    int max = std::stod(splitter[3]);
    cout << width << " " << height << " " << max << endl;

    for(int x = 0; x < width; x++){
        input.push_back(vector<int>());
        gradient.push_back(vector<int>());
        outputVector.push_back(vector<int>());
        for(int y = 0; y < height; y++){
            input[x].push_back(0);
            gradient[x].push_back(0);
            outputVector[x].push_back(0);
            
        }
    }
    vector<int> tempList;
    while(getline(txt, line)){
        splitter = split(line, " ");
        for(int x = 0; x < (int)(splitter.size()/3); x++){
            //cout << a[3 * x] << " " << a[(3 * x) + 1] << " " << a[(3 * x) + 2] << endl;
            if(splitter[3 * x] != ""){
                int r = std::stod(splitter[3 * x]);
                int g = std::stod(splitter[(3 * x) + 1]);
                int b = std::stod(splitter[(3 * x) + 2]);
                int sum = r + g + b;
                sum = (int)(sum/3);
                tempList.push_back(sum);
            }
        }
    }
    int running_total = 0;
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            //cout << input[x].size() << endl;
            //cout << running_total << input[x][y] << endl;
            input[x][y] = tempList[running_total];
            //cout << "h" << endl;
            running_total+=1;
        }
    }//input contains a greyscale image.
    cout << "created greyscale" << endl;
    draw_grid(input, width, height, max, "imageg.ppm");
    int a,b,c,d,e,f,g,h,i, gx, gy, grad;
    for(int y = 1; y < height-1; y++){
        for(int x = 1; x < width-1; x++){
            // if((x == 0) || (x == (width-1)) || (y == 0) || (y == (height-1))){
            //     gradient[x][y] = 0;
            //     //input[x][y] = 0;
            //     outputVector[x][y] = 0;
            // }
            // else{
                
                a = input[x-1][y-1];
                b = input[x][y-1];
                c = input[x+1][y-1];
                d = input[x-1][y];
                e = input[x][y];
                f = input[x+1][y];
                g = input[x-1][y+1];
                h = input[x][y+1];
                i = input[x+1][y+1];
                
                gx = c - a - (2 * d) + (2*f) - g + i;
                gy = g - a - (2*b) + (2*h) + i - c;
                grad = (int)(sqrt((pow(gx, 2.0)+pow(gy, 2.0))));
                
                //gradient[x][y] = grad;
                if(gx > 60){
                    outputVector[x][y] = 1;
                }
                else{
                    outputVector[x][y] = 0;
                }
            //}
        }
    }//gradient contains the image with the sobal operator in place.
    draw_grid(outputVector, width, height, 1, "imagem.ppm");
   
    
    return 1;
}
