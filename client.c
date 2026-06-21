#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}
int main(int argc,char *argv[]){
    if(argc < 3){
        fprintf(stderr ,"usage %s hotname port \n",argv[0]);
        exit(1);}
    int sockfd , portno,n;
    char buffer[255],str[255];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    portno=atoi(argv[2]);
    if(sockfd<0){
        error("Error oprning Socket");}
        server = gethostbyname(argv[1]);
        if (server==NULL){
        fprintf(stderr,"Error,no such host");}
        bzero((char *)&serv_addr,sizeof(serv_addr));
        serv_addr.sin_family=AF_INET;
        bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
        serv_addr.sin_port=htons(portno);
        if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
            error("connection error");
        }
        pid_t pid = fork();
    if (pid==0){
        while(1){
        bzero(buffer,255);
        n = read(sockfd,buffer,255);
        if (n<0){
            error("error in reading");
        }
        printf("client : %s\n",buffer);
    }}
    if (pid>0){
        while(1){
        bzero(str,255);
        fgets(str,255,stdin);
        n=write(sockfd,str,strlen(str));
        if(n<0){
            error("error while writeing.");
        }
        }
    }

        close(sockfd);
        return 0;
}
