#include "ros/ros.h"
#include "/root/catkin_ws/devel/include/mission_control/motion.h"

/*
 * Axis ROS node. This node is in charge of dealing with
 * the x, y and z coordinates. This values are then send
 * to the odom node.
 */


mission_control::motion speed_calc_from_IMU_data()
{
  mission_control::motion msg;
  msg.x = 0.0;
  msg.y = 0.0;
  msg.z = 0.0;

  return msg;
}


int main(int argc, char **argv)
{
  // Call ros::init before using any other part of the ROS system
  ros::init(argc, argv, "axis_speed_calculator");

  // Main access point to communication with the ROS system.
  ros::NodeHandle n;

  // First parameter represent the message and the second the size of the message. 
  ros::Publisher pub_axis_speed = n.advertise<mission_control::motion>("axis_speed_for_odom", 10);
  
 // This is the message object. Fill it with data and then pubblish it.
  mission_control::motion msg;

  while(n.ok())
  {
    //ros::spinOnce() //need IMU connection
    
    //speed_calc_from_IMU_data() //need IMU connection
    
    msg = speed_calc_from_IMU_data();

    pub_axis_speed.publish(msg);

  }


}
