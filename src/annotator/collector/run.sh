
# source init.sh first
#
# ./run.sh "/home/szekeres/tutorials/HelloWorld_MicroBlaze6/" "HelloWorld" 1 "HelloWorld_hw.c" 0 2 "HelloWorld.c" "HelloWorld_sw.c" 0
#

SHP_COLLECTOR_BIN=${SHP_HOME}/bin/annotator/collector

cd $1

echo =======Run CoDeveloper=======
${SHP_COLLECTOR_BIN}/generate_proj $@ 

icProj2make.pl $2'.icProj'
make -f _Makefile build
make -f _Makefile export_hardware
make -f _Makefile export_software 

cd EDK
mkdir etc
${SHP_COLLECTOR_BIN}/generate_dw $1
mkdir data
${SHP_COLLECTOR_BIN}/generate_ucf $1

${SHP_COLLECTOR_BIN}/generate_xmp $@
${SHP_COLLECTOR_BIN}/generate_mhs $1
${SHP_COLLECTOR_BIN}/generate_mss $1

echo =======Run XPS=======
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

