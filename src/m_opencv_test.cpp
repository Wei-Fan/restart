//Size of the image is 640*800
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "Eigen/Eigen/Eigen"
#include "Eigen/Eigen/Geometry"

#include <vicon_bridge/Markers.h>
#include <vicon_bridge/Marker.h>

using namespace cv;
using namespace std;
using namespace Eigen;

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



/*test for vicon initialization*/
int g_vehicle_num = 1;
vector<float> x_init_pos;
vector<float> y_init_pos;
vector<int> index_sequence;
float amp_coeff;
void give_index(int index)
{
	index_sequence.push_back(index);
}
void clear_index()
{
	index_sequence.clear();
}
class ViconTest
{
private:
	Mat src;
	int count;
public:
	ViconTest()
	{
		src = Mat(Size(1000,1000), CV_8UC3, Scalar(0));
		count = 0;
	}
	~ViconTest(){}
	void displayFunc()
	{
		float tmp_max = 0;
		for (int i = 0; i < x_init_pos.size(); ++i)
		{
			float tmp = sqrt(x_init_pos[i]*x_init_pos[i]+y_init_pos[i]*y_init_pos[i]);
			if (tmp_max < tmp)
				tmp_max = tmp;	
		}
		amp_coeff = 400.0f/tmp_max;
		printf("tmp_max : %f***********amp_coeff : %f\n", tmp_max, amp_coeff);

		namedWindow("vicon_test");	
		Point p1 = Point(50,50);
		Point p2 = Point(950,950);
		rectangle(src, p1, p2, CV_RGB(0, 0, 255), -1);

		for(int i=0;i<x_init_pos.size();i++){
			circle(src, Point(500-y_init_pos[i]*amp_coeff, 500+x_init_pos[i]*amp_coeff), 2, Scalar(0, 255, 0));  
	    	printf("x%d: %f\n", i, x_init_pos[i]);
	    	printf("y%d: %f\n", i, y_init_pos[i]);
    	}	
		imshow("vicon_test", src);
	}

	static void onMouse(int event, int x, int y, int, void* userInput)
	{
		if (event != EVENT_LBUTTONDOWN) return;
		Mat *img = (Mat*)userInput;
		circle(*img, Point(x, y), 10, Scalar(0, 0, 255));
		imshow("vicon_test", *img);

		float nearest_dist=-1.0f;
		int nearest_index=0;
		for(int i=0;i<x_init_pos.size();i++){
			float sq_dist=(y-x_init_pos[i]*amp_coeff-500)*(y-x_init_pos[i]*amp_coeff-500)+(x+y_init_pos[i]*amp_coeff-500)*(x+y_init_pos[i]*amp_coeff-500);
			if(sq_dist<nearest_dist||nearest_dist<0){
				nearest_dist=sq_dist;
				nearest_index=i;
			}
		}
		give_index(nearest_index);
	}

	void run(double frequency)
	{
		//ros::NodeHandle node;
		bool sequenceIsOk = false;
		while(!sequenceIsOk)
		{
			displayFunc();
			setMouseCallback("vicon_test", onMouse, &src);
			waitKey();
			destroyWindow("vicon_test");
			/*check the click times and exit the initialization*/
			if(index_sequence.size()==g_vehicle_num){
				sequenceIsOk = true;
			}else{
				printf("Initialization fails!! Please click again!!\n");
				clear_index();
			}
		}
		//ros::Timer timer = node.createTimer(ros::Duration(1.0/frequency), &ViconTest::iteration, this);
		//ros::spin();
	}

	/*void iteration(const ros::TimerEvent& e)
	{
		namedWindow("vicon_test");
		displayFunc();
		setMouseCallback("vicon_test", onMouse, &src);
		waitKey();
		//imshow("vicon_test", src);
		if (count == 3)
		{
			destroyWindow("vicon_test");
		}
	}*/
};


/*test for recognizing crazyflie*/
#define RADIUS_SQUARE 0.4
#define VEHICLE_SIZE 0.15
#define VEHICLE_DRIFT 0.1
class RecCrazy
{
private:
	std::vector<ros::Publisher> m_pos_est_v;
	ros::Subscriber m_viconMarkersub;
	std::vector<vicon_bridge::Marker> m_markers;
	bool isFirstVicon;
	std::vector<Vector3f> swarm_pos;
	std::vector<Vector3f> m_swarm_pos;
public:
	RecCrazy(ros::NodeHandle& nh)
	{
		m_viconMarkersub = nh.subscribe<vicon_bridge::Markers>("/vicon/markers",5,&RecCrazy::vicon_markerCallback, this);
		isFirstVicon = false;
	}
	~RecCrazy(){}
	
	void run(double frequency)
	{
		ros::NodeHandle node;

		ros::Timer timer = node.createTimer(ros::Duration(1.0/frequency), &RecCrazy::iteration, this);
		ros::spin();
	}

	void iteration(const ros::TimerEvent& e)
	{
		
	}

	void vicon_markerCallback(const vicon_bridge::Markers::ConstPtr& msg)
	{	
		/*init*/
		m_markers = msg->markers;
		std::vector<Vector3f> markers_pos;
		for (auto& Marker : m_markers)
    	{	
    		Vector3f pos;
    		pos(0) = Marker.translation.x/1000.0f;
    		pos(1) = Marker.translation.y/1000.0f;
    		pos(2) = Marker.translation.z/1000.0f;
    		markers_pos.push_back(pos);
    	}
    		
		/*grand wipe out*/
		std::vector<Vector3f> consider_pos;
		for (int i=0;i<markers_pos.size();i++)
    	{	
    		float norm;
    		vec3f_norm(&markers_pos[i], &norm);
    		bool isInside = true;
    		for (int j=0;j<swarm_pos.size();j++)
    		{
    			float swarm_norm;
    			vec3f_norm(&swarm_pos[j], &swarm_norm);
    			
    			if(norm-swarm_norm > RADIUS_SQUARE || swarm_norm-norm > RADIUS_SQUARE) //max circle
					isInside = false;
    		}
    		if (isInside)
    			consider_pos.push_back(markers_pos[i]);
    	}

		/*find vehicles*/
    	for (int i = 0; i < consider_pos.size(); ++i)
    	{
    		/*small wipe out*/
			std::vector<Vector3f> close_points;
    		for (int j = 0; j < consider_pos.size(); ++j)
    		{
    			if (j!=i)
    			{
    				Vector3f tmp_diff;
    				float tmp_norm;
    				tmp_diff(0) = consider_pos[i](0) - consider_pos[j](0);
    				tmp_diff(1) = consider_pos[i](1) - consider_pos[j](1);
    				tmp_diff(2) = consider_pos[i](2) - consider_pos[j](2);
    				vec3f_norm(&tmp_diff, &tmp_norm);
    				if (tmp_norm < VEHICLE_SIZE)
    					close_points.push_back(consider_pos[j]);
    			}
    		}

    		/*find vehicle center from close_points*/
    		for (int j = 0; j < close_points.size(); ++j)
    		{
    			Vector3f consider_vec;
    			float consider_len;
    			consider_vec(0) = consider_pos[j](0) - consider_pos[i](0);
	    		consider_vec(1) = consider_pos[j](1) - consider_pos[i](1);
	    		consider_vec(2) = consider_pos[j](2) - consider_pos[i](2);
	    		vec3f_norm(&consider_vec, &consider_len);
    			for (int k = 0; k < close_points.size(); ++k)
    			{
    				if (k!=j)
    				{
    					Vector3f compare_vec;
    					float compare_len;
		    			compare_vec(0) = consider_pos[k](0) - consider_pos[i](0);
			    		compare_vec(1) = consider_pos[k](1) - consider_pos[i](1);
			    		compare_vec(2) = consider_pos[k](2) - consider_pos[i](2);
	    				vec3f_norm(&compare_vec, &compare_len);
	    				float ctheta = (consider_vec(0)*compare_vec(0)+consider_vec(1)*compare_vec(1)+consider_vec(2)*compare_vec(2))/(consider_len*compare_len);
    					
    					Vector3f current_pos;
    					if (ctheta < 0.75 && ctheta > 0.65 && consider_len/compare_len < 1.45 && consider_len/compare_len > 1.35)
    					{
    						current_pos(0) = 0.5*(consider_pos[j](0) + consider_pos[i](0));
				    		current_pos(1) = 0.5*(consider_pos[j](1) + consider_pos[i](1));
				    		current_pos(2) = 0.5*(consider_pos[j](2) + consider_pos[i](2));
    					} else if (ctheta < 0.75 && ctheta > 0.65 && compare_len/consider_len < 1.45 && compare_len/consider_len > 1.35)
    					{
    						current_pos(0) = 0.5*(consider_pos[k](0) + consider_pos[i](0));
				    		current_pos(1) = 0.5*(consider_pos[k](1) + consider_pos[i](1));
				    		current_pos(2) = 0.5*(consider_pos[k](2) + consider_pos[i](2));
    					} else if (ctheta > 0.99 && compare_len/consider_len < 1.05 && compare_len/consider_len > 1.05)
    					{
    						current_pos(0) = 0.5*(consider_pos[k](0) + consider_pos[j](0));
				    		current_pos(1) = 0.5*(consider_pos[k](1) + consider_pos[j](1));
				    		current_pos(2) = 0.5*(consider_pos[k](2) + consider_pos[j](2));
    					}
    					
    					/*use last time swarm_pos to identifiy current_pos being and sequence number*/
    					for (int l = 0; l < swarm_pos.size(); ++l)
    					{
    						Vector3f tmp_diff;
		    				float tmp_norm;
		    				tmp_diff(0) = swarm_pos[l](0) - current_pos(0);
		    				tmp_diff(1) = swarm_pos[l](1) - current_pos(1);
		    				tmp_diff(2) = swarm_pos[l](2) - current_pos(2);
		    				vec3f_norm(&tmp_diff, &tmp_norm);
		    				if (tmp_norm < VEHICLE_DRIFT)
		    				{
		    					m_swarm_pos[i] = current_pos;
		    					printf("*******found vehicle No.%d*********\n", l);
		    				}
    					}
    				}
    			}
    		}
    	}

		/*renew and publish*/
    	for (int i = 0; i < swarm_pos.size(); ++i)
    	{
    		swarm_pos[i] = m_swarm_pos[i];
    	}
	}
	void vec3f_norm(const Vector3f* a, float* anwser)
	{
		*anwser = sqrtf((*a)(0)*(*a)(0) + (*a)(1)*(*a)(1) + (*a)(2)*(*a)(2));
	}
};



int main(int argc, char **argv)
{
	ros::init(argc, argv, "find_circle");
	ros::NodeHandle n;

	/*x_init_pos.push_back(-1.0);
	y_init_pos.push_back(1.0);
	x_init_pos.push_back(-0.01);
	y_init_pos.push_back(0.02);
	x_init_pos.push_back(1.1);
	y_init_pos.push_back(-1.04);

	ViconTest v;
	v.run(100);

	for (int i = 0; i < index_sequence.size(); ++i)
	{
		printf("%d\n", index_sequence[i]);
	}*/
	RecCrazy reccrazy(n);
	reccrazy.run(50);

	ros::spin();
	return 0;
}


