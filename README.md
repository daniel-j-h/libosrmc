##### Building libosrmc.so

    make

    ln -s libosrmc.so.1.0 libosrmc.so.1
    ln -s libosrmc.so.1.0 libosrmc.so

##### Using libosrmc.so In C Projects

Compile and link your ANSI C standard (or higher) conforming project.

    gcc -O2 -Wall -Wextra -pedantic -std=c89 -I. -L. example_c89.c -losrmc -o example_c89

We need to point `LD_LIBRARY_PATH` to the directory containing `libosrmc.so` or install `libosrmc.so` into `/usr/lib/`.

    env LD_LIBRARY_PATH="." ldd ./example_c89
    env LD_LIBRARY_PATH="." ./example_c89 /tmp/osrm-backend/test/data/monaco.osrm
    Distance: 1715 meters
    Duration: 119 seconds

Direct dependencies.

    env LD_LIBRARY_PATH="." readelf -d example_c89 | ag needed
     0x0000000000000001 (NEEDED)             Shared library: [libosrmc.so.1]
     0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]

##### Writing Bindings (Python Example)

See `osrmcpy.py` for the FFI bindings and `example_python.py` for usage.

    env LD_LIBRARY_PATH="." python2 example_python2.py /tmp/osrm-backend/test/data/monaco.osrm
    Distance: 1715 meters
    Duration: 119 seconds
    Table
    0s      158s    219s    51s     165s    136s    151s    142s    141s    70s
    192s    0s      166s    153s    30s     83s     98s     18s     19s     122s
    104s    131s    0s      65s     138s    108s    124s    115s    114s    73s
    68s     146s    208s    0s      153s    124s    139s    130s    130s    74s
    198s    35s     173s    160s    0s      89s     105s    25s     26s     129s
    177s    98s     84s     139s    105s    0s      15s     82s     82s     108s
    172s    199s    68s     134s    206s    177s    0s      183s    182s    141s
    174s    16s     148s    135s    23s     65s     80s     0s      1s      104s
    172s    17s     147s    134s    24s     64s     79s     1s      0s      103s
    72s     143s    204s    31s     150s    120s    136s    127s    126s    0s

##### Google's or-tools Integration

Install or-tools.

    wget https://github.com/google/or-tools/releases/download/v2015-12/Google.OrTools.python.examples.3393.tar.gz
    tar xf Google.OrTools.python.examples.3393.tar.gz
    cd ortools_examples
    python2 setup.py install --user

Solve random Traveling Salesman Problem instance with or-tools based on OSRM Table service durations.

    env LD_LIBRARY_PATH="." python2 example_or_tools.py /tmp/osrm-backend/test/data/monaco.osrm
    Solution: 1686 seconds

References:

- https://developers.google.com/optimization/routing/tsp
- https://developers.google.com/optimization/installing#python
- https://or-tools.googlecode.com/svn/trunk/documentation/user_manual/manual/introduction.html
- https://or-tools.googlecode.com/svn/trunk/documentation/user_manual/manual/TSP.html
- https://or-tools.googlecode.com/svn/trunk/documentation/user_manual/manual/VRP.html

##### Writing Bindings (Haskell Example)

See `example_haskell.hs`. Either use `runghc` for convenience.

    env LD_LIBRARY_PATH="." runghc -losrmc example_haskell.hs
    Distance: 1715.3 meters
    Duration: 1715.3 seconds

Or build an executable.

    ghc --make -L. -losrmc example_haskell.hs
    env LD_LIBRARY_PATH="." ./example_haskell
    Distance: 1715.3 meters
    Duration: 1715.3 seconds

##### Todo

- [ ] Remaining Services
- [ ] Install Target
- [ ] Use from Language FFIs
- [ ] Make Python Integration Exception-Safe

##### License

Copyright © 2016 Daniel J. Hofmann

Distributed under the MIT License (MIT).
