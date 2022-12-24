#!/bin/bash
tar -xf linux-5.19.9.tar.xz 
cd ./linux-5.19.9
make mrproper
cp ../.config ./
time make -j6