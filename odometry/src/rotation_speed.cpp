#include "ros/ros.h"
#include "/root/catkin_ws/devel/include/mission_control/motion.h"

/*
 * Rotation ROS node. This node is in charge of dealing with
 * the Yaw, Pitch and Roll of the vehicle. This values are 
 * then send to the odom node.
 */


mission_control::motion speed_calc_from_IMU_data()
{
  mission_control::motion msg;
  msg.yaw = 0.0;
  msg.roll = 1.6;
  msg.pitch = 2.0;

  return msg;
}


int main(int argc, char **argv)
{
  // Call ros::init before using any other part of the ROS system
  ros::init(argc, argv, "rotation_speed_calculator");

  // Main access point to communication with the ROS system
  ros::NodeHandle n;

  // Fist parameter represent the message and the second the size of the message.
  ros::Publisher pub_rotation_speed = n.advertise<mission_control::motion>("rotation_speed_for_odom", 10);
  
  // This is the message object. Fill it with data and then pubblish it.
  mission_control::motion msg;

  while(n.ok())
  {
    //ros::spinOnce() //need IMU connection
    
    //speed_calc_from_IMU_data() //need IMU connection
    
    msg = speed_calc_from_IMU_data();

    pub_rotation_speed.publish(msg);

  }


}
