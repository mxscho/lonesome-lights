#!/bin/sh

autoreconf --install
./configure CXX="/local/clang-3.5/bin/clang++" CPPFLAGS="-I/usr/include -I/local/libsfml-2.1/include" LDFLAGS="-L/local/libsfml-2.1/lib -Wl,--rpath -Wl,/local/libsfml-2.1/lib" PKG_CONFIG_PATH="/local/libsfml-2.1/lib/pkgconfig"


