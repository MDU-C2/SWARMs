#include "tcp.h"
/*
 * This ROS node listen to the control topic and transfer that information to the 
 * PID controller (BBB) throught tcp sockets.
 */

bool goToSurface = false;

//Callcback function used for sending control information to the BBB:
bool TcpSender(const mission_control::motion msg)
{
  tcp_client client;
  //TODO check the buffer size! not sure!
	int buffer_size = 1024;
	char buffer[buffer_size];
  client.init();
  client.sock = socket(AF_INET, SOCK_STREAM, 0);
  //If an error occurs
	if (client.sock < 0)
  {
    ROS_INFO("Creating socket failed");
    return 0;
  }

  std::cout << msg.x << " " << msg.y << std::endl;

  //If no error occurs
  ROS_INFO("Connecting to server");
  ros::Rate rate(10);
  for(int iii=0; iii<10; iii++)
  {
		if (connect(client.sock,(struct sockaddr *) &client.server,sizeof(client.server)) < 0)
		{

			ROS_INFO("Could not connect to server");
      if(iii==9)
      {
        close(client.sock);
        return 0;
      }
		} else {
      break;
    }
    //rate.sleep();
	}
	ROS_INFO("Connection established");
	//Putting the values to be sent into a character array
	sprintf(buffer, "%f", msg.x);
	sprintf(buffer+8, "%f", msg.y);
	sprintf(buffer+16, "%f", msg.z);
	sprintf(buffer+24, "%f", msg.roll);
	sprintf(buffer+32, "%f", msg.pitch);
	sprintf(buffer+40, "%f", msg.yaw);

  if (goToSurface)
  {
    sprintf(buffer+48, "%d", 88);
    goToSurface = false;
  }
  else
  {
    sprintf(buffer+48, "%d", 99);
  }

  //Just a print to see that all is OK
  std::cout << buffer << " " << strlen(buffer);
  //Send the information to the ADA server.
  ROS_INFO("Sending...");
  if (send(client.sock, buffer, strlen(buffer), 0) < 0)
  {
    ROS_INFO("Could not send data");
    return 0;
  }
  close(client.sock);
  //rate.sleep();
}

//
void ControlTcpCallback(const mission_control::motion &msg)
{
  if(TcpSender(msg))
  {
    ROS_INFO("SUCCESS!");
  } else {
    ROS_INFO("FAILURE! GOOD LUCK WITH THAT!!");
  }
}

void GoToSurfaceCallback(const mission_control::motion &msg)
{
  goToSurface = true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "tcp_coord_service");
	ros::NodeHandle n;

	//ros::ServiceServer service = n.advertiseService("coord_service", client_service);
  ros::Subscriber sub = n.subscribe("control", 1, ControlTcpCallback);
  ros::Subscriber sub_surf = n.subscribe("go_to_surface", 1, GoToSurfaceCallback);
	//Instasiate TCP client
	ROS_INFO("Starting Client");

	while(ros::ok()){
    ros::spin();
  }
	//Close the connection and clear the socket
	return 0;

}
