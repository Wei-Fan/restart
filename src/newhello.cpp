#include <ros/ros.h>
#include <iostream>
#include <stdio.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

#define WINDOW_WIDTH 600
void DrawEllipse(Mat, double);
void DrawFilledCircle(Mat, Point);
void DrawLine(Mat, Point, Point);
bool ROI_AddImage();
bool LinearBlending();

int main(int argc, char **argv)
{
	ros::init(argc, argv, "test_platform");
	ros::NodeHandle n;
	ros::Publisher img_pub = n.advertise<sensor_msgs::Image>("TestImage", 1000);
	ros::Rate rate(10);
	
	while (ros::ok()) {
		Mat source = imread("/home/wade/catkin_ws/src/restart/src/HA.jpg");
		if (source.empty())
		{
			cout << "cannot find the source image!" << endl;
			return -1;
		}

		imshow("beforePub", source);
		waitKey(0);

		cv_bridge::CvImage img_bridge;
		sensor_msgs::Image img_msg;

		std_msgs::Header header;
		header.stamp = ros::Time::now();
		img_bridge = cv_bridge::CvImage(header, sensor_msgs::image_encodings::RGB8, source);
		img_bridge.toImageMsg(img_msg);
		img_pub.publish(img_msg);
		
		ros::spinOnce();
		rate.sleep();
	}
	
	/*3 channels matrix (RGB image)*/
	/*
	Mat M(3, 2, CV_8UC3, Scalar(0, 0, 255));
	cout << M << enl;
	*/

	/*Array Object*/
	/*
	Vec3b color;
	color[0] = 255;
	color[1] = 0;
	color[3] = 0;
	cout << color;
	*/

	/*ergodic method 1 (run every pixels in a image)*/
	/*
	Mat grayim(600, 800, CV_8UC1);
	Mat colorim(600, 800, CV_8UC3);

	for (int i = 0; i < grayim.rows; ++i)
	{
		for (int j = 0; j < grayim.cols; ++j)
		{
			grayim.at<uchar>(i, j) = (i - j) % 255;
		}
	}

	for (int i = 0; i < colorim.rows; ++i)
	{
		for (int j = 0; j < colorim.cols; ++j)
		{
			Vec3b pixel;
			pixel[0] = i % 255; //blue
			pixel[1] = j % 255; //green
			pixel[2] = 0; //red
			colorim.at<Vec3b>(i, j) = pixel;
		}
	}

	imshow("grayim", grayim);
	imshow("colorim", colorim);
	waitKey(0);
	*/

	/*ergodic method 2*/
	/*
	Mat grayim(600, 800, CV_8UC1);
	Mat colorim(600, 800, CV_8UC3);

	MatIterator_<uchar> grayit, grayend;
	for (grayit = grayim.begin<uchar>(), 
		grayend = grayim.end<uchar>(); 
		grayit != grayend; ++grayit)
	{
		*grayit = rand() % 255;
	}

	MatIterator_<Vec3b> colorit, colorend;
	for (colorit = colorim.begin<Vec3b>(),
		colorend = colorim.end<Vec3b>();
		colorit != colorend; ++colorit)
	{
		(*colorit)[0] = rand() % 255; //Blue
		(*colorit)[1] = rand() % 255; //Green
		(*colorit)[2] = rand() % 255; //Red
	}

	imshow("grayim", grayim);
	imshow("colorim", colorim);
	waitKey(0);
	*/

	/*Range (select certain rows or columns)*/
	/*
	Mat A = Mat::eye(10, 10, CV_32S);
	Mat B = A(Range::all(), Range(1, 3));
	Mat C = B(Range(5, 9), Range::all());

	cout << B << endl;
	cout << C << endl;
	*/

	/*extract region of interest*/
	/*
	Mat img(Size(600, 800), CV_8UC3);
	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j < img.cols; ++j)
		{
			Vec3b pixel;
			pixel[0] = i % 255; //blue
			pixel[1] = j % 255; //green
			pixel[2] = 0; //red
			img.at<Vec3b>(i, j) = pixel;
		}
	}

	Mat roi(img, Rect(10, 10, 100, 100));
	Mat roi2(img, Range(10, 100), Range(10, 100));
	Mat roi3 = img(Rect(10, 10, 100, 100));
	Mat roi4 = img(Range(10, 100), Range(10, 100));

	imshow("img", img);
	imshow("roi", roi);
	imshow("roi2", roi2);
	imshow("roi3", roi3);
	imshow("roi4", roi4);
	waitKey(0);
	*/
	
	/*Canny an image (read and write)*/
	/*
	Mat im = imread("/home/wade/catkin_ws/src/restart/src/Lee.jpg");
	if (im.empty())
	{
		ROS_WARN_STREAM("Cannot load the image~");
		return -1;
	}

	Mat result;
	Canny(im, result, 50, 150);

	imwrite("/home/wade/catkin_ws/src/restart/src/Oh.jpg", result);
	*/

	/*write a video*/
	/*
	Size s(320, 240);
	VideoWriter writer = VideoWriter("/home/wade/catkin_ws/src/restart/src/myvideo.avi",
		CV_FOURCC('M', 'J', 'P', 'G'), 25, s);

	if (!writer.isOpened())
	{
		cerr << "Cannot create video file" << endl;
		return -1;
	}

	Mat frame(s, CV_8UC3);

	for (int i = 0; i < 100; i++)
	{
		frame = Scalar::all(0);
		char text[128];
		snprintf(text, sizeof(text), "%d", i);

		putText(frame, text, Point(s.width/3, s.height/3),
			FONT_HERSHEY_SCRIPT_SIMPLEX, 3,
			Scalar(0, 0, 255), 3, 8);
		
		writer << frame;
	}	
	*/

	/*read a video from cam*/
	/*
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cerr << "Cannot open a cam~" << endl;
		return -1;
	}

	Mat edges;
	namedWindow("edges", 1);

	while (ros::ok())
	{
		Mat frame;
		cap >> frame;
		if (frame.empty())
		{
			break;
		}
		cvtColor(frame, edges, CV_BGR2GRAY);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (char(waitKey(1)) == 's')
		{
			imwrite("/home/wade/catkin_ws/src/restart/src/HA.jpg", frame);
			cout << "I have already save it!" << endl;
		}
		rate.sleep();
	}
	*/

	/*erode, blur and canny*/
	/*
	Mat srcImage = imread("/home/wade/catkin_ws/src/restart/src/ball.png");
	imshow("origin", srcImage);*/
	/*Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat dstImage;
	erode(srcImage, dstImage, element);
	imshow("erode", dstImage);*/
	/*
	Mat dstImage1, dstImage2;
	blur(srcImage, dstImage1, Size(7, 7));
	blur(srcImage, dstImage2, Size(3, 3));
	imshow("blur1", dstImage1);
	imshow("blur2", dstImage2);

	Mat dstImage, edge, grayImage;
	dstImage.create(srcImage.size(), srcImage.type());
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);

	blur(grayImage, edge, Size(3, 3));
	Canny(edge, edge, 3, 9, 3);

	imshow("border detect", edge);

	waitKey(0);
	*/
	
	/*Draw picture*/
	/*
	Mat atomImage = Mat::zeros(WINDOW_WIDTH, WINDOW_WIDTH, CV_8UC3);
	Mat rookImage = Mat::zeros(WINDOW_WIDTH, WINDOW_WIDTH, CV_8UC3);

	DrawEllipse(atomImage, 90);
	DrawEllipse(atomImage, 0);
	DrawEllipse(atomImage, 45);
	DrawEllipse(atomImage, -45);

	imshow("NO.1", atomImage);
	//moveWindow("NO.1", 0, 200);*/

	/*select ROI and insert an image*/
	/*
	ROI_AddImage();*/

	/**/
	/*
	LinearBlending();
	waitKey(0);*/

	return 0;
}

void DrawEllipse(Mat img, double angle)
{
	int thickness = 2;
	int lineType = 8;

	ellipse(img,
		Point(WINDOW_WIDTH/2, WINDOW_WIDTH/2),
		Size(WINDOW_WIDTH/4, WINDOW_WIDTH/16),
		angle,
		0,
		360,
		Scalar(255, 129, 0),
		thickness,
		lineType);
}

void DrawFilledCircle(Mat img, Point center)
{
	int thickness = -1;
	int lineType = 8;

	circle(img,
		center,
		WINDOW_WIDTH/32,
		Scalar(0, 0, 255),
		thickness,
		lineType);
}

void DrawLine(Mat img, Point start, Point end)
{
	int thickness = 2;
	int lineType = 8;

	line(img,
		start,
		end,
		Scalar(0, 0, 0),
		thickness,
		lineType);
}

bool ROI_AddImage()
{
	Mat srcImage = imread("/home/wade/catkin_ws/src/restart/src/Oh.jpg");
	Mat logoImage = imread("/home/wade/catkin_ws/src/restart/src/ball.png");
	if (!srcImage.data)
	{
		ROS_WARN_STREAM("cannot read srcImage!");
		return false;
	}
	if (!logoImage.data)
	{
		ROS_WARN_STREAM("cannot read logoImage!");
		return false;
	}

	Mat imageROI = srcImage(Rect(200, 250, logoImage.cols, logoImage.rows));
	Mat mask = imread("/home/wade/catkin_ws/src/restart/src/ball.png", 0);

	logoImage.copyTo(imageROI, mask);

	imshow("mask", mask);
	imshow("imageROI", imageROI);
	imshow("ROI for superposition", srcImage);

	return true;
}

bool LinearBlending()
{
	double alphaValue = 0.5;
	double betaValue;
	Mat srcImage1, srcImage2, dstImage;

	srcImage1 = imread("/home/wade/catkin_ws/src/restart/src/ball.png");
	srcImage2 = Mat::zeros(srcImage1.rows, srcImage1.cols, CV_8UC3);

	if (!srcImage1.data)
	{
		ROS_WARN_STREAM("cannot read srcImage1!");
		return false;
	}

	betaValue = (1.0 - alphaValue);
	addWeighted(srcImage1, alphaValue, srcImage2, betaValue, 0.0, dstImage);

	imshow("1", srcImage1);
	imshow("2", srcImage2);
	imshow("mix", dstImage);

	return true;
}