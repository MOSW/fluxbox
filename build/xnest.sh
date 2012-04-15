#!/bin/bash


Xnest :9 &  

DISPLAY=":9" ./build/exec/bin/mosw_fluxbox -rc ./build/data/init
