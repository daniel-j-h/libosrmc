PREFIX = /usr/local

VERSION_MAJOR = 5
VERSION_MINOR = 3

CXXFLAGS = -O2 -Wall -Wextra -pedantic -std=c++11 -fvisibility=hidden -fPIC -fno-rtti $(shell pkg-config --cflags libosrm)
LDFLAGS  = -shared -Wl,-soname,libosrmc.so.$(VERSION_MAJOR)
LDLIBS   = -lstdc++ $(shell pkg-config --libs libosrm)
