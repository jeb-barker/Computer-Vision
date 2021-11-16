//Jeb Barker 
//Due 11/12/21
#include<iostream>
#include<iomanip>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<string>
#include<time.h>
#include<math.h>
#include<list>
#include<chrono>
#include<bits/stdc++.h>
#include<algorithm>
#include<sstream>

#define SIZE 800
#define NUM_POINTS 1000000

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
        double& getx(){
            return x;
        }
        double& gety(){
            return y;
        }
        double dist(Point other){
            return sqrt((pow((other.getx() - x), 2.0)+pow((other.gety() - y), 2.0)));
        }
        string toString(){
            std::stringstream a;
            a << fixed << setprecision(23) << "(" << (x) << ", " << (y) << ")";
            return a.str();
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
            drawing.open("points.ppm");
            drawing << "P3 "<<SIZE<< " "<<SIZE<< " 255";
            for(int i=0; i< SIZE; i++){
                drawing << "\n";
                for(int j=0; j < SIZE; j++){
                    // std::cout<< "abt to check draw";
                    if(grid[j][i] == 10){
                        drawing << "0 0 0 ";
                        // std::cout<< "drawing pixel" << endl;
                    }
                    else if(grid[j][i] == 15){
                        drawing << "255 0 0 ";
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

class DistPoint{
    private: 
        Point a;
        Point b;
        double distance;
    public:
        DistPoint(){
            Line d = Line(a.getx(), a.gety(), b.getx(), b.gety());
            distance = d.distance();
        }
        DistPoint(Point one, Point two){
            Line d = Line(one.getx(), one.gety(), two.getx(), two.gety());
            a = one;
            b = two;
            distance = d.distance();
        }
        double getDistance(){
            return distance;
        }
        Point& getA(){
            return a;
        }
        Point& getB(){
            return b;
        }
};

int generate_points(){
    Canvas c;
    srand(time(NULL));
    list<Point> points;
    ofstream txt;
    //open a file stream into points.txt --> precision is set to 23 decimal places.
    txt.open("points.txt");
    txt<< fixed;
    txt<< setprecision(23);
    //generate points and add to front of list NUM_POINTS is defined at the top of the file (initially should be 60)
    for(int i = 0; i < NUM_POINTS-1; i++){
        Point a = c.generatept();
        points.push_front(a);
        txt << a.getx() << "  " << a.gety() << endl;
    }
    return 1;
}
string part1(){
    Canvas c;
    srand(time(NULL));
    list<Point> points;
    ofstream txt;
    //open a file stream into points.txt --> precision is set to 23 decimal places.
    txt.open("points.txt");
    txt<< fixed;
    txt<< setprecision(23);
    //generate points and add to front of list NUM_POINTS is defined at the top of the file (initially should be 60)
    for(int i = 0; i < NUM_POINTS-1; i++){
        Point a = c.generatept();
        points.push_front(a);
        txt << a.getx() << "  " << a.gety() << endl;
    }
    Point p = c.generatept();
    points.push_front(p);
    txt << p.getx() << "  " << p.gety();
    Point minPoint1;
    Point minPoint2;
    double minDist = 100.0;
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    for(std::list<Point>::iterator iter = points.begin(); iter != points.end(); ++iter){
        for(std::list<Point>::iterator iter2 = points.begin(); iter2 != iter; iter2++){
            if((*iter).dist((*iter2)) < minDist){
                minDist = (*iter).dist((*iter2));
                minPoint1 = (*iter);
                minPoint2 = (*iter2);
            }
        }
    }
    std::chrono::milliseconds ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    //cout << "part 1 duration: " << ms2.count() - ms.count() << endl;
    //cout << "minDist:" << minDist << endl;

    //draw 3 pixel circles around each point and then draw to points.ppm.
    for(std::list<Point>::iterator iter = points.begin(); iter != points.end(); ++iter){
        c.drawCircle((int)((*iter).getx()*SIZE), (int)((*iter).gety()*SIZE), 3, 10);
    }
    //draw circles around the closest 2 points (code 15 is for red circles)
    c.drawCircle((int)(minPoint1.getx()*SIZE), (int)(minPoint1.gety()*SIZE), 3, 15);
    c.drawCircle((int)(minPoint2.getx()*SIZE), (int)(minPoint2.gety()*SIZE), 3, 15);
    c.drawCircle((int)(minPoint1.getx()*SIZE), (int)(minPoint1.gety()*SIZE), 2, 15);
    c.drawCircle((int)(minPoint2.getx()*SIZE), (int)(minPoint2.gety()*SIZE), 2, 15);
    c.draw_grid();
    txt.close();
    std::stringstream a;
    a << fixed << setprecision(23) << "part 1 duration: " << (ms2.count() - ms.count()) << "\nMinimum Distance: " << (minDist) << "\nPoints: " << minPoint1.toString() << " and " << minPoint2.toString() << "\n";
    //cout << a.str() << endl;
    return a.str();
}

bool compare(Point a, Point b){
    return (a.getx() < b.getx());
}
bool compareY(Point a, Point b){
    return (a.gety() < b.gety());
}

DistPoint recursion_helper(int start_index, int end_index, vector<Point> &points){
    //cout << start_index << " to " << end_index << endl;
    if(end_index - start_index <= 0){
        return DistPoint(Point(0.0, 0.0), Point(100.0,100.0));
    }
    if(end_index - start_index == 1){
        //cout << points[start_index].toString() << " and " << points[end_index].toString() <<endl;
        return DistPoint(points[start_index], points[end_index]);
    }
    if(end_index - start_index == 2){
        //DistPoint a, b, c;
        //DistPoint ret;
        //DistPoint a = DistPoint(Point(points[start_index].getx(), points[start_index].gety()), points[start_index+1]);
        DistPoint ret = DistPoint(points[start_index+1], points[end_index]);
        // DistPoint c = DistPoint(points[start_index], points[end_index]);
        //cout << points[start_index].toString() << " and " << points[end_index - 1].toString() << " and " << points[end_index].toString() <<endl;


        // double min_dist = a.getDistance();

        // if(b.getDistance() < min_dist){
        //     min_dist = b.getDistance();
        //     ret = DistPoint(b.getA(), b.getB());
        // }
        // if(c.getDistance() < min_dist){
        //     min_dist = c.getDistance();
        //     ret = DistPoint(c.getA(), c.getB());
        // }
        for(int left = start_index; left <= end_index; left++){
            for(int right = start_index; right < left; right++){
                if(points[left].dist(points[right]) < ret.getDistance()){
                    ret = DistPoint(points[left], points[right]);
                }
            }
        }
        //cout << a.getDistance() << " -- " << b.getDistance() << " -- " << c.getDistance() << " -- \n" << ret.getDistance() << endl;
        return ret;
    }
    DistPoint left, right;
    int middle = ((start_index+end_index)/2);
    left = recursion_helper(start_index, middle, points);

    //left2 = recursion_helper(start_index, (int)((start_index+end_index)/2)+1, points);
    right = recursion_helper(middle, end_index, points);

    //right2 = recursion_helper((int)((start_index+end_index)/2)+2, end_index, points);
    //cout << left.getDistance() << " --- " << right.getDistance() << "\n";
    DistPoint d = (left.getDistance() < right.getDistance()) ? left : right;
    DistPoint maxD = d;
    //int middle = (int)((points.size()-1)/2);
    // int left_index = middle-1;
    //  int right_index = left_index + 2;
    // while(points[left_index].getx() >= points[middle].getx() - d.getDistance() && left_index >= 0){
    //     left_index--;
    // }
    // while(points[right_index].getx() <= points[middle].getx() + d.getDistance() && right_index < NUM_POINTS){
    //     right_index++;
    // }

    for(int left = middle; points[left].getx() > (points[middle].getx() - d.getDistance()) && left >= start_index; left--){
        for(int right = middle+1; points[right].getx() < (points[middle].getx() + d.getDistance()) && right <= end_index; right++){
            double tempD = points[left].dist(points[right]);
            if(tempD < maxD.getDistance()){
                //cout << "LESS THAN" << endl;
                maxD = DistPoint(points[left], points[right]);
            }
//             if(tempD == maxD.getDistance()){
//                 cout << "uh oh: " << points[left].toString() << " " << points[right].toString() << " middle: " << middle << " " << left<<" "<<right << endl;
//             }
        }
    }

    return maxD;
}

string part2_final(){
    Canvas c;
    srand(time(NULL));
    vector<Point> points;
    fstream txt;
    //open a file stream into points.txt --> precision is set to 23 decimal places.
    txt.open("points.txt", ios::in);
    string line;
    int num_points = 0;
    //generate points and add to front of list NUM_POINTS is defined at the top of the file (initially should be 60)
    while(getline(txt, line)){
        points.push_back(Point(std::stod(line.substr(0, 25)), std::stod(line.substr(27, 25))));
        num_points++;
    }
    //get starting time
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    
    std::sort(points.begin(), points.end(), compare);
    DistPoint d = recursion_helper(0, points.size() - 1, points);
    DistPoint maxD = d;
    //cout << d.getDistance() <<  d.getA().toString() << endl;

    int middle = (int)((points.size()-1)/2);
    for(int left = middle-1; points[left].getx() > (points[middle].getx() - d.getDistance()); left--){
        for(int right = middle+1; points[right].getx() < (points[middle].getx() + d.getDistance()); right++){
            double tempD = points[left].dist(points[right]);
            if(tempD < maxD.getDistance()){
                //cout << "LESS THAN" << endl;
                maxD = DistPoint(points[left], points[right]);
            }
        }
    }
    //cout << "found d as: " << maxD.getDistance() << endl;
    //cout << "points of d: " << d.getA() << "   " << d.getB() << endl;

    //cout << "found Minimum Distance as: " << maxD.getDistance() << "\nPoints: " << maxD.getA() << "   " << maxD.getB() << endl;
    //get ending time.
    std::chrono::milliseconds ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    
    
    std::stringstream a;
    a << fixed << setprecision(23) << "part 2 duration: " << (ms2.count() - ms.count()) << "\nMinimum Distance: " << (maxD.getDistance()) << "\nPoints: " << (maxD.getA()).toString() << " and " << (maxD.getB()).toString() << "\n";
    //cout << a.str() << endl;
    txt.close();
    return a.str();
}

DistPoint recursion_helper3(int start_index, int end_index, vector<Point> &points){
    if(end_index - start_index <= 0){
        return DistPoint(Point(0.0, 0.0), Point(100.0,100.0));
    }
    if(end_index - start_index == 1){
        return DistPoint(points[start_index], points[end_index]);
    }
    if(end_index - start_index == 2){
        DistPoint ret = DistPoint(points[start_index+1], points[end_index]);
        for(int left = start_index; left <= end_index; left++){
            for(int right = start_index; right < left; right++){
                if(points[left].dist(points[right]) < ret.getDistance()){
                    ret = DistPoint(points[left], points[right]);
                }
            }
        }
        return ret;
    }
    DistPoint left, right;
    int middle = ((start_index+end_index)/2);
    left = recursion_helper(start_index, middle, points);
    right = recursion_helper(middle, end_index, points);
    
    DistPoint d = (left.getDistance() < right.getDistance()) ? left : right;
    DistPoint maxD = d;
    int left_index = middle;
    int right_index = middle+1;
    vector<Point> strippoints;
    while(points[left_index].getx() >= points[middle].getx() - d.getDistance() && left_index >= start_index){
        strippoints.push_back(points[left_index]);
        left_index--;
    }
    while(points[right_index].getx() <= points[middle].getx() + d.getDistance() && right_index <= end_index){
        strippoints.push_back(points[right_index]);
        right_index++;
    }
    std::sort(strippoints.begin(), strippoints.end(), compareY);
    for(int left = left_index + 1; left < right_index; left++){
        for(int right = 0; right < 16 && right < strippoints.size(); right++){
            double tempD = points[left].dist(strippoints[right]);
            if(tempD != maxD.getDistance()){
                if(tempD < maxD.getDistance()){
                    cout << "oh no" << points[left].toString() << " " << strippoints[right].toString() << endl;
                    maxD = DistPoint(points[left], strippoints[right]);
                }
            }
        }
    }

    return maxD;
}

string part3(){
    Canvas c;
    srand(time(NULL));
    vector<Point> points;
    fstream txt;
    //open a file stream into points.txt --> precision is set to 23 decimal places.
    txt.open("points.txt", ios::in);
    string line;
    int num_points = 0;
    //generate points and add to front of list NUM_POINTS is defined at the top of the file (initially should be 60)
    while(getline(txt, line)){
        points.push_back(Point(std::stod(line.substr(0, 25)), std::stod(line.substr(27, 25))));
        num_points++;
    }
    //get starting time
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    
    std::sort(points.begin(), points.end(), compare);
    DistPoint d = recursion_helper(0, points.size() - 1, points);
    DistPoint maxD = d;
    //cout << d.getDistance() <<  d.getA().toString() << endl;

    int middle = (int)((points.size()-1)/2);
    int left_index = middle;
    int right_index = middle+1;
    vector<Point> strippoints;
    while(points[left_index].getx() >= points[middle].getx() - d.getDistance() && left_index >= 0){
        strippoints.push_back(points[left_index]);
        left_index--;
    }
    while(points[right_index].getx() <= points[middle].getx() + d.getDistance() && right_index <= NUM_POINTS){
        strippoints.push_back(points[right_index]);
        right_index++;
    }
    std::sort(strippoints.begin(), strippoints.end(), compareY);
    for(int left = left_index + 1; left < right_index; left++){
        for(int right = 0; right < 16 && right < strippoints.size(); right++){
            double tempD = points[left].dist(strippoints[right]);
            if(tempD > 0){
                if(tempD < maxD.getDistance()){
                    //cout << "LESS THAN" << endl;
                    maxD = DistPoint(points[left], strippoints[right]);
                }
            }
        }
    }
    std::chrono::milliseconds ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    
    

    std::stringstream a;
    a << fixed << setprecision(23) << "part 3 duration: " << (ms2.count() - ms.count()) << "\nMinimum Distance: " << (maxD.getDistance()) << "\nPoints: " << (maxD.getA()).toString() << " and " << (maxD.getB()).toString() << "\n";
    //cout << a << endl;
    txt.close();
    return a.str();
}

int main(){
    //string b = part1();
    generate_points(); // use for when part 1 isn't in use. Only generates points.
    string b = "";
    string a = part2_final();
    string c = part3();
    //string a = "no part 1 rn \n";
    ofstream txt;
    txt.open("results.txt");
    txt<< fixed;
    txt<< setprecision(23);
    cout << b << endl << a << endl << c << endl;
    txt<< a << endl << b << endl << c << endl;

    return 0;
}