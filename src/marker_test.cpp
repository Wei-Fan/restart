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

#include <vicon_bridge/Markers.h>
#include <vicon_bridge/Marker.h>
#include <restart/pos_est.h>

using namespace cv;
using namespace std;
using namespace Eigen;

#define RADIUS_SQUARE 0.4
#define VEHICLE_SIZE 0.15
#define VEHICLE_DRIFT 0.3

vector<int> index_sequence;
int g_vehicle_num=1;
float about_edge = 1;
vector<float> x_init_pos;
vector<float> y_init_pos;
vector<float> x_marker_init;
vector<float> y_marker_init;
float amp_coeff;
void give_index(int index)
{
	index_sequence.push_back(index);
}
void clear_index()
{
	index_sequence.clear();
}

class MarkerTest
{
private:
	std::vector<ros::Publisher> m_pos_est_v;
	//ros::Publisher m_pos_est_v;
	ros::Subscriber m_viconMarkersub;
	std::vector<vicon_bridge::Marker> m_markers;
	bool isFirstVicon;
	std::vector<Vector3f> swarm_pos;
	std::vector<Vector3f> m_swarm_pos;

	restart::pos_est m_pos_estmsg;
	float z_ground;
	Mat src;
public:
	MarkerTest(ros::NodeHandle& nh)
	:m_pos_est_v(g_vehicle_num)
	{

		m_viconMarkersub = nh.subscribe<vicon_bridge::Markers>("/vicon/markers",1000,&MarkerTest::vicon_markerCallback, this);
		isFirstVicon = true;

		char msg_name[50];
		for(int i=0;i<g_vehicle_num;i++){
			sprintf(msg_name,"/vehicle%d/pos_est", i);
			m_pos_est_v[i] = nh.advertise<restart::pos_est>("/vehicle%d/pos_est",1);//msg_name, 1);
		}
			
		src = Mat(Size(1000,1000), CV_8UC3, Scalar(0));

		
	}

	~MarkerTest(){}

	/*void run(double frequency)
	{
		printf("********11\n");
		ros::spin();
	}*/

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


		for(int i=0;i<x_marker_init.size();i++){
			circle(src, Point(500-y_marker_init[i]*amp_coeff, 500+x_marker_init[i]*amp_coeff), 2, Scalar(0, 255, 0));  
	    	printf("x1 %d: %f\n", i, x_marker_init[i]);
	    	printf("y1 %d: %f\n", i, y_marker_init[i]);
    	}


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

	void unite()
	{
		vector<bool> all_union(x_marker_init.size(),0);
		for(int i=0;i<x_marker_init.size();++i)
		{
		    int within_circle = 0;
			for(int j=1;j<x_marker_init.size();++j)
			{
				if(sqrt((x_marker_init[i]-x_marker_init[j])*(x_marker_init[i]-x_marker_init[j])+(y_marker_init[i]-y_marker_init[j])*(y_marker_init[i]-y_marker_init[j]))<about_edge)
					++within_circle;
			}
			if(within_circle<3)
				all_union[i]=1;
		}
		for(int i=0;i<x_marker_init.size();++i)
		{
			if(all_union[i])continue;
			all_union[i]=1;
			vector<int> num_of_point(3,-1);
			vector<float> min_dstc(2,-1);
		    float temp_dstc;
		    for(int j=1;j<x_marker_init.size();++j)
		    {
		    	if(all_union[j])continue;
		    	temp_dstc=(x_marker_init[i]-x_marker_init[j])*(x_marker_init[i]-x_marker_init[j])+(y_marker_init[i]-y_marker_init[j])*(y_marker_init[i]-y_marker_init[j]);
		    	if(min_dstc[0]>temp_dstc||min_dstc[0]<0)
		    	{
		    		num_of_point[0]=j;
		    		min_dstc[0]=temp_dstc;
		    	}
		    }
		    all_union[num_of_point[0]]=1;
		    for(int j=1;j<x_marker_init.size();++j)
		    {
		    	if(all_union[j])continue;
		    	temp_dstc=(x_marker_init[i]-x_marker_init[j])*(x_marker_init[i]-x_marker_init[j])+(y_marker_init[i]-y_marker_init[j])*(y_marker_init[i]-y_marker_init[j]);
		    	if(min_dstc[1]>temp_dstc||min_dstc[1]<0)
		    	{
		    		num_of_point[1]=j;
		    		min_dstc[1]=temp_dstc;
		    	}
		    }
		    all_union[num_of_point[1]]=1;
		    temp_dstc=x_marker_init[num_of_point[0]]+x_marker_init[num_of_point[1]];
		    x_init_pos.push_back(temp_dstc/2);
		    temp_dstc=y_marker_init[num_of_point[0]]+y_marker_init[num_of_point[1]];
		    y_init_pos.push_back(temp_dstc/2);

		    float temp_cross_product=-9999;
		    for(int j=1;j<x_marker_init.size();++j)
		    {
		    	if(all_union[j])continue;
		    	float cross_product;
		    	cross_product=(x_marker_init[num_of_point[0]]-x_marker_init[j])*(x_marker_init[num_of_point[1]]-x_marker_init[j])+(y_marker_init[num_of_point[0]]-y_marker_init[j])*(y_marker_init[num_of_point[1]]-y_marker_init[j]);
		    	if(cross_product<temp_cross_product||temp_cross_product<0)
		    	{
		    		num_of_point[2]=j;
		    		temp_cross_product=cross_product;
		    	}
		    }
		    all_union[num_of_point[2]]=1;
		}
	}

	void vicon_markerCallback(const vicon_bridge::Markers::ConstPtr& msg)
	{	
		/*init*/
		//printf("****************received vicon_bridge, number: %d\n", msg->markers.size());
		m_markers = msg->markers;
		printf("received vicon_bridge, number: %d\n", msg->markers.size());
		if (isFirstVicon && msg->markers.size() != 0)
		{
			//printf("****************received vicon_bridge, number: %d\n", msg->markers.size());
			//printf("%d\n",msg->markers.size());
			for (auto& Marker : m_markers)
    		{		
    			//printf("hhhh");
    			Vector3f pos;
    			pos(0) = Marker.translation.x/1000.0f;
    			pos(1) = Marker.translation.y/1000.0f;
    			pos(2) = Marker.translation.z/1000.0f;

    			x_marker_init.push_back(pos(0));
				y_marker_init.push_back(pos(1));
				z_ground = pos(2);
			}

    		/*for(int i=0;i<x_init_pos.size();i++)//print markers position
    		{
		    	printf("x init%d: %f\n", i, x_init_pos[i]);
		    	printf("y init%d: %f\n", i, y_init_pos[i]);
		    	fflush(stdout);
		    }*/

		    /*Sequence initialization*/
			unite();//identify crazyflies and get their position into swarm_pos

			bool sequenceIsOk = false;
			while(!sequenceIsOk)//use mouse to rearrange index of swarm_pos
			{
				displayFunc();
				setMouseCallback("vicon_test", onMouse, &src);
				waitKey();
				destroyWindow("vicon_test");
				//printf("%d\n", index_sequence.size());
				/*check the click times and exit the initialization*/
				if(index_sequence.size()==g_vehicle_num){
					sequenceIsOk = true;
				}else{
					printf("Initialization fails!! Please click again!!\n");
					clear_index();
				}
			}
			
			//printf("%d*****%d\n", index_sequence.size(), index_sequence[0]);

			for (int i=0; i<index_sequence.size();i++)
			{
				Vector3f tmp_pos;
				tmp_pos(0) = x_init_pos[index_sequence[i]];
				tmp_pos(1) = y_init_pos[index_sequence[i]];
				tmp_pos(2) = z_ground;
				swarm_pos.push_back(tmp_pos);
				m_swarm_pos.push_back(tmp_pos);
			}
			/*for (int i = 0; i < index_sequence.size(); ++i)//print index_sequence
			{
				//printf("%d\n", 100);
				int tmp_index = index_sequence[i];
				printf("%d", tmp_index);
				Vector3f tmp;
				tmp = swarm_pos[index_sequence[i]];
				printf("%f %f\n", tmp(0), tmp(1));
				fflush(stdout);
			}*/

    		if(swarm_pos.size()==g_vehicle_num)
    			isFirstVicon = false;
		}
		
		else if (!isFirstVicon && msg->markers.size() != 0)
		{
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
	    		//printf("*****i = %d\n", i);
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
	    			//printf("*******j = %d\n", j);
	    			Vector3f consider_vec;
	    			float consider_len;
	    			consider_vec(0) = consider_pos[j](0) - consider_pos[i](0);
		    		consider_vec(1) = consider_pos[j](1) - consider_pos[i](1);
		    		consider_vec(2) = consider_pos[j](2) - consider_pos[i](2);
		    		vec3f_norm(&consider_vec, &consider_len);
	    			for (int k = 0; k < close_points.size(); ++k)
	    			{

	    				//printf("*********k = %d\n", k);
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
	    					//printf("*********oops!\n");
	    					if (ctheta < 0.2 && compare_len/consider_len < 1.05 && compare_len/consider_len > 1.05)
	    					{
	    						//printf("condition 3\n");
	    						current_pos(0) = 0.5*(consider_pos[k](0) + consider_pos[j](0));
					    		current_pos(1) = 0.5*(consider_pos[k](1) + consider_pos[j](1));
					    		current_pos(2) = 0.5*(consider_pos[k](2) + consider_pos[j](2));
	    					} else if (ctheta < 0.75 && ctheta > 0.65 && compare_len/consider_len < 1.45 && compare_len/consider_len > 1.35)
	    					{
	    						//printf("condition 2 : %f %f\n", ctheta, compare_len/consider_len);
	    						current_pos(0) = 0.5*(consider_pos[k](0) + consider_pos[i](0));
					    		current_pos(1) = 0.5*(consider_pos[k](1) + consider_pos[i](1));
					    		current_pos(2) = 0.5*(consider_pos[k](2) + consider_pos[i](2));
	    					} else if (ctheta < 0.75 && ctheta > 0.65 && consider_len/compare_len < 1.45 && consider_len/compare_len > 1.35)
	    					{
	    						//printf("condition 1 : %f %f\n", ctheta, consider_len/compare_len);
	    						current_pos(0) = 0.5*(consider_pos[j](0) + consider_pos[i](0));
					    		current_pos(1) = 0.5*(consider_pos[j](1) + consider_pos[i](1));
					    		current_pos(2) = 0.5*(consider_pos[j](2) + consider_pos[i](2));
	    					} else {
	    						continue;
	    					}
	    					//printf("*********oops!!\n");
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
			    					m_swarm_pos[l] = current_pos;
			    					printf("vehicle: %f  %f  %f\n", current_pos(0), current_pos(1), current_pos(2));
			    					//printf("*******found vehicle No.%d*********\n", l);
			    				}
	    					}//for l
	    				}//if k!=j
	    			}//for k
	    		}//for j
	    	}// for i

			/*renew and publish*/
	    	for (int i = 0; i < swarm_pos.size(); ++i)
	    	{
	    		swarm_pos[i] = m_swarm_pos[i];
	    		m_pos_estmsg.pos_est.x = swarm_pos[i](0);
				m_pos_estmsg.pos_est.y = swarm_pos[i](1);
				m_pos_estmsg.pos_est.z = swarm_pos[i](2);
				m_pos_estmsg.vehicle_index = i;
	    		m_pos_est_v[i].publish(m_pos_estmsg);
	    		//printf("vehicle%d: %f  %f  %f\n", i, swarm_pos[i](0), swarm_pos[i](1), swarm_pos[i](2));
	    	}
		}//if isFirstVicon
	}

	void vec3f_norm(const Vector3f* a, float* anwser)
	{
		*anwser = sqrtf((*a)(0)*(*a)(0) + (*a)(1)*(*a)(1) + (*a)(2)*(*a)(2));
	}
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "marker_test");
	ros::NodeHandle n;

	MarkerTest m(n);
	//m.run(50);

	ros::spin();
	return 0;
}