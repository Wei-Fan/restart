#include "ros/ros.h"  
#include "restart/CountNumber.h"  
#include <cstdlib>  
  
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "/count_client",ros::init_options::AnonymousName);  
  /*if (argc != 3)  
  {  
    ROS_INFO("usage: add_two_ints_client X Y");  
    return 1;  
  }*/  
  
  ros::NodeHandle n("~");  
  ros::ServiceClient client = n.serviceClient<restart::CountNumber>("/count_number");  
  client.waitForExistence();
  int base_number = -1;

  restart::CountNumber srv; 
  n.getParam("base_num", base_number);
  srv.request.a = base_number;
  //srv.request.b = atoll(argv[2]);  
  if (client.call(srv))  
  {  
    ROS_INFO("Index: %ld", (long int)srv.response.base);  
  }  
  else  
  {  
    ROS_ERROR("Failed to call service count_number");  
    return 1;  
  }  
  
  return 0;  
}