EXTRA_DIST = autogen.sh sonydaemon.conf

ACLOCAL_AMFLAGS = -I m4 --install

bin_PROGRAMS = sonydaemon

sonydaemon_SOURCES := main.h main.cpp 
sonydaemon_SOURCES += display.h display.cpp

sonydaemon_CXXFLAGS = -std=c++11 -pthread

if ENABLE_XBMC
sonydaemon_SOURCES += xbmc.h xbmc.cpp 
sonydaemon_CXXFLAGS += -DENABLE_XBMC
endif

if ENABLE_DEBUG
sonydaemon_CXXFLAGS += -DDEBUG
endif

if ENABLE_PS3
sonydaemon_CXXFLAGS += -DENABLE_PS3
endif

if ENABLE_PS4
sonydaemon_SOURCES += ps4.h ps4.cpp 
sonydaemon_CXXFLAGS += -DENABLE_PS4
endif
