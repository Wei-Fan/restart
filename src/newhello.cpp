#include <ros/ros.h>
#include <iostream>
#include <stdio.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>

using namespace cv;
using namespace std;

#define WINDOW_WIDTH 600
// void DrawEllipse(Mat, double);
// void DrawFilledCircle(Mat, Point);
// void DrawLine(Mat, Point, Point);
// bool ROI_AddImage();
// bool LinearBlending();
bool isFirst = true;
bool recieve = false;
// bool init_done = false;
int p0_x=-1,p0_y=-1,p1_x=-1,p1_y=-1;
int roi_width;
int roi_height;
int target_x, target_y;
float shift_scale = 0.0001;
Mat target;
Mat src_depth_img;
Mat src_rgb_img;
Mat mask;
void depth_imagecb(const sensor_msgs::ImageConstPtr& depth_msg)
{

	cv_bridge::CvImagePtr cv_ptr;
    // sensor_msgs::Image img = *depth_msg;
    cv_ptr = cv_bridge::toCvCopy(depth_msg, sensor_msgs::image_encodings::TYPE_32FC1);
    src_depth_img = cv_ptr->image;
    
    Mat tmp_mask = Mat(src_depth_img != src_depth_img);
    src_depth_img.setTo(10,tmp_mask);
    mask = Mat::zeros(src_depth_img.size(),CV_8UC1);

    blur(src_depth_img, src_depth_img, Size(3,3));

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
	} else if (event == EVENT_LBUTTONUP)
	{
		p1_x = x;
		p1_y = y;
		roi_width = abs(p1_x - p0_x);
		roi_height = abs(p1_y - p0_y);
		target_x = (p0_x+p1_x)/2;
		target_y = (p0_y+p1_y)/2;
		rectangle(*img,Rect(p0_x,p0_y,roi_width,roi_height),Scalar(0,0,255),1,1,0);
		imshow("view",*img);
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
		Mat src_rgb_roi = src_rgb_img(Rect(p0_x,p0_y,roi_width,roi_height));
		// imshow("roi",src_rgb_roi);
		// waitKey(0);
		// destroyWindow("roi");
		Mat src_hsv_img;
		vector<Mat> hsv_vec;
		cvtColor(src_rgb_roi,src_hsv_img,CV_BGR2HSV_FULL);
		split(src_hsv_img,hsv_vec);
		target = hsv_vec[0];

		// imshow("H",img_h);
		// waitKey(0);
		// destroyWindow("H");
		// // img_h.convertTo(img_h,CV_32F);

		// ROS_INFO("convert done ~~~");
		// int mean = 0;
		// for (int i = 0; i < img_h.rows; ++i)
		// {
		// 	for (int j = 0; j < img_h.cols; ++j)
		// 	 {
		// 	 	// ROS_INFO("%d, %d : %d",i,j,img_h.at<uchar>(i,j));
		// 	 	mean += img_h.at<uchar>(i,j);
		// 	 } 
		// }
		// mean = int(mean/(roi_width*roi_height));
		// ROS_INFO("mean : %d",mean);
	  // destroyWindow("view");
	} else {
		/*fitering*/
		src_rgb_img.setTo((0,0,0),mask);
		// imshow("after mask",src_rgb_img);
		// waitKey(0);
		// destroyWindow("after mask");
		bool stop = false;
		while(!stop)
		{
			/*obtain roi in HSV form*/
			Mat src_rgb_roi = src_rgb_img(Rect(target_x-roi_width/2,target_y-roi_height/2,roi_width,roi_height));
			Mat src_hsv_img, img_h;
			vector<Mat> hsv_vec;
			cvtColor(src_rgb_roi,src_hsv_img,CV_BGR2HSV_FULL);
			split(src_hsv_img,hsv_vec);
			img_h = hsv_vec[0];

			/*calculate the shift vector*/
			float shift_x=0.0, shift_y=0.0;
			for (int i = 0; i < img_h.rows; ++i)
			{
				for (int j = 0; j < img_h.cols; ++j)
				 {
					shift_x += (i-target_x)*(255-abs(img_h.at<uchar>(i,j)-target.at<uchar>(i,j)))*shift_scale;
				 	shift_y += (j-target_y)*(255-abs(img_h.at<uchar>(i,j)-target.at<uchar>(i,j)))*shift_scale;
				 } 
			}
			ROS_INFO("need to tune the shift_scale ~~~ shift_x : %f ~~~ shift_y : %f", shift_x, shift_y);
			if (fabs(shift_x) < 5 && fabs(shift_y) < 5)
			{
				stop = true;
			} else {
				bool x_stop = false, y_stop = false;
				if (target_x+shift_x-roi_width/2>0 && target_x+shift_x-roi_width/2<src_rgb_img.rows)
				{
					target_x += shift_x;
				} else {
					x_stop = true;
				}
				if (target_y+shift_y-roi_height/2>0 && target_y+shift_y-roi_height/2<src_rgb_img.cols)
				{
					target_y += shift_y;
				} else {
					y_stop = true;
				}
				if (x_stop && y_stop)
				{
					stop = true;
				}
			}
		}
		rectangle(src_rgb_img,Rect(target_x-roi_width/2,target_y-roi_height/2,roi_width,roi_height),Scalar(0,0,255),1,1,0);
		imshow("monitor",src_rgb_img);
		waitKey(0);
		destroyWindow("monitor");
	}

}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "test_platform");
	ros::NodeHandle n;
	// cv::namedWindow("view");
  	// cv::startWindowThread();
	// ros::Publisher img_pub = n.advertise<sensor_msgs::Image>("TestImage", 1000);
	ros::Subscriber depth_img_sub = n.subscribe<sensor_msgs::Image>("/camera/depth/image_rect", 1, depth_imagecb);
	ros::Subscriber color_img_sub = n.subscribe<sensor_msgs::Image>("/camera/rgb/image_rect_color", 1, rgb_imagecb);
	ros::Rate rate(10);
	ros::Timer timer = n.createTimer(ros::Duration(1), iteration);
    
	// while(ros::ok())
	// {
	// 	imshow("view",tmp_img);
	// 	// waitKey(0);
	// 	ros::spinOnce();
	// 	rate.sleep();
	// }
	ros::spin();

	
	// cv::destroyWindow("view");
	// while (ros::ok()) {
	// Mat source = imread("/home/wade/catkin_ws/src/restart/src/test2.png");
	// if (source.empty())
	// {
	// 	cout << "cannot find the source image!" << endl;
	// 	return -1;
	// }

	// imshow("beforePub", source);
	// waitKey(0);

	// // 	cv_bridge::CvImage img_bridge;
	// // 	sensor_msgs::Image img_msg;

	// // 	std_msgs::Header header;
	// // 	header.stamp = ros::Time::now();
	// // 	img_bridge = cv_bridge::CvImage(header, sensor_msgs::image_encodings::RGB8, source);
	// // 	img_bridge.toImageMsg(img_msg);
	// // 	img_pub.publish(img_msg);
		
	// // 	ros::spinOnce();
	// // 	rate.sleep();
	// }
	
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