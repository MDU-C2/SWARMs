#include "ros/ros.h"
#include "std_msgs/String.h"
#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "string.h"
#include <nav_msgs/Odometry.h>
#include <string>
#include "stdlib.h"
#include "/root/catkin_ws/devel/include/mission_control/motion.h"
 
#define server_port 12321
 
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
			server.sin_addr.s_addr = inet_addr("192.168.1.30");
			server.sin_port = htons(server_port);
  	}
};
