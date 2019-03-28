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
//#include <geometry_msgs/Pose.h>
//#include <geometry_msgs/Twist.h>

#define DEFAULT_RATE 50
#define GRID_SIZE 40
#define CORE_SIZE 20 //CORE_SIZE = GRID_SIZE / 2
#define AREA_SIZE 800

using namespace std;
using namespace cv;
using namespace Eigen;

int robot_number = 0;
vector<int> x_init;
vector<int> y_init;

class SwarmCommander
{
private:
    ros::NodeHandle global;
    ros::NodeHandle local;

    Mat board = Mat(Size(900,900), CV_8UC3, Scalar(0));

    Matrix<int,CORE_SIZE,CORE_SIZE> K;
//    Matrix<int,GRID_SIZE,GRID_SIZE> E; How to define a 3d matrix?
//    MatrixXi E;
    Matrix<double,Dynamic,CORE_SIZE> C;
    Matrix<double,Dynamic,1> m;
    vector<int> robot_grid_x;
    vector<int> robot_grid_y;

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
        display_area();
        
        /*initialize robot positions by clicking (testing code)*/
        namedWindow("monitor");
        imshow("monitor",board);
        setMouseCallback("monitor", onMouse, &board);
        waitKey();
        ROS_INFO("robot number : %d", robot_number);
        destroyWindow("monitor");
        for (int i = 0; i < x_init.size(); ++i) {
            robot_grid_x.push_back((x_init[i] - 50)*CORE_SIZE/AREA_SIZE);
            robot_grid_y.push_back((y_init[i] - 50)*CORE_SIZE/AREA_SIZE);
            ROS_INFO("robot : %d,%d",robot_grid_x[i],robot_grid_y[i]);
        }

        /*these codes need to be moved if the robot number comes from elsewhere*/
//        E = MatrixXi::Zero(robot_number*CORE_SIZE,CORE_SIZE);
        // To access : MatrixXi Ek = E.block<CORE_SIZE,CORE_SIZE>(k*CORE_SIZE,0);
//        ROS_INFO("debug 1");
        C.resize(robot_number*CORE_SIZE,CORE_SIZE);
        C.setOnes();
        m.resize(robot_number,1);
        m.setOnes(); //size : (robot_number,1)

        /*
         * divide area
         * */

//        ROS_INFO("debug 2");
        divide_area();

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
//        ROS_INFO("debug 3");
        int stepSize = (int)800/GRID_SIZE;
        for (int i = stepSize; i < AREA_SIZE; i += stepSize) {
            line(board, Point(50,50+i),Point(AREA_SIZE+50,50+i),Scalar(0,0,0));
        }

        for (int i = stepSize; i < AREA_SIZE; i += stepSize) {
            line(board, Point(50+i,50),Point(50+i,AREA_SIZE+50),Scalar(0,0,0));
        }
//        ROS_INFO("draw grids complete");

        /*draw result*/
        for (int i = 0; i < CORE_SIZE; i += 1) { //cols
            for (int j = 0; j < CORE_SIZE; j += 1) { //rows
                if (K(j,i)==0)
                    continue;
                Point t1 = Point(j*stepSize*2+50,i*stepSize*2+50);
                Point t2 = Point((j+1)*stepSize*2+50,(i+1)*stepSize*2+50);
                rectangle(board, t1, t2, CV_RGB(0,0,255), -1);
            }
        }
//        ROS_INFO("draw K complete");


//        ROS_INFO("debug 4");
//        ROS_INFO("size of board : %d, %d", board.size().height, board.size().width);
        imshow("monitor", board);
        waitKey();

//        destroyWindow("monitor");
    }

    static void onMouse(int event, int x, int y, int, void* userInput)
    {
        if (event != EVENT_LBUTTONDOWN) return;
//        printf("###########onMouse x : %d\n", x);
//        printf("###########onMouse y : %d\n", y);
        int x_world = x - 500;
        int y_world = 500 - y;
        Mat *img = (Mat*)userInput;

        circle(*img, Point(x, y), 6, Scalar(255, 0, 255), 2);
        imshow("monitor", *img);
        x_init.push_back(x);//x_world);
        y_init.push_back(y);//y_world);
        robot_number++;


    }

    bool divide_area(){
        /*Generate the first matrix*/

        /*main loop*/
        bool stop = false;
        int iteration_count = 0;
        while (!stop&&ros::ok())
        {
            iteration_count++;
            ArrayXi S = ArrayXi::Zero(robot_number); // record grid number for each robot
//            ROS_INFO("S size : %d, %d and %d", S.rows(), S.cols(),S(2));
            int count = 0;

            /*check every grid*/
//            ROS_INFO("E size : %d, %d",E.rows(),E.cols());
            for (int i = 0; i < CORE_SIZE; ++i) { //cols
                for (int j = 0; j < CORE_SIZE; ++j) { //rows
                    double tmp = -1;
                    for (int k = 0; k < robot_number; ++k) {
//                        MatrixXi E_t = E.block<CORE_SIZE,CORE_SIZE>(k*CORE_SIZE,0);
//                        MatrixXi C_t = C.block<CORE_SIZE,CORE_SIZE>(k*CORE_SIZE,0);
                        double Ekji = C(j+k*CORE_SIZE,i) * m(k,0) * sqrt((i-robot_grid_x[k])*(i-robot_grid_x[k])+(j-robot_grid_y[k])*(j-robot_grid_y[k]));
//                        double E_t = ;
                        if (tmp < 0 || tmp > Ekji)
                        {
                            tmp = Ekji;
                            K(j,i) = k;
                        }
                    }
                    S(K(j,i),1)++;
                }
            }
//            cout << S <<  endl;
//            stop = true;

            /*update C*/
            // obtain the assignment matrix for every robot
            Matrix<int,Dynamic,CORE_SIZE> Kd;
            Kd.resize(robot_number*CORE_SIZE,CORE_SIZE);
            for (int k = 0; k < robot_number; ++k) {
                for (int i = 0; i < CORE_SIZE; ++i) {
                    for (int j = 0; j < CORE_SIZE; ++j) {
                        if (K(j,i)==k)
                            Kd(j+k*CORE_SIZE,i) = 1;
                        else
                            Kd(j+k*CORE_SIZE,i) = 0;

                    }
                }
            }
            // obtain the connected sets for every robot
//            cout << isconnect(1,1,K,1,2) << endl;
            stop = true;
        }
    }

    bool isconnect(int j0, int i0, Matrix<int,CORE_SIZE,CORE_SIZE> K_t, int rx, int ry){
        int pace = -1;
        Matrix<double,4,2> coordinate;
        coordinate << 0, -1,
                      1, 0,
                      0, 1,
                      -1, 0;
        Vector2i curr_p(j0,i0);// size : (2,1)
        Vector2i robot(ry,rx);
//        cout << curr_p << endl << robot << endl;

        bool stop = false;
        int count = 0;
        while (!stop&&ros::ok())
        {
            /*see if the condition were met*/
            Vector2i dir_i = robot - curr_p;
//            cout << dir_i << endl;
            double dir_l = sqrt((double)(dir_i.transpose()*dir_i));
//            cout << dir_l << endl;
            if (dir_l<0.002)
            {
                stop = true;
                ROS_INFO("start at stop");
                return true;
            }

            /*sort the direction*/
            double dir_d[2];
            dir_d[0] = dir_i[0] / dir_l;
            dir_d[1] = dir_i[1] / dir_l;
            double dir_cost[4];
            int cost_i[4] = {1,2,3,4};
            for (int t = 0; t < 4; ++t) {
                dir_cost[t] = sqrt((coordinate(t,0)-dir_d[0])*(coordinate(t,0)-dir_d[0])+(coordinate(t,1)-dir_d[1])*(coordinate(t,1)-dir_d[1]));
            }
            if (pace>=0)
            {
                dir_cost[pace] = dir_cost[pace] + 2;
            }
            for (int j = 0; j < 4 - 1; ++j) {
                for (int i = 0; i < 4 - 1 - j; ++i) {
                    if (dir_cost[i] > dir_cost[i+1]){
                        Swap(dir_cost,i,i+1);
                        Swap(cost_i,i,i+1);
                    }
                }
            }
            for (int i = 0; i < 4; ++i) {
//                cout << "sorted dir_cost : " << dir_cost[i] << endl;
            }
            vector<Vector2i> sorted_cd;
            for (int i = 0; i < 4; ++i) {
                Vector2i tmp(coordinate(cost_i[i],0),coordinate(cost_i[i],1));
                sorted_cd.push_back(tmp);
            }

            /*choose the nearest apprachable way*/
            bool move = false;
            for (auto sd : sorted_cd)
            {
//                cout << sd << endl;
                Vector2i next_p = curr_p + sd;
                if (next_p(0,0)==-1||next_p(0,0)==CORE_SIZE||next_p(1,0)==-1||next_p(1,0)==CORE_SIZE)
                    continue;

                if (K_t(next_p(0,0),next_p(1,0))==1)
                {
                    curr_p = next_p;
                    move = true;
                    if (sd(0,0)==1)
                        pace = 3;
                    else if (sd(0,0)==-1)
                        pace = 1;
                    else if (sd(1,0)==-1)
                        pace = 2;
                    else
                        pace = 0;
                    break;
                }
            }
            if (!move)
            {
                stop = true;
                return false;
            } else {
                count++;
                if (count==CORE_SIZE*CORE_SIZE)
                {
                    stop = true;
                    return false;
                }
            }
        }

    }

    template<typename T>
    void Swap(T A[], int i, int j)
    {
        T tmp = A[i];
        A[i] = A[j];
        A[j] = tmp;
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