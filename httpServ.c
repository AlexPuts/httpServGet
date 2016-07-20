#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#define MAX_ARG 200
#include <fcntl.h>

int read_in(int socket, char *buf, int len)
{
  char *s =buf;
  int slen=len;
  int c=recv(socket, s, slen,MSG_NOSIGNAL);
  while ((c>0)&&(s[c-1]!='\n')){
  s+=c; slen-=c;
  c=recv(socket, s,slen,0);
}
if (c<0)
  return c;
else if (c==0)
  buf[0]='\0';
else
  s[c-1]='\0';
return len-slen;
}

int open_listener_socket()
{
  int s=socket(PF_INET, SOCK_STREAM,0);
}

void bind_to_port(int socket, int port)
{
  struct sockaddr_in name;
  name.sin_family=PF_INET;
  name.sin_port=(in_port_t)htons(port);
  name.sin_addr.s_addr=htonl(INADDR_ANY);
  int reuse=1;

 setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int));
  int c=bind(socket, (struct sockaddr *)&name, sizeof(name));
}

int say(int socket, char *s)
{
  int result=send(socket, s, strlen(s),0);
}

int listener_d;

int main(int argc, char **argv){

    if(argc == 1) { // если запускаем без аргументов, выводим справку
        printf("usage:\n");
        printf("-h <ip> -p <port> -d <directory>");
        return 0;
    }
    // Arguments
    char ip[200]={0};
    char port[200]={0};
    char directory[200]={0};
    char htmlpath[200]={0};

    char *opts = "h:p:d:"; // доступные опции, каждая принимает аргумент
    char op; // а тут оператор
    int opt; // каждая следующая опция попадает сюда

    while((opt = getopt(argc, argv, opts)) != -1) { // вызываем getopt пока она не вернет -1
        switch(opt) {

                case 'h': strncpy(ip, optarg, MAX_ARG); break;

				case 'p': strncpy(port, optarg, MAX_ARG); break;

				case 'd': strncpy(directory, optarg, MAX_ARG); break;

				default: /*printf("usage:\n -h <ip> -p <port> -d <directory>\n");*/ exit(1);
        }

    }
        //puts(ip);
        //puts(port);
        //puts(directory);

  listener_d=open_listener_socket();
  bind_to_port(listener_d,atoi(port));
  if(listen(listener_d,10)==-1)
    error("Cant listen");
  struct sockaddr_storage client_addr;
  unsigned int address_size=sizeof(client_addr);
  //puts("Waiting for connection");

  char input[200]={0};
  int position_1=0;
  int position_2=0;
  int i=0;
  if(fork()){
  exit(0);
  }
  /*Descriptors close*/
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

char fullpath[250]={0};
strcpy(fullpath,directory);

fork();
fork();
fork();
fork();
fork();
fork();
fork();
fork();
  while (1){
    wait();
    int connect_d=accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
      char buf[1255]={0};
      if(!fork()){
      close(listener_d);

      read_in (connect_d, buf,sizeof(buf));
      if(strncasecmp(" GET",buf,4)){
      strcpy(input,buf);
      while(i<sizeof(buf))
      { //printf("CHAR  =  %c \n INT = %d ",buf[i],buf[i]);
      if(buf[i]==120){
      if(buf[i+1]==46)
      if(buf[i+2]==104)
      if(buf[i+3]==116)
      if(buf[i+4]==109)
      break;
      }
      i++;
      }
      position_2=i+6;
      i=0;
      while(i<sizeof(buf))
      {
      if(buf[i]==32){
      //printf("%d",i);
      break;
      }
      i++;
      }
      position_1=i+1;
      //printf("I = %d", i); //position_2=15;
      strncpy(htmlpath,&buf[position_1],position_2-position_1);
      //printf("%d",position_2);
      strncat(directory,htmlpath,strlen(htmlpath));
      //puts("THE PATH IS:");
      //puts(directory);
     //Reply 200 if file exists
    int fd = open(directory, O_RDONLY|O_NONBLOCK);
    if (fd >0)
    {
        char reply[1024]={0};
        char buf[1024]={0};
        //int sz = lseek(fd, 0, SEEK_END);
        int sz = read(fd, buf, 1000);
        close(fd);
        sprintf(reply, "HTTP/1.0 200 OK\r\n"
                       "Content-Type: text/html\r\n"
                       "Content-length: %d\r\n"
                       "Connection: close\r\n"
                       "\r\n"
                       "%s"
                       , sz,buf);
        say(connect_d, reply);
           close(connect_d);
           exit(0);
    }
    //Reply 404 if file doesnt exist
    else
    {
    char reply[1024]={0};
        sprintf(reply, "HTTP/1.0 404 Not Found\r\n"
                      "Content-Type: text/html\r\n"
                      "Content-length: 0\r\n"
                      "Connection: close\r\n"
                      "\r\n");

        say(connect_d, reply);
        close(connect_d);
        exit(0);
    }
}
close(connect_d);
strcpy(directory,fullpath);
 exit(0);
  }
  }
  return 0;
  }

