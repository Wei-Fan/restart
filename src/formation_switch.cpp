#include <ros/ros.h>
#include <stdio.h> //sprintf
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "Eigen/Eigen/Eigen"
#include "Eigen/Eigen/Geometry"

using namespace cv;
using namespace std;
using namespace Eigen;

int agent_num = 0;
int target_num = 0;
bool isAgentDone = false;

vector<float> x_init;
vector<float> y_init;
vector<float> x_target;
vector<float> y_target;

class FormationSwitch
{
private:
	Mat src = Mat(Size(1000,1000), CV_8UC3, Scalar(0));
	Mat costMatrix;

public:
	FormationSwitch(ros::NodeHandle& nh)
	//:agent_num(0)
	//,target_num(0)
	//,isAgentDone(false)
	{
		/*char msg_name[50];
		for(int i=0;i<g_vehicle_num;i++){
			sprintf(msg_name,"/vehicle%d/pos_est", i);
			m_pos_est_v[i] = nh.advertise<restart::pos_est>(msg_name,1);//msg_name, 1);
			sprintf(msg_name,"/vehicle%d/pos_predict", i);
			m_predict_test[i] = nh.advertise<restart::pos_est>(msg_name,1);//msg_name, 1);
		}*/
	}

	~FormationSwitch(){}

	void run(double frequency)
	{
		bool sequenceIsOk = false;
		while(!sequenceIsOk)//use mouse to rearrange index of swarm_pos
		{
			namedWindow("vicon_test");	
			Point p1 = Point(50,50);
			Point p2 = Point(950,950);
			rectangle(src, p1, p2, CV_RGB(0, 0, 255), -1);	
			imshow("vicon_test", src);

			setMouseCallback("vicon_test", onMouse, &src);
			waitKey();
			isAgentDone = true;
			printf("vehicle number : %d\n", agent_num);

			setMouseCallback("vicon_test", onMouse, &src);
			waitKey();			
			printf("target number : %d\n", target_num);

			/*check the click times and exit the initialization*/
			if(target_num==agent_num){
				sequenceIsOk = true;
				formation_switch();
				//waitKey();
			}else{
				destroyWindow("vicon_test");
				printf("Initialization fails!! Please click again!!\n");
				agent_num = 0;
				target_num = 0;
				isAgentDone = false;
			}
		}
		destroyWindow("vicon_test");
		/*ros::NodeHandle node;
		ros::Timer timer = node.createTimer(ros::Duration(1.0/frequency), &FormationSwitch::iteration, this);
		ros::spin();*/
	}

	void iteration(const ros::TimerEvent& e)
	{	
	}

	static void onMouse(int event, int x, int y, int, void* userInput)
	{
		if (event != EVENT_LBUTTONDOWN) return;
		//printf("###########onMouse x : %d\n", x);
		//printf("###########onMouse y : %d\n", y);
		int x_world = x - 500;
		int y_world = 500 - y;
		Mat *img = (Mat*)userInput;
		if (!isAgentDone)
		{
			circle(*img, Point(x, y), 10, Scalar(255, 0, 255));
			imshow("vicon_test", *img);
			x_init.push_back(x_world);
			y_init.push_back(y_world);
			agent_num++;
			
		} else if (isAgentDone)
		{
			circle(*img, Point(x, y), 10, Scalar(0, 255, 0));
			imshow("vicon_test", *img);
			x_target.push_back(x_world);
			y_target.push_back(y_world);
			target_num++;
		}
	}

	void formation_switch()
	{
		/*build a cost matrix and find the minimum element each rows*/
		costMatrix = Mat::zeros(agent_num, target_num, CV_32SC1);
		vector<int> min_per_rows(agent_num,-1);
		for (int i = 0; i < agent_num; ++i)
		{
			for (int j = 0; j < target_num; ++j)
			{
    			float tmp_dist;	  
    			int tmp_x = x_init[i] - x_target[j];
    			int tmp_y = y_init[i] - y_target[j];
    			tmp_dist = sqrt(tmp_x*tmp_x+tmp_y*tmp_y);

    			if (min_per_rows[i] < 0 || min_per_rows[i] > tmp_dist )
    			{
    				min_per_rows[i] = tmp_dist;
    			}

    			costMatrix.at<int>(i,j) = tmp_dist;
    			//printf("costMatrix[%d,%d] = %d\n", i,j,costMatrix.at<int>(i,j));
			}
			//printf("min_per_rows[%d] = %d\n", i, min_per_rows[i]);
		}

		for (int i = 0; i < agent_num; ++i)
		{
			for (int j = 0; j < target_num; ++j)
			{
				costMatrix.at<int>(i,j) = costMatrix.at<int>(i,j) - min_per_rows[i];
				printf("costMatrix[%d,%d] = %d\n", i,j,costMatrix.at<int>(i,j));
			}
		}

		/*find the assignment Matrix*/
		Mat assignment = Mat::zeros(agent_num, target_num, CV_32SC1);
		int assigned_num = 0;
		vector<int> assigned_rows(agent_num,-1);
		vector<int> assigned_cols(target_num,-1);
		
		while(assigned_num != agent_num)
		{
			for (int i = 0; i < agent_num; ++i)
			{
				if (assigned_rows[i] == 1)
					continue;

				for (int j = 0; j < target_num; ++j)
				{
					if (assigned_cols[j] == 1)
						continue;

					if (costMatrix.at<int>(i,j) == 0)
					{
						assignment.at<int>(i,j) = 1;
						assigned_rows[i] = 1;
						assigned_cols[j] = 1;
						assigned_num++;
					}
				}
			}

			int tmp_min = -1;
			for (int i = 0; i < agent_num; ++i)
			{
				if (assigned_rows[i] == 1)
					continue;

				for (int j = 0; j < target_num; ++j)
				{
					if (assigned_cols[j] == 1)
						continue;

					if (costMatrix.at<int>(i,j) > tmp_min || tmp_min < 0)
					{
						tmp_min = costMatrix.at<int>(i,j);
					}
				}
			}

			for (int i = 0; i < agent_num; ++i)
			{
				if (assigned_rows[i] == 1)
					continue;

				for (int j = 0; j < target_num; ++j)
				{
					if (assigned_cols[j] == 1)
						continue;

					costMatrix.at<int>(i,j) = costMatrix.at<int>(i,j) - tmp_min;
				}
			}
		}

		for (int i = 0; i < agent_num; ++i)
		{
			for (int j = 0; j < target_num; ++j)
			{
				printf("assignment[%d,%d] = %d\n", i,j,assignment.at<int>(i,j));
			}
		}
		//imshow("vicon_test", src);
	}
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "formation_switch");
	ros::NodeHandle n;

	FormationSwitch m(n);
	m.run(50);

	ros::spin();
	return 0;
}