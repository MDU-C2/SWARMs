#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "/root/catkin_ws/devel/include/mission_control/motion.h"

mission_control::motion message;
/*int waypoint_b_x = 50;
int waypoint_b_y = 0;

struct current_waypoint{
  int x;
  int y;
};

struct previus_waypoint{
  int x;
  int y;
};*/

struct position{
  int x;
  int y;
};

position desiered_position;

void callback(const nav_msgs::Odometry::ConstPtr& msg)
{
  if (msg->pose.pose.position.x < desiered_position.x) //Check and ajust x-thrust
    message.x = 10;
  if (msg->pose.pose.position.y < desiered_position.y) //Chech and ajust y-thrust
    message.y = 10;

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "horizontal_speed_publisher");

  ros::NodeHandle n;

  ros::Publisher pub_horizontal_speed = n.advertise<mission_control::motion>("horizontal_control", 10);
  ros::Subscriber sub_horizontal_speed = n.subscribe("odom", 10, callback);

  while(ros::ok())
  {
    ros::spinOnce();
    pub_horizontal_speed.publish(message);
  }
}
