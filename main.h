#ifndef MAIN_H
#define MAIN_H
#include "display.h"
#include "xbmc.h"
#include "ps4.h"
#include <getopt.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <stdio.h> 
#include <thread>
#include <mutex>
#include <sys/types.h> 
#include <curl/curl.h>

#define PROGRAM_NAME "sonydaemon"
#define EXIT_CODE_OK 0 
#define EXIT_CODE_ERROR 1

using namespace std;

void load_config(const string& conf);
CURLcode send_msg(const string& url, long timeout, const char* msg);
static size_t curl_write(char* buf, size_t size, size_t nmemb, void* up);
void watch(Dsp* display);
void ps3_watch(const char* host);
void ps4_watch();
#endif
