#include "ros/ros.h"
//#include "std_msgs/String.h"
#include "nav_msgs/SetMap.h"
#include "mission_control/motion.h"

float x;
float y;
float z;

nav_msgs::SetMap srv;

//void rotation_spped()
//void horizontal_speed()
void height_speed_callback(const mission_control::motion &msg)
{
  srv.request.map.info.origin.position.z = msg.z;
}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "coord_sender");
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<nav_msgs::SetMap>("coord_service");
  //ros::Subscriber rotation_speed = n.subscribe("rotation_control", 10, rotatin_speed);
  //ros::Subscriber horizontal_speed = n.subscribe("horizontal_control", 10, horizontal_speed);
  ros::Subscriber sub_height_speed = n.subscribe("height_control", 10, height_speed_callback);
  
  //nav_msgs::SetMap srv;

	//std::cin >> x;
	//std::cin >> y;
	//std::cin >> z;

	srv.request.map.info.origin.position.x = x;
	srv.request.map.info.origin.position.y = y;
	//srv.request.map.info.origin.position.z = z;

	srv.request.map.info.origin.orientation.x = x;
  srv.request.map.info.origin.orientation.y = y;
  srv.request.map.info.origin.orientation.z = z;
  
  while(ros::ok())
  {
    ros::spinOnce();
	  ROS_INFO("zPosition: %f", srv.request.map.info.origin.position.z);
    if (client.call(srv))
  	{
    	ROS_INFO("Call sucessful");
  	}
  	else
  	{
      ROS_ERROR("Failed to call service");
  	}
  }

	return 0;
}

