#!/bin/sh

autoreconf --install
./configure CXX="/local/clang-3.5/bin/clang++" CPPFLAGS="-I/local/libsfml-2.1/include" LDFLAGS="-L/local/libsfml-2.1/lib -Wl,--rpath -Wl,/local/libsfml-2.1/lib"
