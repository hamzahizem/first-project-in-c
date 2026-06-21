#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}
int main(int argc,char *argv[])
{

    if(argc < 2){
        fprintf(stderr ,"port is not provided , pls write the port pls.\n");
        exit(1);}
    int sockfd , newsockfd , portno,n;
    char buffer[255],str[255];
    struct sockaddr_in serv_addr , cli_addr;
    socklen_t clilen;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        error("Error oprning Socket");}
    bzero((char *) &serv_addr ,sizeof(serv_addr));
    portno=atoi(argv[1]);

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr= INADDR_ANY;
    serv_addr.sin_port=htons(portno);
    if(bind(sockfd,(struct sockaddr *) & serv_addr ,sizeof(serv_addr))<0){
        error("Binding Failed.");
    }
    listen(sockfd,5);
    clilen=sizeof(cli_addr);
    newsockfd =accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
    if (newsockfd<0){
        error("error on accepting the conection");
    }
    pid_t pid = fork();
    if (pid==0){
        while(1){
        bzero(buffer,255);
        n = read(newsockfd,buffer,255);
        if (n<0){
            error("error in reading");
        }
        printf("client : %s\n",buffer);
    }}
    if (pid>0){
        while(1){
        bzero(str,255);
        fgets(str,255,stdin);
        n=write(newsockfd,str,strlen(str));
        if(n<0){
            error("error while writeing.");
        }
        }
    }



    close(newsockfd);
    close(sockfd);
    return 0;
}
