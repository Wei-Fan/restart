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
//#include <geometry_msgs/Pose.h>
//#include <geometry_msgs/Twist.h>

#define DEFAULT_RATE 50
#define GRID_SIZE 40
#define AREA_SIZE 800

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

    Mat board = Mat(Size(900,900), CV_8UC3, Scalar(0));
    Mat assignment;

    Matrix<int,GRID_SIZE,GRID_SIZE> K;

public:
    SwarmCommander(){
        ROS_INFO("SWARM COMMANDER is activated!");
//        this->robot_number = robot_num;
        K.setZero(); //set zeros
//        ROS_INFO("size of K : %s, %s", K.cols(), K.rows());
//        K(1,3) = 1;
    }

    void run(){
        /*
         * initial phase
         * Area size: 40*40
         * */
//        ROS_INFO("debug 1");
        display_area();

        /*
         * divide area
         * */
//        divide_area();

        /*
         * STC planning
         * */
//        path_planning();

    }

    void display_area(){
        /*draw an empty area*/
//        ROS_INFO("debug 2");
        namedWindow("monitor");
        Point p1 = Point(50,50);
        Point p2 = Point(AREA_SIZE+50,AREA_SIZE+50);
        rectangle(board, p1, p2, CV_RGB(245, 245, 245), -1);
//        imshow("monitor", board);

        /*draw grids*/
        ROS_INFO("debug 3");
        int stepSize = (int)800/GRID_SIZE;
        for (int i = stepSize; i < AREA_SIZE; i += stepSize) {
            line(board, Point(50,50+i),Point(AREA_SIZE+50,50+i),Scalar(0,0,0));
        }

        for (int i = stepSize; i < AREA_SIZE; i += stepSize) {
            line(board, Point(50+i,50),Point(50+i,AREA_SIZE+50),Scalar(0,0,0));
        }
        ROS_INFO("draw grids complete");

        /*draw result*/
        for (int i = 0; i < GRID_SIZE; i += 1) { //cols
            for (int j = 0; j < GRID_SIZE; j += 1) { //rows
                if (K(j,i)==0)
                    continue;
                Point t1 = Point(j*stepSize+50,i*stepSize+50);
                Point t2 = Point((j+1)*stepSize+50,(i+1)*stepSize+50);
                rectangle(board, t1, t2, CV_RGB(0,0,255), -1);
            }
        }
        ROS_INFO("draw K complete");


//        ROS_INFO("debug 4");
//        ROS_INFO("size of board : %d, %d", board.size().height, board.size().width);
        imshow("monitor", board);
        setMouseCallback("monitor", onMouse, &board);
        waitKey();
        printf("robot number : %d\n", agent_num);

        destroyWindow("monitor");
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
        imshow("monitor", *img);
        x_init.push_back(x);//x_world);
        y_init.push_back(y);//y_world);
        agent_num++;

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