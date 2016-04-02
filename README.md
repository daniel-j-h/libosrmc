##### Building libosrmc.so

    make

    ln -s libosrmc.so.1.0 libosrmc.so.1
    ln -s libosrmc.so.1.0 libosrmc.so

##### Using libosrmc.so In C Projects

Compile and link your C project.

    gcc -O2 -Wall -Wextra -pedantic -std=c99 -I. -L. example.c -losrmc -o example

We need to point `LD_LIBRARY_PATH` to the directory containing `libosrmc.so` or install `libosrmc.so` into `/usr/lib/`.

    env LD_LIBRARY_PATH="." ldd ./example
    env LD_LIBRARY_PATH="." ./example /tmp/osrm-backend/test/data/monaco.osrm
    Distance: 1715 meters
    Duration: 119 second

Direct dependencies.

    env LD_LIBRARY_PATH="." readelf -d example | ag needed
     0x0000000000000001 (NEEDED)             Shared library: [libosrmc.so.1]
     0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]

##### Writing Bindings (Python Example)

See `osrmcpy.py` for how to use the FFI for interfacing and for an example at the end.

    env LD_LIBRARY_PATH="." python2 osrmcpy.py

##### Todo

    - [ ] Remaining Services
    - [ ] Install Target
    - [ ] Error Handling
    - [ ] Use from Language FFIs
    - [ ] Make Python Integration Exception-Safe
