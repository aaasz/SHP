#!/bin/bash

# Used to parse the output from the synthesizer

#./parse.sh "/home/szekeres/tutorials/fibonacci/hw/" > "/home/szekeres/tutorials/fibonacci/hw/parameters.txt"

echo `cat $1/terminal.log | tail -n 3 | head -n 2`


luts=`cat $1/output.txt | grep "Total Number of 4 input LUTs" | cut -d ":" -f 2 | cut -d " " -f 8 | sed 's/,//'` 
max_luts=`cat $1/output.txt | grep "Total Number of 4 input LUTs"|cut -d ":" -f 2| cut -d " " -f 13 | sed 's/,//'`
slices=`cat $1/output.txt | grep "Number of occupied Slices" | cut -d " " -f 16 | sed 's/,//'`
max_slices=`cat $1/output.txt | grep "Number of occupied Slices"|cut -d " " -f 21 | sed 's/,//'`


echo $luts 
echo $max_luts
echo $slices
echo $max_slices
