CXXFLAGS = -O2 -Wall -Wextra -pedantic -std=c++11 -fvisibility=hidden -fPIC
LDFLAGS  = -shared -Wl,-soname,libosrmc.so.1
LDLIBS   = -lstdc++ -lm
