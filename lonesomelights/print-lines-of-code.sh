#!/bin/sh

find . \( -name "*.h" -o -name "*.tpp" -o -name "*.cpp" -o -name "*.glsl" \) -print0 | xargs -0 wc -l
