//Size of the image is 640*800
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

class FindCircle
{
public:
	FindCircle();
	~FindCircle();
	
private:
	ros::NodeHandle n;
	ros::Subscriber img_sub;
	ros::Publisher circle_pub;

	Mat src_img;
	Mat src_img_resized;
	Mat mModel;

	void imageCallback(const sensor_msgs::Image &msg);
	float Color_Detection_Reverse(Mat &src, Mat &dst, 
									float h_1, float h_2, 
									float s_1, float s_2, 
									float v_1, float v_2);
};

FindCircle::FindCircle()
{
	/*subscribe image message from Cam*/
	img_sub = n.subscribe("CamImage", 1, &FindCircle::imageCallback, this);
	/*publish the information about the finded circle*/
	//circle_pub = n.advertise<geometry_msgs/Point>("circle_position",1);

	src_img_resized.create(640, 800, CV_8UC3);
	mModel = getStructuringElement(MORPH_ELLIPSE,Size(3,3), Point(2,2));
}

FindCircle::~FindCircle(){}

float FindCircle::Color_Detection_Reverse(Mat &src, Mat &dst, 
									float h_1, float h_2, 
									float s_1, float s_2, 
									float v_1, float v_2)
{
	int sum = 0;
	float percent = 0;
	/*median filter*/
	medianBlur(src, src, 5);

	/*convert src to CV_32FC3 img type*/
	Mat src_float;
	src.convertTo(src_float, CV_32FC3);
	/*convert BGR to HSV*/
	Mat hsv_img;
	hsv_img.create(src.size(), CV_32FC3);
	cvtColor(src_float, hsv_img, CV_BGR2HSV);

	/*why not convert src to hsv_img directly?*/
	//cvtColor(src, hsv_img, CV_BGR2HSV);

	int step = hsv_img.step/sizeof(float);
	int channel = hsv_img.channels();
	float * datafloat = (float *)hsv_img.data;
	for (int i = 0; i < hsv_img.rows; i++)
	{
		for (int j = 0; i < hsv_img.cols; j++)
		{
			if ((datafloat[i*step + j*channel + 2] >= v_1 && datafloat[i*step + j*channel + 2] <= v_2) && (datafloat[i*step + j*channel + 1] >= s_1 && datafloat[i*step + j*channel + 1] <= s_2) && (datafloat[i*step + j*channel] >= h_1 && datafloat[i*step + j*channel] <= h_2))
			{
				dst.data[i*(dst.step)+j*(dst.channels())] = 255;
				sum++;
			}
			else
			{
				dst.data[i*(dst.step)+j*(dst.channels())] = 0;
			}
		}
	}
	percent = 1.0*sum/(hsv_img.rows*hsv_img.cols);
	return percent;
}

void FindCircle::imageCallback(const sensor_msgs::Image &msg)
{
	/*convert ROS image msgs to Mat*/
	cv_bridge::CvImagePtr cv_ptr;
	cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
	src_img = cv_ptr->image;
	/*resize the source to a normalized size (if necessary)*/
	resize(src_img, src_img_resized, Size(src_img_resized.cols, src_img_resized.rows));
	
	/*imshow("src_img", src_img);
	imshow("src_img_resized", src_img_resized);
	waitKey(1);*/

	/*prepare to binaryzate*/
	Mat img_threshold;
	img_threshold.create(src_img_resized.size(), CV_8UC1);
	
	/*binaryzate*/
	Color_Detection_Reverse(src_img_resized, img_threshold, 5, 300, 0.5, 1, 40, 255);
	erode(img_threshold, img_threshold, mModel);
	dilate(img_threshold, img_threshold, mModel);

	/*find the contours*/
	vector< vector<Point> > contours;
	findContours(img_threshold, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	/*draw contours*/
	Mat result(src_img_resized.size(), CV_8U, Scalar(0));
	drawContours(src_img_resized, contours, -1, Scalar(255,255,255), 2);
	namedWindow("contours");
	imshow("contours", result);
}	



int main(int argc, char **argv)
{
	ros::init(argc, argv, "find_circle");
	FindCircle Find_circle;
	ros::spin();
}


