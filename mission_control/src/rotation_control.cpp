#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "/root/catkin_ws/devel/include/mission_control/motion.h"

/* 
 * Rotation control
 */

mission_control::motion message;

void callback(const nav_msgs::Odometry::ConstPtr& msg)
{
  //callback stuff in here.
}


int main(int argc, char **argv)
{ 
  ros::init(argc, argv, "rotation_speed_publisher");
  ros::NodeHandle n;
  
  ros::Publisher pub_rotation_speed = n.advertise<mission_control::motion>("rotation_control",10);
  ros::Subscriber sub_rotation_speed = n.subscribe("odom", 10, callback);
 
  while(ros::ok())
  {
    ros::spinOnce();
    pub_rotation_speed.publish(message);
  }
}

