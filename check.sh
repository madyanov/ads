#!/bin/sh
cppcheck -q -j4 --enable=warning,style,performance,portability .
