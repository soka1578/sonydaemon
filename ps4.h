#ifndef PS4_H
#define PS4_H

#include<string>
#include<cstring>   //menset
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netdb.h>
#include<iostream>
#include<sstream> 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> 


#define BUFLEN 512  //Max length of buffer
#define PORT 987   //The port on which to send data

using namespace std;

class Ps4{
        const char* _ip;
        int sock;
        uint slen;
        struct sockaddr_in si_other;
        char buf[BUFLEN];
        char message[BUFLEN];
        bool initialize();
        bool query();

    public:
        Ps4();
        Ps4(const string url);
        ~Ps4();
        bool playing();
        string runningApp(); 

};

#endif
