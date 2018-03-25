# sonydaemon

sonydaemon let's you control your sony smart tv from your pc so that it turns on and off depending of the pc monitor dpms state.

Dependencies
------------

This project requires some libraries and headers to be built :

- OpenSSL
- Curl
- Pthreads
- X.Org xproto protocol header
- X.Org X11 library
- X.Org Xext library
- X C-language Bindings library
- Jansson C library

Compile
-------

This program runs under Linux.

Run ./autogen.sh followed by ./configure (autoconf and automake
need to be installed on your system for autogen.sh to work). 

Finally run make to compile the program.
 
