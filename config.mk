CXXFLAGS = -O2 -Wall -Wextra -pedantic -std=c++11 -fvisibility=hidden -fPIC -I$(shell pkg-config --cflags libosrm)
LDFLAGS  = -shared -Wl,-soname,libosrmc.so.1 $(shell pkg-config --libs libosrm)
LDLIBS   = -lstdc++ -lm
