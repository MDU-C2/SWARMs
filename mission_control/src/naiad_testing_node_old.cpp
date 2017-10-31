#include "mission_control.h"


nav_msgs::Odometry msg;


void CheckInput()
{ 
  std::string input;
  std::cout << "Enter 'q' to quit" << std::endl << "Enter goal for X: " << std::endl;
  std::cin >> input;
  if (input[0] == 'q'){
    std::cout << "Exiting..." << std::endl;
  }
  else{
    msg.pose.pose.position.x = std::atoi(input.c_str());
  }
  std::cout << "Enter goal for Y: " << std::endl;
  std::cin >> input;
  if (input[0] == 'q'){
    std::cout << "Exit .. " << std::endl;
  }
  else{
    msg.pose.pose.position.y = std::atoi(input.c_str());
  }
  std::cout << msg.pose.pose.position.y;
}



int main(int argc, char **argv)
{
  ros::init(argc, argv,"Naiad_test_node");
  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<nav_msgs::Odometry>("naiad_testing", 1);

  while(ros::ok())
  {
    CheckInput();
    //ros::spinOnce();
    pub.publish(msg);
  }
}
