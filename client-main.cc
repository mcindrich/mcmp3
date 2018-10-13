#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <mcmp3/mp-types.h>
#include <dirent.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

mp_signal_t get_sig(char* msg) {
    mp_signal_t res = mp_signal_t_unknown;
    if(strcmp(msg, "play") == 0) res = mp_signal_t_play;
    else if(strcmp(msg, "stop") == 0) res = mp_signal_t_stop;
    else if(strcmp(msg, "pause") == 0) res = mp_signal_t_pause;
    else if(strcmp(msg, "end") == 0) res = mp_signal_t_end_connection;
    else if(strcmp(msg, "cs") == 0) res = mp_signal_t_change_song;
    else if(strcmp(msg, "close") == 0) res = mp_signal_t_close_server;
    return res;
}

int main(int argc , char *argv[]) {
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    mp_signal_t send_signal;
    char* filename = NULL;
     
    if(argc == 3) {
		send_signal = (mp_signal_t) get_sig(argv[1]);
		filename = argv[2];
	} else return 0;

    if(send_signal == mp_signal_t_unknown) {
        printf("Invalid signal to send\nClosing the program...\n");
        return 0;
    }
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("192.168.5.10");
    server.sin_family = AF_INET;
    server.sin_port = htons( 1213 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    printf("Connected.\n");

    int a = (int) send_signal;
    int conv = htonl(a);
    write(sock, &conv, sizeof(int));
    send(sock , filename , strlen(filename) , 0);
    char response[2000];
    recv(sock, response, 2000, 0);
    cout << "Response: " << response << endl;
    conv = htonl(mp_signal_t_end_connection);
    write(sock, &conv, sizeof(int));
    send(sock , filename , strlen(filename) , 0);
    close(sock);
    return 0;
}
