#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdlib.h>
#include <stdio.h>   
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/dpms.h>

#define ON 1
#define OFF 0

class Dsp{
		Display* _dpy;
		char* _disp = NULL;
		int monitor_on();
	public:
		Dsp();
		~Dsp();
		int query_display();
		void dpms_off();
		void dpms_on();

};

#endif
