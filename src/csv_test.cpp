#include <ros/ros.h>
#include <iostream>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "Eigen/Eigen/Eigen"
#include "Eigen/Eigen/Geometry"

#include <string>
#include <vector>
#include <cmath>

using namespace std;
using namespace Eigen;

int main(int argc, char ** argv) {
    ros::init(argc, argv, "csv_test");
    vector<Vector2i> P_grid; // planning result
    for (int i = 0; i < 10; ++i) {
        Vector2i P_t(2*i,0);
        P_grid.push_back(P_t);
    }

    double vmax = 0.2;
    double amax = 0.1;
    double s_c = vmax*vmax/amax;
    double dt = 0.05;//s
    double turn_blank = 8;

    string project_path = "/home/wade/catkin_ws/src/restart/";
    int k = 0;
    string filename = project_path + "src/cover_test" + to_string(k) + ".csv";


    ofstream outfile;
    outfile.open(filename,ios::out);
    for (int i = 0; i < P_grid.size()-1; ++i) {
//        for (int j = 0; j < turn_blank/2; ++j) {
//            outfile << P_grid[i](0) << ',' << P_grid[i](1) << ',' << 1.0 << endl;
//        }

        int xoy;
        double s;
        if (P_grid[i](0)==P_grid[i+1](0)) {
            xoy = 1;
            s = abs(P_grid[i](1)-P_grid[i+1](1));
        } else {
            xoy = 0;
            s = abs(P_grid[i](0)-P_grid[i+1](0));
        }
//        cout << P_grid[i](xoy) << endl;
        if (s > s_c) {
            double x = 0;
            double dx = 0.5*amax*dt*dt;
            double line_time = 2*sqrt(s_c/amax)+(s-s_c)/vmax;
            int line_num = (int) (line_time / dt);
            for (int j = 0; j < line_num; ++j) {
                if (x<s_c/2)
                {
                    dx += amax*dt*dt;
                    x += dx;
                } else if (x<s-s_c/2) {
                    x += vmax*dt;
                } else {
                    dx -= amax*dt*dt;
                    x += dx;
                }
                if (xoy==0)
                    outfile << x+P_grid[i](0) << ',' << P_grid[i](1) << ',' << 1.0 << endl;
                else
                    outfile << P_grid[i](0) << ',' << x+P_grid[i](1) << ',' << 1.0 << endl;
            }

        } else {
            double line_time = 2*sqrt(s/amax);
            int line_num = (int) (line_time / dt);
            double x = 0;
            double dx = 0.5*amax*dt*dt;
            for (int j = 0; j < line_num; ++j) {
                if (j<line_num/2) {
                    dx += amax*dt*dt;
                    x += dx;
                } else {
                    dx -= amax*dt*dt;
                    x += dx;
                }
                if (xoy==0)
                    outfile << x+P_grid[i](0) << ',' << P_grid[i](1) << ',' << 1.0 << endl;
                else
                    outfile << P_grid[i](0) << ',' << x+P_grid[i](1) << ',' << 1.0 << endl;
            }
        }

//        for (int j = 0; j < turn_blank/2; ++j) {
//            outfile << P_grid[i+1](0) << ',' << P_grid[i+1](1) << ',' << 1.0 << endl;
//        }
    }
    outfile.close();

    return 0;
}