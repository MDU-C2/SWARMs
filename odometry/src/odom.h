#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include </root/catkin_ws/devel/include/mission_control/motion.h>
 
#define PI 3.14159265
 
struct pos{
  double vx;
  double vy;
  double vz; //probably not needed.
  double vyaw;
  double vroll;
  double vpitch;
  double x;
  double y;
  double z;
  double yaw;
  double roll;
  double pitch;
};

pos position;

ros::Time current_time, last_time;
//Keep track of the distance travelled 
//by computing the delta time
ros::Rate r(10.0);

