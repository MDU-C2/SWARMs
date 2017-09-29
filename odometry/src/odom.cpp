#include "ros/ros.h"
#include "tf/transform_broadcaster.h"
#include "nav_msgs/Odometry.h"
#include </root/catkin_ws/devel/include/mission_control/motion.h>

double vx = 0.3;
double vy = -0.3;
double vz = 0.0;
double vyaw = 0.1;
double vroll = 0.0;
double vpitch = 0.0;

void GetSpeed(const mission_control::motion &msg)
{
  vx = msg.x;
  vy = msg.y;
  vz = msg.z;
  vyaw = msg.yaw;
  vpitch = msg.pitch;
  vroll = msg.roll;

}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "odometry_publisher");

  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  ros::Subscriber rotation_speed_listener = n.subscribe("rotation_speed_for_odom", 1, GetSpeed);
  ros::Subscriber axis_speed_listener = n.subscribe("axis_speed_for_odom", 1, GetSpeed);
  //tf::TransformBroadcaster odom_broadcaster;

  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
  double yaw = 0.0;
  double roll = 0.0;
  double pitch = 0.0;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(100.0);

  while(n.ok())
  {
    ros::spinOnce();
    current_time = ros::Time::now();

    //Compute odometry
    double dt = (current_time - last_time).toSec();
    //delta_x distance traveled in the global X-diriction
    //vx * cos(yaw) * cos(pitch); Local X-velocity contribution to movement in Global X diriction. 
    double delta_x = (vx * cos(yaw) * cos(pitch) - vy * sin(yaw) * cos(roll) + vz * sin(pitch) * sin(roll)) * dt;
    double delta_y = (vx * sin(yaw) * cos(pitch) + vy * cos(yaw) * cos(roll) + vz * sin(pitch) * sin(roll)) * dt;
    double delta_z = (vx * sin(pitch) * sin(yaw) - vy * sin(roll) * cos(yaw) + vz * cos(roll) * cos(pitch)) * dt;
    double delta_yaw = vyaw * dt;
    double delta_roll = vroll * dt;
    double delta_pitch = vpitch *dt;
    

    //Global position/orientation of the Naiad.
    x += delta_x;
    y += delta_y;
    z += delta_z;
    yaw += delta_yaw;
    roll += delta_roll;
    pitch += delta_pitch;

    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(yaw);

    //geometry_msgs::TransformStamped odom_trans;
    //odom_trans.header.stamp = current_time;
    //odom_trans.header.frame_id = "odom";
    //odom_trans.child_frame_id = "base_link";
    
    //odom_trans.transform.translation.x = x;
    //odom_trans.transform.translation.y = y;
    //odom_trans.transform.translation.z = 0.0;
    //odom_trans.transform.rotation = odom_quat;

    //odom_broadcaster.sendTransform(odom_trans);

    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = z;
    odom.pose.pose.orientation = odom_quat;

    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.linear.z = vz;
    odom.twist.twist.angular.z = vyaw;
    odom.twist.twist.angular.y = vpitch;
    odom.twist.twist.angular.x = vroll;

    odom_pub.publish(odom);

    last_time = current_time;
    r.sleep();
  }
}
