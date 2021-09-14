#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include <time.h>
#include <math.h>

#define SIZE 800

using namespace std;
//Canvas class has a method that returns a randomized point --> used to generate 3 points for the Triangle, Euler line, etc.
class Canvas{
    private:
        vector<vector<double>>* trianglePoints;
        int(*grid)[SIZE];
    public:
        Canvas(){
            trianglePoints = new vector<vector<double>>();
            grid = new int[SIZE][SIZE];
        }

        double distance(vector<double> a, vector<double> b){
            double x1 = a[0];
            double y1 = a[1];

            double x2 = b[0];
            double y2 = b[1];
            cout << "distance: " << sqrt((pow((x2-x1), 2.0)+pow((y2-y1), 2.0))) << endl;
            return sqrt((pow((x2-x1), 2.0)+pow((y2-y1), 2.0)));
        }

        //returns a vector with x,y point for circles, and r/R for the incircle/circumcircle
        vector<double> calculateIncirclePoints(){
            double a, b, c;
            vector<double> incirclePoint;
            cout << endl;
            cout << (*trianglePoints)[0][0] << ", " << (*trianglePoints)[0][1] << endl;
            cout << (*trianglePoints)[1][0] << ", " << (*trianglePoints)[1][1] << endl;
            cout << (*trianglePoints)[2][0] << ", " << (*trianglePoints)[2][1] << endl;
            
            a = distance((*trianglePoints)[1], (*trianglePoints)[2]);
            b = distance((*trianglePoints)[2], (*trianglePoints)[0]);
            c = distance((*trianglePoints)[0], (*trianglePoints)[1]);

            double s;
            s = (a+b+c)/2;

            double x, y;
            x = (a * (*trianglePoints)[0][0] + b * (*trianglePoints)[1][0] + c * (*trianglePoints)[2][0])/(a+b+c); 
            y = (a * (*trianglePoints)[0][1] + b * (*trianglePoints)[1][1] + c * (*trianglePoints)[2][1])/(a+b+c); 
            incirclePoint.push_back(x);
            incirclePoint.push_back(y);

            double cirx, ciry;
            // //(Ax2*Ay2))
            // double d = (*trianglePoints)[0][0]*((*trianglePoints)[1][1]-(*trianglePoints)[2][1])+(*trianglePoints)[1][0]*((*trianglePoints)[2][1]-(*trianglePoints)[0][1])+(*trianglePoints)[2][0]*((*trianglePoints)[0][1]-(*trianglePoints)[1][1]);
            // cirx = (((*trianglePoints)[0][0]*(*trianglePoints)[0][0]+(*trianglePoints)[0][1]*(*trianglePoints)[0][1])*((*trianglePoints)[1][1]-(*trianglePoints)[2][1])+((*trianglePoints)[1][0]*(*trianglePoints)[1][0]+(*trianglePoints)[1][1]*(*trianglePoints)[1][1])*((*trianglePoints)[2][1]-(*trianglePoints)[0][1])+((*trianglePoints)[2][0]*(*trianglePoints)[2][0]+(*trianglePoints)[2][1]*(*trianglePoints)[2][1])*((*trianglePoints)[1][1]-(*trianglePoints)[0][1]))/d;
            // ciry = (((*trianglePoints)[0][0]*(*trianglePoints)[0][0]+(*trianglePoints)[0][1]*(*trianglePoints)[0][1])*((*trianglePoints)[1][0]-(*trianglePoints)[2][0])+((*trianglePoints)[1][0]*(*trianglePoints)[1][0]+(*trianglePoints)[1][1]*(*trianglePoints)[1][1])*((*trianglePoints)[2][0]-(*trianglePoints)[0][0])+((*trianglePoints)[2][0]*(*trianglePoints)[2][0]+(*trianglePoints)[2][1]*(*trianglePoints)[2][1])*((*trianglePoints)[1][0]-(*trianglePoints)[0][0]))/d;

            double midx1, midx2, midy1, midy2, midy3, midx3, midx4, midy4;
            //double m1, m2;
            //m1 = ((*trianglePoints)[1][1]-(*trianglePoints)[1][0])/((*trianglePoints)[1][0]-(*trianglePoints)[0][0]);
            midx1 = ((*trianglePoints)[0][0]+(*trianglePoints)[1][0])/2;
            midy1 = ((*trianglePoints)[0][1]+(*trianglePoints)[1][1])/2;
            midx2 = midx1 - (*trianglePoints)[1][1]+(*trianglePoints)[0][1];
            midy2 = midy1 + (*trianglePoints)[1][0]-(*trianglePoints)[0][0];

            midx3 = ((*trianglePoints)[1][0]+(*trianglePoints)[2][0])/2;
            midy3 = ((*trianglePoints)[1][1]+(*trianglePoints)[2][1])/2;
            midx4 = midx3 - (*trianglePoints)[2][1]+(*trianglePoints)[1][1];
            midy4 = midy3 + (*trianglePoints)[2][0]-(*trianglePoints)[1][0];
            cout << midx1 << " " << midy1 << endl << midx2 << " " << midy2 << endl << midx3 << " " << midy3 << endl << midx4 << " " << midy4 << endl;

            // Line AB represented as a1x + b1y = c1
            double a1 = midy2 - midy1;
            double b1 = midx1 - midx2;
            double c1 = a1*(midx1) + b1*(midy1);

            // Line CD represented as a2x + b2y = c2
            double a2 = midy4 - midy3;
            double b2 = midx3 - midx4;
            double c2 = a2*(midx3)+ b2*(midy3);
            double determinant = a1*b2 - a2*b1;
            cirx = (b2*c1 - b1*c2)/determinant;
            ciry = (a1*c2 - a2*c1)/determinant;
            //cout << endl << cirx << ", " << ciry << endl; 

            double orthox1, orthoy1, orthox2, orthoy2;
            double x1, x2, x3, y1, y2, y3;
            x1 = (*trianglePoints)[0][0];
            x2 = (*trianglePoints)[1][0];
            x3 = (*trianglePoints)[2][0];
            y1 = (*trianglePoints)[0][1];
            y2 = (*trianglePoints)[1][1];
            y3 = (*trianglePoints)[2][1];
            orthox1 = (x1 - (y3-y2));
            orthoy1 = (y1 + (x3-x2));
            orthox2 = (x2 - (y3-y1));
            orthoy2 = (y2 + (x3-x1));
            // bresenham_pos((int)(x1*SIZE), (int)(y1*SIZE), (int)(orthox1*SIZE), (int)(orthoy1*SIZE));
            // bresenham_pos((int)(x2*SIZE), (int)(y2*SIZE), (int)(orthox2*SIZE), (int)(orthoy2*SIZE));
            // Line AB represented as a1x + b1y = c1
            a1 = orthoy1 - y1;
            b1 = x1 - orthox2;
            c1 = a1*(x1) + b1*(y1);

            // Line CD represented as a2x + b2y = c2
            a2 = orthoy2 - y2;
            b2 = x2 - orthox2;
            c2 = a2*(x2)+ b2*(y2);
            determinant = a1*b2 - a2*b1;
            double orthox, orthoy;
            orthox = (b2*c1 - b1*c2)/determinant;
            orthoy = (a1*c2 - a2*c1)/determinant;

            double r, R;
            r = sqrt(((s-a)*(s-b)*(s-c))/(s));
            R = ((a*b*c)/(4*r*s));
            incirclePoint.push_back(r);
            incirclePoint.push_back(R);
            cout << x << " - " << y << " - " << r << " - " << R << endl;

            incirclePoint.push_back(cirx);
            incirclePoint.push_back(ciry);

            incirclePoint.push_back(orthox);
            incirclePoint.push_back(orthoy);
            
            double nineptx, ninepty;
            nineptx = (cirx+orthox)/2.0;
            ninepty = (ciry+orthoy)/2.0;
            incirclePoint.push_back(nineptx);
            incirclePoint.push_back(ninepty);
            vector<double> v1, v2;
            v1.push_back(nineptx);
            v1.push_back(ninepty);
            v2.push_back((x2+x1)/2.0);
            v2.push_back((y2+y1)/2.0);
            incirclePoint.push_back(distance(v1, v2));
            cout << "9pt: " << nineptx << ", " << ninepty << ":: " << distance(v1, v2) << endl;

            return incirclePoint;
        }
        
        //Circle rasterization code given in class (slightly modified to work with (x,y) not just (0,0))
        void drawCircle_dep(int x1, int y1, int r){
            int xmax, y2, y2_new, ty, x, y;
            xmax = (int)(r * .70710678);
            //cout << xmax << endl;

            y = r;
            y2 = y * y;
            ty = (2 * y) - 1;
            y2_new = y2;

            // x1 += r;
            // y1 -= r;
            for(int x = 0; x <= xmax; x++){
                if((y2 - y2_new) >= ty){
                    y2 -= ty;
                    y -= 1;
                    ty -= 2;
                }
                grid[x1 + x][y1 + y] = 10;
                grid[x1 + x][y1 - y] = 10;
                grid[x1 - x][y1 + y] = 10;
                grid[x1 - x][y1 - y] = 10;
                grid[x1 + y][y1 + x] = 10;
                grid[x1 + y][y1 - x] = 10;
                grid[x1 - y][y1 + x] = 10;
                grid[x1 - y][y1 - x] = 10;
                //cout << "printed smth" << endl;

                y2_new -= (2 * x) - 3;
            }
        }

        //drawCircle method that works a bit better
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
            int dx = x2 - x1;
            //cout << "dx in bresenham: " << dx << endl << endl;
            int dy = y2 - y1;
            //cout << "dy in bresenham: " << dy << endl << endl; 
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

        void addTPS(vector<double> a){
            (*trianglePoints).push_back(a);
        }

        vector<int>* trianglepts(){
            vector<double> dblpoints;
            vector<int>* points;
            points = new vector<int>;
            dblpoints.push_back(( (double) rand() ) / (RAND_MAX) );
            dblpoints.push_back(( (double) rand() ) / (RAND_MAX) );

            addTPS(dblpoints);

            (*points).push_back(int(dblpoints[0] * double(SIZE)));
            (*points).push_back(int(dblpoints[1] * double(SIZE)));

            return points;
        }

        void build_and_draw_triangle(){
            vector<int>*points[4]; 
            for(int x = 0; x < 3; x++){
                points[x] = trianglepts();
            }
            points[3] = points[0];

            for(int t = 0; t < 3; t++){
                int x2 = (*points[t+1])[0];
                int y2 = (*points[t+1])[1];

                int x1 = (*points[t])[0];
                int y1 = (*points[t])[1];

                if((x2 - x1) < 0){
                    bresenham_pos(x2, y2, x1, y1);
                    printVector(0, *points[t+1]);
                    printVector(1, *points[t]);
                    cout << endl;
                }
                else{
                    bresenham_pos(x1, y1, x2, y2);
                    printVector(0, *points[t]);
                    printVector(1, *points[t+1]);
                    cout << endl;
                }
            }
        }

        void incircle(){
            vector<double> pt = calculateIncirclePoints();
            cout << (int)(pt[0] * SIZE) << ", " << (int)(pt[1] * SIZE) << endl;
            cout << (int)(pt[4] * SIZE) << ", " << (int)(pt[5] * SIZE) << endl;
            drawCircle((int)(pt[0] * SIZE), (int)(pt[1] * SIZE), (int)(pt[2] * SIZE));
            drawCircle((int)(pt[4] * SIZE), (int)(pt[5] * SIZE), (int)(pt[3] * SIZE));
            double orthox, orthoy;
            double circumx, circumy;
            circumx = pt[4];
            circumy = pt[5];
            orthox = pt[6];
            orthoy = pt[7];
            double dx = orthox - circumx;
            double dy = orthoy - circumy;
            while(circumx < SIZE && circumx > 0){
                circumx -= dx;
                circumy -= dy;
            }
            while(orthox < SIZE && orthox > 0){
                orthox += dx;
                orthoy += dy;
            }
            dx = orthox - circumx;
            int x1 = (int)(circumx*SIZE);
            int x2 = (int)(orthox*SIZE);
            int y1 = (int)(circumy*SIZE);
            int y2 = (int)(orthoy*SIZE);
            if(dx < 0){
                bresenham_pos(x2, y2, x1, y1);
            }
            else{
                bresenham_pos(x1, y1, x2, y2);
            }
            drawCircle((int)(pt[8] * SIZE), (int)(pt[9] * SIZE), (int)(pt[10] * SIZE));
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
            delete trianglePoints;
            delete [] grid;
        }
};

int main()
{
    srand(time(NULL));
    Canvas *t = new Canvas();
    (*t).build_and_draw_triangle();
    (*t).incircle();
    (*t).draw_grid();
    delete t;
    return 0;
}

