//-------------------------------
// Mission Control
// Last Modified: 15.11.17
// Extra notes:
//-------------------------------


#include "mission_control.h"
mission_control::motion movingMsg;
std_msgs::String controlMsg;
bool gotGoal = false;
bool onlyAngle = false;
bool square = false;
float height = 0.0;
char buffer[BUFFER_SIZE];


// Set the goal for the NAIAD in 3D coordinates. This will consider also yaw, roll and pitch.
void SetGoal(float x, float y, float z, float yaw, float roll, float pitch)
{
  goal.x = x;
  goal.y = y;
  goal.z = z;
  goal.yaw = yaw;
  goal.roll = roll;
  goal.pitch = pitch;
}

// Set the goal for the NAIAD in 2D coordinates. This will only consider x, y and z axis.
void SetGoal(float x, float y, float z)
{
  goal.x = x;
  goal.y = y;
  goal.z = z;
  ROS_INFO("New goal has been set. (%f, %f, %f)", x,y,z);
  gotGoal = true;
}

// Set the position of the NAIAD in 3D coordinates.
void SetPos(float x, float y, float z, float yaw, float roll, float pitch)
{
  pos.x = x;
  pos.y = y;
  pos.z = z;
  pos.yaw = yaw*180/PI; //conversion from radiant to degree.
  pos.roll = roll;
  pos.pitch = pitch;
}

// TODO REFACTOR THIS FUNCTION!!!
// comment in here.
float CalculateYawAngle()
{
  //ROS_INFO("Goal: %f, %f, %f", goal.x, goal.y, goal.z);
  //ROS_INFO("Position: %f, %f, %f", pos.x, pos.y, pos.z);
  
  float angleToGoal = 0.0;
  float x = goal.x - pos.x;
  float y = goal.y - pos.y;
  angleToGoal = (atan2(y,x) * 180/PI);// -90) * -1);

  std::cout << "Angle to goal: " << angleToGoal << std::endl << "Pos.yaw: " << pos.yaw << std::endl;
  float deltaAngle = angleToGoal - (pos.yaw); 
  //Calculate the angle to the goal and make sure the Naiad is pointed in the right direction before moving forward.
  if (deltaAngle > 180)
  {
    deltaAngle = deltaAngle - 360;
  }
  else if (deltaAngle < -180)
  {
    deltaAngle = deltaAngle + 360;
  }

  if (deltaAngle > 170 || deltaAngle < -170)
  {
    //move forward
    message.yaw = 500;
  }
  else //if (angleToGoal > pos.yaw)
  {
    //turn counterclockwise
    //if (deltaAngle > 10)
   // {
      //message.yaw = pos.yaw+10;
      //message.yaw = angleToGoal;
  
    //}
   // else if (deltaAngle < -10)
    //{
     // message.yaw = pos.yaw-10;
   // }
    //else
   // {
      message.yaw = angleToGoal;
   // }
  }

  return deltaAngle;
  /*else {
    //turn clockwise
    message.yaw = -100;
  }*/
  //float angle = atan2(y,x) - atan2(pos.y, pos.x) * 180/PI; //Need the current angle? Dont think so
  //ROS_INFO("Angle to go in: %f", angle);
  //ROS_INFO("Angle pointing in: %f", pos.yaw);
  //return angle;
}

// This function is usded to control the NAIAD in order to keep it at a desired distance from the sea-floor.
// Check the documentation for better information about this function. 
float HeightControl()
{
  float changeInHeight;
  changeInHeight = -2*(pow((desiered_height-pos.z), 3))+((desiered_height-pos.z)/3);
  if (changeInHeight > 20)
  {
   changeInHeight = 20;
  }else if( changeInHeight < -20)
  {
   changeInHeight = -20;
  } 
  //ROS_INFO("HeightControl: %f", changeInHeight);
  
  return changeInHeight;
}

float CalculateXThrust()
{
  //Update Yaw-Thrust
  float deltaAngle = CalculateYawAngle();
  
  ROS_INFO("ANG: %f", deltaAngle);
  if (deltaAngle <= angleTolerance && deltaAngle > -angleTolerance)
  {
    float distance = pow((goal.x - pos.x),2) + pow((goal.y - pos.y), 2);
    message.x = sqrt(distance) + 20;
    ROS_INFO("X-distance: %f", message.x);
    movingMsg.x = 1;
  }
  else{
    message.x = 0;
    movingMsg.x = -1;
  }

}


// This function is used to plan the movements of the NAIAD.
void GoToCurrentGoal()
{
  //Check if the goal has been reached.
  float deltaAngle = 0.0;
  if (pos.x <= (goal.x+positionTolerance) && pos.x > (goal.x-positionTolerance) && pos.y <= (goal.y+positionTolerance) && pos.y > (goal.y-positionTolerance) && !onlyAngle)
  {
    ROS_INFO("GOAL REACHED");
    //gotGoal = false;
  
  //ROS_INFO("GOAL: (%f, %f, %f)", goal.x, goal.y, goal.z);
  //ROS_INFO("POS: (%f, %f, %f)", pos.x, pos.y, pos.z);
    if(square){ 
      ROS_INFO("SWITCH");
      //Dive 2 meters and go in a square pattern of length 50 meters.
      switch(checkpoint){
        //starting position
        case 'O' :
         SetGoal(0, 0, 0);
         checkpoint = 'A';
         break;
        //move to the initial position but 2 meters lower
        case 'A' :
         SetGoal(0, 0, 2);
         checkpoint = 'B';
         break;
        //move to the upper left corner
        case 'B' :
          SetGoal(0, 50, 2);
          checkpoint = 'C';
          break;
        //move to the upper right corner
        case 'C' :
         SetGoal(50, 50, 2);
         checkpoint = 'D';
         break;
        //move to the lower right corner
        case 'D' :
         SetGoal(50, 0, 2);
         checkpoint = 'A';
         break;
      }
    }
  }
  
 
  //Update X-thrust
  CalculateXThrust();
  //ROS_INFO("YAW: %f", message.yaw);
  
  //Should not move in the lockal y-direction and should not roll but maybe pitch.
  message.y = 0;
  message.roll = goal.roll;
  message.pitch = goal.roll;
  sprintf(buffer, "%f", message.x);
  sprintf(buffer+8, "%f", message.y);
  sprintf(buffer+16, "%f", height);
  sprintf(buffer+24, "%f", message.roll);
  sprintf(buffer+32, "%f", message.pitch);
  sprintf(buffer+40, "%f", message.yaw);
  sprintf(buffer+48, "%d", 88); //Filler so the BBB dont crash!

  std::cout << "mission control msg: " << buffer << std::endl;
}

void callback(const nav_msgs::Odometry::ConstPtr& msg)
{
  if (msg->pose.pose.position.z == desiered_height)
  {
    pos.z = msg->pose.pose.position.z;
    message.z = 0;
  }
  else
  {
    message.z = HeightControl();
    ROS_INFO("HightControl: %f", message.z);
  }
  
  if (gotGoal)
  {
    SetPos(msg->pose.pose.position.x, msg->pose.pose.position.y, msg->pose.pose.position.z, msg->pose.pose.orientation.z, msg->pose.pose.orientation.x, msg->pose.pose.orientation.y);
    GoToCurrentGoal();
  }
  std::cout << std::endl << "---------------------------------" << std::endl;
}



void TestingCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
  ros::Rate r(0.1);
  //int sw = (int)msg->pose.pose.position.z;
  
  switch((int)msg->pose.pose.position.z)
  {
    // Increase the trashold.
    case 1:
      sprintf(buffer+48, "%d", 11);
      std::cout << "" << std::endl;
      break;
    // Decrease the trashold.
    case 2:
      sprintf(buffer+48, "%d", 22);
      break;
    // Go in a square pattern.
    case 3:
      gotGoal = true;
      square = true;
      break;
    // Set the destination in x,y coordinates.
    case 4:
        SetGoal(msg->pose.pose.position.x, msg->pose.pose.position.y, 2);
        //std::cout << "Pos: " << msg->pose.pose.position.x << " " << msg->pose.pose.position.y << std::endl;
        gotGoal = true;
        square = false;
        break;
    // Decrease the height.
    case 5:
        height -= 0.1;
        sprintf(buffer+16, "%f", height);
        break;
    // Increase the height.
    case 6:
        height += 0.1;
        sprintf(buffer+16, "%f", height);
        break;
    // Set an angle.
    case 7:
        sprintf(buffer+40, "%f", msg->pose.pose.orientation.z);
        gotGoal = false;
        break;
    // Stop
    case 8:
        pos.x = 0.0;
        pos.y = 0.0;
        sprintf(buffer, "%f", 0.0);
        sprintf(buffer+8, "%f", 0.0);
        sprintf(buffer+16, "%f", height);
        sprintf(buffer+24, "%f", 0.0);
        sprintf(buffer+32, "%f", 0.0);
        sprintf(buffer+40, "%f", pos.yaw);
        sprintf(buffer+48, "%d", 88); //Filler so the BBB dont crash!
        square = false;
        gotGoal = false;
        break;
    // Quit.
    case 9:
        sprintf(buffer+48, "%d", 99);
        square = false;
        gotGoal = false;
        onlyAngle = false;
        checkpoint = '0';
        break;
        //quit
  }
  /*
  if (msg->pose.pose.position.x == 100)
  {
    goal.yaw = msg->pose.pose.orientation.z; 
    gotGoal = true;
    onlyAngle = true;
    //CalculateYawAngle(1);
  }
  else
  {
    onlyAngle = false;
    SetGoal(msg->pose.pose.position.x, msg->pose.pose.position.y, 2);
  }*/
}


int main(int argc, char **argv)
{
  sprintf(buffer, "%f", 0.0);
  sprintf(buffer+8, "%f", 0.0);
  sprintf(buffer+16, "%f", 0.0);
  sprintf(buffer+24, "%f", 0.0);
  sprintf(buffer+32, "%f", 0.0);
  sprintf(buffer+40, "%f", 0.0);
  sprintf(buffer+48, "%d", 88); //Filler so the BBB dont crash!
  std::string str(buffer);
  controlMsg.data = str;
  ros::init(argc, argv, "control_publisher");
  ros::NodeHandle n;
  
  ros::Publisher pub_control = n.advertise<std_msgs::String>("control", 1);
  ros::Publisher pub_moving = n.advertise<mission_control::motion>("moving", 1);
  ros::Subscriber sub_heght_speed = n.subscribe("odom", 1, callback);
  ros::Subscriber sub_testing = n.subscribe("naiad_testing", 1, TestingCallback); 

  SetPos(0,0,0,0,0,0);
  SetGoal(0,0,0,0,0,0);
  
  ros::Rate r(5);

  while(ros::ok())
  {
    ros::spinOnce();
    std::string str(buffer);
    controlMsg.data = str;
    pub_control.publish(controlMsg);
    //pub_moving.publish(movingMsg);
    r.sleep();
  }
}
