#include <ros/ros.h>
#include <iostream>
#include <stdio.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <opencv2/core.hpp>
// #include <opencv2/video/tracking.hpp>
// #include "opencv2/objdetect/objdetect.hpp"
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Point.h>

using namespace cv;
using namespace std;

bool isFirst = true;
bool recieve = false;
// bool init_done = false;
int p0_x=-1,p0_y=-1,p1_x=-1,p1_y=-1;

int target_x, target_y;
Mat target;
Mat target_hsv;
Rect target_rect;
Mat src_depth_img;
Mat src_rgb_img;
Mat mask;

int histSize = 200;      
float histR[] = {0,255};      
const float *histRange = histR;      
int channels[] = {0,1};     
Mat dstHist;

void depth_imagecb(const sensor_msgs::ImageConstPtr& depth_msg)
{

	cv_bridge::CvImagePtr cv_ptr;
    // sensor_msgs::Image img = *depth_msg;
    cv_ptr = cv_bridge::toCvCopy(depth_msg, sensor_msgs::image_encodings::TYPE_32FC1);
    src_depth_img = cv_ptr->image;
    
    Mat tmp_mask = Mat(src_depth_img != src_depth_img);
    src_depth_img.setTo(10,tmp_mask);
    mask = Mat::zeros(src_depth_img.size(),CV_8UC1);

    // blur(src_depth_img, src_depth_img, Size(3,3));

    int rows = src_depth_img.rows;
    int cols = src_depth_img.cols;

    for (int i = 0; i < rows; ++i)
    {
    	for (int j = 0; j < cols; ++j)
    	{
    		float t = src_depth_img.at<float>(i,j)*255/10;    		
    		if (t > 80)
    			// mask.at<uchar>(i,j) = 255;
    			mask.at<uchar>(i,j) = 255;
    		else
    			// mask.at<uchar>(i,j) = uchar(t);
    			mask.at<uchar>(i,j) = 0;
    	}
    }
    // imshow("mask",mask);
    // waitKey(0);
    // imwrite("src/restart/src/test.png",tmp_img);//test alright
    // ROS_INFO("~~~ max : %f ~~~ min : %f",max, min);
}

void rgb_imagecb(const sensor_msgs::ImageConstPtr& depth_msg)
{

	cv_bridge::CvImagePtr cv_ptr;
    // sensor_msgs::Image img = *depth_msg;
    cv_ptr = cv_bridge::toCvCopy(depth_msg, sensor_msgs::image_encodings::BGR8);
    src_rgb_img = cv_ptr->image;
    
    
    // imwrite("src/restart/src/pm_test.png",src_rgb_img);//test alright
    // ROS_INFO("~~~ max : %f ~~~ min : %f",max, min);
    recieve = true;
}

static void onMouse(int event, int x, int y, int, void* userInput)
{
	if (event != EVENT_LBUTTONDOWN && event != EVENT_LBUTTONUP) return;
	//printf("###########onMouse x : %d\n", x);
	//printf("###########onMouse y : %d\n", y);
	
	Mat *img = (Mat*)userInput;
	if (event == EVENT_LBUTTONDOWN)
	{
		p0_x = x;
		p0_y = y;
		ROS_INFO("p0 : (%d,%d)",p0_x,p0_y);
	} else if (event == EVENT_LBUTTONUP)
	{
		p1_x = x;
		p1_y = y;
		ROS_INFO("p1 : (%d,%d)",p1_x,p1_y);

		target_rect = Rect(p0_x,p0_y,abs(p1_x - p0_x),abs(p1_y - p0_y));
		Mat tmp_img = *img;
		target = tmp_img(target_rect);
		cvtColor(target,target_hsv,CV_BGR2HSV);

		calcHist(&target_hsv,2,channels,Mat(),dstHist,1,&histSize,&histRange,true,false);
		normalize(dstHist,dstHist,0,255,CV_MINMAX);
		target_x = (p0_x+p1_x)/2;
		target_y = (p0_y+p1_y)/2;
		rectangle(*img,target_rect,Scalar(0,0,255),1,1,0);
		imshow("view",*img);
	}
}

float z_predict, z_est=0.0f, z_est_old=0.0f, Rv=0.03, Rz=0.1, Kk, P_predict, P_est=0.0f;
int k_init_count = 0;
bool k_first = true;
ros::Publisher data_pub;

void markercb(const visualization_msgs::MarkerConstPtr &msg)
{
	// ROS_INFO("marker x : %f y : %f z : %f", msg->pose.position.x, msg->pose.position.y,msg->pose.position.z);
	float z_measure = msg->pose.position.z;
	if (k_first)
	{	
		z_est_old = z_est;
		z_est = z_measure;
		k_init_count++;
		if (k_init_count == 2)
		{
			k_first = false;
		}
	}else{
		
		z_predict = z_est + 1.0*(z_est-z_est_old);
		P_predict = P_est + Rv;

		Kk = P_predict/(P_predict+Rz);
		z_est_old = z_est;
		z_est = z_predict + Kk*(z_measure-z_predict);
		P_est = P_predict*(1-Kk);
		geometry_msgs::Point p;
		p.z = z_est;
		data_pub.publish(p);
	}


}

void iteration(const ros::TimerEvent& e)
{
	// ROS_INFO("iteration~~~~");
	if (!recieve)
		return;
	if (isFirst)
	{
		isFirst = false;
		Mat src_rgb_img_0 = src_rgb_img;
		bool init_done = false;
		while(!init_done)
		{
			namedWindow("view");
			imshow("view",src_rgb_img_0);
			setMouseCallback("view", onMouse, &src_rgb_img_0);
			waitKey(0);
			destroyWindow("view");
			if (p1_x>0)
			{
				init_done = true;
			}
		}
		ROS_INFO("init done ~~~");

	} else {
		/*fitering*/
		// src_rgb_img.setTo((0,0,0),mask);
	
		Mat src_hsv_img;
		Mat calcBackImage;
		cvtColor(src_rgb_img,src_hsv_img,CV_BGR2HSV);

		Rect target_rect_t = target_rect;
		Mat dstHist_t = dstHist;
		Mat target_hsv_t = target_hsv;

		calcBackProject(&src_hsv_img,2,channels,dstHist_t,calcBackImage,&histRange);
		TermCriteria criteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 0.001);
		CamShift(calcBackImage,target_rect_t,criteria);
		Mat roi_img = src_hsv_img(target_rect_t);
		target_hsv_t = src_hsv_img(target_rect_t);
		calcHist(&roi_img,2,channels,Mat(),dstHist_t,1,&histSize,&histRange);

		normalize(dstHist_t,dstHist_t,0.0,1.0,NORM_MINMAX);

		/*simply fliter*/
		float x_t = target_rect.x+target_rect.width/2-target_rect_t.x-target_rect_t.width/2;
		float y_t = target_rect.y+target_rect.height/2-target_rect_t.y-target_rect_t.height/2;
		if (sqrt(x_t*x_t+y_t*y_t)<200)
		{
			target_rect = target_rect_t;
			target_x = target_rect.x+target_rect.width/2;
			target_y = target_rect.y+target_rect.height/2;
			dstHist = dstHist_t;
			target_hsv = target_hsv_t;
			rectangle(src_rgb_img,target_rect,Scalar(255,0,0),3);
			imshow("monitor",src_rgb_img);
			waitKey(0);
			destroyWindow("monitor");
		} else {
			isFirst = true;
			ROS_INFO("former:(%d, %d) ~~~ current:(%d, %d)", target_x,target_y,target_rect_t.x+target_rect_t.width/2,target_rect_t.y+target_rect_t.height/2);
			ROS_ERROR("~~~ init again !!!");
		}
	}
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "turtlebot_test");
	ros::NodeHandle n;
	// cv::namedWindow("view");
  	// cv::startWindowThread();
	// ros::Publisher img_pub = n.advertise<sensor_msgs::Image>("TestImage", 1000);
	data_pub = n.advertise<geometry_msgs::Point>("test_kalman",1000);
	// ros::Subscriber depth_img_sub = n.subscribe<sensor_msgs::Image>("/camera/depth/image_rect", 1, depth_imagecb);
	// ros::Subscriber color_img_sub = n.subscribe<sensor_msgs::Image>("/camera/rgb/image_rect_color", 1, rgb_imagecb);
	ros::Subscriber pose_sub = n.subscribe<visualization_msgs::Marker>("/turtlebot_follower/marker",1,markercb);
	ros::Rate rate(10);
	ros::Timer timer = n.createTimer(ros::Duration(1), iteration);

	ros::spin();

	return 0;
}