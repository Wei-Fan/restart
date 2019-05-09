#include "ros/ros.h"  
#include "restart/AddTwoInts.h"  
  
bool add(restart::AddTwoInts::Request  &req,  
         restart::AddTwoInts::Response &res)  
{  
  res.sum.push_back(req.a[0] + req.b[0]); 
  res.sum.push_back(req.a[1] + req.b[1]);   
  ROS_INFO("request: x1=%ld, x2=%ld, y1=%ld, y2=%ld", (long int)req.a[0], (long int)req.a[1], (long int)req.b[0], (long int)req.b[1]);  
  ROS_INFO("sending back response: [%ld, %ld]", (long int)res.sum[0], (long int)res.sum[1]);  
  return true;  
}  
  
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "add_two_ints_server");  
  ros::NodeHandle n;  
  
  ros::ServiceServer service = n.advertiseService("add_two_ints", add);  
  ROS_INFO("Ready to add two ints.");  
  ros::spin();  
  
  return 0;  
}