#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include "Algo.h"
#include "gui.h"

#define MAX_BUF 1024
#define MSG_SIZE 80
#define MAX_CLIENTS 95
#define MYPORT 7400

void exitClient(int fd, fd_set *readfds, char fd_array[], int *num_clients){
  int i;

  close(fd);
  FD_CLR(fd, readfds); //clear the leaving client from the set
  for (i = 0; i < (*num_clients) - 1; i++)
    if (fd_array[i] == fd)
      break;          
  for (; i < (*num_clients) - 1; i++)
    (fd_array[i]) = (fd_array[i + 1]);
      (*num_clients)--;
}


int main(int argc, char *argv[]) {
      int i=0;

      int port;
      int num_clients = 0;
      int server_sockfd, client_sockfd;
      struct sockaddr_in server_address;
      int addresslen = sizeof(struct sockaddr_in);
      int fd;
      char fd_array[MAX_CLIENTS];
      char* buffer1;
      fd_set readfds, testfds, clientfds;
      char msg[MSG_SIZE + 1];     
      char kb_msg[MSG_SIZE + 10];
      int fdc;
      char * myfifo = "/tmp/myfifo";
      char buf[MAX_BUF];

     int sockfd;
     int result;
     char hostname[MSG_SIZE];
     struct hostent *hostinfo;
     struct sockaddr_in address;
     char alias[MSG_SIZE];
     int clientid;


   /*Client==================================================*/
     if(argc==2 || argc==4){
       if(!strcmp("-p",argv[1])){
         if(argc==2){
           printf("Invalid parameters.\nUsage: chat [-p PORT] HOSTNAME\n");
           exit(0);
         }else{
           sscanf(argv[2],"%i",&port);
           strcpy(hostname,argv[3]);
         }
       }else{
         port=MYPORT;
         strcpy(hostname,argv[1]);
       }
       printf("\n*** Client program starting (enter \"quit\" to stop): \n");
       fflush(stdout);

     /* Create a socket for the client */
       sockfd = socket(AF_INET, SOCK_STREAM, 0);

     /* Name the socket, as agreed with the server */
     hostinfo = gethostbyname(hostname);  /* look for host's name */
       address.sin_addr = *(struct in_addr *)*hostinfo -> h_addr_list;
       address.sin_family = AF_INET;
       address.sin_port = htons(port);

     /* Connect the socket to the server's socket */
       if(connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0){
         perror("connecting");
         exit(1);
       }

       fflush(stdout);

       FD_ZERO(&clientfds);
       FD_SET(sockfd,&clientfds);
     FD_SET(0,&clientfds);//stdin
     initBoard(argc, argv);
     /*  Now wait for messages from the server */
     while (1) {
       testfds=clientfds;
       select(FD_SETSIZE,&testfds,NULL,NULL,NULL);

       for(fd=0;fd<FD_SETSIZE;fd++){
        // while(1){
        //     if(access(myfifo, F_OK) != -1){
        //       fdc = open(myfifo, O_RDONLY);
        //       read(fdc, buf, MAX_BUF);
        //       //printf("Received: %s\n", buf);
        //       write(STDOUT_FILENO, buf, strlen(buf));
        //       close(fdc);
        //       break;
        //     }
        //   }
        if(FD_ISSET(fd,&testfds)){
          
             if(fd==sockfd){   /*Accept data from open socket */
                //printf("client - read\n");

                //read data from open socket
          result = read(sockfd, msg, MSG_SIZE);
                msg[result] = '\0';  /* Terminate string with null */
          buffer1 = (char*)malloc(MSG_SIZE);
          strcpy(buffer1,msg);
          char* buffer = strtok(buffer1, " ");
          if (!strcmp(buffer,"QUIT")) {
            printf("%s", msg +5);                   
            close(sockfd);
            exit(0);
          }    
          else {
            printf("Message from server: %s", msg);
		   // memset(msg, 0, MSG_SIZE);

          }                         
        }
             else if(fd == 0){ /*process keyboard activiy*/
               // printf("client - send\n");

        fgets(kb_msg, MSG_SIZE+1, stdin);
                //printf("%s\n",kb_msg);
        if (strcmp(kb_msg, "QUIT\n")==0) {
                    //sprintf(msg, "QUIT Client is shutting down.\n");
          write(sockfd, kb_msg, strlen(kb_msg));
                    close(sockfd); //close the current client
                    exit(0); //end program
                  }
                  else {
                   /* sprintf(kb_msg,"%s",alias);
                    msg[result]='\0';
                    strcat(kb_msg,msg+1);*/

                    //sprintf(kb_msg, "%s", msg);
                    write(sockfd, kb_msg, strlen(kb_msg));
                  }                                                 
                }          
              }
            }      
          }
        
   }// end client code
 }
