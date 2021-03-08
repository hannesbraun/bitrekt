#!/bin/sh
cd "$(dirname "$0")"
cd ..
astyle *.cpp *.hpp *.h *.c --options=astyle.conf
