//----------------------------
// UDP IMU communication
// Last modified: 04.12.2017
// Extra notes: 
//----------------------------


#include "udp_imu_data.h"
#include "iostream"
#include "fstream"

tcp_client client;
imuData orientation;


//Files for collecting IMU data
std::ofstream accXFile;
std::ofstream angleYawFile;
std::ofstream accYFile;
std::ofstream anglePitchFile;
std::ofstream gyroXFile;
std::ofstream gyroYFile;
std::ofstream gyroZFile;

// This function compute the velocity given the acceleration. 
// The parameters are: the acceleration from the IMU, the previous velocity
// and the previous timestamp used to compute the delta t.

double ComputeVelocity(double accN, double velocityN, ros::Time prevTimeStamp){
  if(abs(accN) > 0.1) {
    velocityN = velocityN + (accN*(ros::Time::now() - prevTimeStamp).toSec());
  }
  return velocityN;
}

void MovingCallback(const mission_control::motion &msg)
{
  if (msg.x != 1)
  {
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
  
  accXFile.open("X_data.txt");
  accYFile.open("Y_data.txt");
  angleYawFile.open("Yaw_data.txt");
  anglePitchFile.open("Pitch_data.txt");
  gyroXFile.open("GyroX_data.txt");
  gyroYFile.open("GyroY_data.txt");
  gyroZFile.open("GyroZ_data.txt");


	int valread;
  char buffer[64];
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
  
  float gyroX = 0.0;
  float gyroY = 0.0;
  float gyroZ = 0.0;

  ros::Time startTimeX = ros::Time::now();
  ros::Time startTimeY = ros::Time::now();
  ros::Time startTimeZ = ros::Time::now();
  
  while(n.ok())
  {
    std::cout << std::endl;
    recv(client.sock, buffer,sizeof(buffer), 0);
    char strX[7];
    char strY[7];
    char strPitch[7];
    char strYaw[7];
    char strGyroX[7];
    char strGyroY[7];
    char strGyroZ[7];
    for(int iii = 0; iii < 7; iii++)
    {
      strPitch[iii] = buffer[iii];
      strYaw[iii] = buffer[iii+7];
      strX[iii] = buffer[iii+14];
      strY[iii] = buffer[iii+21];
      strGyroX[iii] = buffer[iii+28];
      strGyroY[iii] = buffer[iii+35];
      strGyroZ[iii] = buffer[iii+42];    
    }
    std::cout << "Buffer content: " << buffer << std::endl;
    
    // Data contained in the buffer realted to pitch values.
    orientation.pitch = atof(strPitch);
    std::cout << "Pitch: " << orientation.pitch << std::endl;
    odom.pose.pose.orientation.y = orientation.pitch;

    // Data contained in the buffer realted to yaw values.
    orientation.yaw = atof(strYaw);
    angleYawFile << orientation.yaw << std::endl; //write data in file,
    std::cout << "Yaw: " << orientation.yaw << std::endl;
    odom.pose.pose.orientation.z = orientation.yaw;
    
    // Data contained in the buffer related to the acceleration in x axis.
    orientation.accX = atof(strX);
    //correction for excluding gravity effect:
    orientation.accX = orientation.accX - (sin(orientation.pitch*PI/180)*9.81);
    accXFile << orientation.accX << std::endl; //write data into file
    orientation.velocityX = ComputeVelocity(orientation.accX, orientation.velocityX, startTimeX);
    startTimeX = ros::Time::now();
    odom.twist.twist.linear.x = orientation.velocityX;
    std::cout << "Acceleration in X: " << orientation.accX << std::endl;
    
    // Data contatined in the buffer related to the acceleration in y axis
    orientation.accY = atof(strY);
    accYFile << orientation.accY << std::endl; //write data into file
    orientation.velocityY = ComputeVelocity(orientation.accY, orientation.velocityY, startTimeY);
    startTimeY = ros::Time::now();
    odom.twist.twist.linear.y = orientation.velocityY;
    std::cout << "Acceleration in Y: " << orientation.accY << std::endl;
    
    // Data contatined in the buffer related to the gyroX axis
    gyroX = atof(strGyroX);
    gyroXFile << gyroX << std::endl; //write data into file
    std::cout << "GyroX: " << gyroX << std::endl;
    odom.twist.twist.angular.x = gyroX;
    
    // Data contatined in the buffer related to the gyroY axis
    gyroY = atof(strGyroY);
    gyroYFile << gyroY << std::endl;
    std::cout << "GyroY: " << gyroY << std::endl;
    odom.twist.twist.angular.y = gyroY;
    
    // Data contatined in the buffer related to the gyroZ axis
    gyroZ = atof(strGyroZ);
    gyroZFile << gyroZ << std::endl;
    std::cout << "GyroZ: " << gyroZ << std::endl;
    odom.twist.twist.angular.z = gyroZ;

    
    std::cout << std::endl << "==================================" << std::endl;
    
    // Publish data over ROS topic.
    ros::spinOnce();
    pub.publish(odom);
  }
}
