#include "ps4.h"

Ps4::Ps4(){}

Ps4::Ps4(const string url){
    struct hostent *he;
    he = gethostbyname(url.c_str());
    if(he != NULL){
        _ip = inet_ntoa(*((struct in_addr *)he->h_addr_list[0]));
    }
}

Ps4::~Ps4(){
}

bool Ps4::initialize(){
    slen=sizeof(si_other);
    memset((char *) &si_other, 0, sizeof(si_other));
    
    if(inet_aton(_ip , &si_other.sin_addr) == 0){
        fprintf(stderr, "inet_aton() failed\n");
        return false;
    }

    //create socket 
    if((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        #ifdef DEBUG
		//fprintf(stderr,"Error: %i\n",errno);
        perror("socket");
        #endif
        return false;
    }
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    //set timeout interval
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)) == -1){
        return false;
    }
    //set message 
    sprintf(message,"SRCH * HTTP/1.1\n device-discovery-protocol-version:00010010\n");
    return true;
}

bool Ps4::query(){
    //send the message
    if(sendto(sock, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1){
        #ifdef DEBUG
        perror("socket send");
		#endif
        return false;
    }
    //receive a reply, clear the buffer by filling null, it might have previously received data
    memset(buf,'\0', BUFLEN);
    //try to receive some data, this is a blocking call
    if(recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1){
        #ifdef DEBUG
		perror("socket receive");
        #endif
        return false;
    }
    return true;
}

bool Ps4::playing(){
    bool res = false;
    if(initialize()){
        if(query()){
            string reply(buf);
            int code = atoi(reply.substr(9,3).c_str());
            #ifdef DEBUG
	        fprintf(stderr, "code: %i\n",code);
            #endif
            switch(code){
                case 620:
                    break;   //rest mode
                case 200:{
                    if(reply.find("running-app-name:") != string::npos){
                        res = true;
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
    //close socket
    if(sock != -1){
        close(sock);
    }
    return res;
}

string Ps4::runningApp(){
    stringstream ss(buf);
    string line;
    for(int i = 0; i < 7; i++){
        getline(ss,line);
    }
    return line.substr(17);
}
