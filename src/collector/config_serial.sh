#!/bin/bash

# configure serial comunication wth the FPGA

stty -F /dev/ttyUSB0 115200 cs8 -parenb -cstopb ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke

