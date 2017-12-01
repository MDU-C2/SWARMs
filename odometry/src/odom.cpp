#include "odom.h"


bool moving = false;

//ros::NodeHandle n;:

//Get the current axis speed according to EKF
//void GetAxisSpeed(const mission_control::motion &msg)
//{
//}

float degToRad = 3.1416/180;
//Might me removed
void GetImuData(const nav_msgs::Odometry &msg)
{
  double dt = 0.0;//(current_time - last_time).toSec();
  double deltaX = 0.0;
  double deltaY = 0.0;

  std::cout << "Odom twist: " << msg.twist.twist.linear.x << " " << msg.twist.twist.linear.y << std::endl;
  
  if (!moving)
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
  std::cout << "Odom pos yaw: " << position.yaw << " " << msg.pose.pose.orientation.z << " " << position.yaw << " " << cos(position.yaw) << " " << cos(position.pitch) << std::endl; 
  current_time = ros::Time::now();
  dt = (current_time - last_time).toSec();
  deltaX = (position.vx * cos(position.yaw) * cos(position.pitch))*dt; // - position.vy * sin(position.yaw) * cos(position.roll))*dt;// + position.vz * sin(position.pitch) * sin(position.roll)) * dt;
  deltaY = (position.vx * sin(position.yaw) * cos(position.pitch))*dt; // + position.vy * cos(position.yaw) * cos(position.roll))*dt;// + position.vz * sin(position.pitch) * sin(position.roll)) * dt;

  position.x += deltaX;
  position.y += deltaY;

  std::cout << "Pos: " << position.x << " " << position.y << " DT: " << dt << std::endl;
  last_time = ros::Time::now();
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

void ResetCallback(const mission_control::motion &qMsg){
  // received a message to reset
  if(qMsg.x ==  1) {
    position.vx = 0.0;
    position.vy = 0.0;
    position.vz = 0.0;
    position.vyaw = 0.0;
    position.vroll = 0.0;
    position.vpitch = 0.0;
    position.x = 0.0;
    position.y = 0.0;
    position.z = 0.0;
    position.yaw = 0.0;
    position.roll = 0.0;
    position.pitch = 0.0;
    ROS_INFO("Everything has been reset!");;
  }
}
  

int main(int argc, char** argv)
{
  ros::init(argc, argv, "odometry_publisher");

  ros::NodeHandle n;
  ros::start();
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 5);
  ros::Subscriber axis_speed_listener = n.subscribe("IMU_data_for_odom", 1, GetImuData);
  ros::Subscriber moving_listener = n.subscribe("moving", 1, MovingCallback);
  ros::Subscriber position_reset = n.subscribe("qMsg",1, ResetCallback); 
  ros::Time current_time, last_time;
  position.x = 0.0;
  position.y = 0.0;
  position.z = 0.0;
  position.yaw = 0.0;
  position.roll = 0.0;
  position.pitch = 0.0;


  current_time = ros::Time::now();
  last_time = ros::Time::now();

  nav_msgs::Odometry odom;

  while(n.ok())
  {
    ros::spinOnce();

    

    //Global position of the Naiad.
    //ROS_INFO("X: %f, Y: %f", position.x, position.y); 

    //Odom message creation.  
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

    odom_pub.publish(odom);

    last_time = current_time;
    


    //r.sleep(); //Wait the correct amount of time to keep the loop timing
  }
}
