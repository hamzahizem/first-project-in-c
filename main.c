#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>
#include <sys/mman.h>
#include <poll.h>
void error(const char *msg){
    perror(msg);
    exit(1);}

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(stderr ,"port is not provided , pls write the port pls.\n");
        exit(1);}
    int sockfd , newsockfd , portno,n=1,sockfdto,x=1,i=0;
    char buffer[255],str[255];
    struct sockaddr_in serv_addr , cli_addr,clin_addr;
    socklen_t clilen;
    socklen_t clilen2;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        error("Error oprning Socket");}
    bzero((char *) &serv_addr ,sizeof(serv_addr));
    portno=atoi(argv[1]);
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr= INADDR_ANY;
    serv_addr.sin_port=htons(portno);
    if(bind(sockfd,(struct sockaddr *) & serv_addr ,sizeof(serv_addr))<0){
        error("Binding Failed.");}
    listen(sockfd,5);
    struct pollfd fds[3];
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    fds[1].fd = -1;
    fds[2].fd = -1;
    while(1){
        int poll1=poll(fds,3,-1);
        if(poll1<0){
            perror("Accept Failed");}
        if(fds[0].revents & POLLIN && fds[1].fd==-1){
            clilen=sizeof(cli_addr);
            newsockfd =accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
            fds[1].fd=newsockfd;
            fds[1].events =POLLIN;
            i++;
            if(newsockfd<0){
                error("accept failed");}}
        else if(fds[0].revents & POLLIN && fds[1].fd!=-1){
            clilen2=sizeof(clin_addr);
            sockfdto =accept(sockfd,(struct sockaddr *) &clin_addr,&clilen2);
            fds[2].fd=sockfdto;
            fds[2].events=POLLIN;
            i++;
            if(sockfdto<0){
                error("accept failed");}}
        if(fds[1].fd != -1 && (fds[1].revents & POLLIN)){
            bzero(buffer, sizeof(buffer));
            int n = recv(fds[1].fd, buffer, sizeof(buffer), 0);
            if (n <= 0) {
                printf("Client 1 disconnected.\n");
                close(fds[1].fd);
                fds[1].fd = -1;
                i--;
            } else if (fds[2].fd != -1) {
                send(fds[2].fd, buffer, n, 0);}}
        if (fds[2].fd != -1 && (fds[2].revents & POLLIN)) {
            bzero(buffer, sizeof(buffer));
            int n = recv(fds[2].fd, buffer, sizeof(buffer), 0);
            if (n <= 0) {
                printf("Client 2 disconnected.\n");
                close(fds[2].fd);
                fds[2].fd = -1;
                i--;
            } else if (fds[1].fd != -1) {
                send(fds[1].fd, buffer, n, 0);
            }
        }


        }


    close(newsockfd);
    close(sockfdto);
    close(sockfd);
    return 0;}








