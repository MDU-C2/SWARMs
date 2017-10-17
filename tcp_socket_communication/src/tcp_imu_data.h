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
      //Address to the BBB board. (Localhost now)
      server.sin_addr.s_addr = inet_addr("127.0.0.1");
      server.sin_port = htons(server_port);
    }
};
