#include "ros/ros.h"  
#include "yaml-cpp/yaml.h" 
#include <string>

#include <iostream>
#include <fstream>  
  
using namespace std;

int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "yaml_test");  

  ros::NodeHandle n;  
  
  string file_path = "home/wade/catkin_ws/src/restart/src/test.yaml";
  YAML::Node yamlConfig = YAML::LoadFile(file_path);
  int int_param = yamlConfig["num_robots"].as<int>();
  cout << "number of robots: " << int_param << endl;
  string str_param = yamlConfig["behaviour_lib"].as<string>();
  cout << str_param << endl;
  yamlConfig["connectivity_radius"] = 30; 
  ofstream file;
  file.open(file_path);
  file.flush();
  file << yamlConfig;
  file.close();
  
  
  return 0;  
}