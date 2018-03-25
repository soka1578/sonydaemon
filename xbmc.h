#ifndef XBMC_H
#define XBMC_H
#include <curl/curl.h>
#include <string>
#include <jansson.h>

using namespace std;

class Xbmc{
		string _url;
		string _data;
		bool _playing;
		void query();
		static size_t curl_write(char* buf, size_t size, size_t nmemb, void* up);
		size_t handle(char * buf, size_t size, size_t nmemb);
		CURLcode curl_read(const string& url, long timeout);

	public:
		Xbmc();
		Xbmc(const string& url);
		~Xbmc();
		bool query_player();

};



#endif
