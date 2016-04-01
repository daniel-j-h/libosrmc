##### Building libosrmc.so

    make

    ln -s libosrmc.so.1.0 libosrmc.so.1
    ln -s libosrmc.so.1.0 libosrmc.so

##### Using libosrmc.so in C project

    gcc -O2 -Wall -Wextra -pedantic -std=c99 -I. -L. example.c -losrmc -losrm -o example

    env LD_LIBRARY_PATH="." ldd ./a.out
    env LD_LIBRARY_PATH="." ./a.out /tmp/osrm-backend/test/data/monaco.osrm
    Distance: 1715 meters
    Duration: 119 second

##### Todo

    - [ ] Remaining Services
    - [ ] Install Target
    - [ ] Error Handling
    - [ ] Use from Language FFIs
