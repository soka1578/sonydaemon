#include "display.h"

Dsp::Dsp(){
	_dpy = XOpenDisplay(_disp);    //Open display and check for success
	if(_dpy == NULL){
		fprintf(stderr, "unable to open display \"%s\"\n",XDisplayName(_disp));
		exit(EXIT_FAILURE);
	}
}

Dsp::~Dsp(){
	XCloseDisplay(_dpy);	//Close display
}

int Dsp::monitor_on(){
	BOOL onoff;
	CARD16 state;
	DPMSInfo(_dpy, &state, &onoff);
	if(onoff){
		switch(state){
	    	case DPMSModeOn:
			return ON;
			break;
	    	case DPMSModeStandby:
			return OFF;
			break;
	    	case DPMSModeSuspend:
			return OFF;
			break;
	    	case DPMSModeOff:
			return OFF;
			break;
	    	default:
			fprintf(stderr, "Unrecognized response from server\n");
			exit(EXIT_FAILURE);
    	}
	}
	return ON;	//dpms disable, monitor on
}

int Dsp::query_display(){
	return monitor_on();
}

void Dsp::dpms_off(){
	int dummy;
	if(DPMSQueryExtension(_dpy, &dummy, &dummy)){
		DPMSDisable(_dpy);
	}
	else{
		fprintf(stderr, "server does not have extension for -dpms\n");
	}
}

void Dsp::dpms_on(){
	int dummy;
	if(DPMSQueryExtension(_dpy, &dummy, &dummy)){
		DPMSEnable(_dpy);
	}
	else{
		fprintf(stderr, "server does not have extension for +dpms\n");
	}
}

