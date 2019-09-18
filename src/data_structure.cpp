#include <iostream>
#include <ros/ros.h>
#include </home/wade/catkin_ws/src/restart/src/data_structure.h>
using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "data_structure");
	ros::NodeHandle node;

	int a[9] = {2,3,1,5,63,7,74,2,4};
	int b[9] = {2,6,3,3,62,7,714,64,23};
	CLinkList<int> x(a,9);
	CLinkList<int> y(b,9);

//	x.add(y);
//	x.printList();
//    LinkList<int> x(a,9);
//    x.printList();
//    cout<<x.size()<<endl;
//    x.clear();
//    cout<<x.size()<<endl;


	return 0;
}