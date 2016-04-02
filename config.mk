CXXFLAGS = -O2 -Wall -Wextra -pedantic -std=c++11 -fvisibility=hidden -fPIC -fno-rtti -I$(shell pkg-config --cflags libosrm)
LDFLAGS  = -shared -Wl,-soname,libosrmc.so.1
LDLIBS   = -lstdc++ $(shell pkg-config --libs libosrm)
