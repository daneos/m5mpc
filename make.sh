#!/bin/sh

g++ -o m5mpc include/*.cpp m5mpc.cpp $(pkg-config --libs --cflags hildon-1) $(pkg-config --libs --cflags libmpdclient)