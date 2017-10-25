//----------------------------
// UDP IMU communication
// Last modified: 25.10.2017
// Extra notes: 
//----------------------------


#include "udp_imu_data.h"

tcp_client client;

// This function compute the velocity given the acceleration. 
// The parameters are: the acceleration from the IMU, the previous velocity
// and the previous timestamp used to compute the delta t.

double ComputeVelocity(double accN, double velocityN, ros::Time prevTimeStamp){
  if(abs(accN) > 0.1) {
    velocityN = velocityN + (accN*(ros::Time::now() - prevTimeStamp).toSec());
  }
  return velocityN;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "UDP_YAW_Receiver");
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<nav_msgs::Odometry>("Yaw", 1);
  nav_msgs::Odometry odom;

	int valread;
  char buffer[33];
  client.init();
  //int dataSize = sizeof( "{""id"": 2, ""name"": ""abc""}" );
  client.sock = socket(AF_INET, SOCK_DGRAM, 0);
  
  if (client.sock < 0)
  {
    ROS_INFO("Could not create socket");
  }
  
  bind(client.sock, (struct sockaddr *) &client.server, sizeof(client.server));

  double yaw = 0.0;
  double accX = 0.0;
  double accY = 0.0;
  double accZ = 0.0;
  double velocityX = 0.0;
  double velocityY = 0.0;
  double velocityZ = 0.0;
  ros::Time startTimeX = ros::Time::now();
  ros::Time startTimeY = ros::Time::now();
  ros::Time startTimeZ = ros::Time::now();
  while(1)
  {
    std::cout << std::endl;
    recv(client.sock, buffer,sizeof(buffer), 0);
    std::cout << "Buffer content: " << buffer << std::endl;
    std::cout << "buffer[0] " << buffer[0] << std::endl;
    std::cout << "buffer[1] " << buffer[1] << std::endl;
    char str[7];
    for(int iii = 0; iii < 8; iii++)
    {
      str[iii] = buffer[iii+4];
    }
    switch(buffer[1]){
      case '1':
        yaw = atof(str);
        std::cout << "Yaw: " << yaw << std::endl;
        break;
      case '2':
        accX = atof(str);
        velocityX = ComputeVelocity(accX, velocityX, startTimeX);
        startTimeX = ros::Time::now();
        std::cout << "Velocity in X: " << velocityX << std::endl;
        break;
      case '3':
        accY = atof(str);
        velocityY = ComputeVelocity(accY, velocityY, startTimeY);
        startTimeY = ros::Time::now();
        std::cout << "Velocity in Y: " << velocityY << std::endl;
        break;
      case '4':
        accZ = atof(str);
        velocityZ = ComputeVelocity(accZ, velocityZ, startTimeZ);
        startTimeZ = ros::Time::now();
        std::cout << "Velocity in Z: " << velocityZ << std::endl;
        std::cout << std::endl << "==================================" << std::endl;
        break;
      default:
        std::cout << "Invalid data! " << std::endl;
    }
    // Publish data over ROS topic.
    odom.pose.pose.orientation.z = yaw;
    pub.publish(odom);
  }
}
