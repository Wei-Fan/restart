#include "ros/ros.h"  
#include "restart/CountNumber.h"  
#include <thread>
#include <mutex>
  
void test(int x, int y)
{
  std::thread::id this_id = std::this_thread::get_id();
  printf("thread number ########%d is running\n",this_id);
  while(x < 1000&&ros::ok())
  {
    x++;
    ros::Duration(0.5).sleep();
    ROS_INFO("base %d : %d",y,x);

  }
}

bool add_counter(restart::CountNumber::Request  &req,  
                restart::CountNumber::Response &res)  
{  
  int count = req.a;
  res.base = count;
  //res.sum = req.a + req.b;  
  //ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);  
  //ROS_INFO("sending back response: [%ld]", (long int)res.sum);
  std::thread t(&test, res.base, res.base);
  t.detach();
  //test(count, res.base);
  return true;  
}  
  
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "/count_server");  
  ros::NodeHandle n;  
  
  ros::ServiceServer service = n.advertiseService("/count_number", add_counter);  
  ROS_INFO("Ready to count.");  
  ros::spin();  
  
  return 0;  
}