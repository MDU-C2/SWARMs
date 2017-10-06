#include "ros/ros.h"
#include "/root/catkin_ws/devel/include/mission_control/motion.h"
#include "nav_msgs/Odometry.h"

#define PI 3.14159264
#define ANG (10*PI)/180

/*
 * This ROS node has been created for testing purpose.
 * The the first test is used for "simulate" the 
 * drifting effect.
 */


mission_control::motion message;

mission_control::motion SpeedCalc()
{
  


  //return msg;
}

void callback(mission_control::motion msg)
{
  message.x = msg.x*0.1;
  message.y = msg.y*0.1;
  message.z = msg.z*(-0.1);

  message.yaw = msg.yaw*0.1;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "testing_tracking");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<mission_control::motion>("axis_speed_for_odom", 10);
  ros::Subscriber sub = n.subscribe("height_control", 10, callback);
  mission_control::motion msg;
  
  message.x = 0.0;
  message.y = 0.0;
  message.z = 0.0;
  
  while(n.ok())
  {
    //msg = SpeedCalc();
    ros::spinOnce();
    pub.publish(message);
  }

}
