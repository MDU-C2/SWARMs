//----------------------------
// UDP IMU communication
// Last modified: 25.10.2017
// Extra notes: 
//----------------------------


#include "udp_imu_data.h"

tcp_client client;
imuData orientation;

// This function compute the velocity given the acceleration. 
// The parameters are: the acceleration from the IMU, the previous velocity
// and the previous timestamp used to compute the delta t.

double ComputeVelocity(double accN, double velocityN, ros::Time prevTimeStamp){
std::cout << accN << std::endl;
  if(accN > 0.4 || accN < -0.4) {
    velocityN = velocityN + (accN*(ros::Time::now() - prevTimeStamp).toSec());
  }
  std::cout << "Vel: " << velocityN;
  return velocityN;
}

void MovingCallback(const mission_control::motion &msg)
{
  std::cout << "Moving callback" << std::endl;
  if (msg.x != 1)
  { 
    std::cout << "Not moving" << std::endl;
    orientation.velocityX = 0;
    orientation.velocityY = 0;
    orientation.velocityZ = 0;
  }
}


int main(int argc, char **argv)
{
  //instantiation of imuData
  ros::init(argc, argv, "UDP_YAW_Receiver");
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<nav_msgs::Odometry>("IMU_data_for_odom", 1);
  ros::Subscriber sub_moving = n.subscribe("moving", 1, MovingCallback);
  nav_msgs::Odometry odom;

	int valread;
  char buffer[21] = {};
  client.init();
  //int dataSize = sizeof( "{""id"": 2, ""name"": ""abc""}" );
  client.sock = socket(AF_INET, SOCK_DGRAM, 0);
  
  if (client.sock < 0)
  {
    ROS_INFO("Could not create socket");
  }
  
  bind(client.sock, (struct sockaddr *) &client.server, sizeof(client.server));

  //initialization of variables.
  orientation.yaw = 0.0;
  orientation.roll = 0.0;
  orientation.pitch = 0.0;
  orientation.accX = 0.0;
  orientation.accY = 0.0;
  orientation.accZ = 0.0;
  orientation.velocityX = 0.0;
  orientation.velocityY = 0.0;
  orientation.velocityZ = 0.0;
  
  ros::Time startTimeX = ros::Time::now();
  ros::Time startTimeY = ros::Time::now();
  ros::Time startTimeZ = ros::Time::now();
  
  while(n.ok())
  {
    std::cout << std::endl;
    recv(client.sock, buffer,sizeof(buffer), 0);
    char strAccX[7] = {};
    char strAccY[7] = {};
    char strYaw[7] = {};

    std::cout << buffer << std::endl;
    
    for(int iii = 0; iii < 7; iii++)
    {
      strYaw[iii] = buffer[iii];
      strAccX[iii] = buffer[iii+7];
      strAccY[iii] = buffer[iii+14];
      //std::cout << strAccY << " iii: " << iii << " " << buffer[iii+13] << std::endl;
    }
    //std::cout << "strYaw: " << strYaw << std::endl << "strAccX: " << strAccX << std::endl << "strAccY: " << strAccY << std::endl; 
    
    orientation.yaw = atof(strYaw);
    odom.pose.pose.orientation.z = orientation.yaw;

    orientation.accX = atof(strAccX);
    orientation.velocityX = ComputeVelocity(orientation.accX, orientation.velocityX, startTimeX);
    startTimeX = ros::Time::now();
    odom.twist.twist.linear.x = orientation.velocityX;
    //std::cout << "Velocity in X: " << orientation.velocityX << std::endl;

    orientation.accY = atof(strAccY);
    orientation.velocityY = ComputeVelocity(orientation.accY, orientation.velocityY, startTimeY);
    startTimeY = ros::Time::now();
    odom.twist.twist.linear.y = orientation.velocityY;
    //std::cout << "Velocity in Y: " << orientation.velocityY << std::endl;

    std::cout << std::endl << "==================================" << std::endl;

    /*
    switch(buffer[1]){
      // Data contained in the buffer realted to yaw values.
      case '1':
        orientation.yaw = atof(str);
        std::cout << "Yaw: " << orientation.yaw << std::endl;
        odom.pose.pose.orientation.z = orientation.yaw;
        break;
      // Data contained in the buffer related to the acceleration in x axis.
      case '2':
        orientation.accX = atof(str);
        orientation.velocityX = ComputeVelocity(orientation.accX, orientation.velocityX, startTimeX);
        startTimeX = ros::Time::now();
        odom.twist.twist.linear.x = orientation.velocityX;
        std::cout << "Velocity in X: " << orientation.velocityX << std::endl;
        break;
      // Data contatined in the buffer related to the acceleration in y axis
      case '3':
        orientation.accY = atof(str);
        orientation.velocityY = ComputeVelocity(orientation.accY, orientation.velocityY, startTimeY);
        startTimeY = ros::Time::now();
        odom.twist.twist.linear.y = orientation.velocityY;
        std::cout << "Velocity in Y: " << orientation.velocityY << std::endl;
        break;
      // Data contatined in the buffer related to the acceleration in z axis
      case '4':
        orientation.accZ = atof(str);
        orientation.velocityZ = ComputeVelocity(orientation.accZ, orientation.velocityZ, startTimeZ);
        startTimeZ = ros::Time::now();
        odom.twist.twist.linear.z = orientation.velocityZ;
        std::cout << "Velocity in Z: " << orientation.velocityZ << std::endl;
        break;
      // Data contained in the buffer related to the roll angle.
      case '5':
        orientation.roll = atof(str);
        odom.pose.pose.orientation.x = orientation.roll;
        break;
      // Data contatined in the buffer related to the pitch angle.
      case '6':
        orientation.pitch = atof(str);
        odom.pose.pose.orientation.y = orientation.pitch;
        std::cout << std::endl << "==================================" << std::endl;
        break;
      default:
        std::cout << "Invalid data! " << std::endl;
    }
    */
    // Publish data over ROS topic.
    ros::spinOnce();
    pub.publish(odom);
  }
}
