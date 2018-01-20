#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "linklist.c"

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
   
   struct Node *user_list = NULL;
   int i=0;
   
   int port;
   int num_clients = 0;
   int result;
   int server_sockfd, client_sockfd;
   struct sockaddr_in server_address;
   int addresslen = sizeof(struct sockaddr_in);
   int fd;
   char fd_array[MAX_CLIENTS];
   char* buffer1;
   char* buffer2;
   fd_set readfds, testfds, clientfds;
   char msg[MSG_SIZE + 1];     
   char kb_msg[MSG_SIZE + 10];

   if(argc==1 || argc == 3){
     if(argc==3){
       if(!strcmp("-p",argv[1])){
         sscanf(argv[2],"%i",&port);
       }else{
         printf("Invalid parameter.\nUsage: chat [-p PORT] HOSTNAME\n");
         exit(0);
       }
     }else port=MYPORT;
     
     printf("\n*** Server program starting (enter \"quit\" to stop): \n");
     fflush(stdout);

     /* Create and name a socket for the server */
     server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
     server_address.sin_family = AF_INET;
     server_address.sin_addr.s_addr = htonl(INADDR_ANY);
     server_address.sin_port = htons(port);
     bind(server_sockfd, (struct sockaddr *)&server_address, addresslen);

     /* Create a connection queue and initialize a file descriptor set */
     listen(server_sockfd, 1);
     FD_ZERO(&readfds);
     FD_SET(server_sockfd, &readfds);
     FD_SET(0, &readfds);  /* Add keyboard to file descriptor set */
     
     //struct clientinfo c;
     
     //printf("sizeof(readfds)/(testfds): %d/%d\n",sizeof(readfds),sizeof(testfds));
     //printf("Number of bits in readfds/testfds: %d/%d\n", sizeof(readfds)*8,sizeof(testfds)*8);

     /*  Now wait for clients and requests */
     while (1) {
        testfds = readfds;
        select(FD_SETSIZE, &testfds, NULL, NULL, NULL);
                    
        /* If there is activity, find which descriptor it's on using FD_ISSET */
        for (fd = 0; fd < FD_SETSIZE; fd++) {
           if (FD_ISSET(fd, &testfds)) {
              
              if (fd == server_sockfd) { /* Accept a new connection request */
                 client_sockfd = accept(server_sockfd, NULL, NULL);
                 printf("client_sockfd: %d %d\n",client_sockfd,server_sockfd);
                
                                
                 if (num_clients < MAX_CLIENTS) {
                    FD_SET(client_sockfd, &readfds);
                    fd_array[num_clients]=client_sockfd;
                    /*Client ID*/
                    printf("Client %d joined\n",num_clients++);
                    fflush(stdout);
                    
                    //sprintf(msg,"M%2d",client_sockfd);
                    /*write 2 byte clientID */
                    //send(client_sockfd,msg,strlen(msg),0);
                 }
                 else {
                    sprintf(msg, "XSorry, too many clients.  Try again later.\n");
                    write(client_sockfd, msg, strlen(msg));
                    close(client_sockfd);
                 }
              }
              else if (fd == 0)  {  /* Process keyboard activity */                 
                 fgets(kb_msg, MSG_SIZE + 1, stdin);
                 //printf("%s\n",kb_msg);
                 if (strcmp(kb_msg, "QUIT\n")==0) {
                    sprintf(msg, "QUIT Server is shutting down.\n");
                    for (i = 0; i < num_clients ; i++) {
                       write(fd_array[i], msg, strlen(msg));
                       close(fd_array[i]);
                    }
                    close(server_sockfd);
                    exit(0);
                 }
                 else {
                    //printf("server - send\n");
                    sprintf(msg, "M%s", kb_msg);
                    for (i = 0; i < num_clients ; i++)
                       write(fd_array[i], msg, strlen(msg));
                 }
              }
              else if(fd) {  /*Process Client specific activity*/
                 //printf("server - read\n");
                 //read data from open socket
                 result = read(fd, msg, MSG_SIZE);
                 if(result==-1) perror("read()");
                 else if(result>0){
                    /*read 2 bytes client id*/
                    //sprintf(kb_msg,"M%2d",fd);
		    
                    msg[result]='\0';
		    buffer1 = (char*)malloc(MSG_SIZE);
		    buffer2 = (char*)malloc(MSG_SIZE);
		    strcpy(buffer1,msg);
		    
		    
		    
		    /*Exit Client*/
		    if (!strcmp(buffer1,"QUIT\n")) {
			deleteNode(&user_list,fd);
			/*print to server*/
			//printf("%s",kb_msg+1);
			printf("Client from socket %d is shutting down\n",fd);
			exitClient(fd,&readfds, fd_array,&num_clients);
		    }
		    else if (!strncmp(buffer1,"SETNAME",7)) {
			char* buffer = strtok(buffer1, " ");
			buffer = strtok(NULL, " ");buffer[strlen(buffer)-1]='\0';
			if (getFD(user_list,buffer) != -1) {
				strcpy(kb_msg,"SETFAIL\n");
				write(fd,kb_msg,strlen(kb_msg));
			}
			else {
			push(&user_list,fd,0,-1,buffer);
	      		printf("User from socket %d register username %s\n",user_list[0].data,user_list[0].username);
	      		strcpy(kb_msg,"OK\n");
                        //write(fd,kb_msg,strlen(kb_msg));
                    	for(i=0;i<num_clients;i++){
				
				if (fd_array[i] == fd)  /*dont write msg to same client*/
                          	write(fd_array[i],kb_msg,strlen(kb_msg));
                    	}		
			}
	      		
		     }
		     else if (strncmp(buffer1,"LIST",4) == 0) {
	      		//buffer = (char*)malloc(MAXLINE);
	      		//strcpy(buffer,getList(user_list));
	      		int len = strlen(getList(user_list));
	      		
			      write(fd,getList(user_list),len);
	     
	    	    }
		    else if (strncmp(buffer1,"CHALLENGE",9) == 0) {

		      // buffer chua username bi thach dau
		      char* buffer = strtok(buffer1, " ");
		      buffer = strtok(NULL, " ");buffer[strlen(buffer)-1]='\0';
          //printf("ab\n");
          // kiem tra user co available khong
          if (getFD(user_list,buffer) == -1 || getFD(user_list,buffer) == fd) {
              strcpy(buffer2,"INVALID\n");
              write(fd,buffer2,strlen(buffer2));
              continue;
          }
          if (getRoomByFD(user_list,getFD(user_list,buffer)) != 0 ) {
              strcpy(buffer2,"UNAVAILABLE\n");
              write(fd,buffer2,strlen(buffer2));
              continue;
          }
	
		      // send loi thach dau den user bi thach dau
          strcat(buffer2,"CHALLENGED ");
		      strcat(buffer2,getUsername(user_list,fd));
		      strcat(buffer2,"\n");
		      int len = strlen(buffer2);
		      //printf("%d\n",getFD(user_list,buffer));
		      write(getFD(user_list,buffer),buffer2,strlen(buffer2));
		    }
		    else if (strncmp(buffer1,"ACCEPT",6) == 0) {
	   		// buffer chua username thach dau
	      		char* buffer = strtok(buffer1, " ");
	      		buffer = strtok(NULL, " ");buffer[strlen(buffer)-1]='\0';      
	     		  //strcat(buffer2,getUsername(user_list,fd));
	      		//strcat(buffer2," chap nhan thach dau.\n");
            strcpy(buffer2,"ACCEPTED\n");
	      		int len = strlen(buffer2);
	      		
			      write(getFD(user_list,buffer),buffer2,len);
	      		
	      		int new_room = availableRoom(user_list);
			      printf("room: %d\n",new_room);
	      		generateRoom(user_list,getFD(user_list,buffer),new_room,0);
	      		generateRoom(user_list,fd,new_room,1);
	    	    }
		    
		    else if (strncmp(buffer1,"REFUSE",6) == 0) {
			char* buffer = strtok(buffer1, " ");
	      		buffer = strtok(NULL, " ");buffer[strlen(buffer)-1]='\0';	      
	     		strcat(buffer2,getUsername(user_list,fd));
	      		strcat(buffer2," tu choi thach dau.\n");
			int len = strlen(buffer2);
	      		
			write(getFD(user_list,buffer),buffer2,len);
		    }
		    else if (strncmp(buffer1,"MOVE",4) == 0) {
			if (getRoomByFD(user_list,fd) == 0) {
			   strcpy(kb_msg,"Chua co doi thu choi cung\n");
			   write(fd,kb_msg,strlen(kb_msg));
			}
			else {
			   int room = getRoomByFD(user_list,fd);
			   char* buffer = strtok(buffer1, " ");
	      		   buffer = strtok(NULL, " ");
			   strcpy(buffer2,buffer);buffer[strlen(buffer)-1]='\0';	
			   write(getOtherFDByRoom(user_list,room,fd),msg,strlen(msg));
			   printf("Nuoc di %s tu %s da duoc gui den %s\n",buffer,getUsername(user_list,fd),getUsername(user_list,getOtherFDByRoom(user_list,room,fd)));
			}
		    }
        else if (strncmp(buffer1,"END",3) == 0) {
            int room = getRoomByFD(user_list,fd);
         
         write(getOtherFDByRoom(user_list,room,fd),msg,strlen(msg));
         printf("Nuoc di %s tu %s da duoc gui den %s\n",buffer1,getUsername(user_list,fd),getUsername(user_list,getOtherFDByRoom(user_list,room,fd)));
        }
		    else if (strncmp(buffer1,"EXIT",4) == 0) {
			int room = getRoomByFD(user_list,fd);
			generateRoom(user_list,fd,0,-1);
			
			int otherfd = getOtherFDByRoom(user_list,room,fd);
			generateRoom(user_list,otherfd,0,-1);
			printf("Phong so %d available\n",room);
      write(otherfd,buffer1,strlen(buffer1));
		    }
		    else {
                    	/*print to other clients*/
                    	/*for(i=0;i<num_clients;i++){
				
				if (fd_array[i] == fd)  
                          	write(fd_array[i],msg,strlen(msg));
                    	}*/
			
			strcpy(kb_msg,"Loi cu phap\n");
			write(fd,kb_msg,strlen(kb_msg));
			printf("%s",msg);
			
		    }                 
                    free(buffer1);free(buffer2);
                 }                                   
              }                  
              else {  /* A client is leaving */
                 exitClient(fd,&readfds, fd_array,&num_clients);
              }//if
           }//if
        }//for
     }//while
  }//end Server code

}//main

