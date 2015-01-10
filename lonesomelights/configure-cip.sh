#!/bin/sh

autoreconf --install
./configure CPPFLAGS="-I/local/libsfml-2.1/include" LDFLAGS="-L/local/libsfml-2.1/lib -Wl,--rpath -Wl,/local/libsfml-2.1/lib"
