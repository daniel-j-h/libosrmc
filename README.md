##### Quick Start

    cd libosrmc
    make
    sudo make install
    sudo ldconfig

This compiles the `libosrmc.so` shared object and installs it into `/usr/local` (you may have to `export LD_LIBRARY_PATH="/usr/local/lib") or install to `/usr/lib`.
The library's interface `osrmc.h` gets installed into `/usr/local/include/osrmc/osrmc.h`.
You can modify defaults via `config.mk`.

Please refer to [`osrmc/osrmc.h`](https://github.com/daniel-j-h/libosrmc/blob/master/libosrmc/osrmc.h) for library documentation.

Note: works with the stable OSRM 5.3 release; if more recent releases break this library please open a ticket and let me know.

##### Todo

- [ ] Remaining Services
- [ ] Callbacks for Responses
- [ ] Use from Language FFIs
- [ ] Make Python Integration Exception-Safe

##### License

Copyright © 2016 Daniel J. Hofmann

Distributed under the MIT License (MIT).
