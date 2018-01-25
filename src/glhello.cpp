#include <ros/ros.h>
#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <GL/glut.h>

using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "glhello");
	ros::NodeHandle n;
	//ros::Rate rate(10);
	
	/*while (ros::ok()) {
		
		
		ros::spinOnce();
		rate.sleep();
	}*/
	return 0;
}