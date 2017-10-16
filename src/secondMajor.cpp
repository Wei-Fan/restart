#include <iostream>
#include <ros/ros.h>
#include </home/wade/catkin_ws/src/restart/src/secondMajor.h>
using namespace std;
int m_sum(int n){
	int s;
	if (n%2 == 0)
		s = -n/2;
	else
		s = (n+1)/2;
	return s;
}
int main(int argc, char **argv)
{
	//use ROS to run this programe
	ros::init(argc, argv, "secondMajor");
	ros::NodeHandle node;

	cout << "Please type in a number:";
	int N;
	cin >> N;
	cout << "Result = " << m_sum(N) << endl;
	return 0;
}