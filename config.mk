PREFIX = /usr/local

VERSION_MAJOR = 1
VERSION_MINOR = 0

CXXFLAGS = -O2 -Wall -Wextra -pedantic -std=c++11 -fvisibility=hidden -fPIC -fno-rtti $(shell pkg-config --cflags libosrm)
LDFLAGS  = -shared -Wl,-soname,libosrmc.so.$(VERSION_MAJOR)
LDLIBS   = -lstdc++ $(shell pkg-config --libs libosrm)
