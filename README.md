##### Building libosrmc.so

    make

    ln -s libosrmc.so.1.0 libosrmc.so.1
    ln -s libosrmc.so.1.0 libosrmc.so

##### Using libosrmc.so in C project

Compile and link your C project.

    gcc -O2 -Wall -Wextra -pedantic -std=c99 -I. -L. example.c -losrmc -o example

We need to point `LD_LIBRARY_PATH` to the directory containing `libosrmc.so` or install `libosrmc.so` into `/usr/lib/`.

    env LD_LIBRARY_PATH="." ldd ./example
    env LD_LIBRARY_PATH="." ./example /tmp/osrm-backend/test/data/monaco.osrm
    Distance: 1715 meters
    Duration: 119 second

Direct dependencies:

    env LD_LIBRARY_PATH="." readelf -d example | ag needed
     0x0000000000000001 (NEEDED)             Shared library: [libosrmc.so.1]
     0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]


##### Todo

    - [ ] Remaining Services
    - [ ] Install Target
    - [ ] Error Handling
    - [ ] Use from Language FFIs
