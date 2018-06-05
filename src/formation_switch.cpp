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
	Mat assignment;

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
			rectangle(src, p1, p2, CV_RGB(245, 245, 245), -1);	
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
		drawRst();
		waitKey();
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
			circle(*img, Point(x, y), 10, Scalar(255, 0, 255), 2);
			imshow("vicon_test", *img);
			x_init.push_back(x);//x_world);
			y_init.push_back(y);//y_world);
			agent_num++;
			
		} else if (isAgentDone)
		{
			circle(*img, Point(x, y), 10, Scalar(0, 255, 0), 2);
			imshow("vicon_test", *img);
			x_target.push_back(x);//x_world);
			y_target.push_back(y);//y_world);
			target_num++;
		}
	}

	void drawRst()
	{
		for (int i = 0; i < agent_num; ++i)
		{
			for (int j = 0; j < target_num; ++j)
			{
				if (assignment.at<int>(i,j) == 1)
				{
					line(src, Point(x_init[i],y_init[i]), Point(x_target[j],y_target[j]), Scalar(50,50,0),2);
				}
			}
		}
		imshow("vicon_test", src);
	}

	int find(Mat &costM,vector<Vector2i> &ass_vec,vector<int> &indep_zero_rows_index,vector<int> &indep_zero_cols_index)
	{
		int z_n_indep = 0;

		while(true)
		{
			Vector2i tmp_z_n;

			/*count the number of zero*/
			vector<int> zero_num_rows(agent_num,0);
			vector<int> zero_num_cols(target_num,0);
			for (int i = 0; i < agent_num; ++i)
			{
				if (indep_zero_rows_index[i] == 1)
					continue;

				for (int j = 0; j < target_num; ++j)
				{
					if (indep_zero_cols_index[j] == 1)
						continue;

					if (costM.at<int>(i,j) == 0)
					{
						zero_num_rows[i]++;
						zero_num_cols[j]++;
					}
				}
			}

			/*find the row that has minimum zero number*/
			int tmp = -1;
			int tmp_index = -1;
			bool isRow;
			for (int i = 0; i < agent_num+target_num; ++i)
			{
				if (i < agent_num) //row
				{
					if (indep_zero_rows_index[i] == 1 || zero_num_rows[i] == 0)
						continue;

					if (zero_num_rows[i] < tmp || tmp < 0)
					{
						tmp = zero_num_rows[i];
						tmp_index = i;
					}
				} else {
					if (indep_zero_cols_index[i-agent_num] == 1 || zero_num_cols[i-agent_num] == 0)
						continue;

					if (zero_num_cols[i-agent_num] < tmp || tmp < 0)
					{
						tmp = zero_num_rows[i-agent_num];
						tmp_index = i;
					}
				}
			}

			if (tmp_index < 0)
				break;

			if (tmp_index < agent_num)
			{
				indep_zero_rows_index[tmp_index] = 1;
				isRow = true;
				tmp_z_n(0) = tmp_index;
			} else {
				indep_zero_cols_index[tmp_index-agent_num] = 1;
				isRow = false;
				tmp_z_n(1) = tmp_index-agent_num;
			}

			/*find the independent zero element in that row or col*/
			if (isRow)
			{
				for (int i = 0; i < target_num; ++i)
				{
					if (indep_zero_cols_index[i] == 1)
						continue;

					if (costM.at<int>(tmp_index,i) == 0)
					{
						indep_zero_cols_index[i] = 1;
						tmp_z_n(1) = i;
					}
				}
			} else {
				for (int i = 0; i < agent_num; ++i)
				{
					if (indep_zero_rows_index[i] == 1)
						continue;

					if (costM.at<int>(i,tmp_index-agent_num) == 0)
					{
						indep_zero_rows_index[i] = 1;
						tmp_z_n(0) = i;
					}
				}
			}

			//tmp_z_n(2) = 0;
			ass_vec.push_back(tmp_z_n);
		}

		for (int i = 0; i < agent_num; ++i)
		{
			if (indep_zero_rows_index[i] == 1)
			{
				z_n_indep++;
			}
		}

		return z_n_indep;
	}

	void formation_switch()
	{
		costMatrix = Mat::zeros(agent_num, target_num, CV_32SC1);
		
		/*build a cost matrix and find the minimum element each rows*/
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

		/*subtract row minima*/
		vector<int> assigned_cols(target_num,-1);
		for (int i = 0; i < agent_num; ++i)
		{
			for (int j = 0; j < target_num; ++j)
			{
				costMatrix.at<int>(i,j) = costMatrix.at<int>(i,j) - min_per_rows[i];
				if (costMatrix.at<int>(i,j) == 0)
					assigned_cols[j] = 1;
				//printf("costMatrix[%d,%d] = %d\n", i,j,costMatrix.at<int>(i,j));
			}
		}

		/*subtract column minima*/
		for (int j = 0; j < target_num; ++j)
		{
			if (assigned_cols[j] == 1)
				continue;

			int min_cols = -1;
			for (int i = 0; i < agent_num; ++i)
			{
				if (costMatrix.at<int>(i,j) < min_cols || min_cols < 0)
					min_cols = costMatrix.at<int>(i,j);
			}

			for (int i = 0; i < agent_num; ++i)
			{
				costMatrix.at<int>(i,j) = costMatrix.at<int>(i,j) - min_cols;
				//printf("costMatrix[%d,%d] = %d\n", i,j,costMatrix.at<int>(i,j));
			}
		}

		/*show init*/
		for (int i = 0; i < agent_num; ++i)
		{
			for (int j = 0; j < target_num; ++j)
			{
				printf("costMatrix[%d,%d] = %d\n", i,j,costMatrix.at<int>(i,j));
			}
		}

		/*find the assignment Matrix*/
		assignment = Mat::zeros(agent_num, target_num, CV_32SC1);
		vector<int> indep_zero_rows_index(agent_num,-1);
		vector<int> indep_zero_cols_index(target_num,-1);
		vector<Vector2i> ass_vec;
		int count = 0;
		//int assigned_num = 0;
		//vector<int> assigned_rows(agent_num,-1);
		//vector<int> assigned_cols(target_num,-1);
		
		while(find(costMatrix,ass_vec,indep_zero_rows_index,indep_zero_cols_index) < agent_num)
		{
			printf("*********** %d **********\n\n", count++);
			/*mark the rows without independent zero*/
			vector<bool> check_rows(agent_num,true);
			vector<bool> check_cols(target_num,false);
			for (int i = 0; i < agent_num; ++i)
			{
				if (indep_zero_rows_index[i] != 1)
				{
					check_rows[i] = false;
				}
			}

			bool is = true;
			while(is)
			{
				is = false;
				/*mark the cols which has a zero element in these rows*/
				for (int i = 0; i < agent_num; ++i)
				{
					if (check_rows[i] != false)
						continue;

					for (int j = 0; j < target_num; ++j)
					{
						if (costMatrix.at<int>(i,j) == 0 && check_cols[j] != true)
						{
							check_cols[j] = true;
							is = true;
						}
					}
				}

				/*mark the rows which has a independent zero element in these cols*/
				for (int j = 0; j < target_num; ++j)
				{
					if (check_cols[j] != true)
						continue;

					for (int i = 0; i < ass_vec.size(); ++i)
					{
						if (ass_vec[i](1) == j && check_rows[ass_vec[i](0)] != false)
						{
							check_rows[ass_vec[i](0)] = false;
							is = true;
						}
					}
				}
			}

			/*update the costMatrix*/
			//find the minima element in cost matrix from those which have not been covered
			int tmp_min = -1;
			for (int i = 0; i < agent_num; ++i)
			{
				if (check_rows[i] == true)
					continue;

				for (int j = 0; j < target_num; ++j)
				{
					if (check_cols[j] == true)
						continue;

					if (costMatrix.at<int>(i,j) < tmp_min || tmp_min < 0)
					{
						tmp_min = costMatrix.at<int>(i,j);
					}
				}
			}
			//uncovered area subtracts the minima and cross elements add the minima
			for (int i = 0; i < agent_num; ++i)
			{
				if (check_rows[i] == true)
				{
					for (int j = 0; j < target_num; ++j)
					{
						if (check_cols[j] == true)
							costMatrix.at<int>(i,j) = costMatrix.at<int>(i,j) + tmp_min;
					}
				} else {
					for (int j = 0; j < target_num; ++j)
					{
						if (check_cols[j] == true)
							continue;

						costMatrix.at<int>(i,j) = costMatrix.at<int>(i,j) - tmp_min;
					}
				}
			}

			/*empty the ass_vec, independent zero indecies*/
			for (int i = 0; i < ass_vec.size(); ++i)
			{
				printf("independent zero : (%d, %d)\n", ass_vec[i](0), ass_vec[i](1));
			}
			ass_vec.clear();
			for (int i = 0; i < agent_num; ++i)
			{
				indep_zero_rows_index[i] = -1;
				indep_zero_cols_index[i] = -1;
			}
			/*show init*/
			for (int i = 0; i < agent_num; ++i)
			{
				for (int j = 0; j < target_num; ++j)
				{
					printf("costMatrix[%d,%d] = %d\n", i,j,costMatrix.at<int>(i,j));
				}
			}
			waitKey();
		}


		for (int i = 0; i < ass_vec.size(); ++i)
		{
			assignment.at<int>(ass_vec[i](0),ass_vec[i](1)) = 1;
			printf("assignment[%d,%d] = %d\n", ass_vec[i](0),ass_vec[i](1),assignment.at<int>(ass_vec[i](0),ass_vec[i](1)));
		}

		//printf("find() : %d\n", find(costMatrix,assignment));
		
		/*show the results*/
		/*for (int i = 0; i < agent_num; ++i)
		{
			for (int j = 0; j < target_num; ++j)
			{
				printf("assignment[%d,%d] = %d\n", i,j,assignment.at<int>(i,j));
			}
		}*/
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