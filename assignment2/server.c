// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 80 
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    void do_execvp(int server_fd);

    if (argc == 1) {   
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    // Privilege separation
    pid_t child_process = fork();
    if (child_process < 0) 
    {
        perror("Failed to fork the process");
        exit(EXIT_FAILURE);
    } else if (child_process == 0) 
    {
      do_execvp(server_fd);
    }

    else 
    {
      // Wait to finish
       int status = 0;
       while ((wait(&status)) > 0);
    }
    }

    /* This will be executed when the server is re-exec using execvp 
      with an argument having the server socket file descriptor
    */ 
    else 
    {
      // Privilege dropped to nobody user    
      if(setuid(65534) < 0){
         perror("Failed to drop privilege");
         exit(EXIT_FAILURE);
      }
      const char *server_fd_str = argv[1];
      server_fd = atoi(server_fd_str);

      if (listen(server_fd, 3) < 0)
      { 
        perror("listen"); 
        exit(EXIT_FAILURE);
      } 
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0)
      { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
      }
      valread = read( new_socket , buffer, 1024); 
      printf("%s\n",buffer ); 
      send(new_socket , hello , strlen(hello) , 0 ); 
      printf("Hello message sent\n"); 
    } 
    return 0; 
}

// Runs execvp
void do_execvp(int server_fd)
{
  char server_fd_str[12];
  sprintf(server_fd_str, "%d", server_fd);
  char *args[] = {"./server", server_fd_str, NULL};
  if (execvp(args[0], args) < 0)
  {
    perror("exec");
    exit(EXIT_FAILURE);
  };
}

