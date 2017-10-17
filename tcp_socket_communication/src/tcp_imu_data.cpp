#include "tcp_imu_data.h"

tcp_client client;

int main(int argc, char **argv)
{
	int valread;
  char buffer[6];
  client.init();
  //int dataSize = sizeof( "{""id"": 2, ""name"": ""abc""}" );
  //std::cout << dataSize;
  client.sock = socket(AF_INET, SOCK_DGRAM, 0);
  
  if (client.sock < 0)
  {
    ROS_INFO("Could not create socket");
  }
  
  bind(client.sock, (struct sockaddr *) &client.server, sizeof(client.server));

  /*
  if (connect(client.sock, (struct sockaddr *) &client.server, sizeof(client.server)) < 0)
  {
    ROS_INFO("Could not connect!");
    exit(1);
  }
  */

    valread = read(client.sock, buffer, 6);
    printf("%s\n",buffer );
    //send(new_socket , hello , strlen(hello) , 0 );
    //printf("Hello message sent\n");
    return 0;
  //read(client.sock, buffer[0], 512);
  //for (int iii = 0; iii < 10; iii++)
  //{
  //  std::cout << buffer[iii];
  //}
  //std::cout << buffer.c_str();
}
