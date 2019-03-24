#include <ros/ros.h>
#include <iostream>
#include <string>
#include <cv_bridge/cv_bridge.h>
#include "Eigen/Eigen/Eigen"
#include "Eigen/Eigen/Geometry"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
//#include <geometry_msgs/Pose.h>
//#include <geometry_msgs/Twist.h>

#define DEFAULT_RATE 50

using namespace std;
using namespace cv;
using namespace Eigen;

int agent_num = 0;
int target_num = 0;
bool isAgentDone = false;
vector<float> x_init;
vector<float> y_init;
vector<float> x_target;
vector<float> y_target;

class SwarmCommander
{
private:
    int robot_number;

    ros::NodeHandle global;
    ros::NodeHandle local;

    Mat src = Mat(Size(1000,1000), CV_8UC3, Scalar(0));
    Mat assignment;
    int gird_size = 40;
    int area_size = 800;

public:
    SwarmCommander(){
        ROS_INFO("SWARM COMMANDER is activated!");
//        this->robot_number = robot_num;
    }

    void run(){
        /*
         * initial phase
         * Area size: 40*40
         * */
        display_area();
        /*
         * divide area
         * */
        divide_area();

        /*
         * STC planning
         * */
        path_planning();

    }

    static void onMouse(int event, int x, int y, int, void* userInput)
    {
        if (event != EVENT_LBUTTONDOWN) return;
        //printf("###########onMouse x : %d\n", x);
        //printf("###########onMouse y : %d\n", y);
        int x_world = x - 500;
        int y_world = 500 - y;
        Mat *img = (Mat*)userInput;

        circle(*img, Point(x, y), 6, Scalar(255, 0, 255), 2);
        imshow("coverage_monitor", *img);
        x_init.push_back(x);//x_world);
        y_init.push_back(y);//y_world);
        agent_num++;


    }

    void display_area(){
        /*draw an empty area*/
        namedWindow("coverage_monitor");
        Point p1 = Point(50,50);
        Point p2 = Point(area_size+50,area_size+50);
        rectangle(src, p1, p2, CV_RGB(245, 245, 245), -1);
//        imshow("coverage_monitor", src);

        /*draw grids*/
        int stepSize = 800/gird_size;
        for (int i = stepSize; i < area_size; i += stepSize) {
            line(src, Point(50,50+i),Point(area_size+50,50+i),Scalar(0,0,0));
        }

        for (int i = stepSize; i < area_size; i += stepSize) {
            line(src, Point(50+i,50),Point(50+i,area_size+50),Scalar(0,0,0));
        }
        imshow("coverage_monitor", src);

        setMouseCallback("coverage_monitor", onMouse, &src);
        waitKey();
        printf("robot number : %d\n", agent_num);

        destroyWindow("coverage_monitor");
    }

    bool divide_area(){

    }

    void path_planning(){

    }
};

int main(int argc, char ** argv) {
    ros::init(argc, argv, "swarm_commander");

    SwarmCommander node;
    node.run();

    return 0;
}