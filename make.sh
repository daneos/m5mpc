#!/bin/sh

g++ -o m5mpc include/*.cpp m5mpc.cpp $(pkg-config --cflags --libs gtk+-2.0 gmodule-2.0) $(pkg-config --libs --cflags hildon-1) $(pkg-config --libs --cflags libmpdclient) -export-dynamic -Wl,--export-dynamic