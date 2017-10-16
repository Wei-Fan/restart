#include <iostream>
#include <ros/ros.h>
#include <math.h>
using namespace std;

int main(int argc, char **argv)
{
	//use ROS to run this programe
	ros::init(argc, argv, "sM");
	ros::NodeHandle node;
	
	int a, n;
	cout << "Please put in a, n:" << endl;
	cin >> a >> n;

	int t = 0;
	for (int i = 0; i < n; i++)
	{
		t = t + pow(10,i)*(n-i);
	}

	cout << "hi! ";
	cout << "Sum = " << a*t << endl;


	return 0;
}