#include <ros/ros.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc,argv, "find_contours");
	ros::NodeHandle node;

	/*read in source image and resize it*/
	Mat src_img = imread("/home/wade/catkin_ws/src/restart/src/HA.jpg", 1);
	if (src_img.empty())
	{
		cout << "cannot find the source image!" << endl;
		return -1;
	}
	namedWindow("init_show");
	imshow("init_show", src_img);
	waitKey(0);
	resize(src_img, src_img, Size(640, 800));
	
	/*primarily image processing*/
	Mat gray;
	cvtColor(src_img, gray, CV_BGR2GRAY);
	GaussianBlur(gray, gray, Size(3,3), 3, 3);
	namedWindow("gray_show");
	imshow("gray_show", gray);
	waitKey(0);

	/*get the threshold image*/
	Mat ths;
	threshold(gray, ths, 100, 255, THRESH_OTSU);
	namedWindow("ths_show");
	imshow("ths_show", ths);
	waitKey(0);

	/*find the contours*/
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	findContours(ths, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	waitKey(0);

	/*erase unqualified contours*/
	vector<vector<Point> >::iterator it = contours.begin();
	while (it != contours.end())
	{
		if (contourArea(*it, true) < 1000)
			it = contours.erase(it);
		else
			++it;
	}

	/*find the bounding rectangle*/
	Mat rst = Mat::zeros(src_img.size(), CV_8UC3);
	//drawContours(rst, contours, -1, Scalar(255,0,255));

	Rect rb = boundingRect(Mat(contours[1]));
	rectangle(rst, rb, Scalar(0,255,0), 3);

	namedWindow("rst_show");
	imshow("rst_show", rst);
	waitKey(0);

	/*publish the position message*/
	Point midP;
	midP = 0.5*(rb.tl() + rb.br());

	cout<< contours.size() << endl;
	cout<< midP.x << ", "<< midP.y << endl;
	ros::spin();
	return 0;
}