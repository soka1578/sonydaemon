#include "xbmc.h"

Xbmc::Xbmc(){}

Xbmc::Xbmc(const string& url){
	_url = url;
	_playing = false;
}

Xbmc::~Xbmc(){
}

size_t Xbmc::curl_write(char* buf, size_t size, size_t nmemb, void* up){
	return static_cast<Xbmc*>(up)->handle(buf, size, nmemb);
}

size_t Xbmc::handle(char* buf, size_t size, size_t nmemb){
	_data.append(buf, size * nmemb);
    return size * nmemb;
}

CURLcode Xbmc::curl_read(const string& url, long timeout){
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Xbmc::curl_write);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		code = curl_easy_perform(curl);
		if(code != CURLE_OK){
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(code));
		}
		curl_easy_cleanup(curl);
        curl_global_cleanup();
	}
	return code;
}

void Xbmc::query(){
	_data.clear();
	if(curl_read(_url,30)==CURLE_OK){		//read xbmc api page
		json_error_t error;
		int status;
		int playerid = -1;
		const char * type;
		json_t *root = json_loads(_data.c_str(), 0, &error);
		if(!root){
			_playing = false; //xbmc down
			fprintf(stderr, "%s error: on line %d: %s\n",__PRETTY_FUNCTION__,error.line, error.text);
	        return;
		}
		root = json_object_get(root,"result");
		if(!json_is_array(root)){
	        fprintf(stderr, "%s error: not an array\n",__PRETTY_FUNCTION__);
	        json_decref(root);
			exit(1);
    	}
		for(uint i=0; i < json_array_size(root); i++){
			json_t *data;
			data = json_array_get(root, i);
			status = json_unpack_ex(data, &error, 0, "{s:F,s:s}", \
			"playerid", &playerid, \
			"type", &type);
			if(status){
				fprintf(stderr, "%s error: %s\n", __PRETTY_FUNCTION__, error.text);
		        exit(1);
			}
		}

		if(playerid != -1){
			_playing = true;		
		}
		else{
			_playing = false;
		}
	}
}

bool Xbmc::query_player(){
	query();
	return _playing;
}
