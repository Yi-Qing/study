#!/usr/bin/env bash

gcc main.c -L . -lm -Wl,-rpath .

ldd a.out
