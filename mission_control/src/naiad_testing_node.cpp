#include "mission_control.h"


nav_msgs::Odometry msg;
//std_msgs::Int16 commandMsg;


int CheckInput()
{ 
  float inputX;
  float inputY;
  float inputAngle;
  char firstArgument;
  //print command list:
  std::cout << "Enter '0' for help" << std::endl;
  std::cin >> firstArgument;
  //int check = 0.0;
  //check = std::atoi(inputX);
  
  //switch for checking the frist input
  std::cout << firstArgument << std::endl;
  switch(firstArgument)
  {
    case '0':
      std::cout << "Enter '1' to increase the thrusters threshold" << std::endl;
      std::cout << "Enter '2' to decrease the thrusters threshold" << std::endl;
      std::cout << "Enter '3' to activate the square pattern" << std::endl;
      std::cout << "Enter '4' to insert custom coordiantes" << std::endl;
      std::cout << "Enter '5' go up" << std::endl;
      std::cout << "Enter '6' go down" << std::endl;
      std::cout << "Enter '7' to turn to a angle" << std::endl;
      std::cout << "Enter '8' to stop" << std::endl;
      std::cout << "Enter '9' to quit" << std::endl;
      break;
    case '1': 
      msg.pose.pose.position.z = 1;
      break;
    case '2':
      msg.pose.pose.position.z = 2;
      break;
    case '3':
      msg.pose.pose.position.z = 3;
      break;
    case '4':
      msg.pose.pose.position.z = 4;
      std::cout << "Enter x coordinate: ";
      std::cin >> inputX;
      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore();
        std::cout << "Not a number..." << std::endl;
        break;
      }
      std::cout << std::endl;
      std::cout << "Enter y coordinate: ";
      std::cin >> inputY;
      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore();
        std::cout << "Not a number..." << std::endl;      
        break;
      }
      msg.pose.pose.position.x = inputX;
      msg.pose.pose.position.y = inputY;

      std::cout << std::endl;
      break;
    case '5':
      msg.pose.pose.position.z = 5;
      break;
    case '6':
      msg.pose.pose.position.z = 6;
      break;
    case '7' :
      msg.pose.pose.position.z = 7;
      std::cout << "Enter a angle from -180 - 180: ";
      std::cin >> inputAngle;
      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore();
        std::cout << "Not a number..." << std::endl;
        break;
      }
      if (inputAngle > 180 || inputAngle < -180)
      {
        std::cout << "Value to small or to big..." << std::endl;
        break;
      }
      msg.pose.pose.orientation.z = inputAngle;
      break;
    case '8':
      msg.pose.pose.position.z = 8;
    case '9':
      msg.pose.pose.position.z = 9;
      std::cout << "Exitng...\n" << std::endl;
      break;
others:
      break;
  }
  
  
  /*
  switch(input[0])
  case 'q':
  case 'm':
  case 'r':
    */

  /*
  if (inputX[0] == 'q'){
    std::cout << "Exiting...\n" << std::endl;
    return -1;
  }
  else if (check == 100)
  {
    std::cout << "Enter angle: ";
    std::cin >> msg.pose.pose.orientation.z;
    msg.pose.pose.position.x = 100;
    return 1;
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
  */
}



int main(int argc, char **argv)
{
  int inputChecked;
  ros::init(argc, argv,"Naiad_test_node");
  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<nav_msgs::Odometry>("naiad_testing", 1);
  ros::Publisher quit = n.advertise<mission_control::motion>("command", 1);

  std::cout << "Enter '1' to increase the thrusters threshold" << std::endl;
  std::cout << "Enter '2' to decrease the thrusters threshold" << std::endl;
  std::cout << "Enter '3' to activate the square pattern" << std::endl;
  std::cout << "Enter '4' to insert custom coordiantes" << std::endl;
  std::cout << "Enter '5' go up" << std::endl;
  std::cout << "Enter '6' go down" << std::endl;
  std::cout << "Enter '7' to turn to a angle" << std::endl;
  std::cout << "Enter '8' to stop" << std::endl;
  std::cout << "Enter '9' to quit" << std::endl;
  
  while(ros::ok())
  {
    inputChecked = CheckInput();
    pub.publish(msg);
  }
}
