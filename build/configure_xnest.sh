#!/bin/bash

# configure command for running in xnest


P=`pwd`

./configure --program-prefix=mosw_ 
			--prefix=$P/build/prefix/ 
			--exec-prefix=$P/build/exec/ 
			--disable-randr 
			--disable-randr1.2 
			--disable-shape 
			--disable-xrender 
			--enable-debug 
			--enable-test 
			--disable-xinerama
