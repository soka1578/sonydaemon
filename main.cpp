#include "main.h"

string config="/etc/sonydaemon.conf";
string url;
string xbmc_url = "http://127.0.0.1:8080/jsonrpc?request={%22jsonrpc%22:%20%222.0%22,%20%22method%22:%20%22Player.GetActivePlayers%22,%20%22id%22:%201}";
string ps3_host;
string ps4_host;
string wake;
int status;
const char* cmd_off = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"> <s:Body> <u:X_SendIRCC xmlns:u=\"urn:schemas-sony-com:service:IRCC:1\"> <IRCCCode>AAAAAQAAAAEAAAAvAw==</IRCCCode> </u:X_SendIRCC> </s:Body> </s:Envelope>";
mutex player_mutex;
mutex ps3_mutex;
mutex ps4_mutex;
bool player = false;
bool ps3 = false;
bool ps4 = false;

int main(int argc, char *argv[]){
	load_config(config);		//parse configuration file
	Dsp dsp;
	int last_status = ON;		//we start with monitor on	
	int status;	
	#ifdef ENABLE_XBMC
	thread t_display(watch, &dsp);	//so that it does not go out of scope
	#endif
	#ifdef ENABLE_PS3
	thread t_ps3(ps3_watch, ps3_host.c_str());
	#endif	
    #ifdef ENABLE_PS4
	thread t_ps4(ps4_watch);
	#endif
	while(1){
		status = dsp.query_display();
		#ifdef DEBUG
		fprintf(stderr, "status=%i, last_status=%i \n",status,last_status);
		#endif
		player_mutex.lock();
		ps3_mutex.lock();
        ps4_mutex.lock();
		if(!player && !ps3 && !ps4){	
			if(status==ON && last_status==OFF){
				last_status=ON;			//monitor went ON
				#ifdef DEBUG
				fprintf(stderr, "Monitor went ON\n");
				#endif
				FILE* wake_p = popen(wake.c_str(), "r");	//turn on tv
                if(wake_p != NULL){
    				pclose(wake_p);
                }
			}
			if(status==OFF && last_status==ON){
				last_status=OFF;			//monitor went OFF
				#ifdef DEBUG
				fprintf(stderr, "Monitor went OFF\n");
				#endif
				send_msg(url,30,cmd_off);	// turn off tv
			}
		}
        ps4_mutex.unlock();
		ps3_mutex.unlock();
		player_mutex.unlock();	
		sleep(5);
	}
	return 0;
}

CURLcode send_msg(const string& url, long timeout, const char* msg){
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
//		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msg);
		code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return code;
}

size_t curl_write(char* buf, size_t size, size_t nmemb, void* up){
//	string data;
//	data.append(buf, size * nmemb);
	return size * nmemb;
}

void load_config(const string& conf){
	ifstream ifs;
	ifs.open(conf.c_str(),ifstream::in);
	if(!ifs){
		fprintf(stderr, "File %s not found\n",conf.c_str());
		exit(1);
	}
	string line;
	string key;
	string value;
	while(getline(ifs,line)){
		istringstream is(line);
		if(is.peek()!='#'){		//omit comments
			getline(is,key,'=');
			is >> ws;			//remove white space
			key.erase(key.end()-1);
			getline(is,value);
			if(key=="url"){
				url=value;
			}
			if(key=="mac"){
				wake="wakeonlan ";
				wake = wake +value;
			}
			if(key=="ps3"){
				ps3_host=value;
			}
            if(key=="ps4"){
				ps4_host=value;
			}
		}
	}
	ifs.close();
}

#ifdef ENABLE_XBMC
void watch(Dsp* display){
	Xbmc xbmc(xbmc_url);//xbmc initialization
	bool status;
	while(1){
		#ifdef DEBUG
		fprintf(stderr, "player=%i, status=%i\n",player,status);
		#endif
		status = xbmc.query_player();
		player_mutex.lock();
		if(status && !player){	//player starts
			#ifdef DEBUG
			fprintf(stderr, "Player starts\n");
			#endif
			player=true;
			display->dpms_off();	//turn off dpms
		}
		if(!status && player){	//player stops
			#ifdef DEBUG
			fprintf(stderr, "Player stops\n");
			#endif
			player=false;	
			display->dpms_on();	//turn on dpms
		}
		player_mutex.unlock();
		sleep(30);
	}
}
#endif

#ifdef ENABLE_PS3
void ps3_watch(const char* host){
	char buffer[50];
	char res[20];
	while(1){
        sprintf(buffer,"ping -c 3 %s | grep -c ms",host);
        memset(&res, 0, sizeof(res));   //clear res buffer
        FILE *p = popen(buffer,"r");
        if(p != NULL){
            fgets(res, 5, p);   //so that it doesn't segfault on NULL File returned from popen
            pclose(p);
        }
		ps3_mutex.lock();
        if(strcmp(res,"5\n") == 0){
			#ifdef DEBUG
			fprintf(stderr, "PS3 running\n");
			#endif
			ps3 = true;
        }
        else{
			#ifdef DEBUG
			fprintf(stderr, "PS3 stopped\n");
			#endif
			ps3 = false;
        }
		ps3_mutex.unlock();
        sleep(30);
	}
}
#endif

#ifdef ENABLE_PS4
void ps4_watch(){
    Ps4 ps(ps4_host);
//    ps.initialize();
	while(1){
        ps4_mutex.lock();
        if(ps.playing()){
			#ifdef DEBUG
			fprintf(stderr, "PS4 running: %s\n",ps.runningApp().c_str());
			#endif
			ps4 = true;
        }
        else{
			#ifdef DEBUG
			fprintf(stderr, "PS4 stopped\n");
			#endif
			ps4 = false;
        }
		ps4_mutex.unlock();
        sleep(30);
	}
}
#endif
