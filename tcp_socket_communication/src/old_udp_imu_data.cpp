//----------------------------------------
// Old Version of the equivalent file
// Last modified: 25.10.2017
//----------------------------------------


#include "udp_imu_data.h"

tcp_client client;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "UDP_YAW_Receiver");
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

  int count = 0;
  int indexBegin = 0;
  int indexEnd = 0;
  int tempStrIndex = 0;
  double yaw = 0.0;
  float imuData[5] = {};
  int imuDataIndex = 0;
  char tempStr[6]; 
  while(1)
  {

    recv(client.sock, buffer,sizeof(buffer), 0);
    std::cout << "Buffer content: " << buffer << std::endl;
    std::cout << "buffer[0] " << buffer[0] << std::endl;
    std::cout << "buffer[1] " << buffer[1] << std::endl;
    //if (buffer[0] == '1')
    //{
      //std::cout << "buffer[0]" << std::endl;
      //while(buffer[count] != '#' && count < 35)
      //{
        /*std::cout << "buffer count: " << count << std::endl;
        if (buffer[count] == '*'){
          
          std::cout << "first * encountered" << std::endl;
          indexBegin = count + 1;
          //move to the next star. This is for indexing reference.
          while(buffer[count] != '*'){
          count++;
          for(int iii = count; iii < count+5; iii++){
            tempStr[iii] = buffer[iii];
            //indexEnd = count;
            //count ++;
            //tempStrIndex ++;
            //std::cout << "temp index: " << tempStrIndex << std::endl;
          }
          count = count+5;
          tempStrIndex = 0;
          std::cout << "second * encountered" << std::endl;
          imuData[imuDataIndex] = atof(tempStr);
          //tempStr = {};
          imuDataIndex ++;
        } else {
          count ++;
        }

      }
      count = 0;
      std::cout << "encounter #" << std::endl;
      std::cout << "DATA IMU ARRAY: " << imuData << std::endl;*/
      while(buffer[count] != ' '){
        count++;
        //std::cout << "Count: " << count << std::endl;
      }
      char str[count];
      for(int iii = 2; iii < 8; iii++)
      {
        str[iii-2] = buffer[iii];
      }
      std::cout << "Count: " << count << std::endl;
      std::cout << str << std::endl;
      yaw = atof(str);
      std::cout << "Yaw: " << yaw << std::endl;
      count = 2;
    }
    odom.pose.pose.orientation.z = yaw;
    pub.publish(odom);
    std::cout << "Yaw: " << yaw << std::endl;
  }
 ////}
}
