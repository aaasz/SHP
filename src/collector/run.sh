#!/bin/bash

#export SHP_HOME=/home/szekeres/SHP
#export PATH=$PATH:/home/szekeres/SHP/collector/bin

# ./run.sh "/home/szekeres/tutorials/function_five/hw/" "FV" "hw" 3 1 3 "stub_function.c" "function_five.c" "config_function_five.c" 0 2 "main.c" "test_function_five.c" 0

# ./run.sh "/home/szekeres/tutorials/function_five/sw/" "FV" "sw" 0 0 0 0 2 "function_five.c" "test_function_five.c" 0

# ./run.sh "/home/szekeres/tutorials/HelloWorld_MicroBlaze6/" "HelloWorld" "hw" 1 1 1 "HelloWorld_hw.c" 0 2 "HelloWorld.c" "HelloWorld_sw.c" 0

#./run.sh "/home/szekeres/tutorials/function_sw/" "TFunction" "sw" 0 0 0 0 1 "function.c" 0

# ./run.sh "/home/szekeres/tutorials/test/" "HelloWorld" "hw" 1 1 3 "co_initialize.c" "say_hello.c" "config_hello.c" 0 2 "main.c" "hear_hello.c" 0
 
#./run.sh "/home/szekeres/tutorials/fibonacci/hw/" "Fibonacci" "hw" 1 1 1 "my_function.c" 0 2 "main.c" "my_test_function.c" 0 > /home/szekeres/tutorials/fibonacci/hw/output.txt

cd $1
echo $3
if [ $3 = "hw" ];then
	echo ======= Run CoDeveloper =======
	generate_proj $@ 

	icProj2make.pl $2'.icProj'
	make -f _Makefile build
	make -f _Makefile export_hardware
	make -f _Makefile export_software 
else
	mkdir EDK	
	mkdir EDK/code
	cp *.* EDK/code	
fi

cd EDK
mkdir etc
generate_dw $1
mkdir data
generate_ucf $1

generate_xmp $@
generate_mhs $1 $3 $4 $5
generate_mss $1 $3 

echo ====== Start listening to serial ======
cat /dev/ttyUSB0 > $1/terminal.log &

echo ======= Run XPS =======
xps -nw << EOF
xload xmp system.xmp
run netlist
run bits
run libs
run program
run init_bram
run download
exit
EOF

# Wait until the process has finished running on MicroBlaze or FPGA

temp=`cat $1/terminal.log | tail -n 1`

while [ "$temp" != "SERIALSTOP" ]; do
	temp=`cat $1/terminal.log | tail -n 1`
done 

echo ====== Stop listening to serial ======
var=`ps ax | grep "cat /dev/ttyUSB0" | head -n 1`
pid=${var:0:5}   

echo $pid

kill -9 $pid
