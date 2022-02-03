//Jeb Barker
//1/13/21
#include<iostream>
#include<iomanip>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<string>
#include<string.h>
#include <time.h>
#include <math.h>
#include<list>

#define SIZE 100
#define THRESHOLD_1 150
#define THRESHOLD_2 200
#define PI 3.1415926535

typedef std::vector<std::vector<int>> vector2;
typedef std::vector<std::vector<double>> vector2double;

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
        int(*griddy)[SIZE];
        Point points;
    public:
        Canvas(){
            polygonPoints = new vector<Point>();
            griddy = new int[SIZE][SIZE];
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

        void drawCircle(int x1, int y1, int r, int code, vector2& grid){
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
                if(x1+x < SIZE && y1+y < SIZE && x1+x >= 0 && y1+y >= 0){grid[x1 + x][y1 + y] += code;}
                if(x1+x < SIZE && y1-y < SIZE && x1+x >= 0 && y1-y >= 0){grid[x1 + x][y1 - y] += code;}
                if(x1-x < SIZE && y1+y < SIZE && x1-x >= 0 && y1+y >= 0){grid[x1 - x][y1 + y] += code;}
                if(x1-x < SIZE && y1-y < SIZE && x1-x >= 0 && y1-y >= 0){grid[x1 - x][y1 - y] += code;}
                if(x1+y < SIZE && y1+x < SIZE && x1+y >= 0 && y1+x >= 0){grid[x1 + y][y1 + x] += code;}
                if(x1-y < SIZE && y1+x < SIZE && x1-y >= 0 && y1+x >= 0){grid[x1 - y][y1 + x] += code;}
                if(x1+y < SIZE && y1-x < SIZE && x1+y >= 0 && y1-x >= 0){grid[x1 + y][y1 - x] += code;}
                if(x1-y < SIZE && y1-x < SIZE && x1-y >= 0 && y1-x >= 0){grid[x1 - y][y1 - x] += code;}

            }

        }
        //Helper Method --> The Bresenham algorithm that works in all cases.
        void bresenham_pos(int x1, int y1, int x2, int y2, vector2& grid){
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
                    if(i >= 0 && i < SIZE && j >= 0 && j < SIZE){grid[i][j] += 1;}
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
                    if(i >= 0 && i < SIZE && j >= 0 && j < SIZE){grid[i][j] += 1;}
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
                    if(griddy[j][i] == 10){
                        drawing << "0 0 0 ";
                        // std::cout<< "drawing pixel" << endl;
                    }
                    else if(griddy[j][i] == 15){
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
            delete [] griddy;
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

void draw_grid(vector2 &grid, int width, int height, int max, string fname){
    ofstream drawing;
    drawing.open(fname);
    drawing << "P3 "<< width << " "<< height << " " << max;
    for(int j=0; j < height; j++){
        drawing << "\n";
        for(int i=0; i < width; i++){
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
        pos = str.find(delim, prev); //find the next delimiter
        if (pos == string::npos) pos = str.length(); 
        string token = str.substr(prev, pos-prev);//token is just the substring from the prev
        if (!token.empty()) tokens.push_back(token); //add token if it exists
        prev = pos + delim.length();//reset prev to the current (pos + the length of the delimiter)
    }
    while (pos < str.length() && prev < str.length()); //keep doing this while in bounds
    return tokens;
}

int areaFillRecursive(vector2 &gradient, int currentx, int currenty, int width, int height){
    if(currentx < 0 || currentx >= width || currenty < 0 || currenty >= height || gradient[currentx][currenty] == 0){
        return 0;
    }
    else if(gradient[currentx][currenty] == 2){
        gradient[currentx][currenty] = 3;
        areaFillRecursive(gradient, currentx-1, currenty-1, width, height);
        areaFillRecursive(gradient, currentx-1, currenty, width, height);
        areaFillRecursive(gradient, currentx-1, currenty+1, width, height);
        areaFillRecursive(gradient, currentx, currenty-1, width, height);
        areaFillRecursive(gradient, currentx, currenty+1, width, height);
        areaFillRecursive(gradient, currentx+1, currenty-1, width, height);
        areaFillRecursive(gradient, currentx+1, currenty, width, height);
        areaFillRecursive(gradient, currentx+1, currenty+1, width, height);
    }
    else if(gradient[currentx][currenty] == 1){
        gradient[currentx][currenty] = 3;
        areaFillRecursive(gradient, currentx-1, currenty-1, width, height);
        areaFillRecursive(gradient, currentx-1, currenty, width, height);
        areaFillRecursive(gradient, currentx-1, currenty+1, width, height);
        areaFillRecursive(gradient, currentx, currenty-1, width, height);
        areaFillRecursive(gradient, currentx, currenty+1, width, height);
        areaFillRecursive(gradient, currentx+1, currenty-1, width, height);
        areaFillRecursive(gradient, currentx+1, currenty, width, height);
        areaFillRecursive(gradient, currentx+1, currenty+1, width, height);
    }
    else{
        return 0;
    }
    return 0;
}

int areaFill(vector2 &gradient, int width, int height){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(gradient[x][y] == 2){
                areaFillRecursive(gradient, x, y, width, height);
            }
        }
    }
    return 0;
}

int part3(int t1, int t2, string fname){
    //cout << "hereeee";
    fstream txt;
    //cout << "here" << endl;
    int height, width, max;
    vector2 input = vector2(); //vector2 is a typedef of vector<vector<int>> defined at the top.
    vector2 gradient = vector2();
    vector2 finalVector = vector2();
    vector2 outputVector = vector2();
    vector2 angleVector = vector2(); 
    vector2double doubleAngleVector = vector2double();
    
    txt.open(fname, ios::in);
    string line;
    vector<string> splitter;
    
    vector<string> outerList;
    vector<int> tempList;

    while(getline(txt, line)){
        splitter = split(line, " ");
        for(int x = 0; x < splitter.size(); x++){
            //cout << a[3 * x] << " " << a[(3 * x) + 1] << " " << a[(3 * x) + 2] << endl;
            if(splitter[x] != ""){
                outerList.push_back(splitter[x]);
            }
        }
    }
    width = stod(outerList[1]);
    height = stod(outerList[2]);
    max = stod(outerList[3]);
    //std::cout << width << " " << height << " " << max << endl;

    for(int x = 4; x < outerList.size(); x+=3){
        int r = std::stod(outerList[x]);
        int g = std::stod(outerList[x+1]);
        int b = std::stod(outerList[x+2]);
        int sum = r + g + b;
        sum = (int)(sum/3);
        tempList.push_back(sum);
    }

    for(int x = 0; x < width; x++){
        input.push_back(vector<int>());
        gradient.push_back(vector<int>());
        outputVector.push_back(vector<int>());
        angleVector.push_back(vector<int>());
        finalVector.push_back(vector<int>());
        doubleAngleVector.push_back(vector<double>());
        for(int y = 0; y < height; y++){
            input[x].push_back(0);
            gradient[x].push_back(0);
            outputVector[x].push_back(0);
            angleVector[x].push_back(0);
            finalVector[x].push_back(0);
            doubleAngleVector[x].push_back(0.0);
        }
    }
    int running_total = 0;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            //cout << input[x].size() << endl;
            //cout << running_total << input[x][y] << endl;
            input[x][y] = tempList[running_total];
            //cout << "h" << endl;
            running_total+=1;
        }
    }//input contains a greyscale image.
    //std::cout << "created greyscale" << endl;
    draw_grid(input, width, height, max, "imageg.ppm");
    int a,b,c,d,e,f,g,h,i, gx, gy, grad;
    double angleTemp;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if((x == 0) || (x == (width-1)) || (y == 0) || (y == (height-1))){
                gradient[x][y] = 0;
                input[x][y] = 0;
                outputVector[x][y] = 0;
            }
            else{
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
                gy = g - a - (2*b) + (2*h) + i - c; //apply sobel operator matrix
                grad = (int)(sqrt((pow(gx, 2.0)+pow(gy, 2.0)))); //magnitude of g.
                gradient[x][y] = grad;

                angleTemp = atan2(gy, gx); //if you have errors maybe check the order of gx and gy in this function...
                doubleAngleVector[x][y] = angleTemp;
                angleTemp = angleTemp * 180 / PI;
                angleTemp = angleTemp / 45;
                angleTemp = round(angleTemp);
                angleTemp = 45.0 * angleTemp;
                if(angleTemp < -179.0){
                    angleTemp = 0.0;
                }
                else if(angleTemp < -134.0){
                    angleTemp = 45.0;
                }
                else if(angleTemp < -89.0){
                    angleTemp = 90.0;
                }
                else if(angleTemp < -44.0){
                    angleTemp = 135.0;
                }
                else if(angleTemp == -0){
                    angleTemp = 0;
                }
                //angleTemp now contains multiples of 45
                //cout << angleTemp << endl;
                angleVector[x][y] = (int)(angleTemp); // put the angle calculated into the corresponding grid box.
                //gradient[x][y] = grad;
                if(grad < t1){//Anything less than T1 is not an edge
                    outputVector[x][y] = 0;
                }
                else if(grad < t2){//anything above T1 and below T2 is a weak edge
                    outputVector[x][y] = 1;
                }
                else{
                    outputVector[x][y] = 2; //anything above both T1 and T2 is a strong edge
                }
            }
        }
    }//gradient contains the image with the sobal operator in place.
    //std::cout << "gradients computed." << endl;
    //apply areaFill to the vector2 gradient...

    //weird error possibly due to stack size... try using pointers to make sure it doesn't blow the stack.
    areaFill(outputVector, width, height);

    //std::cout << "areaFilled outputVector" << endl;
    //outputVector = gradient;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(outputVector[x][y] == 3){
                outputVector[x][y] = 1;
            }
            else if(outputVector[x][y] == 1 || outputVector[x][y] == 0){
                outputVector[x][y] = 0;
            }
        }
    }
    //at this point outputVector contains the Hysteresis grid, and angleVector contains the grid of angles used for non-max-suppression
    /*
    for NMS, round the angles in angleVector to nearest multiple of 45.
    Then subtract the values from the gradient on the line going at that angle.
    if the middle value (c) is the maximum, then c - a and c - b will be positive.
    if the middle value is negative, it's not an edge, if it's positive, it's an edge.
    */

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if((x == 0) || (x == (width-1)) || (y == 0) || (y == (height-1))){
                angleVector[x][y] = 0;
            }
            else{
                a = gradient[x-1][y-1];
                b = gradient[x][y-1];
                c = gradient[x+1][y-1];
                d = gradient[x-1][y];
                e = gradient[x][y];
                f = gradient[x+1][y];
                g = gradient[x-1][y+1];
                h = gradient[x][y+1];
                i = gradient[x+1][y+1];
                // a b c
                // d e f
                // g h i

                //  d < e > f
                if(angleVector[x][y] == 0 || angleVector[x][y] == 180){
                    if(d < e && e > f){
                        angleVector[x][y] = 1;
                    }
                    else{
                        angleVector[x][y] = 0;
                    }
                }
                //  g < e > c
                else if(angleVector[x][y] == 45){
                    if(g < e && e > c){
                        angleVector[x][y] = 1;
                    }
                    else{
                        angleVector[x][y] = 0;
                    }
                }
                //  b < e > h
                else if(angleVector[x][y] == 90){
                    if(b < e && e > h){
                        angleVector[x][y] = 1;
                    }
                    else{
                        angleVector[x][y] = 0;
                    }
                }
                //  a < e < i
                else if(angleVector[x][y] == 135){
                    if(a < e && e > i){
                        angleVector[x][y] = 1;
                    }
                    else{
                        angleVector[x][y] = 0;
                    }
                }
            }
            
            
        }
    }

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(angleVector[x][y] == outputVector[x][y] && angleVector[x][y]==1){
                finalVector[x][y] = 1;
            }
            else{
                finalVector[x][y] = 0;
            }
        }
    }

    draw_grid(outputVector, width, height, 1, "image1.ppm");
    draw_grid(angleVector, width, height, 1, "image2.ppm");
    draw_grid(finalVector, width, height, 1, "imagef.ppm");
    
    //Canny edge detection complete. Start identifying circles using algorithm described in class.
   return 1;
}

int circlePart1(int t1, int t2, string fname){
    fstream txt;
    int height, width, max;
    vector2 input = vector2(); //vector2 is a typedef of vector<vector<int>> defined at the top.
    vector2 gradient = vector2();
    vector2 finalVector = vector2();
    vector2 outputVector = vector2();
    vector2 angleVector = vector2(); 
    vector2 voteVector = vector2();
    vector2double doubleAngleVector = vector2double();
    
    txt.open(fname, ios::in);
    string line;
    vector<string> splitter;
    
    vector<string> outerList;
    vector<int> tempList;

    while(getline(txt, line)){
        splitter = split(line, " ");
        for(int x = 0; x < splitter.size(); x++){
            //cout << a[3 * x] << " " << a[(3 * x) + 1] << " " << a[(3 * x) + 2] << endl;
            if(splitter[x] != ""){
                outerList.push_back(splitter[x]);
            }
        }
    }
    width = stod(outerList[1]);
    height = stod(outerList[2]);
    max = stod(outerList[3]);
    //std::cout << width << " " << height << " " << max << endl;

    for(int x = 4; x < outerList.size(); x+=3){
        int r = std::stod(outerList[x]);
        int g = std::stod(outerList[x+1]);
        int b = std::stod(outerList[x+2]);
        int sum = r + g + b;
        sum = (int)(sum/3);
        tempList.push_back(sum);
    }

    for(int x = 0; x < width; x++){
        input.push_back(vector<int>());
        gradient.push_back(vector<int>());
        outputVector.push_back(vector<int>());
        angleVector.push_back(vector<int>());
        finalVector.push_back(vector<int>());
        voteVector.push_back(vector<int>());
        doubleAngleVector.push_back(vector<double>());
        for(int y = 0; y < height; y++){
            input[x].push_back(0);
            gradient[x].push_back(0);
            outputVector[x].push_back(0);
            angleVector[x].push_back(0);
            finalVector[x].push_back(0);
            doubleAngleVector[x].push_back(0.0);
            voteVector[x].push_back(0);
        }
    }
    int running_total = 0;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            //cout << input[x].size() << endl;
            //cout << running_total << input[x][y] << endl;
            input[x][y] = tempList[running_total];
            //cout << "h" << endl;
            running_total+=1;
        }
    }//input contains a greyscale image.
    //std::cout << "created greyscale" << endl;
    draw_grid(input, width, height, max, "imageg.ppm");
    int a,b,c,d,e,f,g,h,i, gx, gy, grad;
    double angleTemp;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if((x == 0) || (x == (width-1)) || (y == 0) || (y == (height-1))){
                gradient[x][y] = 0;
                input[x][y] = 0;
                outputVector[x][y] = 0;
            }
            else{
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
                gy = g - a - (2*b) + (2*h) + i - c; //apply sobel operator matrix
                grad = (int)(sqrt((pow(gx, 2.0)+pow(gy, 2.0)))); //magnitude of g.
                gradient[x][y] = grad;

                angleTemp = atan2(gy, gx); //if you have errors maybe check the order of gx and gy in this function...
                doubleAngleVector[x][y] = angleTemp;
                angleTemp = angleTemp * 180 / PI;
                angleTemp = angleTemp / 45;
                angleTemp = round(angleTemp);
                angleTemp = 45.0 * angleTemp;
                if(angleTemp < -179.0){
                    angleTemp = 0.0;
                }
                else if(angleTemp < -134.0){
                    angleTemp = 45.0;
                }
                else if(angleTemp < -89.0){
                    angleTemp = 90.0;
                }
                else if(angleTemp < -44.0){
                    angleTemp = 135.0;
                }
                else if(angleTemp == -0){
                    angleTemp = 0;
                }
                //angleTemp now contains multiples of 45
                //cout << angleTemp << endl;
                angleVector[x][y] = (int)(angleTemp); // put the angle calculated into the corresponding grid box.
                //gradient[x][y] = grad;
                if(grad < t1){//Anything less than T1 is not an edge
                    outputVector[x][y] = 0;
                }
                else if(grad < t2){//anything above T1 and below T2 is a weak edge
                    outputVector[x][y] = 1;
                }
                else{
                    outputVector[x][y] = 2; //anything above both T1 and T2 is a strong edge
                }
            }
        }
    }//gradient contains the image with the sobal operator in place.
    //std::cout << "gradients computed." << endl;
    //apply areaFill to the vector2 gradient...

    //weird error possibly due to stack size... try using pointers to make sure it doesn't blow the stack.
    areaFill(outputVector, width, height);

    //std::cout << "areaFilled outputVector" << endl;
    //outputVector = gradient;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(outputVector[x][y] == 3){
                outputVector[x][y] = 1;
            }
            else if(outputVector[x][y] == 1 || outputVector[x][y] == 0){
                outputVector[x][y] = 0;
            }
        }
    }
    //at this point outputVector contains the Hysteresis grid, and angleVector contains the grid of angles used for non-max-suppression
    /*
    for NMS, round the angles in angleVector to nearest multiple of 45.
    Then subtract the values from the gradient on the line going at that angle.
    if the middle value (c) is the maximum, then c - a and c - b will be positive.
    if the middle value is negative, it's not an edge, if it's positive, it's an edge.
    */

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if((x == 0) || (x == (width-1)) || (y == 0) || (y == (height-1))){
                angleVector[x][y] = 0;
            }
            else{
                a = gradient[x-1][y-1];
                b = gradient[x][y-1];
                c = gradient[x+1][y-1];
                d = gradient[x-1][y];
                e = gradient[x][y];
                f = gradient[x+1][y];
                g = gradient[x-1][y+1];
                h = gradient[x][y+1];
                i = gradient[x+1][y+1];
                // a b c
                // d e f
                // g h i

                //  d < e > f
                if(angleVector[x][y] == 0 || angleVector[x][y] == 180){
                    if(d < e && e > f){
                        angleVector[x][y] = 1;
                    }
                    else{
                        angleVector[x][y] = 0;
                    }
                }
                //  g < e > c
                else if(angleVector[x][y] == 45){
                    if(g < e && e > c){
                        angleVector[x][y] = 1;
                    }
                    else{
                        angleVector[x][y] = 0;
                    }
                }
                //  b < e > h
                else if(angleVector[x][y] == 90){
                    if(b < e && e > h){
                        angleVector[x][y] = 1;
                    }
                    else{
                        angleVector[x][y] = 0;
                    }
                }
                //  a < e < i
                else if(angleVector[x][y] == 135){
                    if(a < e && e > i){
                        angleVector[x][y] = 1;
                    }
                    else{
                        angleVector[x][y] = 0;
                    }
                }
            }
            
            
        }
    }

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(angleVector[x][y] == outputVector[x][y] && angleVector[x][y]==1){
                finalVector[x][y] = 1;
            }
            else{
                finalVector[x][y] = 0;
            }
        }
    }

    draw_grid(outputVector, width, height, 1, "image1.ppm");
    draw_grid(angleVector, width, height, 1, "image2.ppm");
    draw_grid(finalVector, width, height, 1, "imagef.ppm");
    
    //For every 1 in finalVector, bresenham in the voting vector2 in the direction of the Angle in doubleAngleVector.
    double ex;
    double why;
    Canvas circler;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(finalVector[x][y] == 1){
                ex = cos(doubleAngleVector[x][y]);
                why = sin(doubleAngleVector[x][y]);
                circler.bresenham_pos(x - (ex*100), y - (why*100), x + (ex*100), y + (why*100), voteVector);
            }
        }
    }
    
   return 1;
}

int main(int argc, char *argv[]){
    int t1 = THRESHOLD_1; // these are the default threshold values (150 and 200)
    int t2 = THRESHOLD_2;
    string fname = "image.ppm"; //image.ppm is the default file name.
    //defaults ^ will be overridden if the -L -l -H -h -f -F flags are present and properly utilized.
    //proper usage would be -l 100 to set the lower threshold to 100 or -H 150 to set the upper threshold to 150, for example.

    if(argc > 1){
        for(int i = 1; i < argc; i++){
            if(strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "-l") == 0){
                t1 = stod(argv[i+1]);
            }
            else if(strcmp(argv[i], "-H") == 0 || strcmp(argv[i], "-h") == 0){
                t2 = stod(argv[i+1]);
            }
            else if(strcmp(argv[i], "-F") == 0 || strcmp(argv[i], "-f") == 0){
                fname = argv[i+1];
            }
        }
    }
    
    //part1(); // single threshold
    //part2(); //double threshold
    part3(t1, t2, fname); //TRIPLE THRESHOLD... just kidding: NMS
    return 1;
}
