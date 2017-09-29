#include "ros/ros.h"
#include "std_msgs/String.h"
#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "string.h"
#include <nav_msgs/Odometry.h>
#include "nav_msgs/GetMap.h"
#include "nav_msgs/SetMap.h"
#include <string>
#include "stdlib.h"

#define server_port 12321

//TCP client socket class
class tcp_client
{
	public:
		//float X;
		//float Y;
		//float Z;
		int sock;
	        struct sockaddr_in server;
	public:
	//Constructor kindoff
	void init_client()
	{
		server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("192.168.1.30");
    //server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(server_port);
		ROS_INFO("Socket created");
	}
};

float X;
float Y;
float Z;
float ori_X;
float ori_Y;
float ori_Z;
bool got_service_call = false;

bool client_service(nav_msgs::SetMap::Request &req, nav_msgs::SetMap::Response &res)
{
	ROS_INFO("Service call");
	//res.map.info.origin.position.x = req.map.info.origin.position.x;
	X = req.map.info.origin.position.x;
	Y = req.map.info.origin.position.y;
	Z = req.map.info.origin.position.z;
	ori_X = req.map.info.origin.position.x;
  ori_Y = req.map.info.origin.position.y;
  ori_Z = req.map.info.origin.position.z;
	//ROS_INFO("%f %f %f", X, Y, Z);
	got_service_call = true;
	return true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "tcp_coord_service");
	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("coord_service", client_service);

	//Dummy variabls for testing
	//float X = 11.111;
	//float Y = 22.222;
	//float Z = 33.333;


	//Nesesery variabls
	//int sock;
	//struct sockaddr_in server;
	int buffer_size = 1024;
	char buffer[buffer_size];

	//Instasiate TCP client
	ROS_INFO("Starting Client");
	tcp_client client;
	client.init_client();

	//char buffer[client.buffer_size];
	int connect_count = 0;
	ros::Rate loop_rate(100); //Loop at 10 Hz
	while(ros::ok()){
	  client.sock = socket(AF_INET, SOCK_STREAM, 0);
    while(ros::ok())
      {
        //ROS_INFO("LOOPING!");
		    if (got_service_call && connect_count < 10)
		    {
			    //Creating the socket
			    if (client.sock < 0)
			    {
				    ROS_INFO("Creating socket failed");
				    connect_count++;
				    break;
			    }

			    //ROS_INFO("Socket created");

			    ROS_INFO("Connecting to server");

			    //Connect to the ADA server that want X,Y,Z,Roll,Pitch,Yaw for the PID condtroller of the moters.
			    if (connect(client.sock,(struct sockaddr *) &client.server,sizeof(client.server)) < 0)
			    {
				    ROS_INFO("Could not connect to server");
				    connect_count++;
				    //loop_rate.sleep();
				    break;
			    }
			    ROS_INFO("Connection established");

			    //Putting the values to be sent into a character array
			    //std::cin >> X;
			    sprintf(buffer, "%f", X);
			    //std::cin >> Y;
			    sprintf(buffer+8, "%f", Y);
			    //std::cin >> Z;
			    sprintf(buffer+16, "%f", Z);

			    sprintf(buffer+24, "%f", ori_X);
          //std::cin >> Y;
          sprintf(buffer+32, "%f", ori_Y);
          //std::cin >> Z;
          sprintf(buffer+40, "%f", ori_Z);



			    //Just a print to see that all is OK
			    std::cout << buffer << " " << strlen(buffer);

			    //Send the information to the ADA server.
			    ROS_INFO("Sending...");
			    if (send(client.sock, buffer, strlen(buffer), 0) < 0)
			    {
				    ROS_INFO("Could not send data");
				    //exit(1);
			    }
			    connect_count = 0;
			    got_service_call = false;
			    break;
		    } //if
		    else if (connect_count >= 10)
		    {
			    ROS_INFO("Server offline");
			    got_service_call = false;
			    connect_count = 0;
		    } //end if
		    ros::spinOnce();
		    //Might wanna receive som kind of ACK that the message was received.
		    //recv(sock, buffer, buffer_size, 0);
	    }
	    close(client.sock);
	  }
	//Close the connection and clear the socket

	return 0;

}
