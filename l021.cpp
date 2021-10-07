//Jeb Barker 9/14/21
#include<iostream>
#include<iomanip>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<string>
#include <time.h>
#include <math.h>

#define SIZE 800

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

        void drawCircle(int x1, int y1, int r){
            int x = 0;
            int y = r;
            int ty = 3 - (2*r);
            //if(!(x1+x > SIZE || x1-x < 0 || x1 +y > SIZE || x1-y < 0 || y1+x > SIZE || y1-x < 0 || y1 +y > SIZE || y1-y < 0)){
            if(x1+x < SIZE && y1+y < SIZE && x1+x >= 0 && y1+y >= 0){grid[x1 + x][y1 + y] = 10;}
            if(x1+x < SIZE && y1-y < SIZE && x1+x >= 0 && y1-y >= 0){grid[x1 + x][y1 - y] = 10;}
            if(x1-x < SIZE && y1+y < SIZE && x1-x >= 0 && y1+y >= 0){grid[x1 - x][y1 + y] = 10;}
            if(x1-x < SIZE && y1-y < SIZE && x1-x >= 0 && y1-y >= 0){grid[x1 - x][y1 - y] = 10;}
            if(x1+y < SIZE && y1+x < SIZE && x1+y >= 0 && y1+x >= 0){grid[x1 + y][y1 + x] = 10;}
            if(x1-y < SIZE && y1+x < SIZE && x1-y >= 0 && y1+x >= 0){grid[x1 - y][y1 + x] = 10;}
            if(x1+y < SIZE && y1-x < SIZE && x1+y >= 0 && y1-x >= 0){grid[x1 + y][y1 - x] = 10;}
            if(x1-y < SIZE && y1-x < SIZE && x1-y >= 0 && y1-x >= 0){grid[x1 - y][y1 - x] = 10;}
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
                if(x1+x < SIZE && y1+y < SIZE && x1+x >= 0 && y1+y >= 0){grid[x1 + x][y1 + y] = 10;}
                if(x1+x < SIZE && y1-y < SIZE && x1+x >= 0 && y1-y >= 0){grid[x1 + x][y1 - y] = 10;}
                if(x1-x < SIZE && y1+y < SIZE && x1-x >= 0 && y1+y >= 0){grid[x1 - x][y1 + y] = 10;}
                if(x1-x < SIZE && y1-y < SIZE && x1-x >= 0 && y1-y >= 0){grid[x1 - x][y1 - y] = 10;}
                if(x1+y < SIZE && y1+x < SIZE && x1+y >= 0 && y1+x >= 0){grid[x1 + y][y1 + x] = 10;}
                if(x1-y < SIZE && y1+x < SIZE && x1-y >= 0 && y1+x >= 0){grid[x1 - y][y1 + x] = 10;}
                if(x1+y < SIZE && y1-x < SIZE && x1+y >= 0 && y1-x >= 0){grid[x1 + y][y1 - x] = 10;}
                if(x1-y < SIZE && y1-x < SIZE && x1-y >= 0 && y1-x >= 0){grid[x1 - y][y1 - x] = 10;}

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
            drawing.open("drawing.ppm");
            drawing << "P3 "<<SIZE<< " "<<SIZE<< " 255";
            for(int i=0; i< SIZE; i++){
                drawing << "\n";
                for(int j=0; j < SIZE; j++){
                    // std::cout<< "abt to check draw";
                    if(grid[j][i] == 10){
                        drawing << "0 0 0 ";
                        // std::cout<< "drawing pixel" << endl;
                    }
                    else{
                        drawing << "255 255 255 ";
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

vector<vector<Point>*> part1(){
    vector< vector<Line> >* lines = new vector< vector<Line> >();
    vector<vector<Point>*> points;
    points.push_back(new vector<Point>());
    points.push_back(new vector<Point>());
    points.push_back(new vector<Point>());
    points.push_back(new vector<Point>());
    Point testpt;
    bool valid = false;
    
    int intersections;
    int vertexIntersections;
    Canvas *t = new Canvas();
    while(!valid){
        delete lines;
        for (int x = 0; x < 4; x++){
            delete points[x];
        }
        lines = new vector< vector<Line> >();
        points = vector<vector<Point>*>();
        intersections = 0;
        vertexIntersections = 0;
        //The following groupings are all of the different combinations of three points
        //that can make up a Triangle (with an extra point left off for testing)
        points.push_back(new vector<Point>());
        (*points[0]).push_back((*t).generatept());
        (*points[0]).push_back((*t).generatept());
        (*points[0]).push_back((*t).generatept());
        testpt = (*t).generatept();
        (*points[0]).push_back((*points[0])[0]);

        points.push_back(new vector<Point>());
        (*points[1]).push_back((*points[0])[0]);
        (*points[1]).push_back((*points[0])[1]);
        (*points[1]).push_back(testpt);
        (*points[1]).push_back((*points[0])[0]);

        points.push_back(new vector<Point>());
        (*points[2]).push_back((*points[0])[0]);
        (*points[2]).push_back(testpt);
        (*points[2]).push_back((*points[0])[2]);
        (*points[2]).push_back((*points[0])[0]);

        points.push_back(new vector<Point>());
        (*points[3]).push_back(testpt);
        (*points[3]).push_back((*points[0])[1]);
        (*points[3]).push_back((*points[0])[2]);
        (*points[3]).push_back(testpt);

        points.push_back(new vector<Point>());
        (*points[4]).push_back(testpt);
        (*points[4]).push_back((*points[0])[1]);
        (*points[4]).push_back((*points[0])[2]);
        (*points[4]).push_back((*points[0])[0]);
        for(int y = 0; y < 4; y++){
            (*lines).push_back(vector<Line>());
            for(int x = 0; x < 3; x++){
                Line a = Line((*points[y])[x].getx(), (*points[y])[x].gety(), (*points[y])[x+1].getx(), (*points[y])[x+1].gety());
                (*lines)[y].push_back(a);
            }
        }

        Line testLine = Line(0.0, testpt.gety(), testpt.getx(), testpt.gety());
        bool keep_going = true;
        for(int y = 0; y < 4; y++){
            intersections = 0;
            vertexIntersections = 0;
            if(keep_going){
                for(int x = 0; x < 3; x++){
                    Point intersection = testLine.intersection((*lines)[y][x]);
                    if(intersection.getx() == 5.0 && intersection.gety() == 5.0){
                        //VERY VERY VERY rare case where the lines that are being tested are colinear
                        //the intersection method in the Line class returns a Point of (5.0, 5.0) when
                        //the two lines are colinear --> 5.0 is well outside the range of [0, 1).
                        std::cout<< "COLINEAR!!!" << endl;
                    }
                    //boudaries for where an intersection is allowed to be (based on end points of lines)
                    if((*lines)[y][x].getA().getx() > intersection.getx()-0.00001 && (*lines)[y][x].getA().getx() < intersection.getx()+0.00001){
                        vertexIntersections++;
                    }
                    else if(( (*lines)[y][x].getA().getx() > intersection.getx() && 
                        (*lines)[y][x].getB().getx() > intersection.getx()) || 
                    ( (*lines)[y][x].getA().getx() < intersection.getx() && 
                        (*lines)[y][x].getB().getx() < intersection.getx() ) || 
                        intersection.getx() < 0 || intersection.getx() > testpt.getx()){
                    }
                    else{
                        intersections++;
                    }
                    //a vertex intersection is handled slightly differently from a normal one-->
                    //it counts as 2 intersections in practice, so they need to be divided by 2 to get the real value.
                }
            }
            //TODO: Remove the random couts (not necessary, just might be nice).
            std::cout<< "vertInts: " << vertexIntersections << endl;
            intersections = intersections + (int)(vertexIntersections / 2);
            std::cout<< "ints: " << intersections << endl;
            //if number of intersections is even, it is outside the polygon
            if(intersections % 2 == 0){
                valid = true;
            }
            //if number of intersections is odd, try again with a newly generated set of points.
            else{
                std::cout<< "try again --" << endl;
                keep_going = false;
                valid = false;
            }
        }
    }
    //cleanup for part1().
    //@Myself --> remember to check for any memory leaks/lazy programming...
    delete lines;
    delete t;
    return points;
}

void part2(vector<Point> pts){
    vector<vector<Line>> lines_to_constr = vector<vector<Line>>();
    double ax, bx, cx, dx, ay, by, cy, dy;
    ax = pts[0].getx();
    ay = pts[0].gety();
    bx = pts[1].getx();
    by = pts[1].gety();
    cx = pts[2].getx();
    cy = pts[2].gety();
    dx = pts[3].getx();
    dy = pts[3].gety();
    for(int i = 0; i < 3; i++){
        lines_to_constr.push_back(vector<Line>());
    }
    lines_to_constr[0].push_back(Line(ax, ay, bx, by));
    lines_to_constr[0].push_back(Line(dx, dy, cx, cy));

    lines_to_constr[1].push_back(Line(ax, ay, cx, cy));
    lines_to_constr[1].push_back(Line(bx, by, dx, dy));

    lines_to_constr[2].push_back(Line(ax, ay, dx, dy));
    lines_to_constr[2].push_back(Line(bx, by, cx, cy));

    for(int iter = 0; iter < 3; iter++){
        //TODO: drop perpendiculars and connect.
  Point e = Point(lines_to_constr[iter][0].perpendicular(lines_to_constr[iter][1].getA())[1].getB().getx(), lines_to_constr[iter][0].perpendicular(lines_to_constr[iter][1].getA())[1].getB().gety());
        lines_to_constr[iter].push_back(Line(lines_to_constr[iter][1].getB().getx(), lines_to_constr[iter][1].getB().gety(), e.getx(), e.gety()));
        
        e = Point(lines_to_constr[iter][0].perpendicular(lines_to_constr[iter][1].getB())[0].getB().getx(), lines_to_constr[iter][0].perpendicular(lines_to_constr[iter][1].getB())[0].getB().gety());
        lines_to_constr[iter].push_back(Line(lines_to_constr[iter][1].getA().getx(), lines_to_constr[iter][1].getA().gety(), e.getx(), e.gety()));
        
        e = Point(lines_to_constr[iter][1].perpendicular(lines_to_constr[iter][0].getA())[0].getB().getx(), lines_to_constr[iter][1].perpendicular(lines_to_constr[iter][0].getA())[0].getB().gety());
        lines_to_constr[iter].push_back(Line(lines_to_constr[iter][0].getB().getx(), lines_to_constr[iter][0].getB().gety(), e.getx(), e.gety()));
        
        e = Point(lines_to_constr[iter][1].perpendicular(lines_to_constr[iter][0].getB())[1].getB().getx(), lines_to_constr[iter][1].perpendicular(lines_to_constr[iter][0].getB())[1].getB().gety());
        lines_to_constr[iter].push_back(Line(lines_to_constr[iter][0].getA().getx(), lines_to_constr[iter][0].getA().gety(), e.getx(), e.gety()));
        Canvas p;
        p.drawCircle((int)(ax*SIZE), (int)(ay*SIZE), 2);
        p.drawCircle((int)(bx*SIZE), (int)(by*SIZE), 2);
        p.drawCircle((int)(cx*SIZE), (int)(cy*SIZE), 2);
        p.drawCircle((int)(dx*SIZE), (int)(dy*SIZE), 2);
        for(int i = 2; i < 6; i++){
            p.bresenham_pos((int)(lines_to_constr[iter][i].getA().getx()*SIZE), (int)(lines_to_constr[iter][i].getA().gety()*SIZE), (int)(lines_to_constr[iter][i].getB().getx()*SIZE), (int)(lines_to_constr[iter][i].getB().gety()*SIZE));
            cout << (int)(lines_to_constr[iter][i].getA().getx()*SIZE) << endl;
            cout << (int)(lines_to_constr[iter][i].getA().gety()*SIZE) << endl;
            cout << (int)(lines_to_constr[iter][i].getB().getx()*SIZE) << endl;
            cout << (int)(lines_to_constr[iter][i].getB().gety()*SIZE) << endl << endl;
        }
        p.draw_grid();
    }
}

int main()
{
    //seed the random number generator based on current time (makes it more random.)
    srand(time(NULL));
    
    std::cout<< fixed;
    std::cout<< setprecision(17);
    //ofstream txt will be used as a file outstream into a file
    //points.txt contains the four randomly generated points that 
    //follow the constraints laid out by part 1.
    ofstream txt;
    txt.open("points.txt");
    txt<< fixed;
    txt<< setprecision(17);

    vector<vector<Point>*> points;
    points = part1();
    for(int x = 0; x < 4; x++){
        txt << "(" << (*points[4])[x].getx() << ", " << (*points[4])[x].gety() << "), " << endl;
    }
    txt.close();
    for (int x = 0; x < 4; x++){
        delete points[x];
    }

    fstream pointFile;
    pointFile.open("points.txt", ios::in);
    string line;
    vector<Point> point;
    //read in the points from points.txt
    while(getline(pointFile, line)){
        point.push_back(Point(std::stod(line.substr(1, 19)), std::stod(line.substr(22, 19))));
    }
    //std::cout << point[0].getx() << point[0].gety();
    part2(point);

    //cleanup
    pointFile.close();
    return 0;
}
