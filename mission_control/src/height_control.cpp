#include "ros/ros.h"
#include "mission_control/motion.h"
#include "nav_msgs/Odometry.h"

mission_control::motion message;
float desiered_height = 2.0;

void callback(const nav_msgs::Odometry::ConstPtr& msg)
{
   message.z = -2*pow((desiered_height-msg->pose.pose.position.z), 3);

   ROS_INFO("%f %f", message.z, msg->pose.pose.position.z);
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "height_speed_publisher");
  ros::NodeHandle n;
  
  ros::Publisher pub_height_speed = n.advertise<mission_control::motion>("height_control", 10);
  ros::Subscriber sub_heght_speed = n.subscribe("odom", 10, callback);

  while(ros::ok())
  {
    ros::spinOnce();
    pub_height_speed.publish(message);
  }
}
