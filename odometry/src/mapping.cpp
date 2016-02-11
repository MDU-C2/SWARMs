#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include <iostream>
#include <fstream>


std::ofstream myfile;

void mapping(const nav_msgs::Odometry::ConstPtr& msg)
{
  std::cout << "Mapping" << std::endl;
  myfile << msg->pose.pose.position.x << " " << msg->pose.pose.position.y << " " << msg->pose.pose.orientation.z << " " << msg->twist.twist.linear.x << " " << msg->twist.twist.linear.y << std::endl;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "odom_listener_map");
  myfile.open("tracked_coordinates.txt");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("odom", 1000, mapping);

  ros::spin();
}
