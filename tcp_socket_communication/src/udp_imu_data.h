//--------------------------------
// UDP communication header file
// Last modified: 25.10.2017
//--------------------------------


#include "ros/ros.h"
#include "std_msgs/String.h"
#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include <nav_msgs/Odometry.h>

//define a prot to be used.
#define server_port 12345

//Definition of IMU variables
struct imuData{
  double yaw;
  double roll;
  double pitch;
  double accX;
  double accY; 
  double accZ;   
  double velocityX;
  double velocityY;
  double velocityZ;
};

//TCP client socket class
class tcp_client
{
  public:
    int sock;
    struct sockaddr_in server;
  public:
    void init()
    {
      server.sin_family = AF_INET;
      server.sin_addr.s_addr = INADDR_ANY;
      server.sin_port = htons(server_port);
    }
};
