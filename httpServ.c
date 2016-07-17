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
#define MAX_ARG 20
#include <fcntl.h>
int catch_signal(int sig, void(*handler)(int))
{
  struct sigaction action;
  action.sa_handler=handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags=0;
  return sigaction(sig,&action, NULL);
}

int read_in(int socket, char *buf, int len)
{
  char *s =buf;
  int slen=len;
  int c=recv(socket, s, slen,0);
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

void error(char *msg)
{
fprintf(stderr, "%s: %s\n", msg,strerror(errno));
exit(1);
}

int open_listener_socket()
{
  int s=socket(PF_INET, SOCK_STREAM,0);
  if (s==-1)
    //error("Can't open socket");
  return s;
}

void bind_to_port(int socket, int port)
{
  struct sockaddr_in name;
  name.sin_family=PF_INET;
  name.sin_port=(in_port_t)htons(port);
  name.sin_addr.s_addr=htonl(INADDR_ANY);
  int reuse=1;

  if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int))==-1)
    //error("Cant set up socket reuse parameter");
  int c=bind(socket, (struct sockaddr *)&name, sizeof(name));
  if (c==-1)
    //error("Cant bind the socket");
}

int say(int socket, char *s)
{
  int result=send(socket, s, strlen(s),0);
  if (result==-1)
    //fprintf(stderr, "%s: %s\n", "Communication Error !", strerror(errno));
  return result;
}

int listener_d;

void handle_shutdown(int sig)
{
  if(listener_d)
    close(listener_d);
  //fprintf(stderr, "Пока !\n");
  exit(0);
}

int main(int argc, char **argv){

    if(argc == 1) { // если запускаем без аргументов, выводим справку
        //printf("usage:\n");
        //printf("-h <ip> -p <port> -d <directory>");
        return 0;
    }
    // Arguments
    char ip[20]={0};
    char port[20]={0};
    char directory[30]={0};
    char htmlpath[20]={0};

    char *opts = "h:p:d:"; // доступные опции, каждая принимает аргумент
    char op; // а тут оператор
    int opt; // каждая следующая опция попадает сюда

    while((opt = getopt(argc, argv, opts)) != -1) { // вызываем getopt пока она не вернет -1
        switch(opt) {

                case 'h': strncpy(ip, optarg, MAX_ARG); break;

				case 'p': strncpy(port, optarg, MAX_ARG); break;

				case 'd': strncpy(directory, optarg, MAX_ARG); break;

				default: printf("usage:\n -h <ip> -p <port> -d <directory>\n"); exit(1);
        }

    }
        //puts(ip);
        //puts(port);
        //puts(directory);

 if(catch_signal(SIGINT, handle_shutdown)==-1)
  //error("Cant set the interrupt handler");
  listener_d=open_listener_socket();
  bind_to_port(listener_d,atoi(port));
  if(listen(listener_d,10)==-1)
    //error("Cant listen");
  struct sockaddr_storage client_addr;
  unsigned int address_size=sizeof(client_addr);
  //puts("Waiting for connection");
  char buf[255]={0};;
  char input[200]={0};
  int position_1=4;
  int position_2=0;
  int i=position_1;
  if(fork()){
  exit(0);
  }

 if(fork()){
  wait();
  }

char fullpath[50]={0};
strcpy(fullpath,directory);
  while (1){
    int connect_d=accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
    if (connect_d==-1)
      error("Cant open secondary socket");

      if(!fork()){
      close(listener_d);
      if (say(connect_d,"Server \r\n Version 1.0\r\n")!=-1)
      read_in (connect_d, buf,sizeof(buf));
      //puts(buf);
      if(strncasecmp(" GET",buf,4)){
      strcpy(input,buf);
      while(i<20)
      { //printf("CHAR  =  %c \n INT = %d ",buf[i],buf[i]);
      if(buf[i]==108){
      //printf("%d",i);
      break;
      }
      i++;
      }
      position_2=i+1;
      //printf("I = %d", i); //position_2=15;
      strncpy(htmlpath,&buf[position_1],position_2-position_1);
      //printf("%d",position_2);
      strncat(directory,htmlpath,strlen(htmlpath));
      puts("THE PATH IS:");
      puts(directory);

     //Reply 200 if file exists
          char reply[1024];
    if (access(directory, F_OK) != -1)
    {
        // file exists, get its size
        int fd = open(directory, O_RDONLY);
        int sz = lseek(fd, 0, SEEK_END);;

        sprintf(reply, "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html\r\n"
                       "Content-length: %d\r\n"
                       "Connection: close\r\n"
                       "\r\n", sz);
        say(connect_d, reply);

           off_t offset = 0;
        while (offset < sz)
        {
            offset = sendfile(connect_d, fd, &offset, sz - offset);
        }
           close(connect_d);
           exit(0);
    }

    //Reply 404 if file doesnt exist
 else
    {
        strcpy(reply, "HTTP/1.1 404 Not Found\r\n"
                      "Content-Type: text/html\r\n"
                      "Content-length: 107\r\n"
                      "Connection: close\r\n"
                      "\r\n");
        say(connect_d, reply);
        strcpy(reply, "<body>\n<p>404 Request file not found.</p>\n</body>\n</html>\r\n");
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
}







