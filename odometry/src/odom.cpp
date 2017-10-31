#include "odom.h"


bool moving = false;

//ros::NodeHandle n;

//Get the current axis speed according to EKF
//void GetAxisSpeed(const mission_control::motion &msg)
//{
//}

float degToRad = 3.1416/180;
//Might me removed
void GetImuData(const nav_msgs::Odometry &msg)
{
  if (moving)
  {
    position.vx = msg.twist.twist.linear.x;
    position.vy = msg.twist.twist.linear.y;
  }
  else
  {
    position.vx = 0;
    position.vy = 0;
  }
  position.vz = msg.twist.twist.linear.z;
  position.yaw = msg.pose.pose.orientation.z * degToRad;
  position.roll = msg.pose.pose.orientation.x * degToRad;
  position.pitch = msg.pose.pose.orientation.y * degToRad;
  
  //std::cout << position.yaw << std::endl;
}

/* Get distance from seefloor with Gimmi2(camera)
void GetHeight(heightmessage &msg)
{
  z = msg;
}
*/

void MovingCallback(const mission_control::motion &msg)
{
  if (msg.x == 1)
  {
    moving = true;
    ROS_INFO("Moving is true");
  }else{
    moving = false;
  }
}
  

int main(int argc, char** argv)
{
  ros::init(argc, argv, "odometry_publisher");

  ros::NodeHandle n;
  ros::start();
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 5);
  //ros::Subscriber rotation_speed_listener = n.subscribe("orientation_for_odom", 1, GetOrientation);
  ros::Subscriber axis_speed_listener = n.subscribe("IMU_data_for_odom", 1, GetImuData);
  ros::Subscriber moving_listener = n.subscribe("moving", 1, MovingCallback);
  //ros::Time::init();
  ros::Time current_time, last_time;
  ros::Rate r(10.0);
  position.x = 0.0;
  position.y = 0.0;
  position.z = 0.0;
  position.yaw = 0.0;
  position.roll = 0.0;
  position.pitch = 0.0;

  current_time = ros::Time::now();
  last_time = ros::Time::now();

  while(n.ok())
  {
    ros::spinOnce();
    current_time = ros::Time::now();

    //Compute odometry
    double dt = (current_time - last_time).toSec();
    //delta_x distance traveled in the global X-diriction
    //vx * cos(yaw) * cos(pitch); Local X-velocity contribution to movement in Global X diriction.
    std::cout << "Odom Yaw: " << cos(position.yaw) << std::endl << "Odom velocity X: " << position.vx << std::endl << "Time: " << dt << std::endl; 
    double delta_x = (position.vx * cos(position.yaw) * cos(position.pitch) - position.vy * sin(position.yaw) * cos(position.roll))*dt;// + position.vz * sin(position.pitch) * sin(position.roll)) * dt;
    double delta_y = (position.vx * sin(position.yaw) * cos(position.pitch) + position.vy * cos(position.yaw) * cos(position.roll))*dt;// + position.vz * sin(position.pitch) * sin(position.roll)) * dt;
    std::cout << "Delta X: " << delta_x << std::endl << "Delta Y: " << delta_y << std::endl << std::endl;
    //double delta_z = (vx * sin(pitch) * sin(yaw) - vy * sin(roll) * cos(yaw) + vz * cos(roll) * cos(pitch)) * dt;
    //double delta_yaw = position.vyaw * dt;
    //double delta_roll = position.vroll * dt;
    //double delta_pitch = position.vpitch *dt;
    

    //Global position/orientation of the Naiad.
    position.x += delta_x;
    position.y += delta_y;
    //z += delta_z;
    /*position.yaw += delta_yaw;
    if (position.yaw > 360)
    {
      position.yaw = position.yaw-360;
    }*/

    //yaw = vyaw;
    //position.roll += delta_roll;
    //position.pitch += delta_pitch;
    
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    odom.pose.pose.position.x = position.x;
    odom.pose.pose.position.y = position.y;
    odom.pose.pose.position.z = position.z;
    odom.pose.pose.orientation.z = position.yaw;
    odom.pose.pose.orientation.x = position.roll;
    odom.pose.pose.orientation.y = position.pitch;
    

    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = position.vx;
    odom.twist.twist.linear.y = position.vy;
    odom.twist.twist.linear.z = position.vz;
    //odom.twist.twist.angular.z = position.vyaw;
    //odom.twist.twist.angular.y = position.vpitch;
    //odom.twist.twist.angular.x = position.vroll;

    odom_pub.publish(odom);

    last_time = current_time;
    
    r.sleep(); //Wait the correct amount of time to keep the loop timing
  }
}
