#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>   
#include<sys/ioctl.h>
#include<unistd.h>  
#include<iostream>
#include<fstream>
#include<errno.h>
using namespace std;

//This function receives the image pair from the server

int receive_image(int socket)
{  

  int buffersize = 0, recv_size = 0,size = 0, read_size, write_size, packet_index =1,stat,stat2;

  char imagearray[10241]; ////,verify = '1';
  char recvd_filename[512];
  //recvd_filename.resize(256);
  FILE *image;

do{
stat = read(socket, &size, sizeof(int)); //receive size of image
}while(stat<0);


printf("Packet received.\n");
printf("Packet size: %i\n",stat);
printf("Image size: %i\n",size);
printf(" \n");
char buffer[] = "Got the image";


//Send a verification signal
do{
stat = write(socket, &buffer, sizeof(int));
}while(stat<0);

printf("Verification sent!\n");
printf(" \n");


//read the name of the image
//do{
stat2 = recv(socket, &recvd_filename, 512, 0);// sizeof(recvd_filename));
printf("stat2, recv filename: %i\n",stat2);
//}while(stat2 < 0);
printf("name received\n");

//if (stat2 < 0)
//{
//  std::cout << "Error stat2" << endl;
//}

std::cout << recvd_filename << endl;
char recvd_veri[] = "name verified\n";

//send verification of name
do{
stat2 = write(socket, &recvd_veri, sizeof(int));
}while(stat2 < 0);
printf("verification sent\n");
image = fopen(recvd_filename, "w");

if( image == NULL) {
printf("Error when opening the image\n");
return -1; }

//int need_exit = 0;   /////////
struct timeval timeout = {10,0};

fd_set fds;
int buffer_fd;///////, buffer_out;

//Loop while the image is transferred
while(recv_size < size) {
//while(packet_index < 2){

    FD_ZERO(&fds);
    FD_SET(socket,&fds);

    buffer_fd = select(FD_SETSIZE,&fds,NULL,NULL,&timeout);

    if (buffer_fd < 0)
       printf("error: bad file descriptor set.\n");

    if (buffer_fd == 0)
       printf("error: buffer read timeout expired.\n");

    if (buffer_fd > 0)
    {
        do{
               read_size = read(socket,imagearray, 10241);
            }while(read_size <0);

          //  printf("Packet number received: %i\n",packet_index);
        //printf("Packet size: %i\n",read_size);


        //Write the currently read data into our image file
         write_size = fwrite(imagearray,1,read_size, image);
      //   printf("Written image size: %i\n",write_size); 

             if(read_size !=write_size) {
                 printf("error in read write\n");    }


             //Increment the total number of bytes read
             recv_size += read_size;
             packet_index++;
             //printf("Total received image size: %i\n",recv_size);
             //printf(" \n");
             //printf(" \n");
    }
}
  fclose(image);
  printf("Image successfully Received!\n");

  char buffer1[] = "Done";
  //Send a verification signal
  do{
   stat = write(socket, &buffer1, sizeof(int));
  }while(stat<0);
  std::cout << "Done!" << endl;
    return 1;
  }



int main(int argc , char *argv[])
  {

  int socket_desc;
  struct sockaddr_in server;
  //////////char *parray;


  //Create a socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);//ipv4 and tcp

  if (socket_desc == -1) {
  printf("Could not create socket");
  }

  memset(&server,0,sizeof(server));
  server.sin_addr.s_addr = inet_addr("192.168.1.10");
  server.sin_family = AF_INET;
  server.sin_port = htons( 8765 );
   puts("Connecting...");
   
    //Connect to the server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) {
      cout<<strerror(errno);
      close(socket_desc);
      puts("Connect Error");
      return 1;
    }

    puts("Connected\n");
  
    receive_image(socket_desc);
    
//    receive_image(socket_desc);

    close(socket_desc);

  return 0;
  }

