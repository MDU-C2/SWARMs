#include "mission_control.h"


nav_msgs::Odometry msg;
mission_control::motion quitMsg;


int CheckInput()
{ 
  std::string inputX;
  std::string inputY;
  std::cout << "Enter 'q' to quit" << std::endl << "Enter goal for X: " << std::endl;
  std::cin >> inputX;
  if (inputX[0] == 'q'){
    std::cout << "Exiting...\n" << std::endl;
    return -1;
  }
  else if (abs(std::atoi(inputX.c_str())) < MAX_DIST){
    msg.pose.pose.position.x = std::atoi(inputX.c_str());
    std::cout << "Enter goal for Y: " << std::endl;
    std::cin >> inputY;
    if (inputY[0] == 'q'){
      std::cout << "Exit ..\n" << std::endl;
      return -1;
    }
    else if(abs(std::atoi(inputY.c_str())) < MAX_DIST){
      msg.pose.pose.position.y = std::atoi(inputY.c_str());
      return 1;
    }
    else {
      std::cout << "Invalid input y!" << std::endl;
      return 0;
    }
  }
  else {   
    std::cout << "Invalid input x!" << std::endl;
    return 0;
  }
}



int main(int argc, char **argv)
{
  int inputChecked;
  ros::init(argc, argv,"Naiad_test_node");
  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<nav_msgs::Odometry>("naiad_testing", 1);
  ros::Publisher quit = n.advertise<mission_control::motion>("go_to_surface", 1);

  while(ros::ok())
  {
    inputChecked = CheckInput();
    if(inputChecked == 1){
      pub.publish(msg);
    // If the quit is called
    } else if(inputChecked < 0) {
      quit.publish(quitMsg);
    }
  }
}
