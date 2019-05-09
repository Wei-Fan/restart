#include "ros/ros.h"  
#include "restart/AddTwoInts.h"  
#include <cstdlib>  
  
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "add_two_ints_client");  
  if (argc != 5)  
  {  
    ROS_INFO("usage: add_two_ints_client X1 X2 Y1 Y2");  
    return 1;  
  }  
  
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<restart::AddTwoInts>("add_two_ints");  
  restart::AddTwoInts srv;  
  srv.request.a.push_back(atoll(argv[1]));  
  srv.request.a.push_back(atoll(argv[2]));  
  srv.request.b.push_back(atoll(argv[3]));  
  srv.request.b.push_back(atoll(argv[4])); 
  if (client.call(srv))  
  {  
    ROS_INFO("Sum: %ld, %ld", (long int)srv.response.sum[0], (long int)srv.response.sum[1]);  
  }  
  else  
  {  
    ROS_ERROR("Failed to call service add_two_ints");  
    return 1;  
  }  
  
  return 0;  
}